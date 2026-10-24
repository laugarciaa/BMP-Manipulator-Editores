#include "funciones_grupo.h"
#include <stdio.h>
#include <stdlib.h>
#include "funciones_pagliettini.h"

void filtroTonalidadRoja(Imagen *copia, int valor)
{
    int i, j;
    unsigned int alto = copia->header.alto;
    unsigned int ancho = copia->header.ancho;

    for(i = 0; i < alto; i++)
    {
        for(j = 0; j < ancho; j++)
        {
            int nuevoRojo = copia->pixeles[i][j].r +
                           (copia->pixeles[i][j].r * valor) / 100;

            if(nuevoRojo > 255)
                nuevoRojo = 255;

            copia->pixeles[i][j].r = (unsigned char)nuevoRojo;
        }
    }
}

void filtroTonalidadVerde(Imagen *copia, int valor)
{
    int i, j;
    unsigned int alto = copia->header.alto;
    unsigned int ancho = copia->header.ancho;

    for(i = 0; i < alto; i++)
    {
        for(j = 0; j < ancho; j++)
        {
            int nuevoVerde = copia->pixeles[i][j].g +
                            (copia->pixeles[i][j].g * valor) / 100;

            if(nuevoVerde > 255)
                nuevoVerde = 255;

            copia->pixeles[i][j].g = (unsigned char)nuevoVerde;
        }
    }
}


void filtroNegativo(Imagen *copia)
{
    int i, j;
    unsigned int alto = copia->header.alto;
    unsigned int ancho = copia->header.ancho;

    for(i = 0; i < alto; i++)
    {
        for(j = 0; j < ancho; j++)
        {
            copia->pixeles[i][j].r =
                255 - copia->pixeles[i][j].r;

            copia->pixeles[i][j].g =
                255 - copia->pixeles[i][j].g;

            copia->pixeles[i][j].b =
                255 - copia->pixeles[i][j].b;
        }
    }
}

void filtroEscalaGrises(Imagen *copia)
{
    int i, j;
    int gris;

    unsigned int alto = copia->header.alto;
    unsigned int ancho = copia->header.ancho;

    for(i = 0; i < alto; i++)
    {
        for(j = 0; j < ancho; j++)
        {
            gris =
                (copia->pixeles[i][j].r +
                 copia->pixeles[i][j].g +
                 copia->pixeles[i][j].b) / 3;

            copia->pixeles[i][j].r = gris;
            copia->pixeles[i][j].g = gris;
            copia->pixeles[i][j].b = gris;
        }
    }
}




void filtroAchicar(Imagen *copia, int valor)
{
    unsigned int alto = copia->header.alto;
    unsigned int ancho = copia->header.ancho;

    unsigned int nuevoAlto =
        (alto * valor) / 100;

    unsigned int nuevoAncho =
        (ancho * valor) / 100;

    Pixel **nueva;

    int i,j;

    if(nuevoAlto == 0)
        nuevoAlto = 1;

    if(nuevoAncho == 0)
        nuevoAncho = 1;

    nueva = (Pixel**)malloc(nuevoAlto * sizeof(Pixel*));

    for(i = 0; i < nuevoAlto; i++)
    {
        nueva[i] = (Pixel*)malloc(nuevoAncho * sizeof(Pixel));
    }

    for(i = 0; i < nuevoAlto; i++)
    {
        for(j = 0; j < nuevoAncho; j++)
        {
            nueva[i][j] =
                copia->pixeles[
                    i * alto / nuevoAlto
                ][
                    j * ancho / nuevoAncho
                ];
        }
    }

    for(i = 0; i < alto; i++)
        free(copia->pixeles[i]);

    free(copia->pixeles);

    copia->pixeles = nueva;

    copia->header.alto = nuevoAlto;
    copia->header.ancho = nuevoAncho;
}




void filtroRotarIzquierda(Imagen *copia)
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
            int k;

            for(k = 0; k < i; k++)
                free(matrizDimensionesInvertidas[k]);

            free(matrizDimensionesInvertidas);
            return;
        }
    }

    for(i = 0; i < altoOriginal; i++)
    {
        for(j = 0; j < anchoOriginal; j++)
        {
            matrizDimensionesInvertidas[j][altoOriginal - 1 - i] =
                copia->pixeles[i][j];
        }
    }

    for(i = 0; i < altoOriginal; i++)
        free(copia->pixeles[i]);

    free(copia->pixeles);

    copia->pixeles = matrizDimensionesInvertidas;

    copia->header.alto = anchoOriginal;
    copia->header.ancho = altoOriginal;
}
