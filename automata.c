#include <stdio.h>
#include <stdbool.h>
#define MATRIX_SIZE 8

typedef struct point {
	int x;
	int y;
} POINT;

typedef struct list {
	POINT point;
	int id;
	struct list* next;
} LIST;

bool checkMatrix(int** matrix, POINT p );
void move(int** matrix, POINT* p, POINT* door );

int main(void)
{
	int i, j;

	int matrix[MATRIX_SIZE][MATRIX_SIZE];
	for(i = 0; i < MATRIX_SIZE; i++){
		for(j = 0; j < MATRIX_SIZE; j++){
			matrix[i][j] = 0;
		}
	}

	matrix[7][5] = 1;
	matrix[0][1] = 2;

	for(i = 0; i < MATRIX_SIZE; i++){
		for(j = 0; j < MATRIX_SIZE; j++){
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}

	POINT door = { .x = 0, .y = 1};
	POINT aux;

	while(true)
	{
		for(i = 0; i < MATRIX_SIZE; i++){
			for(j = 0; j < MATRIX_SIZE; j++){
				if(matrix[i][j] == 1){
					aux.x = i;
					aux.y = j;
					move(matrix, &aux, &door);
				}
			}
		}
		for(i = 0; i < MATRIX_SIZE; i++){
			for(j = 0; j < MATRIX_SIZE; j++){
				printf("%d ", matrix[i][j]);
			}
			printf("\n");
		}
	}
}

bool checkMatrix(int** matrix, POINT p ){
	if ( matrix[p.x][p.y] != 0 ){
		return false;
	}
	return true;
}

void move(int** matrix, POINT* p, POINT* door )
{
	POINT right = { .x = p->x+1, .y = p->y};
	POINT left = { .x = p->x-1, .y = p->y};
	POINT down = { .x = p->x, .y = p->y+1};
	POINT up = { .x = p->x, .y = p->y-1};
	if(p->x < door->x && checkMatrix(matrix, right)){
		p->x++;
	}
	else if(p->y < door->y && checkMatrix(matrix, down)){
		p->y++;
	}
	else if(p->x > door->x && checkMatrix(matrix, left)){
		p->x--;
	}
	else if(p->y > door->y && checkMatrix(matrix, up)){
		p->y--;
	}

}


