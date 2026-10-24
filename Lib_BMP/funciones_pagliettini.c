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
