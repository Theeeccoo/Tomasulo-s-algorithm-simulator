#include "reorder_buffer.h"
#include "instructions.h"
#include "register_status.h"
#include <string.h>
#include <stdio.h>

/**
 * @brief Initializing FP_register_status
 *
 * @details Instantiating a new FP_register_status
 *
 * @return Reference to the newly created FP_register_status
 */
Register_status* registerStatusInitializer(){
	Register_status* rStatus = (Register_status*) malloc( sizeof(Register_status) );
	
	rStatus->column[0].field = "F0";
	rStatus->column[0].reorder_entry = -1;
	rStatus->column[0].register_busy = NOT_BUSY;

	rStatus->column[1].field = "F1";
	rStatus->column[1].reorder_entry = -1;
	rStatus->column[1].register_busy = NOT_BUSY;

	rStatus->column[2].field = "F2";
	rStatus->column[2].reorder_entry = -1;
	rStatus->column[2].register_busy = NOT_BUSY;

	rStatus->column[3].field = "F3";
	rStatus->column[3].reorder_entry = -1;
	rStatus->column[3].register_busy = NOT_BUSY;

	rStatus->column[4].field = "F4";
	rStatus->column[4].reorder_entry = -1;
	rStatus->column[4].register_busy = NOT_BUSY;

	rStatus->column[5].field = "F5";
	rStatus->column[5].reorder_entry = -1;
	rStatus->column[5].register_busy = NOT_BUSY;

	rStatus->column[6].field = "F6";
	rStatus->column[6].reorder_entry = -1;
	rStatus->column[6].register_busy = NOT_BUSY;

	rStatus->column[7].field = "F7";
	rStatus->column[7].reorder_entry = -1;
	rStatus->column[7].register_busy = NOT_BUSY;

	rStatus->column[8].field = "F8";
	rStatus->column[8].reorder_entry = -1;
	rStatus->column[8].register_busy = NOT_BUSY;

	rStatus->column[9].field = "F9";
	rStatus->column[9].reorder_entry = -1;
	rStatus->column[9].register_busy = NOT_BUSY;

	rStatus->column[10].field = "F10";
	rStatus->column[10].reorder_entry = -1;
	rStatus->column[10].register_busy = NOT_BUSY;


	return rStatus;
}

//comentário

int findPosRegister(char* field, Register_status* register_status){
	int pos = -1;

	for(int i = 0; i < MAX_REGISTERS_FP; i++){
		if(strcmp(field, register_status->column[i].field) == 0){
			pos = i;
			i = MAX_REGISTERS_FP;
		}
	}

	return pos;
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

int insertRegisterStatus(char* fieldRegister, int entryReorderBuffer, Register_status* register_status){
	int posRegisterStatus = -1;

	if(entryReorderBuffer == -1){
		printf("** Error: Invalid position reorder buffer!**\n");
	
	}else{
		posRegisterStatus = findPosRegister(fieldRegister, register_status);
		if(posRegisterStatus == -1){
			printf("** Error: Not found register field!**\n");
		
		}else{
			register_status->column[posRegisterStatus].reorder_entry = entryReorderBuffer;
			register_status->column[posRegisterStatus].register_busy = BUSY;
		}
	}
	return posRegisterStatus;
}	
		

/**
 * @brief Prints the Reservation Station
 * 
 * @param reservationStation Reservation station to be printed
 *
 */
void printRegisterStatus(Register_status* register_status){
	printf("\n\n\nPrinting Register Status. . .\n");

	for(int i = 0; i < MAX_REGISTERS_FP; i++){
		printf("Field: %s\n", register_status->column[i].field);
		printf("Reorder#: %d\n", register_status->column[i].reorder_entry);
		printf("Busy: %s\n\n", ( register_status->column[i].register_busy == NOT_BUSY ) ? "No\0" : "Yes\0");
		
	}
}