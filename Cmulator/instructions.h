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
	#define SIZE_TIME	5

	/**
	 * @name Instruction types
	 */
	/**@{*/
	#define LOAD		0	/**< Operating unit load */
	#define ADD     	1	/**< Operating unit add */
	#define MULT	   	2	/**< Operating unit mult */
	#define BRANCH	   	3	/**< Operating unit branch */
	#define COMPARISON  4	/**< Operating unit comparison */
	#define LOGICAL	   	5	/**< Operating unit logical */
	/**@}*/

	typedef struct Instruction {
		/**
		 * @name Instruction data;
		 */
		/**@{*/
		int		reorder_buffer_position;	/**< Position where this instruction was inserted reorder buffer */
		int 	type;		 				/**< Type of operating unit*/
		int		issued; 					/**< Instruction issued? */
		char*	full_instruction;			/**< Full instruction */
		char**	splitted_instruction;		/**< Instruction splitted */
		float*	time;						/**< Time when instruction was sent to reorder buffer, reservation station, executed and committed */
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
extern Instruction* cloneInstruction (Instruction* instruction_origin);
extern int findNumberOfJumps( Instruction*, int, char*, int );
extern void printInstructions(Instruction*, char*);
extern void insertTime (char* full_instruction, Instruction *instructions, int position, int amount_instructions, float seconds);
extern void printTimeInstructions(Instruction *instructions, int amount_instructions);
/**@}*/
