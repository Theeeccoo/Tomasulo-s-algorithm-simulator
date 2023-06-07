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

	//TODO - Inicializar as variáveis que precisa de colocar a posição do reorder buffer com -1
	
	rs->line[0].name = "Load1";
	rs->line[0].type = LOAD;
	rs->line[0].reservation_busy = NOT_BUSY;

	rs->line[1].name = "Load2";
	rs->line[1].type = LOAD;
	rs->line[1].reservation_busy = NOT_BUSY;

	rs->line[2].name = "ADD1";
	rs->line[2].type = ADD;
	rs->line[2].reservation_busy = NOT_BUSY;

	rs->line[3].name = "ADD2";
	rs->line[3].type = ADD;
	rs->line[3].reservation_busy = NOT_BUSY;

	rs->line[4].name = "ADD3";
	rs->line[4].type = ADD;
	rs->line[4].reservation_busy = NOT_BUSY;

	rs->line[5].name = "MULT1";
	rs->line[5].type = MULT;
	rs->line[5].reservation_busy = NOT_BUSY;

	rs->line[6].name = "MULT2";
	rs->line[6].type = MULT;
	rs->line[6].reservation_busy = NOT_BUSY;

	return rs;
}

/**
 * @brief Inserting new instruction into one of reservation station
 *
 * @param instruction to be added
 * @param reservationStation reservation to add the instruction
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
int insertInstructionRS(Instruction *instruction, Reservation_Station *reservationStation, Instruction *instructions, Reorder_Buffer *rb){
	int positionRS = -1, pos = 0;

	// TODO - Caso tenha dependência falsa (escrita após escrita, ou escrita após leitura)
	// renomear o registrador com o nome da estação de reserva, analisando todas as 
	// instruções anteriores a esta no buffer de reordenamento

	// TODO - Caso tenha dependência verdadeira (leitura após escrita) escrever em Qj e Qk de 
	// qual posição do buffer de reordenamento o registrador de leitura da instrução inserida depende

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
	} else {
		for ( pos = 5; pos < 7; pos++ ) {
		    if ( reservationStation->line[pos].reservation_busy == NOT_BUSY ) {
			positionRS = pos;

			pos = 7;
		    }
		}
	}

	if (positionRS != -1) {
		// TODO - Atualmente as dependências estão sendo mostradas pela posição no reorder buffer na qual
		// a instrução depende, precisamos verificar se é assim mesmo no algoritmo de tomasulo, pois a outra
		// opção é colocar ao invés da posição no reorder buffer o nome da estação de reserva que se depende

		reservationStation->line[positionRS].reservation_busy = BUSY;
		reservationStation->line[positionRS].instruction_op = instruction->splitted_instruction[0];
		reservationStation->line[positionRS].value_register_read_Vj = "T4";
		reservationStation->line[positionRS].value_register_read_Vk = "R1";
		if (instruction->type == LOAD) {
			// TODO - Pegar a dependência de forma especial para Load e Store
		} else {
			reservationStation->line[positionRS].information_dependency_Qj = warDependencyIdentifier(instructions, instruction->splitted_instruction[2], instruction->reorder_buffer_position, rb);
			reservationStation->line[positionRS].information_dependency_Qk = warDependencyIdentifier(instructions, instruction->splitted_instruction[3], instruction->reorder_buffer_position, rb);
		}
		
		reservationStation->line[positionRS].position_destination_rb = instruction->reorder_buffer_position;
		reservationStation->line[positionRS].memory_address = "Mem[32 + REG[R1]]";
	}

	return positionRS;
}

/**
 * @brief Identify true dependency on a statement
 *
 * @param all_instructions All instructions
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
// TODO - Talvez seja interessante colocar em Instruction o state (existente no Reorder_buffer) para não precisar de chamar ele aqui
int warDependencyIdentifier(Instruction *all_instructions, char *analyzed_register, int position, Reorder_Buffer *rb){
	int	i = 0;
	int result = -1;
	if ( position != 0 ) {
		/* Retrieving the last dependecy found */
		for ( i = 0; i < position; i++ ) {
			if ( rb->line[i].instruction_state == WRITE_RESULT || rb->line[i].instruction_state == COMMITED ) 
				continue;
			
			char *destination = all_instructions[i].splitted_instruction[1];

			/* RAW (Read After Write) dependency */
			if ( strcmp(destination, analyzed_register) == 0 ) {
				result = all_instructions[i].reorder_buffer_position;
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
	printf("\n\n\nPrinting Reservation Station. . .\n");
	for( i = 0; i < MAX_LINES_RS; i++ ) {
		printf("Name: %s\n", reservationStation->line[i].name);
		printf("Busy: %s\n", ( reservationStation->line[i].reservation_busy == NOT_BUSY ) ? "No\0" : "Yes\0");
		printf("Instruction Op: %s\n", reservationStation->line[i].instruction_op);
		printf("Vj: %s\n", reservationStation->line[i].value_register_read_Vj);
       	printf("Vk: %s\n", reservationStation->line[i].value_register_read_Vk);
        printf("Qj: %d\n", reservationStation->line[i].information_dependency_Qj);
        printf("Qk: %d\n", reservationStation->line[i].information_dependency_Qk);
		printf("Destination: %d\n", reservationStation->line[i].position_destination_rb);
		printf("Memory Address: %s\n\n", reservationStation->line[i].memory_address);
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
