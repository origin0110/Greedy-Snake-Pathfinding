#ifndef _SNAKE_
#define _SNAKE_

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
typedef struct pos pos;

pos wsad(pos p, char c);
int is_input_legal(char world[W][H], pos head, char c);
void generate_food(char world[W][H]);
int is_head(char c);
int is_body(char c);
int is_food(char c);
int out_of_wall(pos p);
int will_dead(pos p, char w[W][H]);
int no_space(int len);
void tail_move(char world[W][H], pos* tail);
void head_move(pos* head, char c);

char ai_input(char world[W][H], pos head, pos tail);

#endif
