#define _CRT_SECURE_NO_WARNINGS
#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <random>
#include <iostream>

unsigned char chip8_fontset[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip8::initialize() {
        pc = 0x200; //program counter starts at x200
        opcode = 0;
        I = 0;
        sp = 0;
        drawflag = 0;
        //clear display
        for (int i = 0; i < 2048; ++i)
            gfx[i] = 0;
        //clear stack
        for (int i = 0; i < 16; i++)
            stack[i] = 0;
        //clear registers
        for (int i = 0; i < 16; i++)
            V[i] = 0; 
        //clear memory
        for (int i = 0; i < 4096; i++)
            memory[i] = 0;
        //clear keyboards
        for (int i = 0; i < 16; i++)
            key[i] = 0;
        //load fontset
        for (unsigned int i = 0; i < 80; i++) {
            memory[i] = chip8_fontset[i];
        }
        //reset timers
        delay_timer = 0;
        sound_timer = 0;

        srand(time(NULL));

    }
    //load rom
void chip8::loadGamec(const char* filename) {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        const unsigned int start_address = 0x200;
        if (file.is_open())
        {
            // Get size of file and allocate a buffer to hold the contents
            std::streampos size = file.tellg();
            char* buffer = new char[size];

            // Go back to the beginning of the file and fill the buffer
            file.seekg(0, std::ios::beg);
            file.read(buffer, size);
            file.close();

            // Load the ROM contents into the Chip8's memory, starting at 0x200
            for (long i = 0; i < size; ++i)
            {
                memory[start_address + i] = buffer[i];
            }

            // Free the buffer
            delete[] buffer;
        }
        else {
            std::cout << "Error opening file";
            exit(0);
        }

    }
    
void chip8::loadGame(std::string path) {
        std::ifstream file;
        file.open(path, std::ios::binary);

        if (!file.is_open())
        {
            std::cerr << "failed to open rom File ";
        }
        file.seekg(0, std::ios::end);
        const int rom_size = file.tellg();
        file.seekg(0, std::ios::beg);

        // if rom fits in memory
        if ((4096 - 512) > rom_size)
        {
            //allocate buffer to store rom

            char* buff = new char[rom_size];
            // read to a buffer
            file.read(buff, rom_size);

            // copy buffer to memory

            for (int i = 0; i < rom_size; ++i)
            {
                memory[i + 512] = buff[i];
            }

            delete []buff;
        }
        else
        {
            std::cerr << "rom is to big to read in memory " << std::endl;
            exit(1);
        }

        file.close();
        
    }
void chip8::loadRom(char const* filename) {
   
    }


void chip8::emulateCycle() {
        //Fetch opcode
        opcode = memory[pc] << 8 | memory[pc + 1];
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;
        uint8_t nn = (opcode & 0x00FF);
        uint8_t lsb = V[x] & 0x01;
        uint16_t address = opcode & 0x0FFF;
        uint8_t msb = V[x] >> 7;
        //Decode
        std::cout <<"opcode is: "<< opcode << std::endl;
        //std::cout <<std::hex << opcode << std::endl;

        switch (opcode & 0xF000)
        {
        case 0xA000: // ANNN sets  I to the address  NNN 
            //execute opcode
            I = opcode & 0x0FFF;
            pc += 2;
            break;
        case 0x1000: // 0x1NNN: Jumps to address NNN
            pc = opcode & 0x0FFF;
            break;
        case 0x2000: //2NNN call subroutine at NNN
            stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFF;
            break;
        case 0x3000: //3XNN  skips next instruction if  VX equals NN
            //pc += (V[x] == nn) ? 4 : 2;
            if (V[x] == nn)
                pc += 4;
            else
                pc += 2;
            break;
        case 0x4000: //4XNN skips if VX not equal to NN
            pc += (V[x] != nn) ? 4 : 2;
            break;
        case 0x5000: // 0x5XYO skips if VX equals VY
            pc += (V[x] == V[y]) ? 4 : 2;
            break;
        case 0x6000: // 6XNN Sets VX to NN. 
            V[x] = nn;
            pc += 2;
            break;
        case 0x7000: // 0x7XNN adds nn to vx
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
            case 0x0004: //0x8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't		 //check again
                V[x] += V[y];
                if(V[y] > (0xFF - V[x])) 
					V[0xF] = 1; //carry
				else 
					V[0xF] = 0;					
				pc += 2;	
                break;
            case 0x0005:  // 0x8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
                V[x] -= V[y];
                if (V[y] > V[x])
                    V[0xF] = 0; // there is a borrow
                else
                    V[0xF] = 1;
                pc += 2;
                break;
            case 0x0006: // 0x8XY6: Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift
                V[0xF] = V[x] & 0x1;
                V[x] >>= 1;
                pc += 2;
                break;
            case 0x0007:// 0x8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
                if (V[y] < V[x])	// VY-VX
                    V[0xF] = 0; // there is a borrow
                else
                    V[0xF] = 1;
                V[x] = V[y] - V[x];
                pc += 2;
                break;
            case 0x000E: // 0x8XYE: Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift
                V[0xF] = V[x] >> 7;
                V[x] <<= 1;
                pc += 2;
                break;
            default:
                break;
            }
            break;
        case 0x9000:
            pc += (V[x] != V[y]) ? 4 : 2;
            break;
        case 0xB000: //Jumps to the address NNN plus V0. 
        {   uint16_t address = opcode & 0x0FFF;
            pc = V[0] + address;
        }   break;
        case 0xC000: //Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN. 
        {   V[x] = (rand() % (0xFF + 1)) & (opcode & 0x00FF);
            pc += 2;
        }   break;
        case 0xD000: // draw sprites at coordinate
        {
            uint8_t height = opcode & 0x000F;
            uint8_t xcord = V[x] % 64; 
            uint8_t ycord = V[y] % 32;
            uint8_t sprite_data;
            unsigned short x = V[(opcode & 0x0F00) >> 8];
            unsigned short y = V[(opcode & 0x00F0) >> 4];
            unsigned short h = opcode & 0x000F;
            unsigned short pixel;
            V[0xF] = 0;
            for (int Yi = 0; Yi < h; Yi++) {
                pixel = memory[I + Yi];
                for (int Xi = 0; Xi < 8; Xi++) {
                    if ((pixel & (0x80 >> Xi)) != 0 ){
                        if (gfx[(x + Xi + ((y + Yi) * 64))] == 1) {
                            V[0xF] = 1;
                        }
                        gfx[x + Xi + ((y + Yi) * 64)] ^= 1;
                    }
                }
            }
            pc += 2;
            drawflag = 1;
        }
            break;
        case 0xE000: //key
            switch (0x00F0 & opcode)
            {
            case 0x0090:
                if (key[V[(opcode & 0x0F00) >> 8]] == 1)
                    pc += 2;
                break;
            case 0x00A0:
                if (key[V[x]] == 0)
                    pc += 2;
                break;
            default:
                break;
            }
            break;
        case 0xF000:
            switch (0x00FF & opcode)
            {
            case 0x0007:
                V[x] = delay_timer;
                pc += 2;
                break;
             //keys
            case 0x000A:{ 
                if (key[0]) {
                    V[x] = 0;
                }
                else if (key[1]) {
                    V[x] = 1;
                }
                else if (key[2]) {
                    V[x] = 2;
                }
                else if (key[3]) {
                    V[x] = 3;
                }
                else if (key[4]) {
                    V[x] = 4;
                }
                else if (key[5]) {
                    V[x] = 5;
                }
                else if (key[6]) {
                    V[x] = 6;
                }
                else if (key[7]) {
                    V[x] = 7;
                }
                else if (key[8]) {
                    V[x] = 8;
                }
                else if (key[9]) {
                    V[x] = 9;
                }
                else if (key[10]) {
                    V[x] = 10;
                }
                else if (key[11]) {
                    V[x] = 11;
                }
                else if (key[12]) {
                    V[x] = 12;
                }
                else if (key[13]) {
                    V[x] = 13;
                }
                else if (key[14]) {
                    V[x] = 14;
                }
                else if (key[15]) {
                    V[x] = 15;
                }
                else {
                    pc += 2;
                }}
                break;
            case 0x0015:
                delay_timer = V[x]; 
                pc += 2;
                break;
            case 0x0018:
                sound_timer = V[x];
                pc += 2;
                break;
            case 0x001E:
                if (I + V[x] > 0xFFF)
                    V[0xF] = 1;
                else
                    V[0xF] = 0;

                I = I + V[x];
                pc += 2;
                break;
            case 0x0029:// FX29: Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
                I = V[x] * 0x5;
                pc += 2;
                break;
            case 0x0033: // FX33: Stores the Binary-coded decimal representation of VX at the addresses I, I plus 1, and I plus 2
                memory[I] = V[x] / 100;
                memory[I + 1] = (V[x] / 10) % 10;
                memory[I + 2] = (V[x]) % 10;
                pc += 2;
                break;
            case 0x0055:
                for (uint8_t i = 0; i <= x; i++) {
                    memory[I + i] = V[i];
                }
                pc += 2;
                break;
            case 0x0065: // FX65: Fills V0 to VX with values from memory starting at address I					
                for (uint8_t i = 0; i <= x; i++) {
                    V[i] = memory[I + i];
                }
                pc += 2;
                break;
            default:
                break;
            }
            break;
        case 0x0000:
            switch (0x00FF & opcode) {
            case 0x00E0:
                //std::fill(gfx,gfx + (64*32) ,0);
                for (int i = 0; i < 2048; ++i)
                    gfx[i] = 0;

                drawflag = 1;
                pc += 2;
                break;
            case 0x00EE:
                --sp;
                pc = stack[sp];
                pc += 2;
                break;
            }
            break;
            //break 0 series
        default:
            std::cout << "Unknown opcode: " << opcode << "\n";
            break;
        }

        // Update timers
        if (delay_timer > 0)
            --delay_timer;

        if (sound_timer > 0)
        {
            if (sound_timer == 1)
                printf("BEEP!\n");
            --sound_timer;
        }
}

void chip8::setKeys(int k,int ud) {
    if (k == 0) {
        key[0] = ud;
    }
    else if (k ==  1) {
        key[k] = ud;
    }
    else if (k == 2) {
        key[k] = ud;
    }
    else if (key[3]) {
        key[k] = ud;
    }
    else if (key[4]) {
        key[k] = ud;
    }
    else if (key[5]) {
        key[k] = ud;
    }
    else if (key[6]) {
        key[k] = ud;
    }
    else if (key[7]) {
        key[k] = ud;
    }
    else if (key[8]) {
        key[k] = ud;
    }
    else if (key[9]) {
        key[k] = ud;
    }
    else if (key[10]) {
        key[k] = ud;
    }
    else if (key[11]) {
        key[k] = ud;
    }
    else if (key[12]) {
        key[k] = ud;
    }
    else if (key[13]) {
        key[k] = ud;
    }
    else if (key[14]) {
        key[k] = ud;
    }
    else if (key[15]) {
        key[k] = ud;
    }
}