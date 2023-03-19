#ifndef RMDISK_H
#define RMDISK_H

#include <iostream>
using namespace std;


class rmdisk
{
public:
    rmdisk();
    string ruta;
    void removedisk();
};

rmdisk::rmdisk(){
    
}



void rmdisk::removedisk(){
    char i =0;
    cout<<"Desea eliminar el archivo? S/N"<<endl;
    cin.get(i);
    cin.get(i);
    if(toupper(i)=='S'){
        ifstream ejecutable;
        ejecutable.open(ruta,ios::in);
        if(ejecutable.fail()){
            cout<<"El archivo no existe"<<endl;
            return;
        }
        string limpiar ="rm -fr \""+this->ruta+"\"";
        cout<<limpiar<<endl;
        system(limpiar.c_str());
        
        
        cout<<"Eliminacion ejecutada"<<endl;
    }else{
        cout<<"Eliminacion cancelada"<<endl;
    }
    
}

#endif