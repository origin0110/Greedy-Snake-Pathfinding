#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define H 16
#define W 16

#define	FIFO_LEN (W*W*H*H)

struct pos {
	int x;
	int y;
};

COORD debug_pos;


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

int is_body(struct pos p, char w[W][H]) {
	char c = w[p.x][p.y];
	return c == 'w' || c == 'a' || c == 's' || c == 'd';
}

int will_dead(struct pos p, char w[W][H]) {
	return out_of_wall(p) || is_body(p, w);
}

int no_space(len) {
	return len >= W * H;
}

void fifo_i(struct pos fifo[W * H], int* p1, int x, int y) {
	fifo[*p1].x = x;
	fifo[*p1].y = y;
	(*p1) = (*p1) + 1 >= FIFO_LEN ? 0 : (*p1) + 1;
}

struct pos fifo_o(struct pos fifo[W * H], int* p0) {
	struct pos out_pos = { 0,0 };
	out_pos.x = fifo[*p0].x;
	out_pos.y = fifo[*p0].y;
	(*p0) = (*p0) + 1 >= FIFO_LEN ? 0 : (*p0) + 1;
	return out_pos;
}

void get_choice(char choice[2], struct pos head) {
	choice[0] = head.y & 1 ? 'a' : 'd';
	choice[1] = head.x & 1 ? 's' : 'w';
}

//Artificial Idiot
char ai_input(char world[W][H], struct pos head, struct pos tail, struct pos food) {
	//复制世界
	char ai_world[W][H] = { 0 };
	for (int i = 0; i < W; i++)
		for (int j = 0; j < H; j++)
			ai_world[i][j] = world[i][j];
	struct pos ai_head = head;
	struct pos ai_tail = tail;
	struct pos ai_food = food;

	//生成可能的方向选择
	char choice[2];
	get_choice(choice, ai_head);

	struct pos next_0 = wsad(ai_head, choice[0]);
	struct pos next_1 = wsad(ai_head, choice[1]);

	//如果只有一个选择是活路，直接跳过搜索
	if (will_dead(next_0, ai_world))
		return choice[1];
	if (will_dead(next_1, ai_world))
		return choice[0];

	int been_found[W][H] = { 0 };	//用于排除已经被搜索过的坐标
	int distance[W][H] = { 0 };		//蛇头到此节点的最短路径长度
	char father_node[W][H] = { 0 }; //用于传递父节点的信息

	struct pos fifo[FIFO_LEN];		//广度优先搜索队列
	int p0 = 0;						//数组下标模拟指针，入队列
	int p1 = 0;						//数组下标模拟指针，出队列


	//把可能的坐标加入队列并记录父结点
	fifo_i(fifo, &p1, next_0.x, next_0.y);
	father_node[next_0.x][next_0.y] = choice[0];
	distance[next_0.x][next_0.y] = 1;
	fifo_i(fifo, &p1, next_1.x, next_1.y);
	father_node[next_1.x][next_1.y] = choice[1];
	distance[next_1.x][next_1.y] = 1;

	//搜索深度=0
	int depth = 0;

	while (1) {
		//从队列中取出一个坐标
		struct pos search = fifo_o(fifo, &p0);

		//如果当前节点的距离大于搜索深度，模拟蛇尾移动一格，这可能为蛇头移动让出更好的路径
		if (distance[search.x][search.y] > depth) {
			//模拟移动
			struct pos tmp_ai_tail = ai_tail;
			ai_tail = wsad(ai_tail, ai_world[ai_tail.x][ai_tail.y]);
			ai_world[tmp_ai_tail.x][tmp_ai_tail.y] = 0;

			//搜索深度++
			depth++;
		}

		if (ai_world[search.x][search.y] == 'f') {
			//如果找到了食物，返回最短路径的方向
			return father_node[search.x][search.y];
		}
		else {
			//如果没找到食物，则把所有新出现的可用坐标加入队列，并传递父节点信息，计算新坐标距离
			char choice[2];
			get_choice(choice, search);

			struct pos next_0 = wsad(search, choice[0]);
			struct pos next_1 = wsad(search, choice[1]);

			if (!will_dead(next_0, ai_world) && !been_found[next_0.x][next_0.y]) {
				fifo_i(fifo, &p1, next_0.x, next_0.y);
				father_node[next_0.x][next_0.y] = father_node[search.x][search.y];
				distance[next_0.x][next_0.y] = distance[search.x][search.y] + 1;
			}

			if (!will_dead(next_1, ai_world) && !been_found[next_1.x][next_1.y]) {
				fifo_i(fifo, &p1, next_1.x, next_1.y);
				father_node[next_1.x][next_1.y] = father_node[search.x][search.y];
				distance[next_1.x][next_1.y] = distance[search.x][search.y] + 1;
			}
		}
		been_found[search.x][search.y] = 1;
	}
}

char input() {
	static char tmp = 0;
	Sleep((tmp == 'p') ? 1 : 150);
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
	if (is_body(test_head, world))
		return 2;
	head = test_head;
	world[head.x][head.y] = 'h';
	(*step)++;
	return 0;
}

int main(void) {
	COORD screen_pos = getxy();
	debug_pos = getxy();
	int dead = 0;
	int step = 0;
	int snake_len = 1;
	srand(time(NULL));
	generate_food();
	while (dead == 0) {
		dead = move(&snake_len, &step);
		draw(screen_pos, world);
	}
	printf("your length is\t%d\nyour step is\t%d\n", snake_len, step);
	return 0;
}
