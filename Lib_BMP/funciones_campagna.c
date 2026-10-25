#include "funciones_grupo.h"
#include "funciones_campagna.h"
#include <stdio.h>
#include <stdlib.h>

Imagen* concatenarHorizontal(Imagen* copia1,Imagen* copia2)
{
    int anchoFinal, altoFinal;
    anchoFinal = copia1->header.ancho + copia2->header.ancho;
    altoFinal = (copia1->header.alto > copia2->header.alto) ? copia1->header.alto : copia2->header.alto;

    Imagen* fin = malloc(sizeof(Imagen));
    if(fin == NULL)
        return NULL;
    fin->header = copia1->header;
    fin->header.ancho=anchoFinal;
    fin->header.alto=altoFinal;

    Pixel relleno = {240,180,80};

    fin->pixeles=malloc(altoFinal * sizeof(Pixel*));
    if(fin->pixeles == NULL)
    {
        free(fin);
        return NULL;
    }
    for(int i = 0; i < altoFinal; i++)
    {
        fin->pixeles[i]=malloc(anchoFinal * sizeof(Pixel));
        if(fin->pixeles[i] == NULL)
        {
            for(int j = 0; j < i; j++)
                free(fin->pixeles[j]);

            free(fin->pixeles);
            free(fin);
            return NULL;
        }
    }

    for(int i = 0; i < altoFinal; i++)
    {
        for(int j = 0; j < anchoFinal; j++)
        {
            if(j < copia1->header.ancho)
                fin->pixeles[i][j] = (i < copia1->header.alto) ? copia1->pixeles[i][j] : relleno;
            else
                fin->pixeles[i][j] = (i < copia2->header.alto) ? copia2->pixeles[i][j - copia1->header.ancho] : relleno;
        }
    }

    return fin;
}

Imagen* concatenarVertical(Imagen* copia1,Imagen* copia2)
{
    int anchoFinal, altoFinal;
    anchoFinal = (copia1->header.ancho > copia2->header.ancho) ? copia1->header.ancho : copia2->header.ancho;
    altoFinal = copia1->header.alto + copia2->header.alto;

    Imagen* fin = malloc(sizeof(Imagen));
    if(fin == NULL)
        return NULL;
    fin->header = copia1->header;
    fin->header.ancho=anchoFinal;
    fin->header.alto=altoFinal;

    Pixel relleno = {240,180,80};

    fin->pixeles=malloc(altoFinal * sizeof(Pixel*));
    if(fin->pixeles == NULL)
    {
        free(fin);
        return NULL;
    }
    for(int i = 0; i < altoFinal; i++)
    {
        fin->pixeles[i]=malloc(anchoFinal * sizeof(Pixel));
        if(fin->pixeles[i] == NULL)
        {
            for(int j = 0; j < i; j++)
                free(fin->pixeles[j]);

            free(fin->pixeles);
            free(fin);
            return NULL;
        }
    }

    for(int i = 0; i < altoFinal; i++)
    {
        for(int j = 0; j < anchoFinal; j++)
        {
            if(i < copia1->header.alto)
                fin->pixeles[i][j] = (j < copia1->header.ancho) ? copia1->pixeles[i][j] : relleno;
            else
                fin->pixeles[i][j] = (j < copia2->header.ancho) ? copia2->pixeles[i - copia1->header.alto][j] : relleno;
        }
    }

    return fin;
}

void filtroComodin(Imagen* copia)
{
    Imagen* copiac = copiarImagen(copia);
    if (copiac == NULL)
        return;

    int b, g, r;

    for (int i = 0; i < copia->header.alto; i++)
    {
        for (int j = 0; j < copia->header.ancho; j++)
        {
            b = 0;
            g = 0;
            r = 0;

            for (int f = -1; f <= 1; f++)
            {
                for (int c = -1; c <= 1; c++)
                {
                    if (f >= 0 && f < copia->header.alto &&
                        c >= 0 && c < copia->header.ancho)
                    {
                        b += copiac->pixeles[i + f][j + c].b;
                        g += copiac->pixeles[i + f][j + c].g;
                        r += copiac->pixeles[i + f][j + c].r;
                    }
                }
            }

            copia->pixeles[i][j].b = b / 3;
            copia->pixeles[i][j].g = g / 3;
            copia->pixeles[i][j].r = r / 3;
        }
    }

    liberarImagen(copiac);
}

void filtroAumentarContraste(Imagen* copia, int porcentaje)
{
    if(porcentaje < 0 || porcentaje > 100)
        return NULL;

    float factor = 1.0f + (porcentaje/100.0f);
    for(int i = 0; i < copia->header.alto; i++)
    {
        for(int j = 0; j < copia->header.ancho; j++)
        {
            copia->pixeles[i][j].b = limitarColor(factor * (copia->pixeles[i][j].b - 128) + 128);
            copia->pixeles[i][j].g = limitarColor(factor * (copia->pixeles[i][j].g - 128) + 128);
            copia->pixeles[i][j].r = limitarColor(factor * (copia->pixeles[i][j].r - 128) + 128);
        }
    }
}

void filtroReducirContraste(Imagen* copia, int porcentaje)
{
    if(porcentaje < 0 || porcentaje > 100)
        return NULL;

    float factor = 1.0f - (porcentaje/100.0f);
    for(int i = 0; i < copia->header.alto; i++)
    {
        for(int j = 0; j < copia->header.ancho; j++)
        {
            copia->pixeles[i][j].b = limitarColor(factor * (copia->pixeles[i][j].b - 128) + 128);
            copia->pixeles[i][j].g = limitarColor(factor * (copia->pixeles[i][j].g - 128) + 128);
            copia->pixeles[i][j].r = limitarColor(factor * (copia->pixeles[i][j].r - 128) + 128);
        }
    }
}

int limitarColor(float valor)
{
    if(valor < 0)
        return 0;
    if(valor > 255)
        return 255;
    return (int)valor;
}
