  /***** =========== ****/
 /***** DEFINITIONS ****/
/***** =========== ****/

/**
 * @file Cmulator/instructions.h
 *
 * @brief Struct of a instruction
 */
#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <stdio.h>
#include <stdlib.h>

	#define SIZE_STR	30 

	/**
	 * @name Instruction types
	 */
	/**@{*/
	#define LOAD		0	/**< Operating unit load */
	#define ADD     	1	/**< Operating unit add */
	#define MULT	   	2	/**< Operating unit mult */
	/**@}*/

	typedef struct Instruction {
		/**
		 * @name Instruction data;
		 */
		/**@{*/
		int	reorder_buffer_position;	/**< Position where this instruction was inserted reorder buffer */
		int 	type;		 		/**< Type of operating unit*/
		char*	full_instruction;		/**< Full instruction */
		char**	splitted_instruction;		/**< Instruction splitted */
		/**@}*/
	} Instruction;

#endif /* INSTRUCTION_H_ */

/** 
 * @name Reorder buffer operations
 */
/**@{*/
extern Instruction* instructionsInitializer(char*);
extern char** readInstructions(char*, int);
extern int numberOfLines(char*);
extern char** splitInstruction(char*);
extern void printInstructions(Instruction*, char*);
/**@}*/
