# Instructions
At the moment, in our simulation we support 17 RISC-V-like instructions, and we divide them as follows:
<ol>
    <li> MEMORY:
        <ol> 
            <li> SW REGISTER, Offset(REGISTER) </li>
            <li> LW REGISTER, Offset(REGISTER) </li>
        </ol>
    </li>
    <li> ADD:
        <ol> 
            <li> ADD DESTINATION, REGISTER, REGISTER </li>
            <li> SUB DESTINATION, REGISTER, REGISTER </li>
        </ol>
    </li>
    <li> MULT:
        <ol> 
            <li> MUL DESTINATION, REGISTER, REGISTER </li>
            <li> DIV DESTINATION, REGISTER, REGISTER </li>
            <li> REM DESTINATION, REGISTER, REGISTER </li>
        </ol>
    </li>
    <li> BRANCH:
        <ol> 
            <li> BNE, REGISTER, REGISTER, LABEL, #VALUE </li>
            <li> BEQ, REGISTER, REGISTER, LABEL, #VALUE </li>
        </ol>
        In this instructions, "LABEL" means where the branch should go. For safety reasons, our simulator only allows labels with length equals to 3. #VALUE can have only two values, 0 or 1. If 0, means that it WON'T branch, if 1, will BRANCH.
    </li>
        <li> COMPARISON:
        <ol> 
            <li> SLT, REGISTER, REGISTER </li>
            <li> SLTI, REGISTER, NUMBER </li>
        </ol>
    </li>
    <li> LOGICAL:
        <ol> 
            <li> OR, REGISTER, REGISTER </li>
            <li> AND, REGISTER, REGISTER </li>
            <li> XOR, REGISTER, REGISTER </li>
            <li> SLL, REGISTER, REGISTER </li>
            <li> SRL, REGISTER, REGISTER </li>
            <li> SRA, REGISTER, REGISTER </li>
        </ol>
    </li>
</ol>

# Files
## Examples Folder
Contains examples created by the authors, fell free to use and change them. Remember to change File Path in `simulator.c` (inside main).

## Instructions
Contains the implementation of our Instructions. In `instructions.h` we have multiple global variables that represents the type of our instruction based on examples given in #INSTRUCTIONS. 

## Register_Status
Contains the implementation of our Buffer_of_registers. In `register_status.h` we have a global variable "MAX_REGISTERS", it's the controller of the amount of registers that your simulation has.

## Reorder_Buffer
Contains the implementation of our Reorder_Buffer. In `reorder_buffer.h` we have a global variable "MAX_LINES", it's the controller of the amount of lines that reorder buffer will have.

## Reservation_Station
Contains the implementation of our Reservation_Station. In `reservation_station.h` we have a global variable "MAX_LINES_RS", it's the controller of the amount of lines that reservation station will have. Also, in `reservation_station.c` (reservationStationInitializer) we instantiate the functional unities

## Simulator
Contains the logic behind our simulation.