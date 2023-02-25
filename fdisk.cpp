#ifndef FDISK_H
#define FDISK_H

#include <iostream>
#include <dirent.h>
#include "estructuras.cpp"
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


    void makeFdisk();
    void imprimir();
};

fdisk::fdisk(){
    this->unidad='K';
    this->type='P';
    this->fit="WF";
    this->add=0;
    this->borrar="";
}



void fdisk::imprimir(){
    cout << "path: " << this->ruta << endl;
    cout << "size: " << this->tamanio << endl;
    cout << "unit: " << this->unidad << endl;
    cout << "fit: " << this->fit << endl;

}

void fdisk::makeFdisk(){
    string path = this->ruta;
    cout<<"Creando disco en: "<<path<<endl;
    FILE *disco;
    disco=fopen(path.c_str(),"wb");
    if (disco==NULL){
        exit(1);
        cout<<"ERROR en creacion de disco \n"<<path;
    }
    fclose(disco);
    disco=fopen(path.c_str(),"ab");
    fseek(disco, 0, SEEK_SET);
    if (disco==NULL){
    exit(1);
    }

    char Pesokilo [1024];
    for(int j =0;j<1024;j++){
        Pesokilo[j]=0;
    }
    int tam ;
    if(tolower(this->unidad)=='k'){
    tam = tamanio;
    }
    else if(tolower(this->unidad)=='m'){
    tam = 1024* tamanio;
    }

    for(int i =0;i<tam;i++){
        fwrite(&Pesokilo, sizeof(Pesokilo), 1, disco);
    }
    fclose(disco);
    disco=fopen(path.c_str(),"ab");
    fseek(disco, 0, SEEK_SET);
    Mbr actual;
    actual.mbr_tamano=tamanio;
    time_t now;
    now = time(NULL);

    actual.mbr_fecha_creacion=now;
    actual.mbr_signature=rand()%1000;
    fwrite(&actual,sizeof(actual),1,disco);
}

#endif