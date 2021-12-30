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
	//�����������������
	char ai_world[W][H];
	pos ai_head = head;
	pos ai_tail = tail;

	copy_world(ai_world, world);

	//������ص�����
	int been_found[W][H] = { 0 };		//�����Ƿ�������
	int distance[W][H] = { 0 };			//��ͷ������������·������
	char father_node[W][H] = { 0 };		//���ڴ��ݸ��ڵ����Ϣ

	pos fifo[W * H];					//���������������
	int p0 = 0;							//�����±�ģ��ָ�������ڵ�ָ��
	int p1 = 0;							//�����±�ģ��ָ����г��ڵ�ָ��

	int depth = 1;						//��ʼ�������=1

	//����ͷ��Χ�����������У�����¼��Ҫ����Ϣ
	char choice[2];
	get_choice(choice, ai_head);

	pos next_0 = wsad(ai_head, choice[0]);
	pos next_1 = wsad(ai_head, choice[1]);

	//ģ����β�ƶ�һ�������Ϊ��ͷ�ƶ��ó�·��
	tail_move(ai_world, &ai_tail);

	father_node[ai_head.x][ai_head.y] = choice[0];
	if (!will_dead(next_0, ai_world))
		mark(fifo, &p1, father_node, distance, been_found, ai_head, next_0);
	father_node[ai_head.x][ai_head.y] = choice[1];
	if (!will_dead(next_1, ai_world))
		mark(fifo, &p1, father_node, distance, been_found, ai_head, next_1);

	while (1) {
		//�Ӷ�����ȡ��һ������
		pos here = fifo_o(fifo, &p0);

		//�����ǰ�ڵ�ľ������������ȣ�ģ����β�ƶ�һ�������Ϊ��ͷ�ƶ��ó�·��
		if (distance[here.x][here.y] >= depth) {
			tail_move(ai_world, &ai_tail);
			depth++;
		}

		if (is_food(ai_world[here.x][here.y]))
			//����ҵ���ʳ��������·���ķ���
			return father_node[here.x][here.y];
		else {
			//���û�ҵ�ʳ�����µ����������У�����¼��Ҫ����Ϣ
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
