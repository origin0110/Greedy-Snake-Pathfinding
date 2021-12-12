#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define H 16
#define W 16

struct pos {
	int x;
	int y;
};

//'f' food
//'h' head
//'wsad' body
//'\0' air
char world[W][H] = { 0 };

struct pos head = { W / 2, H / 2 };
struct pos tail = { W / 2, H / 2 };
struct pos food;

COORD getxy() {
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bInfo);
	return bInfo.dwCursorPosition;
}

void gotoxy(COORD pos) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

struct pos wsad(struct pos p, char c) {
	switch (c) {
	case'w': p.y--; break;
	case'a': p.x--; break;
	case's': p.y++; break;
	case'd': p.x++; break;
	}
	return p;
}

int is_input_legal(char c) {
	struct pos tmp;
	tmp = wsad(head, c);
	tmp = wsad(tmp, world[tmp.x][tmp.y]);
	return tmp.x != head.x || tmp.y != head.y;
}

void generate_food() {
	int x, y;
	do {
		x = rand() % W;
		y = rand() % H;
	} while (world[x][y] != 0);
	world[x][y] = 'f';
}

int out_of_wall(struct pos p) {
	return p.x < 0 || p.x >= W || p.y < 0 || p.y >= H;
}

int is_body(char c) {
	return c == 'w' || c == 'a' || c == 's' || c == 'd';
}

int no_space(len) {
	return len >= W * H;
}

void get_choice(char choice[2]) {
	choice[0] = head.y & 1 ? 'a' : 'd';
	choice[1] = head.x & 1 ? 's' : 'w';
}

//Artificial Idiot
char ai_input(char world[W][H], struct pos head, struct pos tail, struct pos food) {
	int r = rand() & 1;
	char choice[2];
	get_choice(choice);
	struct pos next = wsad(head, choice[r]);
	int will_dead = out_of_wall(next) || is_body(world[next.x][next.y]);
	return choice[will_dead ^ r];
}

char input() {
	static char tmp = 0;
	Sleep((tmp == 'p') ? 0 : 100);
	if (_kbhit())
		tmp = _getch();
	return (tmp == 'p') ? ai_input(world, head, tail, food) : tmp;
}

int move(int* snake_len, int* step) {
	static char facing = 0;
	char c = input();
	facing = is_input_legal(c) ? c : facing;
	struct pos test_head = head;
	test_head = wsad(test_head, facing);
	if (out_of_wall(test_head))
		return 1;
	world[head.x][head.y] = facing;
	if (world[test_head.x][test_head.y] != 'f') {
		struct pos tmp = tail;
		tail = wsad(tail, world[tail.x][tail.y]);
		world[tmp.x][tmp.y] = 0;
	}
	else {
		if (no_space(++(*snake_len)))
			return -1;
		generate_food();
	}
	if (is_body(world[test_head.x][test_head.y]))
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
		dead = move(&snake_len, &step);
		draw(screen_pos);
	}
	printf("your length is\t%d\nyour step is\t%d\n", snake_len, step);
	return 0;
}
