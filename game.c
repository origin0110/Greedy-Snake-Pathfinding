#include "snake.h"

COORD get_xy(void) {
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bInfo);
	return bInfo.dwCursorPosition;
}

void goto_xy(COORD pos) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void draw(COORD pos, char world[W][H]) {
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
				case'h': output[n++] = '@'; break;
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
	goto_xy(pos);
	puts(output);
}

void input(char* facing, int* speed_up, char world[W][H], pos head, pos tail) {
	static char tmp = 0;
	if (_kbhit())
		tmp = (char)_getch();
	char c = (tmp == 'p') ? ai_input(world, head, tail) : tmp;
	*speed_up = tmp == 'p';
	*facing = is_input_legal(world, head, c) ? c : *facing;
}

int move(char world[W][H], char facing, pos* head, pos* tail, int* snake_len, int* step) {
	pos test_head = *head;
	head_move(&test_head, facing);
	if (out_of_wall(test_head))
		return 1;
	world[head->x][head->y] = facing;
	if (!is_food(world[test_head.x][test_head.y]))
		tail_move(world, tail);
	else {
		if (no_space(++(*snake_len)))
			return -1;
		generate_food(world);
	}
	if (is_body(world[test_head.x][test_head.y]))
		return 2;
	*head = test_head;
	world[head->x][head->y] = 'h';
	(*step)++;
	return 0;
}

int main(void) {
	char world[W][H] = { 0 };

	pos head = { W / 2, H / 2 };
	pos tail = { W / 2, H / 2 };
	char facing = 0;

	COORD screen_pos = get_xy();
	int dead = 0;
	int step = 0;
	int snake_len = 1;
	srand((unsigned int)time(NULL));
	int speed_up = 0;

	generate_food(world);
	while (dead == 0) {
		Sleep(speed_up ? 1 : 150);
		input(&facing, &speed_up, world, head, tail);
		dead = move(world, facing, &head, &tail, &snake_len, &step);
		draw(screen_pos, world);
	}
	printf("your length is\t%d\nyour step is\t%d\n", snake_len, step);
	return 0;
}
