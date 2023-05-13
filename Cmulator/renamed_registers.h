 /***** =========== ****/
 /***** DEFINITIONS ****/
/***** =========== ****/

/**
 * @file Cmulator/renamed_registers.h
 *
 * @brief rename registers management 
 */

#ifndef RENAMED_REGISTERS_H_
#define RENAMED_REGISTERS_H_

#include <stdio.h>
#include <stdlib.h>

#define REGISTERS 32 // total number of registers
#define RENAMED_REGISTERS 64 // number of renamed registers


#ifndef RENAMED_REGISTERS_

        /**
         * @brief represents a physical register of the processor
         *
         * @param busy register is busy
         * @param value register current value
         * @param renaming corresponding renamed register (-1 if not renamed)
         *
         * @details with a value and a renaming field that indicates whether 
         * this register is being renamed by an instruction
         */

        typedef struct {
            int renaming; 
            int value; 
            int busy; 
        } Register;

        /**
         * @brief represents a renamed register
         *
         * @param busy register is busy
         * @param value register current value
         * @param physical_register corresponding physical register (-1 if no mapping)
         */

        typedef struct {
            int physical_register; 
            int busy; 
            int value; 
        } Renamed_Register;

        /**
         * @brief represents the renaming bank itself
         *
         * @details which contains a list of physical registers, a list of renamed 
         * registers and a vector that maps each renamed register to its respective physical register.
         */

        typedef struct {
            Register registers[REGISTERS]; // vector of physical registers
            Renamed_Register renamed_registers[RENAMED_REGISTERS]; // vector of renamed registers
            int renamed_to_physical[RENAMED_REGISTERS]; // vector that maps renamed to physical registers
        } Renaming_Bank;

#endif /* RENAMED_REGISTERS_H_ */

