#include "funciones_grupo.h"
#include <stdio.h>
#include <stdlib.h>


Imagen* leerImagen(const char *rutaArch)
{
    FILE *ArchImg = fopen(rutaArch, "rb");

    if(ArchImg == NULL)
    {
        puts("Error al abrir archivo.");
        return NULL;
    }

    Imagen *img = (Imagen*)malloc(sizeof(Imagen));

    if(img == NULL)
    {
        puts("Error memoria.");
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

    /* Descartar los bytes extra del header (por ejemplo, BITMAPV5HEADER de 124 bytes).
       Se leen secuencialmente y se tiran, igual que se hace con el padding. */

    long bytesExtra = (long)img->header.ini - (long)sizeof(HeaderIMG);

    if(bytesExtra > 0)
    {
        unsigned char *relleno = (unsigned char*)malloc((size_t)bytesExtra);

        if(relleno != NULL)
        {
            fread(relleno, sizeof(unsigned char), (size_t)bytesExtra, ArchImg);
            free(relleno);
        }
    }

    img->pixeles = (Pixel**)malloc(img->header.alto * sizeof(Pixel*));

    if(img->pixeles == NULL)
    {
        puts("Error memoria.");
        free(img);
        fclose(ArchImg);
        return NULL;
    }

    int padding = (4 - (img->header.ancho * sizeof(Pixel)) % 4) % 4;

    for(unsigned int i = 0; i < img->header.alto; i++)
    {
        img->pixeles[i] = (Pixel*)malloc(img->header.ancho * sizeof(Pixel));

        if(img->pixeles[i] == NULL)
        {
            puts("Error memoria.");

            for(unsigned int j = 0; j < i; j++)
                free(img->pixeles[j]);

            free(img->pixeles);
            free(img);
            fclose(ArchImg);
            return NULL;
        }

        fread(img->pixeles[i], sizeof(Pixel), img->header.ancho, ArchImg);

        if(padding > 0)
        {
            unsigned char basura[4];
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

    const char *p = arg + lenClave + 1;

    if(*p == '\0')
        return 0;

    const char *q = p;

    while(*q != '\0')
    {
        if(!EsNum(*q))
            return 0;

        q++;
    }


    *valorOut = atoi(p);

    return 1;
}

int parsearArgumentos(int argc, char *argv[], Configuracion *config)
{
    if(config == NULL || argv == NULL)
        return ERROR_MEM;

    int i = 1;

    while(i < argc)
    {
        char *arg = argv[i];

        /*  Filtros SIN =  */
        if(strcmp(arg, "--negativo") == 0)
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
            else
            {
                char *esBmp = strrchr(arg,'.');

                if(esBmp != NULL)
                {
                    if(strcmp(esBmp + 1,"bmp") == 0)
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
                    else
                    {
                        printf("[AVISO] Formato incorrecto de imagen: '%s'.\n", arg);
                        i++;
                    }
                }
                else
                {
                    printf("[AVISO] Argumento desconocido o invalido: '%s'.\n", arg);
                    i++;
                }
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

int guardarImagen(const Imagen *img, const char *rutaSalida)
{
    if(img == NULL || rutaSalida == NULL)
        return ERROR_MEM;

    FILE *arch = fopen(rutaSalida, "wb");

    if(arch == NULL)
        return ERROR_ARCH;

    /* Normalizamos el header a formato clasico de 54 bytes:
       escribimos header de 54 bytes, asi que los datos
       de los pixeles empiezan justo despues. */
    HeaderIMG h = img->header;

    h.ini = (int)sizeof(HeaderIMG);
    h.tamHeader = 40;

    /* Recalculamos tamIMG y tam con las dimensiones actuales */

    unsigned int bytesFila = h.ancho * sizeof(Pixel);
    unsigned int pad = (4 - (bytesFila % 4)) % 4;

    h.tamIMG = (bytesFila + pad) * h.alto;
    h.tam = (unsigned int)sizeof(HeaderIMG) + h.tamIMG;

    fwrite(&h, sizeof(HeaderIMG), 1, arch);

    for(unsigned int i = 0; i < img->header.alto; i++)
    {
        fwrite(img->pixeles[i], sizeof(Pixel), img->header.ancho, arch);

        if(pad > 0)
        {
            unsigned char padByte = 0;

            for(unsigned int j = 0; j < pad; j++)
                fwrite(&padByte, sizeof(unsigned char), 1, arch);
        }
    }

    fclose(arch);
    return TODO_PIOLA;
}

void armarNombreSalida(char *dest, int tamDest, const char *nombreOriginal, const char *nombreFiltro)
{
    if(dest == NULL || nombreOriginal == NULL || nombreFiltro == NULL)
        return;

    snprintf(dest, tamDest, "%s_%s_%s", NOMBRE_GRUPO, nombreFiltro, nombreOriginal);
}

int llamarFiltroSinValor(Imagen *imgOriginal, const char *rutaOriginal, const char *nombreFiltro, filtroSinValor aplicar)
{
    if(imgOriginal == NULL || rutaOriginal == NULL || nombreFiltro == NULL || aplicar == NULL)
        return ERROR_MEM;

    /* copia la imagen original */
    Imagen *copia = copiarImagen(imgOriginal);

    if(copia == NULL)
        return ERROR_MEM;

    /* Aplicamos el filtro sobre la copia */
    aplicar(copia);

    /* Armamos el nombre y guardamos */
    char nombreSalida[256];

    armarNombreSalida(nombreSalida, sizeof(nombreSalida), rutaOriginal, nombreFiltro);

    int resGuardar = guardarImagen(copia, nombreSalida);

    if(resGuardar == TODO_PIOLA)
        printf("[OK] Generado: %s\n", nombreSalida);

    else
        printf("[ERROR] No se pudo guardar: %s\n", nombreSalida);

    /* Liberamos la copia */
    liberarImagen(copia);

    return resGuardar;
}

int llamarFiltroConValor(Imagen *imgOriginal, const char *rutaOriginal, const char *nombreFiltro, filtroConValor aplicar, int valor)
{
    if(imgOriginal == NULL || rutaOriginal == NULL || nombreFiltro == NULL || aplicar == NULL)
        return ERROR_MEM;

    /* Copiamos la imagen original */
    Imagen *copia = copiarImagen(imgOriginal);

    if(copia == NULL)
        return ERROR_MEM;

    /* Aplicamos el filtro con valor sobre la copia */
    aplicar(copia, valor);

    /* Armamos el nombre y guardamos */
    char nombreSalida[256];

    armarNombreSalida(nombreSalida, sizeof(nombreSalida), rutaOriginal, nombreFiltro);

    int resGuardar = guardarImagen(copia, nombreSalida);

    if(resGuardar == TODO_PIOLA)
        printf("[OK] Generado: %s\n", nombreSalida);

    else
        printf("[ERROR] No se pudo guardar: %s\n", nombreSalida);

    /* Liberamos la copia */
    liberarImagen(copia);

    return resGuardar;
}

int Solucion(int argc, char *argv[])
{
    Configuracion config = {0};

    parsearArgumentos(argc, argv, &config);

    /* ===================== CONCATENACION ===================== */
    if(config.concatenarHorizontal == 1 || config.concatenarVertical == 1)
    {
        if(config.cantidadImagenes == 2 && config.imagenes[0] != NULL && config.imagenes[1] != NULL)
        {
            Imagen *img1 = leerImagen(config.imagenes[0]);
            Imagen *img2 = leerImagen(config.imagenes[1]);

            if(img1 != NULL && img2 != NULL)
            {
                Imagen *imgResultado = NULL;
                char nombreSalida[256];

                if(config.concatenarHorizontal == 1)
                {
                    imgResultado = concatenarHorizontal(img1, img2);
                    armarNombreSalida(nombreSalida, sizeof(nombreSalida), config.imagenes[0], "concatenar-horizontal");
                }
                else if(config.concatenarVertical == 1)
                {
                    imgResultado = concatenarVertical(img1, img2);
                    armarNombreSalida(nombreSalida, sizeof(nombreSalida), config.imagenes[0], "concatenar-vertical");
                }

                if(imgResultado != NULL)
                {
                    if(guardarImagen(imgResultado, nombreSalida) == TODO_PIOLA)
                        printf("[OK] Generado: %s\n", nombreSalida);
                    else
                        printf("[ERROR] No se pudo guardar: %s\n", nombreSalida);

                    liberarImagen(imgResultado);
                }
                else
                {
                    printf("[ERROR] No se pudo generar la imagen concatenada.\n");
                }
            }
            else
            {
                printf("[ERROR] No se pudo leer alguna de las imagenes para concatenar.\n");
            }

            if(img1 != NULL) liberarImagen(img1);
            if(img2 != NULL) liberarImagen(img2);
        }

    }

    /* ===================== FILTROS NORMALES (A CADA IMAGEN) ===================== */
    if(config.cantidadImagenes == 0)
    {
        puts("[AVISO] No se especifico ninguna imagen BMP.");
        return TODO_PIOLA;
    }

    for(int j = 0; j < config.cantidadImagenes; j++)
    {
        if(config.imagenes[j] != NULL)
        {
            Imagen *imgOriginal = leerImagen(config.imagenes[j]);
            if(imgOriginal != NULL)
            {
                /* Filtros SIN valor */

                if(config.negativo == 1)
                    llamarFiltroSinValor(imgOriginal, config.imagenes[j], "negativo", filtroNegativo);

                if(config.escalaGrises == 1)
                    llamarFiltroSinValor(imgOriginal, config.imagenes[j], "escala-de-grises", filtroEscalaGrises);

                if(config.espejarHorizontal == 1)
                    llamarFiltroSinValor(imgOriginal, config.imagenes[j], "espejar-horizontal", filtroEspejarHorizontal);

                if(config.espejarVertical == 1)
                    llamarFiltroSinValor(imgOriginal, config.imagenes[j], "espejar-vertical", filtroEspejarVertical);

                if(config.rotarDerecha == 1)
                    llamarFiltroSinValor(imgOriginal, config.imagenes[j], "rotar-derecha", filtroRotarDerecha);

                if(config.rotarIzquierda == 1)
                    llamarFiltroSinValor(imgOriginal, config.imagenes[j], "rotar-izquierda", filtroRotarIzquierda);

                if(config.Blur == 1)
                    llamarFiltroSinValor(imgOriginal, config.imagenes[j], "comodin", filtroComodin);

            /* Filtros CON valor */

                /* if(config.aumentarContraste == 1)
                    llamarFiltroConValor(imgOriginal, config.imagenes[j], "aumentar-contraste", filtroAumentarContraste, config.valorAumentarContraste);

                if(config.reducirContraste == 1)
                    llamarFiltroConValor(imgOriginal, config.imagenes[j], "reducir-contraste", filtroReducirContraste, config.valorReducirContraste); */

                if(config.tonalidadAzul == 1)
                    llamarFiltroConValor(imgOriginal, config.imagenes[j], "tonalidad-azul", filtroTonalidadAzul, config.valorTonalidadAzul);

                if(config.tonalidadVerde == 1)
                    llamarFiltroConValor(imgOriginal, config.imagenes[j], "tonalidad-verde", filtroTonalidadVerde, config.valorTonalidadVerde);

                if(config.tonalidadRoja == 1)
                    llamarFiltroConValor(imgOriginal, config.imagenes[j], "tonalidad-roja", filtroTonalidadRoja, config.valorTonalidadRoja);

                if(config.recortar == 1)
                    llamarFiltroConValor(imgOriginal, config.imagenes[j], "recortar", filtroRecortar, config.valorRecortar);

                if(config.achicar == 1)
                    llamarFiltroConValor(imgOriginal, config.imagenes[j], "achicar", filtroAchicar, config.valorAchicar);

                liberarImagen(imgOriginal);
            }
            else
            {
                printf("[ERROR] No se pudo leer '%s'\n", config.imagenes[j]);
            }
        }
    }

    return TODO_PIOLA;
}

