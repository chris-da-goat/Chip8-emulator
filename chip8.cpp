#include "chip8.hpp"
#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <random>


const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;
const unsigned int START_ADDRESS = 0x200;


uint8_t fontset[FONTSET_SIZE] =
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

Chip8::Chip8()
	: randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
	
	pc = START_ADDRESS;
	sp = 0;

	for (unsigned int i = 0; i < FONTSET_SIZE; i++){

		memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}

	randByte = std::uniform_int_distribution<uint8_t>(0, 255U);

	



}


void Chip8::LoadROM(char const* filename){
	
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (file.is_open()){
		
		std::streampos size = file.tellg();
		char* buffer = new char[size];

		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		for (long i = 0; i < size; ++i)
		{
			memory[START_ADDRESS + i] = buffer[i];
		}

		delete[] buffer;
	}
}


void Chip8::Cycle(){

	opcode = (memory[pc] << 8u) | memory[pc + 1];
	
	pc += 2;

	
	Decode(opcode);


	if (delayTimer > 0){
		--delayTimer;
	}

	if (soundTimer > 0){
		--soundTimer;
	}

}


void Chip8::Decode(int opcode){
	
	switch ( opcode & 0xF000){

		case 0x0000:
			
			switch (opcode & 0x00FF){
				case 0x00E0:
					OP_00E0();
					break;

				case 0x00EE:
					OP_00EE();
					break;
			}
			break;

		case 0x1000:
			OP_1nnn();
			break;
		
		case 0x2000:
			OP_2nnn();
			break;

		case 0x3000:
			OP_3xnn();
			break;

		case 0x4000:
			OP_4xnn();
			break;

		case 0x5000:
			OP_5xy0();
			break;
		
		case 0x6000:
			OP_6xnn();
			break;
		
		case 0x7000:
			OP_7xnn();
			break;

		case 0x8000:
			
			switch (opcode & 0x000F){
			
				case 0x0000u:
					OP_8xy0();
					break;
				
				case 0x0001u:
					OP_8xy1();
					break;
				
				case 0x0002:
					OP_8xy2();
					break;
				
				case 0x0003:
					OP_8xy3();
					break;

				case 0x0004:
					OP_8xy4();
					break;

				case 0x0005:
					OP_8xy5();
					break;
		
				case 0x0006:
					OP_8xy6();
					break;

				case 0x0007:
					OP_8xy7();
					break;
			
				case 0x000E:
					OP_8xyE();
					break;

			}
			break;
			
		case 0x9000:
			OP_9xy0();
			break;
		
		case 0xA000:
			OP_Annn();
			break;
	
		case 0xB000:
			OP_Bnnn();
			break;

		case 0xC000:
			OP_Cxnn();
			break;
		
		case 0xD000:
			OP_Dxyn();
			break;

		case 0xE000:
			
			switch (opcode & 0x00FF){

				case 0x009E:
					OP_Ex9E();
					break;
		
				case 0x00A1:
					OP_ExA1();
					break;
			}
			break;

		case 0xF000:
		
			switch (opcode & 0x00FF){	

				case 0x0007:
					OP_Fx07();
					break;

				case 0x000A:
					OP_Fx0A();
					break;

				case 0x0015:
					OP_Fx15();
					break;
				
				case 0x0018:
					OP_Fx18();
					break;

				case 0x001E:
					OP_Fx1E();
					break;
	
				case 0x0029:
					OP_Fx29();
					break;

				case 0x0033:
					OP_Fx33();
					break;
			
				case 0x0055:
					OP_Fx55();
					break;
	
				case 0x0065:
					OP_Fx65();
					break; 			

			}
		break;

	default:
		OP_NULL();

	}

}


void Chip8::OP_NULL(){
	
}


void Chip8::OP_00E0(){

	memset(video, 0, sizeof(video));

}


void Chip8::OP_00EE(){

	--sp;
	pc = stack[sp];

}


void Chip8::OP_1nnn(){

	uint16_t address = opcode & 0x0FFFu;
	pc = address;
}


void Chip8::OP_2nnn(){

	uint16_t address = opcode & 0x0FFFu;

	stack[sp] = pc;
	++sp;
	pc = address;

}


void Chip8::OP_3xnn(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (registers[Vx] == byte)
	{
		pc += 2;
	}

}


void Chip8::OP_4xnn(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = (opcode & 0x00FFu);

	if (registers[Vx] != byte){

		pc += 2;
	
	}

}


void Chip8::OP_5xy0(){

        uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] == registers[Vy]){

		pc += 2;

	}

}


void Chip8::OP_6xnn(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = (opcode & 0x00FFu);

	registers[Vx] = byte;

}


void Chip8::OP_7xnn(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = (opcode & 0x00FFu);

	registers[Vx] += byte;

}


void Chip8::OP_8xy0(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] = registers[Vy];

}


void Chip8::OP_8xy1(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] |= registers[Vy];

}


void Chip8::OP_8xy2(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] &= registers[Vy];

}


void Chip8::OP_8xy3(){

        uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] ^= registers[Vy];

}


