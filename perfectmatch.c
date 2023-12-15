#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <windows.h>

int boardSize = 16;
int minPosicoes;
int maxPosicoes;


void generateBoard(int *board, int size) {
    // Inicializa o gerador de números pseudo-aleatórios com base no tempo atual

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

void generateVisibleBoard(int *board, int tamanho) {
    minPosicoes = boardSize / 2;
    maxPosicoes = 2 * boardSize / 3;
    
    for (int i = 0; i < tamanho; i++){
        board[i] = 0;
    }

    int numeroPosicoes = rand() % (maxPosicoes - minPosicoes + 1) + minPosicoes;

    for (int i = 0; i < numeroPosicoes; i++) {
        int posicao = rand() % tamanho;
        board[posicao] = 1;
    }

}

int chosenValue(){
    return rand() % 6 + 1;
}

void printBoard(int *board){
    for (int i = 1; i <= boardSize; ++i) {
        printf("%d ", board[i-1]);
        if(i % 4 == 0){
            printf("\n");
        }
    }
    printf("\n");
}

void printBoardOnlyVisibles(int *board){

    int viravira[boardSize];

    generateVisibleBoard(viravira, 16);

    for (int k = 1; k <= boardSize; k++) {

        if(viravira[k])
            printf("%d ", board[k-1]);
        else
            printf("  ");

        if(k % 4 == 0){
            printf("\n");
        }
    }
}

void printBoardOnlyPickedFruit(int *board, int pickedFruit){
    
    int viravira[boardSize];

    generateVisibleBoard(viravira, 16);

    for (int k = 1; k <= boardSize; k++) {

        if(board[k-1] == pickedFruit)
            printf("%d ", board[k-1]);
        else
            printf("  ");

        if(k % 4 == 0){
            printf("\n");
        }
    }
}

// int main(){

//     srand(time(NULL));
//     boardSize = 16;
//     minPosicoes = boardSize / 2;
//     maxPosicoes = 2 * boardSize / 3;

//     int board[boardSize];

//     for(int i = 0; i < 3; i++){

//         generateBoard(board, 16);
//         printf("TABULEIRO GERADO:\n");
//         printBoard(board);

//         for(int j = 0; j < 7; j++){    // Tabuleiro pisca no console por 14 segundos
//             //Sleep(1000);
//             //system("cls");
//             //Sleep(1000);
//             printBoardOnlyVisibles(board);
//             printf("\n");
//         }

//         //Sleep(2000);   // 2 segundos para revelar o elemento sorteado
//         int fruta_escolhida = chosenValue();
    
//         printf("Numero sorteado: %d da rodada %d\n\n", fruta_escolhida, i);
//         printBoardOnlyPickedFruit(board, fruta_escolhida);
        
//         printf("\n");

//         //Sleep(5000);    // 5 segundos para se locomover ate a plataforma segura
//         //system("cls");
//     }

//     return 0;
// }