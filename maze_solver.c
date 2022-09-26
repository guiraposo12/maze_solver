#include "trabalho3.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// normaliza a imagem (tratamento)
void normalizaImagem(Imagem1C *m){
    int
    i = 0,
    j = 0,
    menor = 255,
    maior = 0;

    for (i = 0; i < m->largura; i++){
        menor = 255, maior = 0;
        for (j = 0; j < m->altura; j++){
            if (m->dados[j][i] > maior)
                maior = m->dados[j][i];
            if (m->dados[j][i] < menor)
                menor = m->dados[j][i];
        }
        for (j = 0; j < m->altura; j++)
            m->dados[j][i] = (m->dados[j][i]-menor)*(255/(maior-menor));
    }

}

// função principal que encontra o caminho e salva as coordenadas em vetor
int encontraCaminho (Imagem1C* img, Coordenada** caminho){
    int corte, i, n_passos = 0, distancia;
    int inicio, pos_y = -1, pos_x = 0;
    int **matriz;
    int naoAchou = 0;
    normalizaImagem(img);
    corte = calculaInicio(img,&inicio);
    matriz = alocaMatriz(img);

    while(pos_y == -1){
        distancia = arrumaMatriz(img, corte, matriz);
        for (i = inicio - 5; i < inicio + 5; i ++){
            if(matriz[i][0] <= distancia && matriz[i][0] > 0){
                pos_y = i;
                distancia = matriz[i][0];
            }
            if(matriz[i][0] == -1){
                naoAchou = 1;
            }
        }
        corte += 5;
        if(corte >= 255)
            pos_y = inicio;
    }
    printf("%d %d 2\n", distancia, corte);
    n_passos = distancia + 1;
    *caminho = (Coordenada*) malloc(n_passos * sizeof(Coordenada));
    (*caminho)[0].x = 0;
    (*caminho)[0].y = pos_y;
    pos_x = (*caminho)[0].x;
    pos_y = (*caminho)[0].y;
    printf("%d %d \n", distancia, corte);
    for(i = 1; i < n_passos; i++){
        if((pos_x != img->largura) && matriz[pos_y][pos_x + 1] == (distancia - 1)){
            (*caminho)[i].x = (*caminho)[i-1].x + 1;
            (*caminho)[i].y = (*caminho)[i-1].y;
        }
        else if((pos_y != img->altura) && matriz[pos_y + 1][pos_x] == distancia - 1){
            (*caminho)[i].x =  (*caminho)[i-1].x;
            (*caminho)[i].y = (*caminho)[i-1].y +1;
        }
        else if((pos_x != 0) && matriz[pos_y][pos_x - 1] == distancia - 1){
            (*caminho)[i].x = (*caminho)[i-1].x - 1;
            (*caminho)[i].y = (*caminho)[i-1].y;
        }
        else if(matriz[pos_y - 1][pos_x] == distancia - 1){
            (*caminho)[i].x = (*caminho)[i-1].x;
            (*caminho)[i].y = (*caminho)[i-1].y -1;
        }
        else{
            printf("ops \n");
            (*caminho)[i].x = (*caminho)[i-1].x + 1;
            (*caminho)[i].y = (*caminho)[i-1].y;
        }
        distancia = distancia - 1;
        pos_y = (*caminho)[i].y;
        pos_x = (*caminho)[i].x;
    }
    for(i = 0; i < img->altura; i ++)
        free(matriz[i]);
    free(matriz);
    return n_passos;
}

//identifica o começo do labirinto 
int calculaInicio(Imagem1C* img, int* comeco){
    int i, menor = 255, corte = 100;

    for(i = 0; i < img->altura; i ++)
        if (img->dados[i][0] < menor){
                menor = img->dados[i][0];
                *comeco = i;
        }
    return corte;
}

//reestrutura a matriz de imagem para o desenho do caminho
int arrumaMatriz(Imagem1C* img, int corte, int** matriz){
    int i, j, distancia = 0, achou = 1, inicio;
    for (i = 0; i < img->altura; i++){
        for (j = 0; j < img->largura; j ++){
            if (img->dados[i][j] <= corte){
                matriz[i][j] = -1;
                if (i!= 0)
                    matriz[i-1][j] = -1;
                if (i!= img->altura -1)
                    matriz[i+1][j] = -1;
                if (j != 0)
                    matriz[i][j-1] = -1;
                if (j != img->largura -1)
                    matriz[i][j+1] = -1;
            }
            else
                matriz[i][j] = -2;
        }
    }
    for(i = 0; i < img->altura; i++){
        if(matriz[i][(img->largura) -1] == -1){
            matriz[i][(img->largura)-1] = distancia;
        }
    }
    while(achou){
        achou = 0;
        for(i = 0; i < img->altura; i++){
            for(j = 0; j < img->largura; j++){
                if(matriz[i][j] == distancia){
                    if (i != img->altura - 1)
                        if(matriz[i+1][j] == -1){
                            matriz[i+1][j] = distancia + 1;
                            achou = 1;
                        }
                    if(i != 0)
                        if(matriz[i-1][j] == -1){
                            matriz[i-1][j] = distancia + 1;
                            achou = 1;
                        }
                    if(j != img->largura)
                        if(matriz[i][j+1] == -1){
                            matriz[i][j+1] = distancia + 1;
                            achou = 1;
                        }
                    if(j != 0)
                        if(matriz[i][j-1] == -1){
                            matriz[i][j-1] = distancia + 1;
                            achou = 1;
                        }
                }
            }
        }
        if(achou)
            distancia++;
    }
    return distancia;
}

// alocar matriz de imagem 1C
int** alocaMatriz(Imagem1C* img){
    int i,j;
    int **matriz;
    matriz = (int**)malloc(img->altura * sizeof(int*));
    for(i = 0; i < img->altura; i++)
        matriz[i] = (int*) malloc(img->largura * sizeof(int));

    return matriz;
}
