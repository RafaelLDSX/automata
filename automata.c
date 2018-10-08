#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <SDL2/SDL.h>
#define MATRIX_SIZE_X 100
#define MATRIX_SIZE_Y 100
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
float** initMatrix(void);
bool checkMatrix(int** matrix, int x, int y );
void move(int** matrix, int** temp, LIST* list, POINT* door );
void freeMatrix(float** matrix);
void printMatrix(float** matrix);
void thermalStuff(float** matrix, float** new);
float* getNeighboorhood(float** matrix, int x, int y);
void changeColor(SDL_Renderer* renderer, float color);	

static float alpha = 1;

static int sairam = 0;

int main(void)
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* screenSurface = NULL;
	SDL_Event event;
	SDL_Init( SDL_INIT_VIDEO );
	SDL_CreateWindowAndRenderer(MATRIX_SIZE_Y, MATRIX_SIZE_X, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
	float** chapa = initMatrix();
	float** altChapa = initMatrix();
	int i, j, k;
	for (i = 0; i < MATRIX_SIZE_X; i++){
		for (j = 0; j < MATRIX_SIZE_Y; j++){
			if (j == 0){
				chapa[i][j] = 0;
				altChapa[i][j] = 0;
			}
			else if (j == MATRIX_SIZE_Y - 1){
				chapa[i][j] = 100;
				altChapa[i][j] = 0;
			}
			else{
				chapa[i][j] = 35;
				altChapa[i][j] = 0;
			}
		}
	}

	SDL_RenderSetLogicalSize(renderer, MATRIX_SIZE_Y, MATRIX_SIZE_X);
	while(true){
		while(SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT){
				return 0;
			}
		}
		thermalStuff(chapa, altChapa);
		for (j = 0; j < MATRIX_SIZE_X; j++){
			for (k = 0; k < MATRIX_SIZE_Y; k++){
				changeColor(renderer, chapa[j][k]);
				SDL_RenderDrawPoint(renderer, k, j);
			}
		}
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow( window );
	SDL_Quit();
	free(chapa);


}

void changeColor(SDL_Renderer* renderer, float color){
	
	float r, g, b;
	r = color * 2.55;
	g = 127;
	b = (100 - color) * 2.55;
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}

void thermalStuff(float** matrix, float**new){
	int i, j;
	float* n;
	float result, result2;
	for (i = 1; i < MATRIX_SIZE_X-1; i++){
		for (j = 1; j < MATRIX_SIZE_Y-1; j++){
			n = getNeighboorhood(matrix, i, j);
			result = n[0] + n[1] + n[2] + n[3];
			result2 = n[4] + n[5] + n[6] + n[7];
			result = ((4 * result) + result)/20;
			result = alpha * result;
			new[i][j] = result;
			free(n);
		}
	}
	for (i = 1; i < MATRIX_SIZE_X-1; i++){
		for (j = 1; j < MATRIX_SIZE_Y-1; j++){
			matrix[i][j] = new[i][j];
		}
	}
	// float** aux = NULL;
	// aux = *matrix;
	// *matrix = *new;
	// *new = aux;

}

float* getNeighboorhood(float** matrix, int x, int y){
	float* neighboors = malloc(sizeof(float) * 8);
	neighboors[0] = matrix[x-1][y];
	neighboors[1] = matrix[x][y+1];
	neighboors[2] = matrix[x+1][y];
	neighboors[3] = matrix[x][y-1];
	neighboors[4] = matrix[x-1][y-1];
	neighboors[5] = matrix[x+1][y+1];
	neighboors[6] = matrix[x+1][y-1];
	neighboors[7] = matrix[x-1][y+1]; 
	return neighboors;
}

void printMatrix(float** matrix){
	int i, j;
	for (i = 0; i < MATRIX_SIZE_X; i++){
		for (j = 0; j < MATRIX_SIZE_Y; j++){
			printf("%.0f ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
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

float** initMatrix(void) {
	int i, j;
	float** matrix = malloc(sizeof(float*) * MATRIX_SIZE_X);
	for (i = 0; i < MATRIX_SIZE_X; i++){
		matrix[i] = malloc(sizeof(float) * MATRIX_SIZE_Y);
	}

	for(i = 0; i < MATRIX_SIZE_X; i++){
		for(j = 0; j < MATRIX_SIZE_Y; j++){
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

void freeMatrix(float** matrix){
	int i;
	for(i = 0; i < MATRIX_SIZE_X; i++){
		free(matrix[i]);
	}
	free(matrix);
}
