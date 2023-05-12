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
	if ( rb->filled_lines > MAX_LINES ) return -1;
	int	position = rb->filled_lines;

	rb->line[position].instruction_execution = NOT_BUSY;
	
	rb->line[position].instruction = instruction->full_instruction;

	rb->line[position].instruction_state = WAITING;

	rb->line[position].instruction_destination = instruction->splitted_instruction[1];

	rb->line[position].instruction_result = (char*) malloc( SIZE_STR * sizeof(char) );
	strcpy(rb->line[position].instruction_result, "NOT CALCULATED YET\0");

	instruction->reorder_buffer_position = position;

	rb->filled_lines += 1;

	return rb->filled_lines;
}

/**
 * @brief Prints the Reorder Buffer
 * 
 * @param reorder buffer to be printed
 *
 */
void printReorderBuffer(Reorder_Buffer *rb){
	int	i = 0,
		max = rb->filled_lines;
	printf("\n\n\nPrinting Reorder Buffer. . .\n");
	for( i = 0; i < max; i++ ) {
		printf("Entry: %d\n", i);
		printf("Busy: %s\n", ( rb->line[i].instruction_execution == NOT_BUSY ) ? "No\0" : "Yes\0");
		printf("Instruction: %s\n", rb->line[i].instruction);
		printf("State: %d\n", rb->line[i].instruction_state);
		printf("Destination: %s\n", rb->line[i].instruction_destination);
		printf("Values: %s\n", rb->line[i].instruction_result);
		printf("\n");
	}
}

/**
 * @brief Freeing memory allocated to reorder buffer
 * 
 * @return NULL if reorder buffer is free, reorder buffer otherwise
 *
 * @details One should free memory using free() function to avoid memory leaks
 *
 */
Reorder_Buffer* freesReorderBuffer(Reorder_Buffer *rb) {
	int i = 0;
    if (rb == NULL)
		printf("** Error: Invalid reorder buffer!**\n");
        return (NULL);
    if (MAX_LINES < 1)
    {
        printf("** Error: Invalid MAX_LINES!**\n");
        return (rb);
    }
    for (i = 0; i < MAX_LINES; i++) {
		free(rb->line[i].instruction);
		free(rb->line[i].instruction_destination);
		free(rb->line[i].instruction_result);
	}
		
    free(rb);        /* Frees the reorder buffer */
    return (NULL);
}
