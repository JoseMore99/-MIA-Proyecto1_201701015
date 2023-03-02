
#include "../estructuras.cpp"
#include "fdisk.h"

#include <iostream>
#include <dirent.h>
#include <string.h>
using namespace std;

int EspacioUsado(Partition lista []){
    int resultado=sizeof(Mbr);
    for (int i = 0; i < 4; i++){
        if(lista[i].part_s!=-1)
        resultado+= lista[i].part_s;
    }
    return resultado;
}

int BuscarPartition(Partition lista [], string nombe){
   int aux=-1;
    for (int i = 0; i < 4; i++){
        if(lista[i].part_name==nombe)
            return i;
    }
    return aux;
}

int PocisionLibreFF(Partition lista [],int size, int sizembr){
    int pos=0;
    for (int i = 0; i < 4; i++){
        pos = i;
        if(lista[i].part_name[0]=='\0'){
            if (lista[i].part_s==-1){
                if(sizembr-EspacioUsado(lista)>=size){
                    return pos;
                }
                
            }else{
                if(lista[i].part_s>=size){
                    return pos;
                }
                continue;
            }
        }
           
    }
    pos =-1;
    return pos;
}
int PocisionLibreWF(Partition lista [],int size, int sizembr){
    int pos=-1;
    int MGrande=0;
    for (int i = 0; i < 4; i++){
        if(lista[i].part_name[0]=='\0'){
            if (lista[i].part_s==-1){
                if(sizembr-EspacioUsado(lista)>=size){
                    return i;
                }
                
            }else{
                if(lista[i].part_s>=size){
                    if(MGrande<lista[i].part_s){
                        MGrande=lista[i].part_s;
                        pos=i;
                    }
                }
                continue;
            }
        }
           
    }
    return pos;
}
int PocisionLibreBF(Partition lista [],int size, int sizembr){
    int pos=-1;
    int MPequeño=sizembr;
    for (int i = 0; i < 4; i++){
        if(lista[i].part_name[0]=='\0'){
            if (lista[i].part_s==-1){
                if(sizembr-EspacioUsado(lista)>=size){
                    return i;
                }
                
            }else{
                if(lista[i].part_s>=size){
                    if(MPequeño>lista[i].part_s){
                        MPequeño=lista[i].part_s;
                        pos=i;
                    }
                    
                }
                continue;
            }
        }
           
    }
    if(MPequeño==sizembr){
        pos=-1;
    }
    return pos;
}

bool ExisteExtendida(Partition lista[]){
    for (int i = 0; i < 4; i++){
        if(lista[i].part_type=='E')
            return true;
    }
    return false;
}

Partition buscarExtendida(Partition lista[]){
    Partition buscador;
    for (int i = 0; i < 4; i++){
        if(lista[i].part_type=='E')
            return lista[i];
    }
    return buscador;
}

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


void fdisk::makePrimaria(){
    string path = this->ruta;
    FILE *disco;
    disco=fopen(path.c_str(),"rb+");
    fseek(disco, 0, SEEK_SET);
    if (disco==NULL)
        exit(1);
    Mbr aux;
     
    fread(&aux, sizeof(Mbr), 1, disco);
    fclose(disco);
    if (this->borrar=="full"){
         cout<<"Eliminando una particion en: "<<path<<endl;
        string vaciar="";
        int seleccion = BuscarPartition(aux.mbr_partition,this->name);
        strcpy(aux.mbr_partition[seleccion].part_name,vaciar.c_str());
        for(Partition x: aux.mbr_partition){
        cout<<x.part_name<<" pp "<<x.part_s<<endl;
        }
        disco=fopen(path.c_str(),"rb+");
        fseek(disco, 0, SEEK_SET);
        fwrite(&aux,sizeof(Mbr),1,disco);
        fclose(disco);
        return;
    }
    //printf("Tamanio: %i  fecha: %s id: %i fit: %c \n", aux.mbr_tamano, ctime(&aux.mbr_fecha_creacion), aux.mbr_signature,aux.dsk_fit);
    cout<<"Haciendo una particion en: "<<path<<endl;
    Partition nueva;
    int size = this->tamanio;
    if(this->unidad!='B'){
        if(this->unidad=='K')
            size = size *1024;
        if(this->unidad=='M')
            size = size *1024*1024;
    }
    int pos = -1;
    if(aux.dsk_fit=='F'){
    pos = PocisionLibreFF(aux.mbr_partition,size,aux.mbr_tamano);
    }
    if(aux.dsk_fit=='W'){
    pos = PocisionLibreWF(aux.mbr_partition,size,aux.mbr_tamano);
    }
    if(aux.dsk_fit=='B'){
    pos = PocisionLibreBF(aux.mbr_partition,size,aux.mbr_tamano);
    }
    if(pos ==-1){
        cout<<"no hay espacio disponible Para la particion"<<endl;
        return;
    }
    cout<<"pos "<<pos<<endl;
    nueva.part_fit = this->fit[0];
    strcpy(nueva.part_name, this->name.c_str());
    nueva.part_s = size;
    if(pos ==0){
        nueva.part_start=sizeof(Mbr);
    }else{
        int anterior = pos-1;
        nueva.part_start=aux.mbr_partition[anterior].part_start+aux.mbr_partition[anterior].part_s;
    }
    nueva.part_status='0';
    nueva.part_type = this->type;
    aux.mbr_partition[pos]= nueva;
    for(Partition x: aux.mbr_partition){
        cout<<x.part_name<<" pp "<<x.part_start<<endl;
    }
    disco=fopen(path.c_str(),"rb+");
    fseek(disco, 0, SEEK_SET);
    fwrite(&aux,sizeof(Mbr),1,disco);
    fclose(disco);
}

