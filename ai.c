#include "snake.h"

void fifo_i(pos fifo[W * H], int* p1, pos tmp) {
	fifo[(*p1)++] = tmp;
}

pos fifo_o(pos fifo[W * H], int* p0) {
	return fifo[(*p0)++];
}

void get_choice(char choice[2], pos head) {
	choice[0] = head.y & 1 ? 'a' : 'd';
	choice[1] = head.x & 1 ? 's' : 'w';
}

void mark(pos fifo[W * H], int* p1, char father_node[W][H], int distance[W][H], int been_found[W][H], pos here, pos next) {
	fifo_i(fifo, p1, next);
	father_node[next.x][next.y] = father_node[here.x][here.y];
	distance[next.x][next.y] = distance[here.x][here.y] + 1;
	been_found[next.x][next.y] = 1;
}

void copy_world(char a[W][H], char b[W][H]) {
	for (int i = 0; i < W; i++)
		for (int j = 0; j < H; j++)
			a[i][j] = b[i][j];
}

char ai_input(char world[W][H], pos head, pos tail) {
	//复制世界和其他数据
	char ai_world[W][H];
	pos ai_head = head;
	pos ai_tail = tail;

	copy_world(ai_world, world);

	//搜索相关的数据
	int been_found[W][H] = { 0 };		//坐标是否被搜索过
	int distance[W][H] = { 0 };			//蛇头到此坐标的最短路径长度
	char father_node[W][H] = { 0 };		//用于传递父节点的信息

	pos fifo[W * H];					//广度优先搜索队列
	int p0 = 0;							//数组下标模拟指向队列入口的指针
	int p1 = 0;							//数组下标模拟指向队列出口的指针

	int depth = 1;						//初始搜索深度=1

	//把蛇头周围的坐标加入队列，并记录必要的信息
	char choice[2];
	get_choice(choice, ai_head);

	pos next_0 = wsad(ai_head, choice[0]);
	pos next_1 = wsad(ai_head, choice[1]);

	//模拟蛇尾移动一格，这可能为蛇头移动让出路径
	tail_move(ai_world, &ai_tail);

	father_node[ai_head.x][ai_head.y] = choice[0];
	if (!will_dead(next_0, ai_world))
		mark(fifo, &p1, father_node, distance, been_found, ai_head, next_0);
	father_node[ai_head.x][ai_head.y] = choice[1];
	if (!will_dead(next_1, ai_world))
		mark(fifo, &p1, father_node, distance, been_found, ai_head, next_1);

	while (1) {
		//从队列中取出一个坐标
		pos here = fifo_o(fifo, &p0);

		//如果当前节点的距离大于搜索深度，模拟蛇尾移动一格，这可能为蛇头移动让出路径
		if (distance[here.x][here.y] >= depth) {
			tail_move(ai_world, &ai_tail);
			depth++;
		}

		if (is_food(ai_world[here.x][here.y]))
			//如果找到了食物，返回最短路径的方向
			return father_node[here.x][here.y];
		else {
			//如果没找到食物，则把新的坐标加入队列，并记录必要的信息
			get_choice(choice, here);

			next_0 = wsad(here, choice[0]);
			next_1 = wsad(here, choice[1]);

			if (!will_dead(next_0, ai_world) && !been_found[next_0.x][next_0.y])
				mark(fifo, &p1, father_node, distance, been_found, here, next_0);
			if (!will_dead(next_1, ai_world) && !been_found[next_1.x][next_1.y])
				mark(fifo, &p1, father_node, distance, been_found, here, next_1);
		}
	}
}
