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

