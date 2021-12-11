#pragma once

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

void gotoxy(COORD);
struct pos wasd(struct pos, char);
char keyboard_input();
char ai_input();
int is_input_legal(char);
void input();
void generate_food();
int move(int*, int*);
void draw();
