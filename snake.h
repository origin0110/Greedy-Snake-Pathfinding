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

char ai_input(char world[W][H], struct pos, struct pos, struct pos);

int is_input_legal(char);
