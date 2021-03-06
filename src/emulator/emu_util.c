/////////////////////////////////////////////////////////////////////////////////////////
//C Group Project - First Year (2016-2017)
//File:emu_util.c
//Group: 6
//Member: Cheung, Ka (klc116), Mang, Hao (hxm16), Cheuk, Ki (kfc216), Chan, Chun (ckc116)
////////////////////////////////////////////////////////////////////////////////////////
#include "emu_decode.h"
#include "emulate.h"

//Helper function for GENERATEMASK
u32 GENERATEMASKHELPER(int pos) {
    if(pos >= 31) {
        return (u32) ~0;
    } else {
        return (u32) (1 << (pos + 1)) - 1;
    }
}

//Generate a mask to extract the bits of position start to position end
u32 GENERATEMASK(int start, int end) {
    return (GENERATEMASKHELPER(end)) - (GENERATEMASKHELPER(start - 1));
}

//Get the bits from position start to position end
u32 GETBITS(u32 input, int start, int end) {
    return (GENERATEMASK(start, end) & input) >> (start);
}

/////////////////////////////////////////////////////////////////////////////////////////
//Checking if the state of the machine satisfy the condition by checking the CPSR flags of the machine
//PRE: State of the machine, the 4 bytes instruction
//POST: Return 1 for true (satisfy condition) or return 0 for false (not satisfy condition)
/////////////////////////////////////////////////////////////////////////////////////////

int satisfyCondition(MACHINE* ARM, u32 condition) {
    int NFlag = GETBITS(ARM->REGISTER[16], 31, 31);
    int ZFlag = GETBITS(ARM->REGISTER[16], 30, 30);
    int VFlag = GETBITS(ARM->REGISTER[16], 28, 28);
    int bool = NOT_EXIST;
    switch(condition) {
        case eq:
            bool = (ZFlag == 1) ? 1 : 0; break;
        case ne:
            bool = (ZFlag == 0) ? 1 : 0; break;
        case ge:
            bool = (NFlag == VFlag) ? 1 : 0; break;
        case lt:
            bool = (NFlag != VFlag) ? 1 : 0; break;
        case gt:
            bool = (ZFlag == 0) && (NFlag == VFlag) ? 1 : 0; break;
        case le:
            bool = (ZFlag == 1) || (NFlag != VFlag) ? 1 : 0; break;
        case al:
            bool = 1; break;
    }
    return bool;
}

/////////////////////////////////////////////////////////////////////////////////////////
//This is the function to print bit of a unsigned 32 bit integer
//PRE: An unsigned 32bit integer
//POST: Print the bitwise representation of the integer
/////////////////////////////////////////////////////////////////////////////////////////

void printBit(u32 x){
    int i;
    u32 mask = (u32) 1 << 31;
    for(i = 0;i < 32;++i){
        printf("%i", (x & mask) != 0);
        if(i % 4 == 3){
            printf(" ");
        }
        x <<= 1;
    }
    printf("\n");
}

/////////////////////////////////////////////////////////////////////////////////////////
//Print all registers of the machine with a specific format
//PRE: State of the Machine (MACHINE pointer)
//POST: Print all the contents in the general registers including PC and CPSR register
/////////////////////////////////////////////////////////////////////////////////////////

void printRegisters(MACHINE* ARM) {
    for(int index = 0; index <= 12; index++) {
        printf("$%-3d: %10d (0x%08x)\n", index, ARM->REGISTER[index], ARM->REGISTER[index]);
    }
    printf("PC  : %10d (0x%08x)\n", ARM->REGISTER[15], ARM->REGISTER[15]);
    printf("CPSR: %10d (0x%08x)\n", ARM->REGISTER[16], ARM->REGISTER[16]);
}

