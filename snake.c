#include "snake.h"

pos wsad(pos p, char c) {
	switch (c) {
	case'w': p.y--; break;
	case'a': p.x--; break;
	case's': p.y++; break;
	case'd': p.x++; break;
	}
	return p;
}

int is_input_legal(char world[W][H], pos head, char c) {
	pos tmp = wsad(head, c);
	tmp = wsad(tmp, world[tmp.x][tmp.y]);
	return tmp.x != head.x || tmp.y != head.y;
}

void generate_food(char world[W][H]) {
	int x, y;
	do {
		x = rand() % W;
		y = rand() % H;
	} while (world[x][y] != 0);
	world[x][y] = 'f';
}

void tail_move(char world[W][H], pos* tail) {
	pos tmp_tail = *tail;
	*tail = wsad(*tail, world[tail->x][tail->y]);
	world[tmp_tail.x][tmp_tail.y] = 0;
}

void head_move(pos* head, char c) {
	*head = wsad(*head, c);
}

int out_of_wall(pos p) {
	return p.x < 0 || p.x >= W || p.y < 0 || p.y >= H;
}

int is_head(char c) {
	return c == 'h';
}

int is_body(char c) {
	return c == 'w' || c == 'a' || c == 's' || c == 'd';
}

int is_food(char c) {
	return c == 'f';
}

int will_dead(pos p, char w[W][H]) {
	return out_of_wall(p) || is_body(w[p.x][p.y]);
}

int no_space(int len) {
	return len >= W * H;
}
