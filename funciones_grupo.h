#ifndef FUNCIONES_GRUPO_H_INCLUDED
#define FUNCIONES_GRUPO_H_INCLUDED
#include "funciones_campagna.h"
#include "funciones_alba.h"
#include "funciones_garcia.h"
#include "funciones_pagliettini.h"


typedef struct{
    char[2] bm;

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
}HeaderIMG

typedef struct{
    unsigned char b;

    unsigned char r;

    unsigned char g;
}Pixel

typedef struct{
    HeaderIMG header;
    Pixel** pixeles
}Imagen

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

} Configuracion;

#endif // FUNCIONES_GRUPO_H_INCLUDED
