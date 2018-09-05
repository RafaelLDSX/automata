#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
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

POINT* initPOINT(void);
int** initMatrix(void);
bool checkMatrix(int** matrix, int x, int y );
void move(int** matrix, int** temp, POINT* p, POINT* door );
void freeMatrix(int** matrix);

static int sairam = 0;

int main(void)
{
	int i, j;

	int** matrix = initMatrix();

	matrix[7][5] = 1; 	
	matrix[0][1] = 2;

	POINT* door = initPOINT();
	door->x = 0;
	door->y = 1;
	POINT* aux = initPOINT();
	aux->x = 7;
	aux->y = 5;

	int counter = 0; 	

	int** temp = initMatrix();

	for(i = 0; i < MATRIX_SIZE; i++){
			for(j = 0; j < MATRIX_SIZE; j++){
				printf("%d ", matrix[i][j]);
			}
			printf("\n");
		}
		printf("\n");

	while(counter < 15)
	{

		// for(i = 0; i < MATRIX_SIZE; i++){
		// 	for(j = 0; j < MATRIX_SIZE; j++){
		// 		if(matrix[i][j] == 1){
		// 			aux->x = i;
		// 			aux->y = j;
					move(matrix, temp, aux, door);
		// 		}
		// 	}
		// }
		for(i = 0; i < MATRIX_SIZE; i++){
			for(j = 0; j < MATRIX_SIZE; j++){
				printf("%d ", matrix[i][j]);
			}
			printf("\n");
		}
		printf("\n");
		counter++;
	}
	free(door);
	free(aux);
	freeMatrix(matrix);
	return 0;
}

bool checkMatrix(int** matrix, int x, int y){
	if ( matrix[x][y] != 0 ){
		return false;
	}
	return true;
}

void move(int** matrix, int** temp, POINT* p, POINT* door )
{

	POINT* novo = initPOINT();

	novo->x = p->x;
	novo->y = p->y;

	POINT right = { .x = p->x+1, .y = p->y};
	POINT left = { .x = p->x-1, .y = p->y};
	POINT down = { .x = p->x, .y = p->y+1};
	POINT up = { .x = p->x, .y = p->y-1};
	if(p->x < door->x && checkMatrix(matrix, p->x+1, p->y)){
		novo->x = p->x + 1;
	}
	if(p->y < door->y && checkMatrix(matrix, p->x, p->y+1)){
		novo->y = p->y + 1;
	}
	if(p->x > door->x && checkMatrix(matrix, p->x-1, p->y)){
		novo->x = p->x - 1;
	}
	if(p->y > door->y && checkMatrix(matrix, p->x, p->y-1)){
		novo->y = p->y - 1;
	}

	matrix[p->x][p->y] = 0;
	if(matrix[novo->x][novo->y] == 2){
		sairam++;
	}
	else{
		matrix[novo->x][novo->y] = 1;
	}

	temp[novo->x][novo->y] = 1;

	p->x = novo->x;
	p->y = novo->y;

	free(novo);
}

int** initMatrix(void) {
	int i, j;
	int** matrix = malloc(sizeof(int*) * MATRIX_SIZE);
	for (i = 0; i < MATRIX_SIZE; i++){
		matrix[i] = malloc(sizeof(int) * MATRIX_SIZE);
	}

	for(i = 0; i < MATRIX_SIZE; i++){
		for(j = 0; j < MATRIX_SIZE; j++){
			matrix[i][j] = 0;
		}
	}
	return matrix;
}

POINT* initPOINT(void) {
	POINT* point = malloc(sizeof(POINT));
	point->x = 0;
	point->y = 0;
	return point;
}

void freeMatrix(int** matrix){
	int i;
	for(i = 0; i < MATRIX_SIZE; i++){
		free(matrix[i]);
	}
	free(matrix);
}
