#include "snake.h"

struct pos head = { W / 2, H / 2 };
struct pos tail = { W / 2, H / 2 };
struct pos food;
char facing = 0;
char world[W][H] = { 0 };

COORD getxy() {
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bInfo);
	return bInfo.dwCursorPosition;
}

void gotoxy(COORD pos) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

struct pos wasd(struct pos p, char c) {
	switch (c) {
	case'w': p.y--; break;
	case'a': p.x--; break;
	case's': p.y++; break;
	case'd': p.x++; break;
	}
	return p;
}

char keyboard_input() {
#if 0
	ai_input(&world, head, tail, food);
#else
	if (_kbhit())
		return _getch();
#endif
}

int is_input_legal(char c) {
	struct pos tmp = wasd(head, c);
	tmp = wasd(tmp, world[tmp.x][tmp.y]);
	return tmp.x != head.x || tmp.y != head.y;
}

void input() {
	char c = keyboard_input();
	facing = is_input_legal(c) ? c : facing;
}

void generate_food() {
	int x, y;
	do {
		x = rand() % W;
		y = rand() % H;
	} while (world[x][y] != 0);
	world[x][y] = 'f';
}

int move(int* snake_len, int* step) {
	struct pos test_head = head;
	test_head = wasd(test_head, facing);

	if (test_head.x < 0 || test_head.x >= W || test_head.y < 0 || test_head.y >= H)
		return 1;

	world[head.x][head.y] = facing;

	if (world[test_head.x][test_head.y] != 'f') {
		struct pos tmp = tail;
		tail = wasd(tail, world[tail.x][tail.y]);
		world[tmp.x][tmp.y] = 0;
	}
	else {
		generate_food();
		(*snake_len)++;
	}

	char f = world[test_head.x][test_head.y];
	if (f == 'w' || f == 'a' || f == 's' || f == 'd')
		return 2;

	head = test_head;
	world[head.x][head.y] = 'h';

	(*step)++;

	return 0;
}

void draw(COORD pos) {
	char output[2 * (W + 2) * (H + 2)] = { 0 };
	int n = 0;
	for (int y = 0; y < H + 2; y++) {
		for (int x = 0; x < W + 2; x++) {
			if (x > 0 && x < W + 1 && y > 0 && y < H + 1) {
				switch (world[x - 1][y - 1]) {
				case'f': output[n++] = 'X'; break;
				case'w':
				case'a':
				case's':
				case'd': output[n++] = 'o'; break;
				case'h': output[n++] = 'e'; break;
				default: output[n++] = ' '; break;
				}
			}
			else
				output[n++] = '#';

			output[n++] = ' ';
		}
		output[n - 1] = '\n';
	}
	output[n - 1] = '\0';
	gotoxy(pos);
	puts(output);
}

int main(void) {
	COORD screen_pos = getxy();
	int dead = 0;
	int step = 0;
	int snake_len = 1;
	srand(time(NULL));
	generate_food();
	while (dead == 0) {
		input();
		dead = move(&snake_len, &step);
		draw(screen_pos);
		Sleep(200);
	}
	printf("your length is %d, your step is %d.\n", snake_len, step);
	printf("dead code: %x\n", dead);
	return 0;
}