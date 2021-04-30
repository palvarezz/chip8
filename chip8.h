#include <iostream>   
  class chip8{
      private:
      unsigned short opcode;
      unsigned char memory[4096];
      unsigned char V[16];
      unsigned short I;
      unsigned short pc;
      unsigned char gfx[64 * 32];
      unsigned char delay_timer;
      unsigned char sound_timer;
      unsigned short stack[16];
      unsigned short sp;
      unsigned char key[16];
      public:
      void initialize();
      void loadGame();
      void emulateCycle();
      void drawflag();
      void setKeys();
  };