//Initialize a new machine with all the registers and memory initialized as 0
MACHINE* createMachine() {
    MACHINE* ARM = malloc(sizeof(MACHINE));
    ARM->REGISTER[15] = 0;
    ARM->REGISTER[16] = 0;
    for(int index = 0; index <= NUM_OF_GENERAL_REGISTER; index++) {
        ARM->REGISTER[index] = 0;
    }
    for(int index = 0; index <MAX_MEMORY; index++) {
        ARM->MEMORY[index] = 0;
    }
    return ARM;
}

/////////////////////////////////////////////////////////////////////////////////////////
//Print the non-zero memory of the machine with a specific format
//PRE: State of the Machine (MACHINE pointer)
//POST: Print all the contents in the memory that are non-zero
/////////////////////////////////////////////////////////////////////////////////////////

void printMemory(MACHINE* ARM) {

    for(int index = 0; index < MAX_MEMORY; index += 4) {
        u32 instruction = 0;
        for(int byte = 0; byte < 4; byte++) {
            instruction += ARM->MEMORY[index + byte] << ((3 - byte) * 8);
        }
        if(instruction != 0) {
            printf("0x%08x: 0x%08x\n", index, instruction);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
//Print both the registers and the non-zero memory in the machine
//PRE: State of the Machine (MACHINE pointer)
//POST: Print all the contents in the machine including non-zero memory and registers
/////////////////////////////////////////////////////////////////////////////////////////

void printMachineState(MACHINE* ARM) {
    printf("Registers:\n");
    printRegisters(ARM);
    printf("Non-zero memory:\n");
    printMemory(ARM);
}

/////////////////////////////////////////////////////////////////////////////////////////
// Read a 32-bit instruction from the machine at memory address (address).
// In the case of GPIO addresses, an output is produced indicating the functions and
// the same address is returned.
// PRE: State of the Machine (MACHINE pointer) and the memory address
// POST: 4 bytes starting from the address is read from the memory and returned
/////////////////////////////////////////////////////////////////////////////////////////

u32 readMemory(MACHINE* ARM, u32 address) {
    u32 memToRegister = 0;
    switch(address) {
        case 0x20200000:
            printf("One GPIO pin from 0 to 9 has been accessed\n");
            return address;
        case 0x20200004:
            printf("One GPIO pin from 10 to 19 has been accessed\n");
            return address;
        case 0x20200008:
            printf("One GPIO pin from 20 to 29 has been accessed\n");
            return address;
        case 0x2020001c:
            printf("PIN ON\n");
            return address;
        case 0x20200028:
            printf("PIN OFF\n");
            return address;
        default:
            if(address > 0xffff) {
                printf("Error: Out of bounds memory access at address 0x%08x\n", address);
                return 0;
            } else {
                for (int byte = 0; byte < 4; byte++) {
                    memToRegister += ARM->MEMORY[address + byte] << (byte * 8);
                }
                return memToRegister;
            }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
// Load a 32-bit instruction from the machine at memory address (address).
// In the case of GPIO addresses, an output is produced indicating the functions
// PRE: State of the Machine (MACHINE pointer), the instruction and the memory address to
// load the instruction
// POST: The state of the machine is modified with the instruction inserted to the memory array
/////////////////////////////////////////////////////////////////////////////////////////

void writeMemory(MACHINE* ARM, u32 address, u32 content) {
    u32 start = 0, end = 7;
    switch(address) {
        case 0x20200000:
            printf("One GPIO pin from 0 to 9 has been accessed\n");
            break;
        case 0x20200004:
            printf("One GPIO pin from 10 to 19 has been accessed\n");
            break;
        case 0x20200008:
            printf("One GPIO pin from 20 to 29 has been accessed\n");
            break;
        case 0x2020001c:
            printf("PIN ON\n");
            break;
        case 0x20200028:
            printf("PIN OFF\n");
            break;
        default:
            if(address > 0xffff) {
                printf("Error: Out of bounds memory access at address 0x%08x\n", address);
            } else {
                for (int byte = 0; byte < 4; byte++) {
                    ARM->MEMORY[address + byte] = GETBITS(content, start, end);
                    start += 8;
                    end += 8;
                }
            }
    }
}