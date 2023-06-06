  /***** =========== ****/
 /***** DEFINITIONS ****/
/***** =========== ****/

/**
 * @file Cmulator/reservation_station.h
 *
 * @brief register_status management 
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

#ifndef REGISTER_STATUS_

		/**
		 * @name Information about register_status
		 */
		/**@{*/
		#define	NOT_BUSY	FALSE	/**< register_status is not with instruction */
		#define BUSY		TRUE	/**< register_status is with instruction */
		/**@}*/

		/**
		 * @brief Column of a register_status
		 */
		typedef struct Register_status_column {
			/**
			 * @name register_status status
			 */
			/**@{*/
			BOOL register_busy;	/**< register_status is with instruction? */
			/**@}*/
			
			/**
			 * @name FP register status data
			 */
			/**@{*/
			char* field;     /**< Name of register_status*/
            int reorder_entry;

			/**@}*/
		} Register_status_column;

#endif /* REGISTER_STATUS_ */

	/**
	 * @name register_status column controller
	 */
	/**@{*/
	#define MAX_REGISTERS_FP	 11	/* Maximum ammount of possible registers in FP*/
	/**@}*/

	typedef struct Register_status {
		Register_status_column column[MAX_REGISTERS_FP];	/**< column of our register_status */
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
/**@}*/