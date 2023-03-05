#include "mount.h"
#include "../estructuras.cpp"

#include <iostream>
#include <dirent.h>
#include <string.h>
#include <vector>
using namespace std;

int BuscarPart(Partition lista [], string nombe){
   int aux=-1;
    for (int i = 0; i < 4; i++){
        if(lista[i].part_name==nombe)
            return i;
    }
    return aux;
}

mount::mount(){
    
}

int mount::numeroParticion(){
    int bul = 0;
    FILE *disco;
    disco=fopen(this->path.c_str(),"rb+");
    fseek(disco, 0, SEEK_SET);
    if (disco==NULL)
        exit(1);
    Mbr aux;
    fread(&aux, sizeof(Mbr), 1, disco);
    fclose(disco);
    bul= BuscarPart(aux.mbr_partition,this->name);
    
    return bul;
}
