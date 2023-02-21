#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string.h>

using namespace std;

string *ObtenerParametro(char parametro[]){
    char delimitador[] = "=";
    string *vec;
    char * token = strtok(parametro,delimitador);
    if(token != NULL){
        vec[0]=token;
    }
    token = strtok(NULL, delimitador);
    if(token != NULL){
        vec[1]=token;
    }
    return vec;
}

void LeerComando(char comando[]){
    char delimitador[] = " ";
    string vec;
    char * token = strtok(comando,delimitador);
    if(token != NULL){
            vec=token;
    }
    cout<<vec<<endl;
    if (vec=="execute"){
        token = strtok(NULL, delimitador);
        vec = token;
        string *compo = ObtenerParametro(&vec[0]);
        if(compo[0]==">path"){
            cout<<compo[1]<<endl;
        }
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