/* Beatriz Barbosa Santos Nascimento */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#define   Threads  8
#define   Ngeracao  4
#define   N  50


float getNeighbords_media (float ** grid, int i,int j);
int  getNeighbords (float ** grid, int i,int j);
void imprimirMatriz(float** matriz);



int main()
{
    int aux,aux1,aux2,aux3,aux4;
    float **auxgrid ;
    int vizinhosVivos;
    int soma;
    double start, end;
    int threadID;


    float **grid = (float **)malloc(N* sizeof(float *));
    float **newgrid = (float **)malloc(N* sizeof(float *));


    for (aux = 0; aux < N; aux++) {
        grid[aux] = (float *)malloc(N * sizeof(float));
        newgrid[aux] = (float *)malloc(N * sizeof(float));

      }

    for (aux1 = 0; aux1 < N; aux1++) {
         for (aux2 = 0; aux2 < N; aux2++) {
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

    start = omp_get_wtime();
    for (aux = 0; aux < Ngeracao; aux++) {

        soma = 0;
        #pragma omp parallel private (aux1,aux2,vizinhosVivos,threadID) shared (newgrid,grid) num_threads(Threads) reduction (+:soma)
        {
        #pragma omp for

           for (aux1 = 0; aux1 < N; aux1++) {
              for (aux2 = 0; aux2 < N; aux2++) {

                    vizinhosVivos = getNeighbords(grid,aux1,aux2);

                    // vivas
                      if (grid[aux1][aux2] > 0 ){
                            if (vizinhosVivos == 2 || vizinhosVivos == 3 ) {
                            newgrid[aux1][aux2] = grid[aux1][aux2];
                            soma = soma + 1;
                            }else{
                            newgrid[aux1][aux2] = 0.0;
                            }
                    // morta
                      }else {

                           if (vizinhosVivos == 3  ) {
                                newgrid[aux1][aux2] = getNeighbords_media(grid,aux1,aux2);
                                soma = soma + 1;
                           }else{
                                newgrid[aux1][aux2] = 0.0;
                            }
                      }
                }
          }

        } // fim pragma


    auxgrid = grid;
    grid = newgrid;
    newgrid = auxgrid;
    imprimirMatriz(grid);
    }
    end = omp_get_wtime();
    printf(" soma : %d\n",soma );
    printf(" took %lf seconds.\n", end-start);


    return 0;
}
void imprimirMatriz(float** matriz) {
int soma=0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
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

    if (jmais1 > N - 1 ){
        jmais1 = 0;
    }
    if (jmenos1 < 0){
        jmenos1 = N - 1;
    }
    if (imais1 > N - 1){
        imais1 = 0;
    }
    if (imenos1 < 0){
        imenos1 = N - 1;
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

    if (jmais1 > N - 1 ){
        jmais1 = 0;
    }
    if (jmenos1 < 0){
        jmenos1 = N - 1;
    }
    if (imais1 > N - 1){
        imais1 = 0;
    }
    if (imenos1 < 0){
        imenos1 = N - 1;
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

