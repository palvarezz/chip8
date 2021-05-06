#include "chip8.h"

void chip8::initialize(){
   pc =  0x200; //program counter starts at x200
    opcode = 0;
    I = 0;
    sp = 0;
}

void chip8::loadGame(){

}

void chip8::emulateCycle(){
   //Fetch opcode
   opcode =  memory[pc] << 8 |  memory[pc+1];
   uint8_t x = (opcode & 0x0F00) >> 8;
   uint8_t y = (opcode & 0x00F0)>> 4; 
   uint8_t nn = (opcode & 0x00FF); 
   uint8_t lsb = V[x] & 0x01;
   uint16_t address = opcode & 0x0FFF;
   uint8_t msb = V[x]>>8;
   //Decode
   switch (opcode & 0xF000)
   {
    case 0xA000: // ANNN sets  I to the address  NNN 
       //execute opcode
       I = opcode  & 0x0FFF; 
       pc += 2;
       break;
   
   default:
       break;
   }
}