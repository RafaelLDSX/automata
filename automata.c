#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#define MATRIX_SIZE 8

typedef struct point {
	int x;
	int y;
	int id;
	int alive;
} POINT;

typedef struct list {
	POINT point;
	struct list* next;
} LIST;

void addLIST(LIST* list, LIST* new);
void addPOINTtoLIST(LIST* list, POINT* p);
void removefromLIST(LIST* list, int id);
LIST* initLIST(void);
POINT* initPOINT(void);
int** initMatrix(void);
bool checkMatrix(int** matrix, int x, int y );
void move(int** matrix, int** temp, LIST* list, POINT* door );
void freeMatrix(int** matrix);

static int sairam = 0;

int main(void)
{
	int i, j;

	int** matrix = initMatrix();

	matrix[7][5] = 1; 	
	matrix[0][1] = 2;

	LIST* list = initLIST();

	POINT* door = initPOINT();
	door->x = 0;
	door->y = 1;
	door->id = 2;

	POINT* aux = initPOINT();
	aux->x = 7;
	aux->y = 5;
	aux->id = 1;
	aux->alive = 1;

	addPOINTtoLIST(list, aux);

	aux->x = 4;
	aux->y = 6;
	aux->id = 4;

	addPOINTtoLIST(list, aux);

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
					move(matrix, temp, list, door);
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
		printf("Sairam: %d\n\n", sairam);
		sleep(1);
	}
	free(door);
	free(aux);
	freeMatrix(matrix);
	return 0;
}

bool checkMatrix(int** matrix, int x, int y){
	if ( matrix[x][y] != 0 && matrix[x][y] != 2 ){
		return false;
	}
	return true;
}

void move(int** matrix, int** temp, LIST* list, POINT* door )
{
	LIST* aux = list;
	while(aux != NULL){
		if(aux->point.alive == 1){
			POINT* novo = initPOINT();

			novo->x = aux->point.x;
			novo->y = aux->point.y;
			novo->id = aux->point.id;

			if(aux->point.x < door->x && checkMatrix(matrix, aux->point.x+1, aux->point.y)){
				novo->x = aux->point.x + 1;
			}
			if(aux->point.y < door->y && checkMatrix(matrix, aux->point.x, aux->point.y+1)){
				novo->y = aux->point.y + 1;
			}
			if(aux->point.x > door->x && checkMatrix(matrix, aux->point.x-1, aux->point.y)){
				novo->x = aux->point.x - 1;
			}
			if(aux->point.y > door->y && checkMatrix(matrix, aux->point.x, aux->point.y-1)){
				novo->y = aux->point.y - 1;
			}

			matrix[aux->point.x][aux->point.y] = 0;
			if(matrix[novo->x][novo->y] == 2){
				sairam++;
				aux->point.alive = 0;
				removefromLIST(aux, novo->id);
			}
			else{
				matrix[novo->x][novo->y] = 1;
			}

			temp[novo->x][novo->y] = 1;

			aux->point.x = novo->x;
			aux->point.y = novo->y;

			free(novo);

		}
		aux = aux->next;
	}
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

LIST* initLIST(void){
	LIST* list = malloc(sizeof(LIST));
	list->next = NULL;
	return list;
}

void addLIST(LIST* list, LIST* new){
	LIST* temp = list;
	while(temp->next != NULL){
		temp = temp->next;
	}
	temp->next = new;
}

void addPOINTtoLIST(LIST* list, POINT* p){
	LIST* aux = initLIST();
	aux->point.x = p->x;
	aux->point.y = p->y;
	aux->point.id = p->id;
	aux->point.alive = p->alive;
	addLIST(list, aux);
}

void removefromLIST(LIST* list, int id){
	LIST* aux = NULL;
	LIST* temp = list;
	while(temp->next != NULL && temp->point.id != id){
		aux = temp;
		temp = temp->next;
	}

	if(temp->point.id == id){
		if(aux == NULL){
			list = temp->next;
			free(temp);
		}
		else{
			aux->next = temp->next;
			free(temp);
		}
	}

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
