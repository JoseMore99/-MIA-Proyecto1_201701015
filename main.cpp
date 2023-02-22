#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <dirent.h>
#include "mkdisk.cpp"

using namespace std;

void LeerComando(char []);

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

string ObtenerNombre(string parametro){
    string vec;
    string nombre = "";
    for(int i = 0; i < parametro.length(); i++){
        if(parametro.at(i) == '=') break;
        char caracter = parametro.at(i);
        nombre = nombre + caracter;
    }
    return nombre;
}

string ObtenerValor(string parametro){
    string valor = "";
    bool concatenar = false;
    for(int i = 0; i < parametro.length(); i++){
        if(concatenar){
            char caracter = parametro.at(i);
            valor = valor + caracter;
        }
        if(parametro.at(i) == '=') {concatenar = true;}
    }
    return valor;
}

void LeerComando(char comando[]){
    char delimitador[] = " ";
    string vec;
    char * token = strtok(comando,delimitador);
    if(token != NULL){
            vec=token;
    }
    //cout<<vec<<endl;
    if (vec=="execute"){
        token = strtok(NULL, delimitador);
        string aux = token;
        string compo[2];
        compo[1]=ObtenerValor(aux);
        compo[0]=ObtenerNombre(aux);
        if(compo[0]==">path"){
            execute(compo[1]);
        }
    }
    else if (vec=="mkdisk"){
        mkdisk *actual=new mkdisk();
        token = strtok(NULL, delimitador);
        while (token != NULL){

            string aux = token;
            string compo[2];
            compo[1]=ObtenerValor(aux);
            compo[0]=ObtenerNombre(aux);
            if(compo[0]==">path"){
                actual->ruta = compo[1];
            }else  if(compo[0]==">size"){
                actual->tamanio = stoi(compo[1]);
            }else  if(compo[0]==">unit"){
                actual->unidad = compo[1][0];
            }else  if(compo[0]==">fit"){
                actual->fit = compo[1];
            }
            token = strtok(NULL, delimitador);
        }

        //actual->mk();

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