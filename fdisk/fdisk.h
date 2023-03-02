#ifndef FDISK_H
#define FDISK_H

#include <iostream>
#include <dirent.h>
using namespace std;


class fdisk
{
public:
    fdisk();
    int tamanio;
    string ruta;
    string name;
    char unidad ;
    char type;
    string fit;
    string borrar;
    int add;


    void makePrimaria();
    void makeExtendida();
    void makeLogica();
    void imprimir();
};






#endif