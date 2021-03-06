/////////////////////////////////////////////////////////////////////////////////////////
//C Group Project - First Year (2016-2017)
//File:emu_branch.c
//Group: 6
//Member: Cheung, Ka (klc116), Mang, Hao (hxm16), Cheuk, Ki (kfc216), Chan, Chun (ckc116)
/////////////////////////////////////////////////////////////////////////////////////////
#include "emu_decode.h"
#include "emulate.h"

/////////////////////////////////////////////////////////////////////////////////////////
//Execution of the branch instruction
//PRE: Pointer to the machine and the structure of the branch instruction
//POST: Modified the contents of the machine through the pointer
/////////////////////////////////////////////////////////////////////////////////////////
void branch(MACHINE* ARM, BRANCH_INSTR* br) {
    if(satisfyCondition(ARM, br->COND)) {
        u32 offsetMask = 0xffffffu;
        u32 offset  = br->OFFSET;
        //Left shifting the offset by two bits
        offset = LShiftL(offset, 2);
        if(offset & (1 << 23)){
            //offset is negative
            //changing the two's complement integer to an unisgned integer
            offset = (~offset)+1;
            ARM->REGISTER[15] -= (offsetMask & offset);
        }else{
            //offset is positive
            ARM->REGISTER[15] += (offsetMask & offset);
        }

    }
}