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
   //Decode
   switch (opcode & 0xF000)
   {
   case 0xA000: // ANNN sets  I to the address  NNN 
       //execute opcode
       I = opcode  & 0x0FFF; 
       pc += 2;
       break;
   case 0x1000:
        pc = opcode & 0x0FFF;
   case 0x2000: //2NNN call subroutine at NNN
        stack[sp] = pc;
        sp++;
        pc = opcode & 0x0FFF;
        break;
   case 0x3000: //3XNN  skips next instruction if  VX equals NN
        uint8_t nn = (opcode & 0x00FF); 
        pc += (V[x] == nn) ? 4 : 2;
        break;
   case 0x4000: //4XNN skips if VX not equal to NN
        uint8_t nn = (opcode & 0x00FF); 
        pc += (V[x] != nn) ? 4 : 2;
        break;
   case 0x5000: // 0x5XYO skips if VX equals VY
        
        pc += (V[x] == V[y]) ? 4 : 2;
        break;
   case 0x6000: // 6XNN Sets VX to NN. 
        uint8_t nn = (opcode & 0x00FF);
        V[x] = nn;
        pc += 2; 
        break;
   case 0x7000: // 0x7XNN adds nn to vx
        uint8_t nn = (opcode & 0x00FF);
        V[x] += nn;
        pc += 2;
        break;
   case 0x8000:
        switch (0x000F & opcode)
        {
        case 0x0000: // 8XY0 set vx value to vy
            V[x] = V[y];
            pc += 2;
            break;
        case 0x0001: // 8XY1 Sets VX to VX or VY. (Bitwise OR operation) 
            V[x] = V[x] | V[y];
            pc += 2;
            break;
        case 0x0002:  // 8XY2	Sets VX to VX and VY. (Bitwise AND operation) 
            V[x] = V[x] & V[y];
            pc += 2;
            break;
        case 0x0003: // 8XY3 Sets VX to VX xor VY. 
            V[x] = V[x] ^ V[y];
            pc += 2;
            break;
        case 0x0004: // 8XY4 set vx value to vy
            V[x] = V[y];
            pc += 2;
            break;
        case 0x0005: // 8XY5 set vx value to vy
            V[x] = V[y];
            pc += 2;
            break;
        case 0x0006: // 8XY6 set vx value to vy
            V[x] = V[y];
            pc += 2;
            break;
        case 0x0007: // 8XY7 set vx value to vy
            V[x] = V[y];
            pc += 2;
            break;
        case 0x000E: // 8XYE set vx value to vy
            V[x] = V[y];
            pc += 2;
            break;
        
        
        default:
            break;
        }
   default:
        std::cout<<"Unknown opcode: "<< opcode << "\n";
       break;
   }

}