#include "reorder_buffer.h"
#include "reservation_station.h"
#include "instructions.h"
#include <string.h>
#include <stdio.h>

/**
 * @brief Initializing Reservation_Station
 *
 * @details Instantiating a new Reservation Station
 *
 * @return Reference to the newly created reservation station
 */
Reservation_Station* reservationStationInitializer(){
	Reservation_Station* rs = (Reservation_Station*) malloc( sizeof(Reservation_Station) );
	
	rs->line[0].name = "Load1";
	rs->line[0].type = LOAD;

	rs->line[1].name = "Load2";
	rs->line[1].type = LOAD;

	rs->line[2].name = "ADD1";
	rs->line[2].type = ADD;

	rs->line[3].name = "ADD2";
	rs->line[3].type = ADD;

	rs->line[4].name = "ADD3";
	rs->line[4].type = ADD;

	rs->line[5].name = "MULT1";
	rs->line[5].type = MULT;

	rs->line[6].name = "MULT2";
	rs->line[6].type = MULT;

	rs->line[7].name = "BRANCH";
	rs->line[7].type = BRANCH;

	rs->line[8].name = "COMPARISON";
	rs->line[8].type = COMPARISON;

	rs->line[9].name = "LOGICAL1";
	rs->line[9].type = LOGICAL;

	rs->line[10].name = "LOGICAL2";
	rs->line[10].type = LOGICAL;

	rs->line[11].name = "LOGICAL3";
	rs->line[11].type = LOGICAL;
	
	for(int i = 0; i < MAX_LINES_RS; i++) {
		rs->line[i].reservation_busy = NOT_BUSY;
		rs->line[i].instruction_op = (char*) malloc ( SIZE_STR * sizeof(char) );
		strcpy(rs->line[i].instruction_op, "-");
		rs->line[i].value_register_read_Vj = (char*) malloc ( SIZE_STR * sizeof(char) );
		strcpy(rs->line[i].value_register_read_Vj, "-");
		rs->line[i].value_register_read_Vk = (char*) malloc ( SIZE_STR * sizeof(char) );
		strcpy(rs->line[i].value_register_read_Vk, "-");
		rs->line[i].information_dependency_Qj = -1;
		rs->line[i].information_dependency_Qk = -1;
		rs->line[i].position_destination_rb = -1;
		rs->line[i].memory_address = (char*) malloc ( SIZE_STR * sizeof(char) );
		strcpy(rs->line[i].memory_address, "-");
	}

	return rs;
}

/**
 * @brief Release line containing reservation station of a functional unit
 *
 * @param rs Structure that contains the reserve station that will be released
 * @param positionRS Position where the reserve station is on the structure
 *
 * @details Release reservation station that had an instruction that has already been executed
 */
void clearLineRS (Reservation_Station *rs, int positionRS) {
	strcpy(rs->line[positionRS].instruction_op, "");
	strcpy(rs->line[positionRS].value_register_read_Vj, "");
	strcpy(rs->line[positionRS].value_register_read_Vk, "");
	if (rs->line[positionRS].memory_address != NULL) {
		strcpy(rs->line[positionRS].memory_address, "");
	}
	rs->line[positionRS].reservation_busy = NOT_BUSY;
	rs->line[positionRS].information_dependency_Qj = -1;
	rs->line[positionRS].information_dependency_Qk = -1;
	rs->line[positionRS].position_destination_rb = -1;
}

/**
 * @brief Inserting new instruction into one of reservation station
 *
 * @param instruction to be added
 * @param reservationStation reservation to add the instruction
 * @param rb Reorder buffer
 *
 * @details Receives a new instruction and inserts it into the correct position of a reserve station of 
 * a functional unit.
 * It will check which is the reservation station for the instruction, and if there is not one available, 
 * -1 will be returned. If there is a reservation available, it will return in which line the instruction 
 * is waiting to be executed.
 *
 * @return If it's possible to add a new instruction, returns position where it was inserted.
 * If there's no free room, returns -1.
 */
