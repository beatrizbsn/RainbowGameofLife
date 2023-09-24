/* Beatriz Barbosa Santos Nascimento */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <Windows.h>

#define   Threads  8
#define   Tamanho  50
#define   Ngeracao 4

struct DadosThread {
    float** grid;
    float** newgrid;
    int id;
};

float getNeighbords_media (float ** grid, int i,int j);
int getNeighbords (float ** grid, int i,int j);
void *runThread (void *argumento);
void imprimirMatriz(float** matriz);

int main()
{
    pthread_t threads[Threads];
    struct DadosThread dados[Threads];
    int soma,auxgeracao,auxthread,auxjointhread,aux,aux1,aux2;
    float **auxgrid ;
    float **grid = (float **)malloc(Tamanho* sizeof(float *));
    float **newgrid = (float **)malloc(Tamanho* sizeof(float *));
    int* valorDeRetorno;
    DWORD start, end;


    for (aux = 0; aux < Tamanho; aux++) {
        grid[aux] = (float *)malloc(Tamanho * sizeof(float));
        newgrid[aux] = (float *)malloc(Tamanho * sizeof(float));

      }

    for (aux1 = 0; aux1 < Tamanho; aux1++) {
         for (aux2 = 0; aux2 < Tamanho; aux2++) {
           grid[aux1][aux2]    = 0.0;
           newgrid[aux1][aux2] = 0.0;
         }
     }



    //GLIDER
    grid[1][2]  = 1.0;
    grid[2][3]  = 1.0;
    grid[3][1]  = 1.0;
    grid[3][2]  = 1.0;
    grid[3][3]  = 1.0;

    //R-pentomino
    grid[10][31] = 1.0;
    grid[10][32] = 1.0;
    grid[11][30] = 1.0;
    grid[11][31] = 1.0;
    grid[12][31] = 1.0;

    imprimirMatriz(grid);

    start = GetTickCount();

    for (auxgeracao = 0; auxgeracao < Ngeracao ; auxgeracao++){

       soma = 0;
       for (auxthread = 0; auxthread < Threads; auxthread++){
        dados[auxthread].grid = grid;
        dados[auxthread].newgrid = newgrid ;
        dados[auxthread].id = auxthread+1;

        pthread_create(&threads[auxthread],NULL,runThread,(void*)&dados[auxthread]);
       }

       for(auxjointhread= 0; auxjointhread< Threads; auxjointhread++) {
        pthread_join(threads[auxjointhread],(void**)&valorDeRetorno);
        soma += *valorDeRetorno;
       }

       auxgrid = grid;
       grid = newgrid;
       newgrid = auxgrid;
       imprimirMatriz(grid);
    }
    end = GetTickCount();
    printf("soma %d\n", soma);
    printf("tempo decorrido: %d milissegundos\n", end - start);
    return 0;
}
void imprimirMatriz(float** matriz) {
    int soma=0;
    for (int i = 0; i < Tamanho; i++) {
        for (int j = 0; j < Tamanho; j++) {
            if (matriz[i][j] > 0)    {
                printf("%.1f|", matriz[i][j]);
                soma = soma + 1;
            }
            else {
                 printf("   |");
            }
        }
        printf("\n");
    }
    printf("soma: %d\n",soma);
    printf("\n");
}

float getNeighbords_media (float ** grid, int i,int j){
    float vizinhosVivos;
    int jmenos1, jmais1,imenos1,imais1;

    jmais1  = j + 1;
    jmenos1 = j - 1;
    imais1  = i + 1;
    imenos1 = i - 1;

    // A  | B  | C
    // D  |i,j | E
    // F  |G   | H

    if (jmais1 > Tamanho - 1 ){
        jmais1 = 0;
    }
    if (jmenos1 < 0){
        jmenos1 = Tamanho - 1;
    }
    if (imais1 > Tamanho - 1){
        imais1 = 0;
    }
    if (imenos1 < 0){
        imenos1 = Tamanho - 1;
    }

    vizinhosVivos = grid[imenos1][jmenos1] + grid[imenos1][j] + grid[imenos1][jmais1] +
                    grid[i][jmenos1]       +                    grid[i][jmais1] +
                    grid[imais1][jmenos1]  + grid[imais1][j]  + grid[imais1][jmais1];

return vizinhosVivos/2;}

int getNeighbords (float ** grid, int i,int j){
    int vizinhosVivos;
    int jmenos1, jmais1,imenos1,imais1;

    jmais1  = j + 1;
    jmenos1 = j - 1;
    imais1  = i + 1;
    imenos1 = i - 1;

    // A  | B  | C
    // D  |i,j | E
    // F  |G   | H

    if (jmais1 > Tamanho - 1 ){
        jmais1 = 0;
    }
    if (jmenos1 < 0){
        jmenos1 = Tamanho - 1;
    }
    if (imais1 > Tamanho - 1){
        imais1 = 0;
    }
    if (imenos1 < 0){
        imenos1 = Tamanho - 1;
    }

    vizinhosVivos = 0;
    if (grid[imenos1][jmenos1] > 0){vizinhosVivos = vizinhosVivos + 1 ;}
    if (grid[imenos1][j]> 0){vizinhosVivos = vizinhosVivos + 1 ;}
    if (grid[imenos1][jmais1]> 0){vizinhosVivos = vizinhosVivos + 1; }
    if (grid[i][jmenos1]> 0){vizinhosVivos = vizinhosVivos + 1 ;}
    if (grid[i][jmais1]> 0){vizinhosVivos = vizinhosVivos + 1 ;}
    if (grid[imais1][jmenos1] > 0){vizinhosVivos = vizinhosVivos + 1 ;}
    if (grid[imais1][j]> 0){vizinhosVivos = vizinhosVivos + 1 ;}
    if (grid[imais1][jmais1]> 0){vizinhosVivos = vizinhosVivos + 1 ;}

return vizinhosVivos;}



void *runThread (void *argumento){

    struct DadosThread* dados = (struct DadosThread*)argumento;
    int* soma = malloc(sizeof(int));
    int linha , coluna,vizinhosVivos;

    float** grid = dados->grid;
    float** newgrid = dados->newgrid;
    int id= dados->id;
    *soma = 0;


      for (linha = (Tamanho / Threads)*id - (Tamanho / Threads) ; linha < (Tamanho / Threads)*id ; linha++) {
              for (coluna = 0; coluna < Tamanho; coluna++) {

                     vizinhosVivos = getNeighbords(grid,linha,coluna);

                    // vivas
                      if (grid[linha][coluna] > 0 ){
                            if (vizinhosVivos == 2 || vizinhosVivos == 3 ) {
                            newgrid[linha][coluna] = grid[linha][coluna];
                            *soma = *soma + 1;
                            }else{
                            newgrid[linha][coluna] = 0.0;
                            }
                    // morta
                      }else {

                           if (vizinhosVivos == 3  ) {
                                newgrid[linha][coluna] = getNeighbords_media(grid,linha,coluna);
                                *soma = *soma + 1;
                           }else{
                                newgrid[linha][coluna] = 0.0;
                            }
                      }
                }
          }

  pthread_exit((void*)soma);
}
