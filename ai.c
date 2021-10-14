#include "snake.h"

struct block {
	char been_found;
	char block_type;
	char father_block;
};

char ai_input(char** world, struct pos head, struct pos tail, struct pos food) {
	//复刻世界
	struct block ai_world[W][H];
	for (int x = 0; x < W; x++)
		for (int y = 0; y < H; y++) {
			struct block tmp = { 0,world[x][y],0 };
			ai_world[x][y] = tmp;
		}

	//搜索食物

	return 0; //人工智障
}