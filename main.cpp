#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include "mkdisk.cpp"
#include "./fdisk/fdisk.h"
#include "./rep/rep.h"
#include "rmdisk.cpp"

using namespace std;

struct info
{
   string nombre;
   string valor;
};

void LeerComando(char []);

string rutaAux;


void execute(string ruta){
    ifstream ejecutable;
    ejecutable.open(ruta,ios::in);
    string line;
    if(ejecutable.fail()){
        exit(1);
    }
    while (getline(ejecutable, line)){
        cout<<line<<endl;
        LeerComando(&line[0]);
    }
}

string Obtenerstring(string comando){
    string nuevo="";
    rutaAux="";
    bool cambiar = false;
    for(int i = 0; i < comando.length(); i++){
        char caracter = comando.at(i);
        if(comando.at(i) == '"'){
            if (cambiar){
                nuevo = nuevo +"$";
                cambiar = false;
                continue;
            }
            cambiar = true;
            continue;
        } 
        if(!cambiar){
            nuevo = nuevo + caracter;
        }else{
            rutaAux +=caracter;
        }
        
    }
    //cout<<nuevo<<endl;
    //cout<<rutaAux<<endl;
    return nuevo;
}

info ObtenerValor(string parametro){
    string nombre="";
    string valor = "";
    info retorno;
    bool concatenar = false;
    
    for(int i = 0; i < parametro.length(); i++){
        char caracter = parametro.at(i);
        if(parametro.at(i) == '=') {
            concatenar = true;
            continue;}
        if(concatenar){
            valor = valor + caracter;
        }else{
            nombre = nombre + caracter;
        }
    }
    retorno.nombre = nombre;
    retorno.valor = valor;
    return retorno;
}

void LeerComando(char comando[]){
    string str = Obtenerstring(comando);
    char delimitador[] = " ";
    string vec;
    char aux[str.length()+1];
    strcpy(aux, str.c_str());
    char * token = strtok(aux,delimitador);
    if(token != NULL){
            vec=token;
    }
    //cout<<vec<<endl;
    if (vec=="execute"){
        token = strtok(NULL, delimitador);
        string aux = token;
        info compo = ObtenerValor(aux);
        if(compo.nombre==">path"){
            if(compo.valor=="$")compo.valor=rutaAux;
            execute(compo.valor);
        }
    }else if (vec=="rmdisk"){
        token = strtok(NULL, delimitador);
        string aux = token;
        rmdisk *actual=new rmdisk();
        info compo = ObtenerValor(aux);
        if(compo.nombre==">path"){
            if(compo.valor=="$")compo.valor=rutaAux;
            actual->ruta=compo.valor;
        }
        actual->removedisk();
    }
    else if (vec=="mkdisk"){
        mkdisk *actual=new mkdisk();
        token = strtok(NULL, delimitador);
        while (token != NULL){

            string aux = token;
            info compo = ObtenerValor(aux);
            if(compo.nombre==">path"){
                 if(compo.valor=="$")compo.valor=rutaAux;

                actual->ruta = compo.valor;
            }else  if(compo.nombre==">size"){
                actual->tamanio = stoi(compo.valor);
            }else  if(compo.nombre==">unit"){
                actual->unidad = compo.valor[0];
            }else  if(compo.nombre==">fit"){
                actual->fit = compo.valor;
            }
            token = strtok(NULL, delimitador);
        }
        //actual->imprimir();
        actual->makedisk();
    }else if (vec=="fdisk"){
        fdisk *actual=new fdisk();
        token = strtok(NULL, delimitador);
        while (token != NULL){

            string aux = token;
            info compo = ObtenerValor(aux);
            if(compo.nombre==">path"){
                 if(compo.valor=="$")compo.valor=rutaAux;

                actual->ruta = compo.valor;
            }else  if(compo.nombre==">size"){
                actual->tamanio = stoi(compo.valor);
            }else  if(compo.nombre==">unit"){
                actual->unidad = compo.valor[0];
            }else  if(compo.nombre==">fit"){
                actual->fit = compo.valor;
            }else  if(compo.nombre==">name"){
                actual->name = compo.valor;
            }else  if(compo.nombre==">type"){
                actual->type = compo.valor[0];
            }else  if(compo.nombre==">delete"){
                actual->borrar = compo.valor;
            }else  if(compo.nombre==">add"){
                actual->add = stoi(compo.valor);
            }
            token = strtok(NULL, delimitador);
        }
        //actual->imprimir();
        if(actual->type=='P'){
            actual->makePrimaria();
        }else if (actual->type=='E'){
            actual->makeExtendida();
        }else if(actual->type=='L'){
            actual->makeLogica();
        }else{
            cout<<"ERROR: Tipo de Particion desconocida!"<<endl;
        }
    }else if (vec=="rep"){
        rep *actual=new rep();
        token = strtok(NULL, delimitador);
        while (token != NULL){

            string aux = token;
            info compo = ObtenerValor(aux);
            if(compo.nombre==">path"){
                 if(compo.valor=="$")compo.valor=rutaAux;

                actual->ruta = compo.valor;
            }else  if(compo.nombre==">name"){
                actual->name = compo.valor;
            }else  if(compo.nombre==">ruta"){
                actual->ruta = compo.valor;
            }else  if(compo.nombre==">id"){
                actual->id = compo.valor;
            }
            token = strtok(NULL, delimitador);
        }
        //actual->imprimir();
        actual->makerep();
    }
}

int main()
{
    char rEx[400];
    bool bucle = true;
    cout<<"Proyecto 1-201701015"<<endl;
    do{
        cout<< "> ";
        scanf(" %[^\n]",rEx);
        //LLamamos a la funcion para leer comandos
        LeerComando(rEx);
     } while(bucle);
    //execute >path=./exec.eea
    return 0;
}