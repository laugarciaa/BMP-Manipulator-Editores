#ifndef FUNCIONES_CAMPAGNA_H_INCLUDED
#define FUNCIONES_CAMPAGNA_H_INCLUDED

#include "funciones_grupo.h"

Imagen* concatenarHorizontal(Imagen* copia1,Imagen* copia2);
Imagen* concatenarVertical(Imagen* copia1,Imagen* copia2);
void filtroComodin(Imagen *copia);
void filtroReducirContraste(Imagen *copia, int porcentaje);
void filtroAumentarContraste(Imagen *copia, int porcentaje);

int limitarColor(float valor);
#endif // FUNCIONES_CAMPAGNA_H_INCLUDED
