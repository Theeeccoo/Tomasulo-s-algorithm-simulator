#include "renamed_registers.h"
#include <stdio.h>
#include <stdlib.h>

/**
         * @brief initializes all registers in the renaming bank
         *
         * @details initializes registers with default values, that is,  
         * with a value of zero and without being renamed
 */

void initBank(Renaming_Bank *rb) {
    int i;
    for (i = 0; i < REGISTERS; i++) {
        rb->registers[i].value = 0;
        rb->registers[i].renaming = -1;
    }
    for (i = 0; i < RENAMED_REGISTERS; i++) {
        rb->renamed_registers[i].physical_register = -1;
        rb->renamed_registers[i].busy = 0;
        rb->renamed_registers[i].value = 0;
        rb->renamed_to_physical[i] = -1;
    }
}

/**
         * @brief searches for a free renamed register in the rename bank
         * @param index of renamed register
         * @return returns its index
 */

int getFreeRenamedRegister(Renaming_Bank *rb){
    for (int i = REGISTERS; i < RENAMED_REGISTERS; i++) {
        if (!rb->renamed_registers[i].busy) {
            return i;
        }
    }
    return -1;
}

/**
         * @brief renames a physical register of the processor,
         *
         * @param renamed_register get a free register
         * 
         * @details creating a new renamed register in the rename bank and updating 
         * the mapping between the physical register and the renamed one
         * 
         * @return the index of the renamed register, or -1 if there are no renamed registers available.
 */

int renameRegister(Renaming_Bank *rb, int physical_register){
    int renamed_register = getFreeRenamedRegister(rb);
    if (renamed_register == -1) {
        return -1; // no renamed registers available
    }
    rb->renamed_registers[renamed_register].busy = 1;
    rb->renamed_registers[renamed_register].value = rb->registers[physical_register].value;
    rb->renamed_registers[renamed_register].physical_register = physical_register;
    rb->registers[physical_register].renaming = renamed_register;
    rb->renamed_to_physical[renamed_register] = physical_register; // update mapping
    return renamed_register;
}

/**
         * @brief writes back a register
         *
         * @details atualizando o valor do registrador renomeado correspondente, 
         * caso exista, e liberando os registradores físico e renomeado
         *
         * @return the freed physical register index.
 */

int writeBack(Renaming_Bank *rb, int physical_register, int value){
    if (rb->registers[physical_register].renaming != -1) {
        int renamed_register = rb->registers[physical_register].renaming;
        int renamed_physical_register = rb->renamed_to_physical[renamed_register];
        rb->renamed_registers[renamed_register].value = value;
        rb->registers[physical_register].busy = 0;
        rb->renamed_registers[renamed_register].busy = 0;
        rb->registers[physical_register].renaming = -1;
        rb->renamed_to_physical[renamed_register] = -1; // remove mapping
        return renamed_physical_register;
    } else {
        rb->registers[physical_register].value = value;
        rb->registers[physical_register].busy = 0;
        return physical_register;
    }
}


// int main(){
//     Renaming_Bank rb;
//     initBank(&rb); // inicializa os registradores do banco de renomeação
//     // Teste simples
//     int reg1 = 0;
//     int reg2 = 1;
//     int reg3 = 2;
//     int reg4 = 3;

//     int rreg1 = renameRegister(&rb, reg1);
//     int rreg2 = renameRegister(&rb, reg2);
//     int rreg3 = renameRegister(&rb, reg3);
//     int rreg4 = renameRegister(&rb, reg4);

//     printf("Registradores renomeados:\n");
//     printf("r%d -> p%d\n", rreg1, rb.renamed_to_physical[rreg1]);
//     printf("r%d -> p%d\n", rreg2, rb.renamed_to_physical[rreg2]);
//     printf("r%d -> p%d\n", rreg3, rb.renamed_to_physical[rreg3]);
//     printf("r%d -> p%d\n", rreg4, rb.renamed_to_physical[rreg4]);

//     int result = 42;
//     int rp1 = writeBack(&rb, reg1, result);
//     printf("Registrador físico atualizado: p%d = %d\n", rp1, rb.registers[rp1].value);
//     printf("Registrador renomeado atualizado: r%d = %d\n", rreg1, rb.renamed_registers[rreg1].value);

//     return 0;
// }