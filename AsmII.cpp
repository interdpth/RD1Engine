#include "AsmII.h"

AsmII::AsmII(unsigned char* data)
{
	printf("ASM engine started");
    memset(_registers, 0, sizeof(unsigned long) * 16);
	_dat = data;
}

/*
THUMB.3: move/compare/add/subtract immediate

  15-13  Must be 001b for this type of instructions
  12-11  Opcode
           00b: MOV{S} Rd,#nn      ;move     Rd   = #nn
           01b: CMP{S} Rd,#nn      ;compare  Void = Rd - #nn
           10b: ADD{S} Rd,#nn      ;add      Rd   = Rd + #nn
           11b: SUB{S} Rd,#nn      ;subtract Rd   = Rd - #nn
  10-8   Rd - Destination Register  (R0..R7)
  7-0    nn - Unsigned Immediate    (0-255)




*/


unsigned char AsmII::ReadMove(unsigned long offset) {
    unsigned short cmd = *(unsigned short*)_dat[offset];
   
    
    unsigned char val = cmd & 0xFF;


    return val;

}



bool AsmII::SetMove(unsigned long offset, unsigned char val) {
    unsigned short cmd = *(unsigned short*)_dat[offset] & 0xFF00  || ( val & 0xFF);
    *(unsigned short*)_dat[offset] = cmd;

    unsigned char val2 = cmd & 0xFF;


    return val2==val;

}
unsigned short AsmII::ReadBigMove(unsigned long offset) 
{
    ReadMove(offset);
    ReadLSL(offset + 2);
    return 0;
}

bool AsmII::SetBigMove(unsigned short val, unsigned long offset) { return false; }

/*
THUMB.1: move shifted register

  15-13  Must be 000b for 'move shifted register' instructions
  12-11  Opcode
           00b: LSL{S} Rd,Rs,#Offset   (logical/arithmetic shift left)
           01b: LSR{S} Rd,Rs,#Offset   (logical    shift right)
           10b: ASR{S} Rd,Rs,#Offset   (arithmetic shift right)
           11b: Reserved (used for add/subtract instructions)
  10-6   Offset                     (0-31)
  5-3    Rs - Source register       (R0..R7)
  2-0    Rd - Destination register  (R0..R7)

*/
vector<int> AsmII::ReadLSL(unsigned long offset) 
{
    vector<int> blah;
    unsigned short cmd = 0;
    memcpy(&cmd, &_dat[offset], 2);
    return blah;
}


bool AsmII::SetLSL(int shift, int val, unsigned long offset) { return false; }