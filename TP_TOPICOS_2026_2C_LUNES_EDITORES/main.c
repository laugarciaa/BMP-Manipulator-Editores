#include <stdio.h>
#include <stdlib.h>
#include "../Lib_BMP/funciones_grupo.h"
int main()
{
    Imagen *imagenPrueba = leerImagen("unlam.bmp");

    if(imagenPrueba != NULL)
    {
        puts("VAMO LO PIBEEEEEEEEEEEEEEEEEEEEEEEEE");
        printf("La imagen tiene una resolucion de %dx%d pixeles",imagenPrueba->header.alto,imagenPrueba->header.ancho);
    }

    puts("TUKEN SOLTA MI CANICHE");

    liberarImagen(imagenPrueba);
    return 0;
}
