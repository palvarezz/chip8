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
        case 0x0004: // 8XY4 adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there is not. 
            V[15] = (((int) V[x] + (int)V[y])>255) ? 1: 0 ;
            V[x] += V[y];
            pc += 2;
            break;
        case 0x0005: // 8XY5 VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there is not. 
            V[15] = (((int) V[x] - (int)V[y])<0) ? 1: 0 ;
            V[x] -= V[y];
            pc += 2;
            break;
        case 0x0006: // 8XY6 Stores the least significant bit of VX in VF and then shifts VX to the right by 1
            uint8_t lsb = V[x] & 0x01;
            V[15] = lsb;
            V[x] = V[x]>>1;
            pc += 2;
            break;
        case 0x0007: // 8XY7 Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not. 
           V[15] = (((int) V[y] - (int)V[x])<0) ? 1: 0 ;
            V[x] = V[y] - V[x];
            pc += 2;
            break;
        case 0x000E: // 8XYE Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
            uint8_t msb = V[x]>>8;
            V[x] = V[x] << 1;
            pc += 2;
            break;
        
        default:
            break;
        }
   case 0x9000:
        pc +=(V[x] != V[y]) ? 4: 2;
        break;
   case 0xB000: //Jumps to the address NNN plus V0. 
        uint16_t address = opcode & 0x0FFF;
        pc = V[0] + address;
        break; 
   case 0xC000: //Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN. 
        V[x] = rand() & (opcode & 0x00FF);
        pc +=2;
        break;
   case 0xD000: // draw sprites at coordinate

        break;
   case 0xE000:
        switch (0x00F0 & opcode)
        {
        case 0x0090:
            
            break;
        case 0x00A0:

            break;
        default:
            break;
        }
   case 0xF000:
        switch (0x00FF & opcode)
        {
        case 0x0007:
            break;
        case 0x000A:
            break;
        case 0x0015:
            break;
        case 0x0018:
            break;
        case 0x001E:
            break;
        case 0x0029:
            break;
        case 0x0033:
            break;
        case 0x0055:
            break;
        default:
            break;
        }
   default:
        std::cout<<"Unknown opcode: "<< opcode << "\n";
       break;
   }

}