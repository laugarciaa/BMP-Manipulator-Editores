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

int obtenerValorConIgual(const char *arg, const char *clave, int *valorOut)
{
    size_t lenClave = strlen(clave);

    if(strncmp(arg, clave, lenClave) != 0)
        return 0;

    if(arg[lenClave] != '=')
        return 0;

    char *p = arg + lenClave + 1;

    if(*p == '\0')
        return 0;

    char *q = p;

    while(*q != '\0')
    {
        if(!EsNum(*q))
            return 0;

        q++;
    }


    *valorOut = atoi(p);

    return 1;
}

int parsearArgumentos(int argCount, char *argVec[], Configuracion *config)
{
    if(config == NULL || argVec == NULL)
        return ERROR_MEM;

    int i = 1;

    while(i < argCount)
    {
        char *arg = argVec[i];

        /*  Filtros SIN =  */
        if(strcmp(arg, "--help") == 0)
        {
            config->help = 1;
            i++;
        }
        else if(strcmp(arg, "--info") == 0)
        {
            config->info = 1;
            i++;
        }
        else if(strcmp(arg, "--validar") == 0)
        {
            config->validar = 1;
            i++;
        }
        else if(strcmp(arg, "--negativo") == 0)
        {
            config->negativo = 1;
            i++;
        }
        else if(strcmp(arg, "--escala-de-grises") == 0)
        {
            config->escalaGrises = 1;
            i++;
        }
        else if(strcmp(arg, "--espejar-horizontal") == 0)
        {
            config->espejarHorizontal = 1;
            i++;
        }
        else if(strcmp(arg, "--espejar-vertical") == 0)
        {
            config->espejarVertical = 1;
            i++;
        }
        else if(strcmp(arg, "--rotar-derecha") == 0)
        {
            config->rotarDerecha = 1;
            i++;
        }
        else if(strcmp(arg, "--rotar-izquierda") == 0)
        {
            config->rotarIzquierda = 1;
            i++;
        }
        else if(strcmp(arg, "--concatenar-horizontal") == 0)
        {
            config->concatenarHorizontal = 1;
            i++;
        }
        else if(strcmp(arg, "--concatenar-vertical") == 0)
        {
            config->concatenarVertical = 1;
            i++;
        }
        else if(strcmp(arg, "--comodin") == 0)
        {
            config->Blur = 1;
            i++;
        }
        else
        {
            int val = 0;

            /* Filtros con =NUMERO */
            if(obtenerValorConIgual(arg, "--aumentar-contraste", &val))
            {
                if(val < 0 || val > 100)
                {
                    printf("[AVISO] Valor fuera de rango (0-100) para --aumentar-contraste=%d. No aplica este filtro.\n", val);
                    i++;
                }
                else
                {
                    config->aumentarContraste = 1;
                    config->valorAumentarContraste = val;
                    i++;
                }
            }
            else if(obtenerValorConIgual(arg, "--reducir-contraste", &val))
            {
                if(val < 0 || val > 100)
                {
                    printf("[AVISO] Valor fuera de rango (0-100) para --reducir-contraste=%d. No aplica este filtro.\n", val);
                    i++;
                }
                else
                {
                    config->reducirContraste = 1;
                    config->valorReducirContraste = val;
                    i++;
                }
            }
            else if(obtenerValorConIgual(arg, "--tonalidad-azul", &val))
            {
                if(val < 0 || val > 100)
                {
                    printf("[AVISO] Valor fuera de rango (0-100) para --tonalidad-azul=%d. No aplica este filtro.\n", val);
                    i++;
                }
                else
                {
                    config->tonalidadAzul = 1;
                    config->valorTonalidadAzul = val;
                    i++;
                }
            }
            else if(obtenerValorConIgual(arg, "--tonalidad-verde", &val))
            {
                if(val < 0 || val > 100)
                {
                    printf("[AVISO] Valor fuera de rango (0-100) para --tonalidad-verde=%d. No aplica este filtro.\n", val);
                    i++;
                }
                else
                {
                    config->tonalidadVerde = 1;
                    config->valorTonalidadVerde = val;
                    i++;
                }
            }
            else if(obtenerValorConIgual(arg, "--tonalidad-roja", &val))
            {
                if(val < 0 || val > 100)
                {
                    printf("[AVISO] Valor fuera de rango (0-100) para --tonalidad-roja=%d. No aplica este filtro.\n", val);
                    i++;
                }
                else
                {
                    config->tonalidadRoja = 1;
                    config->valorTonalidadRoja = val;
                    i++;
                }
            }
            else if(obtenerValorConIgual(arg, "--recortar", &val))
            {
                if(val < 0 || val > 100)
                {
                    printf("[AVISO] Valor fuera de rango (0-100) para --recortar=%d. No aplica este filtro.\n", val);
                    i++;
                }
                else
                {
                    config->recortar = 1;
                    config->valorRecortar = val;
                    i++;
                }
            }
            else if(obtenerValorConIgual(arg, "--achicar", &val))
            {
                if(val < 0 || val > 100)
                {
                    printf("[AVISO] Valor fuera de rango (0-100) para --achicar=%d. No aplica este filtro.\n", val);
                    i++;
                }
                else
                {
                    config->achicar = 1;
                    config->valorAchicar = val;
                    i++;
                }
            }
            /* Rutas de imagen */
            else if(arg[0] != '-')
            {
                if(config->cantidadImagenes < 2)
                {
                    config->imagenes[config->cantidadImagenes] = arg;
                    config->cantidadImagenes++;
                    i++;
                }
                else
                {
                    printf("[AVISO] Se ingresaron mas de 2 imagenes. No aplica '%s'.\n", arg);
                    i++;
                }
            }
            /* Empieza con '-' pero no es -- exacto ni filtro válido */
            else
            {
                printf("[AVISO] Argumento desconocido o invalido: '%s'.\n", arg);
                i++;
            }
        }
    }

    /* Validación para concatenación */
    if((config->concatenarHorizontal == 1 || config->concatenarVertical == 1))
    {
        if(config->cantidadImagenes < 2)
        {
            printf("[AVISO] Para concatenar se requieren 2 imagenes. No se aplicara la concatenacion.\n");
            config->concatenarHorizontal = 0;
            config->concatenarVertical = 0;
        }
    }

    return TODO_PIOLA;
}

Imagen* copiarImagen(const Imagen *origen)
{
    if(origen == NULL)
        return NULL;

    Imagen *copia = (Imagen*)malloc(sizeof(Imagen));
    if(copia == NULL)
        return NULL;

    /* Copiar header tal cual */
    copia->header = origen->header;

    /* Reservar matriz de punteros */
    copia->pixeles = (Pixel**)malloc(copia->header.alto * sizeof(Pixel*));
    if(copia->pixeles == NULL)
    {
        free(copia);
        return NULL;
    }

    /* Copiar cada fila */
    for(int i = 0; i < copia->header.alto; i++)
    {
        copia->pixeles[i] = (Pixel*)malloc(copia->header.ancho * sizeof(Pixel));
        if(copia->pixeles[i] == NULL)
        {
            /* Liberar lo que ya se haya reservado si falla */
            for(int j = 0; j < i; j++)
                free(copia->pixeles[j]);
            free(copia->pixeles);
            free(copia);
            return NULL;
        }
        /* Copiar fila completa */
        memcpy(copia->pixeles[i], origen->pixeles[i], copia->header.ancho * sizeof(Pixel));
    }

    return copia;
}
