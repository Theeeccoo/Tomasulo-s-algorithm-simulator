#include "reorder_buffer.h"
#include "instructions.h"
#include "register_status.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Initializing Register Status
 *
 * @details Instantiating a new Register Status
 *
 * @return Reference to the newly created Register Status
 */

Register_status* registerStatusInitializer(){
	Register_status* rStatus = (Register_status*) malloc( sizeof(Register_status) );
	
	for(int i = 0; i < MAX_REGISTERS; i++){
		char* f = (char*) malloc(4*sizeof(char));
		char* F = (char*) malloc(4*sizeof(char));
		F[0] = 'F';
		sprintf(f, "%d", i);
		printf("%s\n", f);
		strcat(F, f);

		rStatus->column[i].field = F;
		rStatus->column[i].reorder_entry = -1;
		rStatus->column[i].register_busy = NOT_BUSY;
	}

	

	// rStatus->column[1].field = "F1";
	// rStatus->column[1].reorder_entry = -1;
	// rStatus->column[1].register_busy = NOT_BUSY;

	// rStatus->column[2].field = "F2";
	// rStatus->column[2].reorder_entry = -1;
	// rStatus->column[2].register_busy = NOT_BUSY;

	// rStatus->column[3].field = "F3";
	// rStatus->column[3].reorder_entry = -1;
	// rStatus->column[3].register_busy = NOT_BUSY;

	// rStatus->column[4].field = "F4";
	// rStatus->column[4].reorder_entry = -1;
	// rStatus->column[4].register_busy = NOT_BUSY;

	// rStatus->column[5].field = "F5";
	// rStatus->column[5].reorder_entry = -1;
	// rStatus->column[5].register_busy = NOT_BUSY;

	// rStatus->column[6].field = "F6";
	// rStatus->column[6].reorder_entry = -1;
	// rStatus->column[6].register_busy = NOT_BUSY;

	// rStatus->column[7].field = "F7";
	// rStatus->column[7].reorder_entry = -1;
	// rStatus->column[7].register_busy = NOT_BUSY;

	// rStatus->column[8].field = "F8";
	// rStatus->column[8].reorder_entry = -1;
	// rStatus->column[8].register_busy = NOT_BUSY;

	// rStatus->column[9].field = "F9";
	// rStatus->column[9].reorder_entry = -1;
	// rStatus->column[9].register_busy = NOT_BUSY;

	// rStatus->column[10].field = "F10";
	// rStatus->column[10].reorder_entry = -1;
	// rStatus->column[10].register_busy = NOT_BUSY;


	return rStatus;
}

/**
 * @brief Finding the position of the register in the Register Status
 *
 * @return The position of the register
 */

int findPosRegister(char* field, Register_status* register_status){
	int pos = -1;

	for(int i = 0; i < MAX_REGISTERS; i++){
		if(strcmp(field, register_status->column[i].field) == 0){
			pos = i;
			i = MAX_REGISTERS;
		}
	}

	return pos;
}

/**
 * @brief Inserting the information of the register on the table
 *
 * @param fieldRegister is the register field
 * @param entryReorderBuffer the entry of the Reorder Buffer
 * @param register_status is the Register Status to be used
 *
 * @details First, check if the entry of the reorder buffer and the field of the register is valid.
 *  Cheking if the register is busy.
 *
 * @return the position in the Register Status that was inserted the information in register
 */

int insertRegisterStatus(char* fieldRegister, int entryReorderBuffer, Register_status* register_status){
	int posRegisterStatus = -1;

	if(entryReorderBuffer == -1){
		printf("** Error: Invalid position reorder buffer: %d!**\n", entryReorderBuffer);
	
	}else{
		posRegisterStatus = findPosRegister(fieldRegister, register_status);
		if(posRegisterStatus == -1){
			printf("** Error: Not found register field: %s!**\n", fieldRegister);
		
		}else{
			if(register_status->column[posRegisterStatus].register_busy == BUSY){
				printf("** Error: The register is busy: %s!**\n", fieldRegister);

			}else{
				register_status->column[posRegisterStatus].reorder_entry = entryReorderBuffer;
				register_status->column[posRegisterStatus].register_busy = BUSY;
			}
		}
	}
	return posRegisterStatus;
}	
		

/**
 * @brief Prints the Register Status
 * 
 * @param register_status Register Status to be printed
 *
 */
void printRegisterStatus(Register_status* register_status){
	printf("\n\n\nPrinting Register Status. . .\n");

	for(int i = 0; i < MAX_REGISTERS; i++){
		printf("Field: %s\n", register_status->column[i].field);
		printf("Reorder#: %d\n", register_status->column[i].reorder_entry);
		printf("Busy: %s\n\n", ( register_status->column[i].register_busy == NOT_BUSY ) ? "No\0" : "Yes\0");
		
	}
}

/**
 * @brief Freeing a register
 *
 * @param fieldRegister Register field to be released
 * @param register_status Table containing the register
 *
 * @details Free the register for the next instruction
 *
 * @return If register found, its position in the table will be returned, otherwise -1
 */
int freeRegister(char * fieldRegister, Register_status* register_status) {
	int posRegisterStatus = findPosRegister(fieldRegister, register_status);

	if (posRegisterStatus == -1) {
		printf("** Error: Not found register field: %s!**\n", fieldRegister);
	} else {
		register_status->column[posRegisterStatus].reorder_entry = -1;
		register_status->column[posRegisterStatus].register_busy = NOT_BUSY;
	}

	return posRegisterStatus;
}