#include "funciones_garcia.h"
#include "funciones_grupo.h"
#include <stdio.h>
#include <stdlib.h>

void filtroTonalidadAzul(Imagen *copia, int valor)
{
    int i, j;
    unsigned int alto = copia->header.alto;
    unsigned int ancho = copia->header.ancho;

    for(i = 0; i < alto; i++)
    {
        for(j = 0; j < ancho; j++)
        {
            int nuevoAzul = copia->pixeles[i][j].b + (copia->pixeles[i][j].b * valor) / 100;

            if(nuevoAzul > 255)
                nuevoAzul = 255;

            copia->pixeles[i][j].b = (unsigned char)nuevoAzul;
        }
    }
}

void filtroEspejarHorizontal(Imagen *copia)
{
    int i, j;
    unsigned int alto = copia->header.alto;
    unsigned int ancho = copia->header.ancho;

    for(i = 0; i < alto; i++)
    {
        for(j = 0; j < ancho / 2; j++)
        {
            Pixel aux;
            aux = copia->pixeles[i][j];
            copia->pixeles[i][j] = copia->pixeles[i][ancho-1-j];
            copia->pixeles[i][ancho-1-j] = aux;
        }
    }
}

void filtroEspejarVertical(Imagen *copia)
{
    int i;
    unsigned int alto = copia->header.alto;

    for(i = 0; i < alto / 2; i++)
    {
            Pixel *aux;
            aux = copia->pixeles[i];
            copia->pixeles[i] = copia->pixeles[alto-1-i];
            copia->pixeles[alto-1-i] = aux;

    }
}

void filtroRotarDerecha(Imagen *copia)
{
    int i, j;
    unsigned int altoOriginal = copia->header.alto;
    unsigned int anchoOriginal = copia->header.ancho;

    Pixel** matrizDimensionesInvertidas = (Pixel**)malloc(anchoOriginal * sizeof(Pixel*));
    if(matrizDimensionesInvertidas == NULL)
        return;

    for(i = 0; i < anchoOriginal; i++)
    {
        matrizDimensionesInvertidas[i] = (Pixel*)malloc(altoOriginal * sizeof(Pixel));

        if(matrizDimensionesInvertidas[i] == NULL)
        {
            for(int k = 0; k < i; k++)
                free(matrizDimensionesInvertidas[k]);

            free(matrizDimensionesInvertidas);
        }
    }

    for(i = 0; i < altoOriginal; i++)
        for(j = 0; j < anchoOriginal; j++)
            matrizDimensionesInvertidas[anchoOriginal - 1 - j][i] = copia->pixeles[i][j];


    for(i = 0; i < altoOriginal; i++)
        free(copia->pixeles[i]);

    free(copia->pixeles);

    copia->pixeles = matrizDimensionesInvertidas;
    copia->header.alto = anchoOriginal;
    copia->header.ancho = altoOriginal;
}

void filtroRecortar(Imagen *copia, int valor)
{
    int i,j;

    unsigned int altoOriginal = copia->header.alto;
    unsigned int anchoOriginal = copia->header.ancho;

    unsigned int nuevoAlto = 0;
    unsigned int nuevoAncho = 0;

    if(valor)
    {
        nuevoAlto = (copia->header.alto * valor) / 100;
        nuevoAncho = (copia->header.ancho * valor) / 100;
    }
    else
    {
        nuevoAlto = altoOriginal;
        nuevoAncho = anchoOriginal;
    }

    if(nuevoAlto == 0)
        nuevoAlto = 1;
    if(nuevoAncho == 0)
        nuevoAncho = 1;

    Pixel** nuevaMatriz = (Pixel**)malloc(nuevoAlto * sizeof(Pixel*));
    if(nuevaMatriz == NULL)
        free(nuevaMatriz);

    for(i = 0; i < nuevoAlto; i++)
    {
        nuevaMatriz[i] = (Pixel*)malloc(nuevoAncho * sizeof(Pixel));

        if(nuevaMatriz[i] == NULL)
        {
            for(int k = 0; k < i; k++)
                free(nuevaMatriz[k]);

            free(nuevaMatriz);
        }
    }

    for(i = 0; i < nuevoAlto; i++)
        for(j = 0; j < nuevoAncho; j++)
            nuevaMatriz[i][j] = copia->pixeles[i][j];

    for(i = 0; i < copia->header.alto; i++)
        free(copia->pixeles[i]);

    free(copia->pixeles);

    copia->pixeles = nuevaMatriz;
    copia->header.alto = nuevoAlto;
    copia->header.ancho = nuevoAncho;

}
