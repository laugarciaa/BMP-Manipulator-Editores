#ifndef FUNCIONES_GRUPO_H_INCLUDED
#define FUNCIONES_GRUPO_H_INCLUDED

#include "funciones_campagna.h"
#include "funciones_alba.h"
#include "funciones_garcia.h"
#include "funciones_pagliettini.h"
#include <string.h>
#define EsNum(c) ( ( (c) >= '0' && (c) <= '9' ) ? 1 : 0 )
#define NOMBRE_GRUPO "EDITORES"

#define TODO_PIOLA 0
#define ERROR_ARCH 1
#define ERROR_MEM 2
#define ERROR_BMP 3

#pragma pack(push, 1)

typedef struct
{
    char bm[2];

    unsigned int tam;

    unsigned short reservado1;
    unsigned short reservado2;

    int ini;
    int tamHeader;

    unsigned int ancho;
    unsigned int alto;

    unsigned short nPlanos;
    unsigned short tamPunto;

    int compresion;

    unsigned int tamIMG;

    unsigned int resHor;
    unsigned int resVert;

    unsigned int tablaColor;
    unsigned int contColores;

}HeaderIMG;

typedef struct
{
    unsigned char b;

    unsigned char g;

    unsigned char r;

}Pixel;

#pragma pack(pop)

typedef struct
{
    HeaderIMG header;
    Pixel** pixeles;

}Imagen;

typedef struct
{
    // ===== IMAGENES =====

    char* imagenes[2];
    int cantidadImagenes;

    // ===== FILTROS BASICOS =====

    int negativo;
    int escalaGrises;

    int espejarHorizontal;
    int espejarVertical;

    // ===== FILTROS CON PARAMETROS =====

    int aumentarContraste;
    int valorAumentarContraste;

    int reducirContraste;
    int valorReducirContraste;

    int tonalidadAzul;
    int valorTonalidadAzul;

    int tonalidadVerde;
    int valorTonalidadVerde;

    int tonalidadRoja;
    int valorTonalidadRoja;

    int recortar;
    int valorRecortar;

    int achicar;
    int valorAchicar;

    // ===== ROTACIONES =====

    int rotarDerecha;
    int rotarIzquierda;

    // ===== CONCATENACIONES =====

    int concatenarHorizontal;
    int concatenarVertical;

    // ===== COMODIN = blurea la imagen =====

    int Blur;

    // ===== UTILIDADES =====

    int info;
    int validar;
    int help;

}Configuracion;

typedef void (*filtroSinValor)(Imagen *img);
typedef void (*filtroConValor)(Imagen *img, int valor);

// ===== FUNCIONES =====

Imagen* leerImagen(const char *rutaArch);
void liberarImagen(Imagen* img);
int obtenerValorConIgual(const char *arg, const char *clave, int *valorOut);
int parsearArgumentos(int argCount, char *argVec[], Configuracion *config);
Imagen* copiarImagen(const Imagen *origen);
int guardarImagen(const Imagen *img, const char *rutaSalida);
void armarNombreSalida(char *dest, int tamDest, const char *nombreOriginal, const char *nombreFiltro);
int llamarFiltroSinValor(Imagen *imgOriginal, const char *rutaOriginal, const char *nombreFiltro, filtroSinValor aplicar);
int llamarFiltroConValor(Imagen *imgOriginal, const char *rutaOriginal, const char *nombreFiltro, filtroConValor aplicar, int valor);
int Solucion(int argCount, char *argVec[]);

#endif // FUNCIONES_GRUPO_H_INCLUDED
