/*
*  display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#include "display.h"
#include "io.h"

//����â
#define INFO_WIDTH 40 
#define INFO_HEIGHT 18
//�ý��� �޽��� 
#define MESSAGE_WIDTH 60
#define MESSAGE_HEIGHT 10
//���â
#define COMMANDS_WIDTH 40 
#define COMMANDS_HEIGHT 10 

// ����� ������� �»��(topleft) ��ǥ
const POSITION resource_pos = { 0, 0 };
const POSITION map_pos = { 1, 0 };
const POSITION display_object_info_pos = { 1,61 };
const POSITION display_system_message_pos = { 19, 0 };
const POSITION display_commands_pos = { 19, 61 };

char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);

//����â
void display_object_info(void) {
	for (int i = 0; i < INFO_HEIGHT; i++) {
		for (int j = 0; j < INFO_WIDTH; j++) {
			if (i == 0 || i == INFO_HEIGHT - 1 || j == 0 || j == INFO_WIDTH - 1) {
				printc(padd(display_object_info_pos, (POSITION) { i, j }), '#', COLOR_DEFAULT);
			}
			else {
				printc(padd(display_object_info_pos, (POSITION) { i, j }), ' ', COLOR_DEFAULT);
			}
		}
	}
}
//�ý��� �޽���
void display_system_message(void) {
	for (int i = 0; i < MESSAGE_HEIGHT; i++) {
		for (int j = 0; j < MESSAGE_WIDTH; j++) {
			if (i == 0 || i == MESSAGE_HEIGHT - 1 || j == 0 || j == MESSAGE_WIDTH - 1) {
				printc(padd(display_system_message_pos, (POSITION) { i, j }), '#', COLOR_DEFAULT);
			}
			else {
				printc(padd(display_system_message_pos, (POSITION) { i, j }), ' ', COLOR_DEFAULT);
			}
		}
	}
}
//���â
void display_commands(void) {
	for (int i = 0; i < MESSAGE_HEIGHT; i++) {
		for (int j = 0; j < COMMANDS_WIDTH; j++) {
			if (i == 0 || i == MESSAGE_HEIGHT - 1 || j == 0 || j == COMMANDS_WIDTH - 1) {
				printc(padd(display_commands_pos, (POSITION) { i, j }), '#', COLOR_DEFAULT);
			}
			else {
				printc(padd(display_commands_pos, (POSITION) { i, j }), ' ', COLOR_DEFAULT);
			}
		}
	}
}

void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor)
{
	display_resource(resource);
	display_map(map);
	display_cursor(cursor);
	
}

void display_resource(RESOURCE resource) {
	set_color(COLOR_RESOURCE);
	gotoxy(resource_pos);
	printf("spice = %d/%d, population=%d/%d\n",
		resource.spice, resource.spice_max,
		resource.population, resource.population_max 
	);
}

// subfunction of draw_map()
void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			for (int k = 0; k < N_LAYER; k++) {
				if (src[k][i][j] >= 0) {
					dest[i][j] = src[k][i][j]; 
				}
			}
		}
	}
}
//����, ���ڻ� ��� �Լ� 
void setColor(unsigned short text, unsigned short back) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text | (back << 4));
}
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project(map, backbuf);

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (frontbuf[i][j] != backbuf[i][j]) {
				POSITION pos = { i, j }; 
				char ch = backbuf[i][j];  // �⺻ ĳ����  
				unsigned short text_color = 15;  // �⺻ �ؽ�Ʈ ���� (���) 
				unsigned short back_color = 0;   // �⺻ ��� ���� (����) 

				if (ch == 'B') { 
					back_color = 1;  // �Ķ� 
					if (i == 1 || i == 2) { 
						back_color = 4;  // ���� 
					}
				}
				else if (ch == '5') { 
					back_color = 6;  // ��Ȳ 
				}
				else if (ch == 'P') { 
					back_color = 0;  // ���� 
				}
				else if (ch == 'R') { 
					back_color = 7;  // ȸ�� 
				}
				else if (ch == 'H') {
					if ((i == MAP_HEIGHT - 4 && j == 1) || (i == 3 && j == MAP_WIDTH - 2)) {
						back_color = 4;
					}
					else {
						back_color = 1;
					}
				}
				else if (ch = 'W') {
					if ((i == 3 && j == 10) || (i == 11 && j == 36)) {
						back_color = 14;
					}
					else {
						back_color = 0;
					}
				}
				int color = text_color | (back_color << 4); 
				printc(padd(map_pos, pos), backbuf[i][j], color);
			}
			frontbuf[i][j] = backbuf[i][j];
		}
	}

}

// frontbuf[][]���� Ŀ�� ��ġ�� ���ڸ� ���� �ٲ㼭 �״�� �ٽ� ���
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	char ch = frontbuf[prev.row][prev.column];
	printc(padd(map_pos, prev), ch, COLOR_DEFAULT);

	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, COLOR_CURSOR);
}