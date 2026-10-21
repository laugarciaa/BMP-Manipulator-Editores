#include <stdio.h>
#include <stdlib.h>
#include "../Lib_BMP/funciones_grupo.h"

int main(int argCount, char *argVec[])
{
    Configuracion config = {0};

    parsearArgumentos(argCount, argVec, &config);

    if(config.help == 1)
    {
        puts("Uso: bmpmanipuleitor [--filtros] imagen1.bmp [imagen2.bmp]");
        return TODO_PIOLA;
    }

    if(config.cantidadImagenes >= 1 && config.imagenes[0] != NULL)
    {
        Imagen *img = leerImagen(config.imagenes[0]);
        if(img == NULL)
        {
            printf("[ERROR] No se pudo leer '%s'\n", config.imagenes[0]);
            return ERROR_ARCH;
        }
        printf("[OK] Imagen cargada: %dx%d\nNombre imagen: %s\n", img->header.ancho, img->header.alto,config.imagenes[0]);
        /* Aplicar filtros que estén en 1 y generar NOMBRE_GRUPO_<filtro>_<nombreOriginal>.bmp */
        liberarImagen(img);
    }

    if((config.concatenarHorizontal == 1 || config.concatenarVertical == 1) && config.cantidadImagenes == 2)
    {
        /* Leer ambas imagenes y concatenar */
    }

    return TODO_PIOLA;
}
