#pragma once

#include <cstdint>
#include <random>

const unsigned int KEY_COUNT = 16;
const unsigned int MEMORY_SIZE = 4096;
const unsigned int REGISTER_COUNT = 16;
const unsigned int STACK_LEVELS = 16;
const unsigned int VIDEO_HEIGHT = 32;
const unsigned int VIDEO_WIDTH = 64;


class Chip8
{
	public:
		Chip8();
		void LoadROM(char const* filename);
		void Cycle();
		void Decode(int opcode);
		uint8_t keypad[KEY_COUNT]{};
		uint8_t video[VIDEO_HEIGHT][VIDEO_WIDTH / 8]{};


	private:


	//Doesn't do anything	
	void OP_NULL();

	//Clears screen
	void OP_00E0();

	//Return from subroutine
	void OP_00EE();

	//Jumps to address NNN
	void OP_1nnn();
	
	//Calls subroutine at NNN
	void OP_2nnn();

	//Skips next instruction if Vx == NN
	void OP_3xnn();

	//Skips next instruction if Vx != NN
	void OP_4xnn();

	//Skips next instruction if Vx == Vy
	void OP_5xy0();
	
	//Sets Vx to nn
	void OP_6xnn();

	//Adds nn to Vx
	void OP_7xnn();

	//Sets Vx to value of Vy
	void OP_8xy0();
	
	//Sets Vx to Vx or Vy
	void OP_8xy1();

	//Sets Vx to Vx and Vy
	void OP_8xy2();

	//Sets Vx to Vx XOR Vy
	void OP_8xy3();

	//Adds Vy to Vx
	void OP_8xy4();
	
	//Vy is subracted from Vx
	void OP_8xy5();

	//Shifts Vx to the right by 1 and etc
	void OP_8xy6();

	//Sets Vx to Vy minuss Vx
	void OP_8xy7();

	//Shifts Vx to the left by 1 and etc
	void OP_8xyE();

	//Skips the next instruction if Vx != Vy
	void OP_9xy0();
	
	//Sets I to the address NNN
	void OP_Annn();

	//Jumps to location nnn + V0
	void OP_Bnnn();

	//Sets Vx to the result of a bitwise and operation on a random number
	void OP_Cxnn();

	//Draws a sprite at coordinate Vx, Vy with a width of 8 pixels and a height of N pixels
	void OP_Dxyn();

	//Skips the next instruction if the key stored in Vx is pressed
	void OP_Ex9E();

	//Skips the next instruction if the key stored in Vx is not pressed
	void OP_ExA1();

	//Sets Vx to the value of the delay timer
	void OP_Fx07();

	//A key press is awaited, and then stored in Vx
	void OP_Fx0A();

	//Sets the delay timer to Vx
	void OP_Fx15();

	//Sets the sound timer to Vx
	void OP_Fx18();

	//Adds Vx to I
	void OP_Fx1E();

	//Sets I to the location of the sprite for the character in Vx
	void OP_Fx29();
	
	//Stores the binary-coded decimal representation of Vx in memory locations I, I+1 and I+2
	void OP_Fx33();

	//Store registers V0 ti Vx in memory starting at location I
	void OP_Fx55();

	//Read registers V0 through Vx from starting at location I
	void OP_Fx65();


	uint8_t memory[MEMORY_SIZE]{};
	uint8_t registers[REGISTER_COUNT]{};
	uint16_t index{};
	uint16_t pc{};
	uint8_t delayTimer{};
	uint8_t soundTimer{};
	uint16_t stack[STACK_LEVELS]{};
	uint8_t sp{};
	uint16_t opcode{};


	std::default_random_engine randGen;
	std::uniform_int_distribution<uint8_t> randByte;




};
