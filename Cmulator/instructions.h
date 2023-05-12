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


	typedef struct Instruction {
		/**
		 * @name Instruction data;
		 */
		/**@{*/
		int	position;		/**< Position where this instruction was inserted reorder buffer */
		char*	full_instruction;	/**< Full instruction */
		char**	splitted_instruction;	/**< Instruction splitted */
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
/**@}*/