void fdisk::makeExtendida(){
    string path = this->ruta;
    FILE *disco;
    disco=fopen(path.c_str(),"rb+");
    fseek(disco, 0, SEEK_SET);
    if (disco==NULL)
        exit(1);
    Mbr aux;
    fread(&aux, sizeof(Mbr), 1, disco);
    fclose(disco);
    if(ExisteExtendida(aux.mbr_partition)){
        cout<<"Ya existe una particion extendida en este disco"<<endl;
        return;
    }
    if (this->borrar=="full"){
         cout<<"Eliminando una particion en: "<<path<<endl;
        string vaciar="";
        int seleccion = BuscarPartition(aux.mbr_partition,this->name);
        strcpy(aux.mbr_partition[seleccion].part_name,vaciar.c_str());
        for(Partition x: aux.mbr_partition){
        cout<<x.part_name<<" pp "<<x.part_s<<endl;
        }
        disco=fopen(path.c_str(),"rb+");
        fseek(disco, 0, SEEK_SET);
        fwrite(&aux,sizeof(Mbr),1,disco);
        fclose(disco);
        return;
    }
    //printf("Tamanio: %i  fecha: %s id: %i \n", aux.mbr_tamano, ctime(&aux.mbr_fecha_creacion), aux.mbr_signature);
    cout<<"Haciendo una particion Extendida en: "<<path<<endl;
    Partition nueva;
    int size = this->tamanio;
    if(this->unidad!='B'){
        if(this->unidad=='K')
            size = size *1024;
        if(this->unidad=='M')
            size = size *1024*1024;
    }
    int pos = -1;
    if(aux.dsk_fit=='F'){
    pos = PocisionLibreFF(aux.mbr_partition,size,aux.mbr_tamano);
    }
    if(aux.dsk_fit=='W'){
    pos = PocisionLibreWF(aux.mbr_partition,size,aux.mbr_tamano);
    }
    if(aux.dsk_fit=='B'){
    pos = PocisionLibreBF(aux.mbr_partition,size,aux.mbr_tamano);
    }
    if(pos ==-1){
        cout<<"no hay espacio disponible Para la particion"<<endl;
        return;
    }
    nueva.part_fit = this->fit[0];
    strcpy(nueva.part_name, this->name.c_str());
    nueva.part_s = size;
    if(pos ==0){
        nueva.part_start=sizeof(Mbr);
    }else{
        int anterior = pos-1;
        nueva.part_start=aux.mbr_partition[anterior].part_start+aux.mbr_partition[anterior].part_s;
    }
    nueva.part_status='0';
    nueva.part_type = this->type;
    aux.mbr_partition[pos]= nueva;
    for(Partition x: aux.mbr_partition){
        cout<<x.part_name<<" pp "<<x.part_start<<endl;
    }
    disco=fopen(path.c_str(),"rb+");
    fseek(disco, 0, SEEK_SET);
    fwrite(&aux,sizeof(Mbr),1,disco);
    Ebr inicio;
    inicio.part_s=0;
    inicio.part_start=nueva.part_start;
    inicio.part_next=-1;
    inicio.part_fit='F';
    inicio.part_status='0';
    fseek(disco,nueva.part_start,SEEK_SET);
    fwrite(&inicio,sizeof(Ebr),1,disco);
    fclose(disco);
}

