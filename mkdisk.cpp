#ifndef MKDISK_H
#define MKDISK_H

#include <iostream>
using namespace std;


class mkdisk
{
public:
    mkdisk();
    int tamanio;
    string ruta;
    string fit;

    char unidad ;
    void makedisk();
    void imprimir();

};

mkdisk::mkdisk(){
    this->unidad='M';
    this->fit="FF";
}

void mkdisk::imprimir(){
    cout<<"entro"<<endl;
    cout << "path: " << this->ruta << endl;
    cout << "size: " << this->tamanio << endl;
    cout << "unit: " << this->unidad << endl;
    cout << "fit: " << this->fit << endl;

}

void mkdisk::makedisk(){
    string path = this->ruta;
    cout<<"Creando disco en: \n"<<path<<endl;
    FILE *disco;
    disco=fopen(path.c_str(),"wb");
    if (disco==NULL){
        exit(1);
        cout<<"ERROR en creacion de disco \n"<<path;
    }
    fclose(disco);

    disco=fopen(path.c_str(),"ab");
    /*mbr actual;
    actual.mbr_tamanio=tamanio;
    time_t now;
    now = time(NULL);

    actual.mbr_fecha_creacion=now;
    actual.mbr_dsk_asignature=rand()%1000;
    fwrite(&actual,sizeof(actual),1,disco);*/
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
}

#endif