int insertInstructionRS(Instruction *instruction, Reservation_Station *reservationStation, Reorder_Buffer *rb){
	int positionRS = -1, pos = 0;

	if ( instruction->type == LOAD ) {
		for ( pos = 0; pos < 2; pos++ ) {
		    if ( reservationStation->line[pos].reservation_busy == NOT_BUSY ) {
				positionRS = pos;
				pos = 2;
		    }
		}

	} else if ( instruction->type == ADD ) {
		for ( pos = 2; pos < 5; pos++ ) {
		    if ( reservationStation->line[pos].reservation_busy == NOT_BUSY ) {
				positionRS = pos;
				pos = 5;
		    }
		}

	} else if ( instruction->type == MULT ) {
		for ( pos = 5; pos < 7; pos++ ) {
		    if ( reservationStation->line[pos].reservation_busy == NOT_BUSY ) {
				positionRS = pos;
				pos = 7;
		    }
		}

	} else if ( instruction->type == BRANCH ) {
		if ( reservationStation->line[7].reservation_busy == NOT_BUSY ) {
			positionRS = 7;
		}

	} else if ( instruction->type == COMPARISON ) {
		    if ( reservationStation->line[8].reservation_busy == NOT_BUSY ) {
				positionRS = 8;
		    }

	} else if ( instruction->type == LOGICAL ) {
		for ( pos = 9; pos < MAX_LINES_RS; pos++ ) {
		    if ( reservationStation->line[pos].reservation_busy == NOT_BUSY ) {
				positionRS = pos;
				pos = MAX_LINES_RS;
		    }
		}
		
	} else {
		printf("** Error: Instruction is not supported in this simulator!**\n");
	}

	if (positionRS != -1) {
		reservationStation->line[positionRS].reservation_busy = BUSY;
		strcpy(reservationStation->line[positionRS].instruction_op, instruction->splitted_instruction[0]);
		if ( instruction->type == LOAD ) {

			if ( strcmp(instruction->splitted_instruction[0], "SW") == 0 ) { 
				reservationStation->line[positionRS].information_dependency_Qj = warDependencyIdentifier(instruction->splitted_instruction[1], instruction->reorder_buffer_position, rb);
			} else {
				reservationStation->line[positionRS].information_dependency_Qj = -1;
			}
			
			reservationStation->line[positionRS].information_dependency_Qk = warDependencyIdentifier(instruction->splitted_instruction[3], instruction->reorder_buffer_position, rb);

		} else if ( instruction->type == BRANCH ) {
			reservationStation->line[positionRS].information_dependency_Qj = warDependencyIdentifier(instruction->splitted_instruction[1], instruction->reorder_buffer_position, rb);
			reservationStation->line[positionRS].information_dependency_Qk = warDependencyIdentifier(instruction->splitted_instruction[2], instruction->reorder_buffer_position, rb);
		
		} else {
			reservationStation->line[positionRS].information_dependency_Qj = warDependencyIdentifier(instruction->splitted_instruction[2], instruction->reorder_buffer_position, rb);
			reservationStation->line[positionRS].information_dependency_Qk = warDependencyIdentifier(instruction->splitted_instruction[3], instruction->reorder_buffer_position, rb);
		}
		
		if ( reservationStation->line[positionRS].information_dependency_Qj == -1 ) {
			if ( strcmp(instruction->splitted_instruction[0], "SW") == 0 || instruction->type == BRANCH) {
				strcpy( reservationStation->line[positionRS].value_register_read_Vj, instruction->splitted_instruction[1] );
			} else {
				strcpy( reservationStation->line[positionRS].value_register_read_Vj, instruction->splitted_instruction[2] );
			}
		}
		if ( reservationStation->line[positionRS].information_dependency_Qk == -1 ) {
			if (instruction->type == BRANCH) {
				strcpy( reservationStation->line[positionRS].value_register_read_Vk, instruction->splitted_instruction[2] );
			} else {
				strcpy( reservationStation->line[positionRS].value_register_read_Vk, instruction->splitted_instruction[3] );
			}
		}
		
		reservationStation->line[positionRS].position_destination_rb = instruction->reorder_buffer_position;
		
		if ( instruction->type == LOAD ) {
			char* memory_address = (char*) malloc( SIZE_STR * sizeof(char) );
			strcpy(memory_address, "");
			size_t vasco = 0;
			for( ; vasco < strlen(instruction->splitted_instruction[2]); vasco++) {
				memory_address[vasco] = instruction->splitted_instruction[2][vasco];
			}
			memory_address[vasco++] = ' ';
			memory_address[vasco++] = '+';
			memory_address[vasco++] = ' ';
			memory_address[vasco++] = 'R';
			memory_address[vasco++] = 'e';
			memory_address[vasco++] = 'g';
			memory_address[vasco++] = 's';
			memory_address[vasco++] = '[';
			size_t flamengo = 0;
			for( ; flamengo < strlen(instruction->splitted_instruction[3]); flamengo++) {
				memory_address[vasco++] = instruction->splitted_instruction[3][flamengo];
			}
			memory_address[vasco++] = ']';
			memory_address[vasco++] = '\0';
			strcpy(reservationStation->line[positionRS].memory_address, memory_address );
		}
	}

	return positionRS;
}

