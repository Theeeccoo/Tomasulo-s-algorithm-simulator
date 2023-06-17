  /***** =========== ****/
 /***** DEFINITIONS ****/
/***** =========== ****/

/**
 * @file Cmulator/register_status.h
 *
 * @brief register status management 
 */

#ifndef REGISTER_STATUS_H_

#define REGISTER_STATUS_H_

#include <stdio.h>
#include <stdlib.h>
#include "instructions.h"
#include "reorder_buffer.h"

#define BOOL	 	int
#define TRUE	 	1
#define FALSE	 	0 
#define SIZE_STR	30 

#ifndef REGISTER_STATUS_COLUMN_

		/**
		 * @name Information about register_status
		 */
		/**@{*/
		#define	NOT_BUSY	FALSE	/**< got from reoder buffer */
		#define BUSY		TRUE	/**< got from reoder buffer */
		/**@}*/

		/**
		 * @brief Column of a register_status
		 */
		typedef struct Register_status_column {
			/**
			 * @name register_status status
			 */
			/**@{*/
			BOOL register_busy;   /**< the information if the register is busy */
			/**@}*/
			
			/**
			 * @name FP register status data
			 */
			/**@{*/
			char* field;     /**< field with the destination of reorder buffer*/
            		int reorder_entry;   /**<Position of the reordering buffer where the register is the destination of the result of the operation*/
			/**@}*/
		} Register_status_column;

#endif /* REGISTER_STATUS_COLUMN_ */

	/**
	 * @name register_status column controller
	 */
	/**@{*/
	#define MAX_REGISTERS	 32	/**<Maximum ammount of possible registers in the table FP register status*/
	/**@}*/

	typedef struct Register_status {
		Register_status_column column[MAX_REGISTERS];	/**< column of our register status */
	} Register_status;

#endif /* REGISTER_STATUS_ */

/** 
 * @name register_status operations
 */
/**@{*/
extern Register_status* registerStatusInitializer();
extern int insertRegisterStatus(char* fieldRegister, int entryReorderBuffer, Register_status* register_status);
extern void printRegisterStatus(Register_status* register_status);
extern int findPosRegister(char* field, Register_status* register_status);
extern int getReorderPosition(char* field, Register_status* register_status);
extern int freeRegister(char * fieldRegister, Register_status* register_status);
extern char* registerNameOrPositionRB (Register_status* register_status, char* name_register, int position_instruction_rb);
extern char* calculateResult(Instruction* instruction, Register_status* register_status);
/**@}*/
