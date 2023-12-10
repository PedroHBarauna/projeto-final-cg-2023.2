#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

void printBoard(int *board){
    for (int i = 1; i <= 16; ++i) {
        printf("%d ", board[i-1]);
        if(i % 4 == 0){
            printf("\n");
        }
    }
}

void generateBoard(int *board, int size) {
    // Inicializa o gerador de números pseudo-aleatórios com base no tempo atual
    srand(time(NULL));

    // Preenche o vetor com pelo menos uma ocorrência de cada número de 1 a 6
    for (int i = 0; i < 6; ++i) {
        board[i] = i + 1;
    }

    // Preenche as posições restantes do vetor com valores aleatórios de 1 a 6
    for (int i = 6; i < size; ++i) {
        board[i] = rand() % 6 + 1;
    }

    // Embaralha o vetor para garantir a aleatoriedade
    for (int i = size - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        // Troca os elementos i e j
        int temp = board[i];
        board[i] = board[j];
        board[j] = temp;
    }
}

int chosenValue(){
    return rand() % 6 + 1;
}

int main(){
    int board[16];

    for(int i = 0; i < 3; i++){
        generateBoard(board, 16);
        for(int j = 0; j < 7; j++){    // Tabuleiro pisca no console por 14 segundos
            printBoard(board);
            Sleep(1000);
            system("cls");
            Sleep(1000);
        }
        Sleep(2000);   // 2 segundos para revelar o elemento sorteado
        printf("Numero sorteado: %d", chosenValue());
        Sleep(5000);    // 5 segundos para se locomover ate a plataforma segura
        system("cls");
    }

    return 0;
}