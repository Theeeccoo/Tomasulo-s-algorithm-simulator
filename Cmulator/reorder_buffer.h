  /***** =========== ****/
 /***** DEFINITIONS ****/
/***** =========== ****/

/**
 * @file Cmulator/reordered_buffer.h
 *
 * @brief Reorderer buffer management 
 */
#ifndef REORDER_BUFFER_H_
#define REORDER_BUFFER_H_

#include <stdio.h>
#include <stdlib.h>
#include "instructions.h"

#define BOOL	 	int
#define TRUE	 	1
#define FALSE	 	0 
#define SIZE_STR	30 

#ifndef _REORDER_BUFFER_LINE_

		/**
		 * @name Information about instruction execution
		 */
		/**@{*/
		#define	NOT_BUSY	FALSE	/**< Instruction is not being executed yet */
		#define BUSY		TRUE	/**< Instruction is being executed */
		/**@}*/

		/**
		 * @name Instruction states
		 */
		/**@{*/
		#define WAITING		0	/**< Instruction is waiting to be inserted into a unity */
		#define EXECUTING	1	/**< Instruction is being executed by a unity */
		#define WRITE_RESULT	2	/**< Instruction is heading to the reorder buffer */
		#define	COMMITED	3	/**< Instruction is done */
		/**@}*/
		

		/**
		 * @brief Line of a Reorder Buffer
		 */
		typedef struct Reoder_Buffer_Line {
			/**
			 * @name Instruction status
			 */
			/**@{*/
			BOOL instruction_execution;	/**< Instruction is being executed? */
			int instruction_state;		/**< Which 'pipeline' stage the instruction is */
			/**@}*/
			
			/**
			 * @name Instruction data
			 */
			/**@{*/
			char* instruction;		/**< Instruction itself */
			char* instruction_result;	/**< Result of executing the instruction */
			char* instruction_destination;  /**< Which register the instruction result should go */
			/**@}*/
		} Reorder_Buffer_Line;

#endif /* _REORDER_BUFFER_LINE_ */

	/**
	 * @name Reorder Buffer line controller
	 */
	/**@{*/
	#define MAX_LINES	 4	/* Maximum ammount of possible lines in reorder buffer*/
	/**@}*/

	typedef struct Reorder_Buffer {
		int filled_lines;			/**< How many lines are filled with instructions*/
		Reorder_Buffer_Line line[MAX_LINES];	/**< Lines of our reorder buffer */
	} Reorder_Buffer;

#endif /* REORDER_BUFFER_H_ */

/** 
 * @name Reorder buffer operations
 */
/**@{*/
extern Reorder_Buffer* reorderBufferInitializer();
extern int insertInstruction(Instruction*, Reorder_Buffer*);
extern void printReorderBuffer(Reorder_Buffer*);
extern Reorder_Buffer* freesReorderBuffer(Reorder_Buffer*);
/**@}*/
