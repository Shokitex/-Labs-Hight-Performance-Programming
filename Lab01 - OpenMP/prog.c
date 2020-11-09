#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

FILE *arqA;
FILE *arqB;
FILE *arqC;
FILE *arqD;

//--------------FUNCAO PARA CRIAR NUMEROS ALEATORIOS--------------
float randFloat()
{
    int min = -10;
    int max = 10;

    return (rand()/((float)(RAND_MAX)+1))*(max-min)+min;
}

//--------------FUNCAO PARA CRIAR OS ARQUIVOS--------------
void createArquivo(int argc, char *argv[],int y, int w, int v){

float num = 0;//variavel auxiliar de gravacao de arquivo
float i;//variavel auxiliar do laco

arqA = fopen(argv[4], "w");
arqB = fopen(argv[5], "w");
arqC = fopen(argv[6], "w");

//Criar matrix A
int totalA = y*w;
for (i=0; i<totalA;i++){
    num = randFloat();
    fprintf( arqA, "%.2f\n" , num);
}

//Criar matrix B
int totalB = w*v;
for (i=0; i<totalB;i++){
    num = randFloat();
    fprintf( arqB, "%.2f\n" , num);
}

//Criar matrix C
for (i=0; i<v;i++){
    num = randFloat();
    fprintf( arqC, "%.2f\n" , num);
}

fclose(arqA);
fclose(arqB);
fclose(arqC);
}

//--------------FUNCAO PARA PRINCIPAL--------------
int main(int argc, char *argv[]){

int y = atoi(argv[1]);
int w = atoi(argv[2]);
int v = atoi(argv[3]);

int A = atoi(argv[4]);
int B = atoi(argv[5]);
int C = atoi(argv[6]);
int D = atoi(argv[7]);

int i,j,x;//contadores
float ave = 0.00;

float *matrizA;
float *matrizB;
float *matrizC;
float *matrizD;
float *matriz_aux;

matrizA = (float *) malloc(y * w * sizeof(float)); //aloca a matriz A
matrizB = (float *) malloc(w * v * sizeof(float)); //ALOCA B
matrizC = (float *) malloc(v * 1 * sizeof(float)); //ALOCA C
matriz_aux = (float *) malloc(y * v * sizeof(float));
matrizD = (float *) malloc(y * 1 * sizeof(float));

createArquivo(argc,argv,y,w,v);//chamando funcao que cria os arquivos

arqA = fopen(argv[4], "r");
for (i=0; i < y; i++){//lendo matriz A
   for (j=0; j < w; j++){
     fscanf(arqA, "%f", &matrizA[i * w + j]); // calcula a posiÃ§Ã£o de cada elemento
   }
}
fclose(arqA);

arqB = fopen(argv[5], "r");
for (i=0; i < w; i++){//lendo matriz B
   for (j=0; j < v; j++){
     fscanf(arqB, "%f", &matrizB[i * v + j]); // calcula a posiÃ§Ã£o de cada elemento
   }
}
fclose(arqB);

arqC = fopen(argv[6], "r");
for (i=0; i < v; i++){//lendo matriz C
   for (j=0; j < 1; j++){
     fscanf(arqC, "%f", &matrizC[i * 1 + j]); // calcula a posiÃ§Ã£o de cada elemento
   }
}
fclose(arqC);

//--------------COMECANDO OS CALCULOS--------------

clock_t Ticks[2];
Ticks[0] = clock();
#pragma omp parallel num_threads(4) private (x,j)
{

	#pragma omp for
        for(i = 0; i < y; i++){
            for(j = 0; j < v; j++){
                matriz_aux[i*v + j] = 0;
                for(x = 0; x < w; x++){
                   matriz_aux[i*v + j] =  matriz_aux[i*v + j] + (matrizA[i*w + x] * matrizB[x*v + j]);
                }
            }
        }

	 #pragma omp for reduction(+:ave)
        for(i = 0; i < y; i++){
            for(j = 0; j < 1; j++){
                matrizD[i*1 + j]=0;
                for(x = 0; x < v; x++){
                   matrizD[i*1 + j] = matrizD[i*1 + j] + (matriz_aux[i*v + x] * matrizC[x*1 + j]);
                }
                ave += matrizD[i*1 + j];
            }
        }
}
Ticks[1] = clock();
    double Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
    printf("Tempo gasto: %g ms.\n", Tempo);
    
arqD = fopen(argv[7], "w");
for (i=0; i < y; i++){
    for (j=0; j < 1; j++){
        fprintf(arqD, "%.2f\n", matrizD[i * 1 + j]);
    }
}
fclose(arqD);

printf("%.2f\n " , ave);

free(matrizA);
free(matrizB);
free(matrizD);
free(matrizC);
free(matriz_aux);

return 0;
}
