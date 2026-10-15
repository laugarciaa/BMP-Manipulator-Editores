#include "funciones_grupo.h"
#include <stdio.h>
#include <stdlib.h>

Imagen* leerImagen(const char *rutaArch)
{
    FILE *ArchImg = fopen(rutaArch,"rb");
    if(ArchImg == NULL)
    {
        puts("Error al abrir archivo.");
        fclose(ArchImg);
        return NULL;
    }

    Imagen *img = (Imagen*)malloc(sizeof(Imagen));
    if(img == NULL)
    {
        puts("Error memoria.");
        free(img);
        fclose(ArchImg);
        return NULL;
    }

    fread(&(img->header), sizeof(HeaderIMG), 1, ArchImg);


    if(img->header.bm[0] != 'B' || img->header.bm[1] != 'M' || img->header.tamPunto != 24)
    {
        puts("Error, el archivo no es un BMP valido de 24 bits.");
        free(img);
        fclose(ArchImg);
        return NULL;
    }

    img->pixeles = (Pixel**)malloc(img->header.alto * sizeof(Pixel*));

    int padding = (4 - (img->header.ancho * sizeof(Pixel)) %4 ) %4;

    for(int i = 0; i < img->header.alto; i++)
    {
        img->pixeles[i] = (Pixel*)malloc(img->header.ancho * sizeof(Pixel));

        fread(img->pixeles[i], sizeof(Pixel), img->header.ancho, ArchImg);

        if(padding > 0)
        {
            unsigned char basura[3];
            fread(basura, sizeof(unsigned char), padding, ArchImg);
        }
    }

    fclose(ArchImg);
    return img;
}

void liberarImagen(Imagen* img)
{
    if (img == NULL)
        return;

    // liberar el contenido de cada fila
    for (int i = 0; i < img->header.alto; i++)
        free(img->pixeles[i]);

    // liberar la columna vertebral (el array de punteros)
    free(img->pixeles);

    // liberar la estructura principal
    free(img);
}
