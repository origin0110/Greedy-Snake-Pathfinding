#include "snake.h"

struct block {
	char been_found;
	char block_type;
	char father_block;
};

char ai_input(char world[W][H], struct pos head, struct pos tail, struct pos food) {
	struct block ai_world[W][H];
	for (int x = 0; x < W; x++)
		for (int y = 0; y < H; y++) {
			struct block tmp = { 0, world[x][y], 0 };
			ai_world[x][y] = tmp;
		}

	//Artificial Idiot
	//ÈË¹¤ÖÇÕÏ
	switch (rand() % 4) {
	case 0:return 'w';
	case 1:return 'a';
	case 2:return 's';
	case 3:return 'd';
	}

	return 0;
}