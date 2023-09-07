#include "reorder_buffer.h"
#include "instructions.h"
#include <string.h>
#include <stdio.h>

/**
 * @brief Initializing Reorder_Buffer
 *
 * @details Instantiating a new Reorder buffer
 *
 * @return Reference to the newly created reorder buffer
 */
Reorder_Buffer* reorderBufferInitializer(){
	Reorder_Buffer* rb = (Reorder_Buffer*) malloc( sizeof(Reorder_Buffer) );

	rb->max_lines_rb_allocated = 0;
	rb->filled_lines = 0;	
	return rb;
}

/**
 * @brief Inserting new instruction into our reorder buffer
 *
 * @param instruction to be added
 * @param reorder buffer to add the instruction
 *
 * @details Receives a new instruction and insert it into
 * the right position of our reorder buffer.
 * Initially all instructions are started as "NOT_BUSY", but when they're dispatched it'll be changed.
 * Initially all instructions are started with a place holder "instruction_result".
 *
 *
 * @return If it's possible to add a new instruction, returns position where it was inserted.
 * If there's no free room, returns -1.
 */
int insertInstructionRB(Instruction *instruction, Reorder_Buffer *rb){
	
	int	position = rb->filled_lines % MAX_LINES;

	rb->line[position].instruction_execution = BUSY;
	
	rb->line[position].instruction = cloneInstruction(instruction);

	rb->line[position].instruction_state = WAITING;

	if (rb->line[position].instruction_result == NULL) {
		rb->line[position].instruction_result = (char*) malloc( SIZE_STR * sizeof(char) );
	}
	strcpy(rb->line[position].instruction_result, "NOT CALCULATED YET\0");

	rb->line[position].instruction->reorder_buffer_position = position;

	rb->filled_lines += 1;

	if (rb->filled_lines < MAX_LINES) {
		rb->max_lines_rb_allocated = rb->filled_lines;
	} else {
		rb->max_lines_rb_allocated = MAX_LINES;
	}

	return position;
}

/**
 * @brief Print the lines of the reorder buffer in order from the first to the last statement placed.
 * 
 * @param rb buffer to be printed
 *
 */
