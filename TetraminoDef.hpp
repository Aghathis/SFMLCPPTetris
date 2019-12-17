#ifndef TETRAMINO_DEF_HPP
#define TETRAMINO_DEF_HPP

bool tetraminoBlock(unsigned int value, int x,unsigned int y){
	value = value>>y*4;
	//value&=0x000F;
	value = value>>x;
	value&=0x0001;
	return value;
}

struct TetraminoInfo{
	char name;
	unsigned int shape[4];
	//sf::Color color;
	int tiles;
};

static TetraminoInfo TetraminoDef[7] = {
	{'I',{0x4444,0x00F0,0x4444,0x00F0},0},//sf::Color(040,128,200)},	// I
	{'O',{0x0660,0x0660,0x0660,0x0660},1},//sf::Color(255,255,000)},	// O
    {'T',{0x0E40,0x4C40,0x4E00,0x4640},2},//sf::Color(255,000,128)},	// T
	{'L',{0xC440,0x2E00,0x4460,0x0E80},3},//sf::Color(000,064,128)},	// L
	{'J',{0x6440,0x0E20,0x44C0,0x8E00},4},//sf::Color(255,128,000)},	// J
	{'S',{0x0360,0x0231,0x0036,0x0462},5},//sf::Color(064,200,100)},	// S
	{'Z',{0x0C60,0x4C80,0xC600,0x2640},6} //sf::Color(200,032,000)}	// Z
};

#endif // TETRAMINO_DEF_HPP

