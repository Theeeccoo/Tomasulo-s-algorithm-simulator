#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reorder_buffer.h"
#include "instructions.h"
#include "reservation_station.h"

void initializeReservationStation(Reservation_Station *rs) {
    int i;
    for (i = 0; i < MAX_LINES_RS; i++) {
        rs->line[i].reservation_busy = FALSE;
        rs->line[i].name = NULL;
        rs->line[i].type = -1;
        rs->line[i].instruction_op = NULL;
        rs->line[i].value_register_read_Vj = NULL;
        rs->line[i].value_register_read_Vk = NULL;
        rs->line[i].information_dependency_Qj = -1;
        rs->line[i].information_dependency_Qk = -1;
        rs->line[i].position_destination_rb = -1;
        rs->line[i].memory_address = NULL;
    }
}

int findFreeReservationStationLine(Reservation_Station *rs) {
    int i;
    for (i = 0; i < MAX_LINES_RS; i++) {
        if (!rs->line[i].reservation_busy) {
            return i;
        }
    }
    return -1; // No free line available
}

void updateReservationStationDependencies(Reservation_Station *rs, int position, Reorder_Buffer *rb) {
    int i;
    for (i = 0; i < MAX_LINES_RS; i++) {
        if (rs->line[i].reservation_busy &&
            (rs->line[i].information_dependency_Qj == position || rs->line[i].information_dependency_Qk == position)) {
            if (rs->line[i].information_dependency_Qj == position) {
                rs->line[i].value_register_read_Vj = rb->entry[position].result;
                rs->line[i].information_dependency_Qj = -1;
            }
            if (rs->line[i].information_dependency_Qk == position) {
                rs->line[i].value_register_read_Vk = rb->entry[position].result;
                rs->line[i].information_dependency_Qk = -1;
            }
        }
    }
}

void checkReservationStationDependencies(Reservation_Station *rs, Reorder_Buffer *rb) {
    int i;
    for (i = 0; i < MAX_LINES_RS; i++) {
        if (rs->line[i].reservation_busy &&
            (rs->line[i].information_dependency_Qj != -1 || rs->line[i].information_dependency_Qk != -1)) {
            int dep_Qj = rs->line[i].information_dependency_Qj;
            int dep_Qk = rs->line[i].information_dependency_Qk;

            if (dep_Qj == -1 && dep_Qk != -1 && rb->entry[dep_Qk].state == STATE_COMMITTED) {
                rs->line[i].value_register_read_Vk = rb->entry[dep_Qk].result;
                rs->line[i].information_dependency_Qk = -1;
            }

            if (dep_Qj != -1 && dep_Qk == -1 && rb->entry[dep_Qj].state == STATE_COMMITTED) {
                rs->line[i].value_register_read_Vj = rb->entry[dep_Qj].result;
                rs->line[i].information_dependency_Qj = -1;
            }

            if (dep_Qj != -1 && dep_Qk != -1 && rb->entry[dep_Qj].state == STATE_COMMITTED && rb->entry[dep_Qk].state == STATE_COMMITTED) {
                rs->line[i].value_register_read_Vj = rb->entry[dep_Qj].result;
                rs->line[i].information_dependency_Qj = -1;
                rs->line[i].value_register_read_Vk = rb->entry[dep_Qk].result;
                rs->line[i].information_dependency_Qk = -1;
            }
        }
    }
}

void executeReservationStation(Reservation_Station *rs, Reorder_Buffer *rb) {
    int i;
    for (i = 0; i < MAX_LINES_RS; i++) {
        if (rs->line[i].reservation_busy &&
            rs->line[i].information_dependency_Qj == -1 &&
            rs->line[i].information_dependency_Qk == -1) {
            // Execute the instruction
            int result = executeInstruction(rs->line[i].instruction_op,
                                            rs->line[i].value_register_read_Vj,
                                            rs->line[i].value_register_read_Vk);

            // Update the result and state in the reorder buffer
            strcpy(rb->entry[rs->line[i].position_destination_rb].result, result);
            rb->entry[rs->line[i].position_destination_rb].state = STATE_COMMITTED;

            // Update the reservation station
            rs->line[i].reservation_busy = FALSE;
            rs->line[i].name = NULL;
            rs->line[i].type = -1;
            rs->line[i].instruction_op = NULL;
            rs->line[i].value_register_read_Vj = NULL;
            rs->line[i].value_register_read_Vk = NULL;
            rs->line[i].information_dependency_Qj = -1;
            rs->line[i].information_dependency_Qk = -1;
            rs->line[i].position_destination_rb = -1;
            rs->line[i].memory_address = NULL;
        }
    }
}


void initializer(char* filename){
    Reorder_Buffer *rb = reorderBufferInitializer();
    Instruction *instructions = instructionsInitializer(filename);
    Reservation_Station *rs = reservationStationInitializer();
    
    printInstructions(instructions, filename);

    int i;
    for (i = 0; i < 2; i++) {
        int rs_line = findFreeReservationStationLine(rs);
        if (rs_line != -1) {
            insertInstructionRB(&instructions[i], rb);
            instructions[i].position_destination_rb = rb->tail;
            insertInstructionRS(&instructions[i], rs, rs_line);
            updateReservationStationDependencies(rs, rs_line, rb);
        } else {
            printf("Reservation Station is full!\n");
            break;
        }
    }

    printReorderBuffer(rb);
    printReservationStation(rs);

    // Execution of instructions out of order
    int executed_instructions = 0;
    while (executed_instructions < 2) {
        checkReservationStationDependencies(rs, rb);
        executeReservationStation(rs, rb);
        executed_instructions++;
    }

    printReorderBuffer(rb);
    printReservationStation(rs);

    free(instructions);
    freesReorderBuffer(rb);
    freesReservationStation(rs);
}

int main(void){
    initializer("input.txt");
    return 0;
}