void Chip8::OP_8xy4(){

        uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	uint16_t sum = registers[Vx] + registers[Vy];

	if (sum > 255U)
	{
		registers[0xF] = 1;
	}
	else
	{
		registers[0xF] = 0;
	}

	registers[Vx] = sum & 0xFFu;

}

void Chip8::OP_8xy5(){

        uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] > registers[Vy]){

		registers[0xF] = 1;
	
	} 
	else {

		registers[0xF] = 0;

	}

	registers[Vx] -= registers[Vy];

}


void Chip8::OP_8xy6(){

        uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        
	registers[0xF] = (registers[Vx] & 0x1u);

	registers[Vx] >>= 1;
	
}

void Chip8::OP_8xy7(){

        uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vy] > registers[Vx]){

		registers[0xF] = 1;

	}
	else {

		registers[0xF] = 0;

	}
	

	registers[Vx] = registers[Vy] - registers[Vx];
}


void Chip8::OP_8xyE(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	registers[0xF] = (registers[Vx] & 0x80u) >> 7u;

	registers[Vx] <<= 1;

}


void Chip8::OP_9xy0(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] != registers[Vy]){

		pc += 2;

	}
}


void Chip8::OP_Annn(){

	uint16_t address = (opcode & 0x0FFFu);

	index = address;

}


void Chip8::OP_Bnnn(){

	uint16_t address = (opcode & 0x0FFFu);

	pc = address + registers[0x0];

}


void Chip8::OP_Cxnn(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = (opcode & 0x00FFu);

	registers[Vx] = randByte(randGen) & byte;
	
}

/*
void Chip8::OP_Dxyn(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t byte = (opcode & 0x000Fu);

	uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
	uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

	registers[0xF] = 0;

	for (unsigned int row = 0; row < byte; row++){

		uint8_t spriteByte = memory[index + row];

		for (unsigned int col = 0; col < 8; col++){

			uint8_t spritePixel = spriteByte & (0x80u >> col);
			uint8_t *screenPixel = &video[yPos + row][(xPos + col) / 8];

			if (spritePixel){
			
				if (*screenPixel == 0xFFFFFFFF){
					
					registers[0xF] = 1;

				} 
			*screenPixel ^= 0xFFFFFFFF;

			}
		}	
	}
}
*/


void Chip8::OP_Dxyn() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    uint8_t height = opcode & 0x000Fu;

    uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
    uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

    registers[0xF] = 0;

    for (unsigned int row = 0; row < height; row++) {
        // Get the sprite byte from memory
        uint8_t spriteByte = memory[index + row];

        // Calculate which row of the video buffer we are drawing to
        uint8_t videoRow = (yPos + row) % VIDEO_HEIGHT;

        for (unsigned int col = 0; col < 8; col++) {
            // Calculate which column of the video buffer we are drawing to
            uint8_t videoCol = (xPos + col) % VIDEO_WIDTH;

            // Check if the current sprite pixel is on
            // (0x80 >> col) isolates each bit from the sprite byte from left to right (MSB first)
            if ((spriteByte & (0x80u >> col)) != 0) {
                // Calculate which byte in the video row contains our target pixel
                // (videoCol / 8) gives us the byte index within the row
                uint8_t& videoByte = video[videoRow][videoCol / 8];

                // Calculate which bit within the byte is our target pixel
                // (7 - (videoCol % 8)) because we use MSB first (SDL_PIXELFORMAT_INDEX1MSB)
                uint8_t bitPosition = 7 - (videoCol % 8);

                // Create a mask to isolate the specific bit
                uint8_t bitMask = 1u << bitPosition;

                // Check if the pixel is already set (collision detection)
                if ((videoByte & bitMask) != 0) {
                    registers[0xF] = 1; // Set collision flag
                }

                // XOR the pixel onto the screen (flip the bit)
                videoByte ^= bitMask;
            }
        }
    }
}


void Chip8::OP_Ex9E(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	uint8_t key = (registers[Vx]);

	if (keypad[key]){

		pc +=2;

	}
}


void Chip8::OP_ExA1(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	uint8_t key = (registers[Vx]);

	if (!keypad[key]){

		pc +=2;	

	}
}


void Chip8::OP_Fx07(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	registers[Vx] = delayTimer;
}


void Chip8::OP_Fx0A(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;


	for (uint8_t i = 0; i < 16; i++){

		if (keypad[i]){

			registers[Vx] = i;
			return;

		}
	}
	
	pc -= 2;
}


void Chip8::OP_Fx15(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	delayTimer = registers[Vx];

}


void Chip8::OP_Fx18(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	soundTimer = registers[Vx];

}


void Chip8::OP_Fx1E(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	index += registers[Vx];

}


void Chip8::OP_Fx29(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	
	index = FONTSET_START_ADDRESS + (5 * registers[Vx]);

}


void Chip8::OP_Fx33(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = registers[Vx];

	memory[index + 2] = value % 10;
	value /= 10;

	memory[index + 1] = value % 10;
	value /= 10;

	memory[index] = value % 10;

}


void Chip8::OP_Fx55(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	
	for (uint8_t i = 0; i <= Vx; i++){

		memory[index + i] = registers[i];

	}
}


void Chip8::OP_Fx65(){

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; i++){

		registers[i] = memory[index + i];

	}
}