void fdisk::makeLogica(){
    string path = this->ruta;
    FILE *disco;
    disco=fopen(path.c_str(),"rb+");
    fseek(disco, 0, SEEK_SET);
    if (disco==NULL)
        exit(1);
    Mbr aux;
    fread(&aux, sizeof(Mbr), 1, disco);
    if(!ExisteExtendida(aux.mbr_partition)){
        cout<<"No existe una particion extendida en este disco"<<endl;
        return;
    }
    Partition Aqui = buscarExtendida(aux.mbr_partition);
    if (this->borrar=="full"){
         cout<<"Eliminando una particion en: "<<path<<endl;
        string vaciar="";
        int seleccion = BuscarPartition(aux.mbr_partition,this->name);
        strcpy(aux.mbr_partition[seleccion].part_name,vaciar.c_str());
        for(Partition x: aux.mbr_partition){
        cout<<x.part_name<<" pp "<<x.part_s<<endl;
        }
        disco=fopen(path.c_str(),"rb+");
        fseek(disco, 0, SEEK_SET);
        fwrite(&aux,sizeof(Mbr),1,disco);
        fclose(disco);
        return;
    }
    //printf("Tamanio: %i  fecha: %s id: %i \n", aux.mbr_tamano, ctime(&aux.mbr_fecha_creacion), aux.mbr_signature);
    cout<<"Haciendo una particion Logica en: "<<path<<endl;
    Ebr nueva;
    int size = this->tamanio;
    if(this->unidad!='B'){
        if(this->unidad=='K')
            size = size *1024;
        if(this->unidad=='M')
            size = size *1024*1024;
    }
   
    nueva.part_fit = this->fit[0];
    strcpy(nueva.part_name, this->name.c_str());
    nueva.part_s = size;
    nueva.part_start=Aqui.part_start;
    nueva.part_status='0';
    nueva.part_next = -1;
    int apunta =Aqui.part_start;
    Ebr auxiliar;
    
    fseek(disco, apunta, SEEK_SET);
    fread(&auxiliar, sizeof(Ebr), 1, disco);
    //printf("next: %i  inicio: %i fit: %c name: %s\n", auxiliar.part_next, auxiliar.part_start,auxiliar.part_fit,auxiliar.part_name);
    if(auxiliar.part_s==0){
        fseek(disco, apunta, SEEK_SET);
        fwrite(&nueva,sizeof(Ebr),1,disco);
        fseek(disco, apunta, SEEK_SET);
        fread(&auxiliar, sizeof(Ebr), 1, disco);
        printf("next: %i  inicio: %i fit: %c name: %s\n", auxiliar.part_next, auxiliar.part_start,auxiliar.part_fit,auxiliar.part_name);
        return;
    }
    while(auxiliar.part_next!=-1){
        printf("next: %i  inicio: %i fit: %c name: %s\n", auxiliar.part_next, auxiliar.part_start,auxiliar.part_fit,auxiliar.part_name);
        apunta = auxiliar.part_next;
        fseek(disco, apunta, SEEK_SET);
        fread(&auxiliar, sizeof(Ebr), 1, disco);
    }
    auxiliar.part_next=auxiliar.part_s+auxiliar.part_start;
    fseek(disco, apunta, SEEK_SET);
    fwrite(&auxiliar,sizeof(Ebr),1,disco);
    printf("next: %i  inicio: %i fit: %c name: %s\n", auxiliar.part_next, auxiliar.part_start,auxiliar.part_fit,auxiliar.part_name);
    nueva.part_start=auxiliar.part_next;
    fseek(disco, nueva.part_start, SEEK_SET);
    fwrite(&nueva,sizeof(Ebr),1,disco);

    apunta =nueva.part_start;
    fseek(disco, apunta, SEEK_SET);
    fread(&auxiliar, sizeof(Ebr), 1, disco);
    printf("L.next: %i  inicio: %i fit: %c name: %s\n", auxiliar.part_next, auxiliar.part_start,auxiliar.part_fit,auxiliar.part_name);

    fclose(disco);
}