/**
 * @brief Find reservation station line according to position in reorder buffer
 *
 * @param rs 			Structure that holds all the lines of the reservation station
 * @param positionRB	Position in reorder buffer
 *
 * @details Find reservation station line according to position in reorder buffer
 *
 * @return 	Returns the line position of the reservation station that the position 
 * 			instruction is in the reorder buffer, or if no line is found, returns -1
*/
int findLineRSAccordingPositionRB (Reservation_Station *rs, int positionRB) {
	int position_line_rs = -1;

	for( int i = 0; i < MAX_LINES_RS; i++ ) {

		if (rs->line[i].position_destination_rb == positionRB) {
			position_line_rs = i;
			i = MAX_LINES_RS;
		}
	}

	return position_line_rs;
}

/**
 * @brief Checks if the instruction performs writing
 *
 * @param nameInstruction Name of the instruction that will be parsed
 *
 * @details Checks if the instruction performs writing
 *
 * @return If the instruction performs writing, 1 will be returned, otherwise 0
*/
int dontDoWrite(char* nameInstruction) {
	return ((strcmp(nameInstruction, "SW") == 0) || (strcmp(nameInstruction, "BEQ") == 0) || (strcmp(nameInstruction, "BNE") == 0));
}

/**
 * @brief Identify true dependency on a statement
 *
 * @param analyzed_register Register that will be analyzed if there is a true dependency
 * @param position Position in the reorder buffer of the instruction that contains the register that will be analyzed
 * @param rb Reorder Buffer
 *
 * @details Receives the register that will be analyzed if there is a true dependency and returns the position in the 
 * 			reorder buffer of the instruction that is depending
 *
 * @return If there is a dependency, the position in the reorder buffer of the instruction that the register depends 
 * 	       on will be returned, otherwise, if there is no dependency, -1 will be returned
*/
int warDependencyIdentifier(char *analyzed_register, int position, Reorder_Buffer *rb){
	int	i = 0;
	int result = -1;
	int init = rb->filled_lines % rb->max_lines_rb_allocated;
	if ( position >= 0 ) {
		i = (position - 1) < 0 ? (rb->max_lines_rb_allocated - 1) : (position - 1) % rb->max_lines_rb_allocated;
		
		/* Retrieving the last dependecy found */
		for ( i = init ; i != position; i = ((i + 1) % rb->max_lines_rb_allocated) ) {
			char* instruction_operation = rb->line[i].instruction->splitted_instruction[0];

			// If the instruction already has its result, or if it does not write to the register, there is no need to analyze the dependency
			if ( rb->line[i].instruction_state == WRITE_RESULT || rb->line[i].instruction_state == COMMITED || (dontDoWrite(instruction_operation) == 1)) 
				continue;

			/* RAW (Read After Write) dependency */
			if ( strcmp(rb->line[i].instruction->splitted_instruction[1], analyzed_register) == 0 ) {
				result = i;
			}
		}
	}
	return result;
}

/**
 * @brief Prints the Reservation Station
 * 
 * @param reservationStation Reservation station to be printed
 *
 */
void printReservationStation(Reservation_Station *reservationStation){
	int	i = 0;
	printf("\n\t\t\t\t\tReservation Station\n\n");
	printf("  Name\t\tBusy\tOp\t\tVj\t\tVk\t\tQj\tQk\tDest\tAddress\n");
	for( i = 0; i < MAX_LINES_RS; i++ ) {
		printf("  %s\t", reservationStation->line[i].name);
		if(i <= 6) {
			printf("\t");
		}
		printf("%s\t", ( reservationStation->line[i].reservation_busy == NOT_BUSY ) ? "No\0" : "Yes\0");
		printf("%s\t\t", (reservationStation->line[i].instruction_op == NULL ? "-" : (strcmp(reservationStation->line[i].instruction_op, "") == 0 ? "-" : reservationStation->line[i].instruction_op) ));
		printf("%s\t\t", (reservationStation->line[i].value_register_read_Vj == NULL ? "-" : (strcmp(reservationStation->line[i].value_register_read_Vj, "") == 0 ? "-" : (strcmp(reservationStation->line[i].instruction_op, "LW") == 0 ? "-" : reservationStation->line[i].value_register_read_Vj))));
       	printf("%s\t\t", (reservationStation->line[i].value_register_read_Vk == NULL ? "-" : (strcmp(reservationStation->line[i].value_register_read_Vk, "") == 0 ? "-" : reservationStation->line[i].value_register_read_Vk)));
        printf("%d\t", reservationStation->line[i].information_dependency_Qj);
        printf("%d\t", reservationStation->line[i].information_dependency_Qk);
		printf(" %d\t", reservationStation->line[i].position_destination_rb);
		printf("%s\n", (reservationStation->line[i].memory_address == NULL ? "-" : (strcmp(reservationStation->line[i].memory_address, "") == 0 ? "-" : reservationStation->line[i].memory_address)));
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
/*
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
		
    free(rb); */       /* Frees the reorder buffer */
    /* return (NULL);
}*/