void printReorderBuffer(Reorder_Buffer *rb){
	int	i = 0,
		init = rb->filled_lines % rb->max_lines_rb_allocated,
		end = ((init - 1) < 0 ? (rb->max_lines_rb_allocated - 1) : (init - 1));
	printf("\n\n\t\t\t\t\tReorder Buffer\n\n");
	printf("  Entry\t\tBusy\tInstruction\t\t\tState\t\tDestination\tValue\n");
	for( i = init; i != end; i = ((i + 1) % rb->max_lines_rb_allocated) ) {
		printf("    %d\t\t", i);
		printf("%s\t", ( rb->line[i].instruction_execution == NOT_BUSY ) ? "No\0" : "Yes\0");
		printf("%s\t\t", (rb->line[i].instruction->full_instruction == NULL ? "-" : (strcmp(rb->line[i].instruction->full_instruction, "") == 0 ? "-" : rb->line[i].instruction->full_instruction)));
		if ( rb->line[i].instruction->type != BRANCH && (strlen(rb->line[i].instruction->splitted_instruction[1]) + strlen(rb->line[i].instruction->splitted_instruction[2]) + strlen(rb->line[i].instruction->splitted_instruction[3])) < 9) {
			printf("\t");
		}
		if ( (rb->line[i].instruction->full_instruction == NULL || strcmp(rb->line[i].instruction->full_instruction, "") == 0 || strcmp(rb->line[i].instruction->full_instruction, "-") == 0) && (strlen(rb->line[i].instruction->splitted_instruction[1]) + strlen(rb->line[i].instruction->splitted_instruction[2]) + strlen(rb->line[i].instruction->splitted_instruction[3])) < 9) {
			printf("\t");
		}
		if (rb->line[i].instruction_execution == NOT_BUSY && i == 5 && (rb->line[i].instruction->full_instruction == NULL || strcmp(rb->line[i].instruction->full_instruction, "") == 0 || strcmp(rb->line[i].instruction->full_instruction, "-") == 0)) {
			printf("\t");
		}
		printf("%s\t", (rb->line[i].instruction_state == WAITING ? "WAITING" : (rb->line[i].instruction_state == EXECUTING ? "EXECUTING" : (rb->line[i].instruction_state == WRITE_RESULT ? "WRITE_RESULT" : (rb->line[i].instruction_state == ISSUE ? "ISSUE" : "COMMITED")))));
		if ( rb->line[i].instruction_state < 2 ) {
			printf("\t");
		}
		if (rb->line[i].instruction->type == BRANCH || (rb->line[i].instruction->splitted_instruction[0] != NULL && strcmp(rb->line[i].instruction->splitted_instruction[0], "SW") == 0)) {
			printf("    %s\t\t", "-");
		} else {
			printf("    %s\t\t", (rb->line[i].instruction->splitted_instruction[1] == NULL ? "-" : (strcmp(rb->line[i].instruction->splitted_instruction[1], "") == 0 ? "-" : rb->line[i].instruction->splitted_instruction[1])));
		}
		printf("%s\n", (rb->line[i].instruction_result == NULL ? "-" : (strcmp(rb->line[i].instruction_result, "") == 0 ? "-" : rb->line[i].instruction_result)));
	}
	// Print the last position alone
	printf("    %d\t\t", end);
	printf("%s\t", ( rb->line[end].instruction_execution == NOT_BUSY ) ? "No\0" : "Yes\0");
	printf("%s\t\t", (rb->line[end].instruction->full_instruction == NULL ? "-" : (strcmp(rb->line[end].instruction->full_instruction, "") == 0 ? "-" : rb->line[end].instruction->full_instruction)));
	if ( rb->line[i].instruction->type != BRANCH && (strlen(rb->line[i].instruction->splitted_instruction[1]) + strlen(rb->line[i].instruction->splitted_instruction[2]) + strlen(rb->line[i].instruction->splitted_instruction[3])) < 9) {
			printf("\t");
	}
	if ( (rb->line[end].instruction->full_instruction == NULL || strcmp(rb->line[end].instruction->full_instruction, "") == 0) && (strlen(rb->line[i].instruction->splitted_instruction[1]) + strlen(rb->line[i].instruction->splitted_instruction[2]) + strlen(rb->line[i].instruction->splitted_instruction[3])) < 9) {
			printf("\t");
		}
	printf("%s\t", (rb->line[end].instruction_state == WAITING ? "WAITING" : (rb->line[end].instruction_state == EXECUTING ? "EXECUTING" : (rb->line[end].instruction_state == WRITE_RESULT ? "WRITE_RESULT" : (rb->line[end].instruction_state == ISSUE ? "ISSUE" : "COMMITED")))));
	if ( rb->line[end].instruction_state < 2 ) {
		printf("\t");
	}
	if (rb->line[end].instruction->type == BRANCH || (rb->line[end].instruction->splitted_instruction[0] != NULL && strcmp(rb->line[end].instruction->splitted_instruction[0], "SW") == 0)) {
		printf("    %s\t\t", "-");
	} else {
		printf("    %s\t\t", (rb->line[end].instruction->splitted_instruction[1] == NULL ? "-" : (strcmp(rb->line[end].instruction->splitted_instruction[1], "") == 0 ? "-" : rb->line[end].instruction->splitted_instruction[1])));
	}
	printf("%s\n", (rb->line[end].instruction_result == NULL ? "-" : (strcmp(rb->line[end].instruction_result, "") == 0 ? "-" : rb->line[end].instruction_result)));
}

/**
 * @brief Clear data from a reorder buffer line
 * 
 * @param rb 				Structure that contains the reorder buffer
 * @param positionToFree	Position of the reorder buffer line that will be cleared
 * 
 * @details Clear data from a reorder buffer line
 */
void freeLineReorderBuffer(Reorder_Buffer *rb, int positionToFree) {
	rb->line[positionToFree].instruction_state = WAITING;
	strcpy(rb->line[positionToFree].instruction->full_instruction, ""); 
	strcpy(rb->line[positionToFree].instruction->splitted_instruction[1], "");
	strcpy(rb->line[positionToFree].instruction_result, "");
	rb->line[positionToFree].instruction_execution = NOT_BUSY;
}

/**
 * @brief Freeing memory allocated to reorder buffer
 * 
 * @return NULL if reorder buffer is free, reorder buffer otherwise
 *
 * @details One should free memory using free() function to avoid memory leaks
 */
Reorder_Buffer* freesReorderBuffer(Reorder_Buffer *rb) {
	int i = 0;
    if (rb == NULL)
		printf("** Error: Invalid reorder buffer!**\n");
        return (NULL);
    if (rb->max_lines_rb_allocated < 1)
    {
        printf("** Error: Invalid maximum rows allocated!**\n");
        return (rb);
    }
    for (i = 0; i < rb->max_lines_rb_allocated; i++) {
		free(rb->line[i].instruction);
		//free(rb->line[i].instruction_destination);
		free(rb->line[i].instruction_result);
	}
		
    free(rb);        /* Frees the reorder buffer */
    return (NULL);
}
