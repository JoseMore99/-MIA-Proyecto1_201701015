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
    string limpiar ="rm -fr \""+this->ruta+"\"";
    cout<<limpiar<<endl;
    system(limpiar.c_str());
}

#endif