#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include "mkdisk.cpp"
#include "./fdisk/fdisk.h"
#include "./rep/rep.h"
#include "./mount/mount.h"
#include "./mkfs/mkfs.h"
#include "./login/login.h"
#include "./mkfile/mkfile.h"
#include "rmdisk.cpp"

using namespace std;

struct info
{
   string nombre;
   string valor;
};

void LeerComando(char []);

string rutaAux;
login *logeado= new login();


vector<mount> montados;

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

string ObtenerNombre(string ruta){
    string retorno="";
    //cout<<ruta<<endl;
    bool valor= false;
    for(int i = 0; i < ruta.length(); i++){
        char caracter = ruta.at(i);
        if(ruta.at(i) == '/') {
            retorno = "";
            valor=true;
            continue;
        }else if(ruta.at(i) == '.' && valor) {
            break;
        }
        retorno = retorno + caracter;

    }
    return retorno;
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
    if (strcasecmp(vec.c_str(),"execute")==0){
        token = strtok(NULL, delimitador);
        string aux = token;
        info compo = ObtenerValor(aux);
        if(strcasecmp(compo.nombre.c_str(),">path")==0){
            if(compo.valor=="$")compo.valor=rutaAux;
            execute(compo.valor);
        }
    }else if (strcasecmp(vec.c_str(),"rmdisk")==0){
        token = strtok(NULL, delimitador);
        string aux = token;
        rmdisk *actual=new rmdisk();
        info compo = ObtenerValor(aux);
        if(strcasecmp(compo.nombre.c_str(),">path")==0){
            if(compo.valor=="$")compo.valor=rutaAux;
            actual->ruta=compo.valor;
        }
        actual->removedisk();
    }
    else if (strcasecmp(vec.c_str(),"mkdisk")==0){
        mkdisk *actual=new mkdisk();
        token = strtok(NULL, delimitador);
        while (token != NULL){

            string aux = token;
            info compo = ObtenerValor(aux);
            if(strcasecmp(compo.nombre.c_str(),">path")==0){
                 if(compo.valor=="$")compo.valor=rutaAux;

                actual->ruta = compo.valor;
            }else  if(strcasecmp(compo.nombre.c_str(),">size")==0){
                actual->tamanio = stoi(compo.valor);
            }else  if(strcasecmp(compo.nombre.c_str(),">unit")==0){
                actual->unidad = compo.valor[0];
            }else  if(strcasecmp(compo.nombre.c_str(),">fit")==0){
                actual->fit = compo.valor;
            }
            token = strtok(NULL, delimitador);
        }
        //actual->imprimir();
        actual->makedisk();
    }else if (strcasecmp(vec.c_str(),"fdisk")==0){
        fdisk *actual=new fdisk();
        token = strtok(NULL, delimitador);
        while (token != NULL){

            string aux = token;
            info compo = ObtenerValor(aux);
            if(strcasecmp(compo.nombre.c_str(),">path")==0){
                 if(compo.valor=="$")compo.valor=rutaAux;

                actual->ruta = compo.valor;
            }else  if(strcasecmp(compo.nombre.c_str(),">size")==0){
                actual->tamanio = stoi(compo.valor);
            }else  if(strcasecmp(compo.nombre.c_str(),">unit")==0){
                actual->unidad = compo.valor[0];
            }else  if(strcasecmp(compo.nombre.c_str(),">fit")==0){
                actual->fit = compo.valor;
            }else  if(strcasecmp(compo.nombre.c_str(),">name")==0){
                actual->name = compo.valor;
            }else  if(strcasecmp(compo.nombre.c_str(),">type")==0){
                actual->type = compo.valor[0];
            }else  if(strcasecmp(compo.nombre.c_str(),">delete")==0){
                actual->borrar = compo.valor;
            }else  if(strcasecmp(compo.nombre.c_str(),">add")==0){
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
    }else if (strcasecmp(vec.c_str(),"mount")==0){
        mount *actual=new mount();
        token = strtok(NULL, delimitador);
        while (token != NULL){

            string aux = token;
            info compo = ObtenerValor(aux);
            if(strcasecmp(compo.nombre.c_str(),">path")==0){
                 if(compo.valor=="$")compo.valor=rutaAux;

                actual->path = compo.valor;
            }else  if(strcasecmp(compo.nombre.c_str(),">name")==0){
                actual->name = compo.valor;
            }
            token = strtok(NULL, delimitador);
        }
        int numero=actual->numeroParticion();
        if(numero!=-1){
            numero++;
            //201701015
            actual->id="15"+to_string(montados.size())+ObtenerNombre(actual->path);
            montados.push_back(*actual);
             for (mount item : montados) {
                cout <<"id:"<< item.id << endl;
                cout <<"name:"<< item.name << endl;
            }
        }else{
            cout<<"La particion que se quiere montar no existe"<<endl;
        }
        
    }else if (strcasecmp(vec.c_str(),"mkfs")==0){
        mkfs *actual=new mkfs();
        token = strtok(NULL, delimitador);
        while (token != NULL){
            string aux = token;
            info compo = ObtenerValor(aux);
            if(strcasecmp(compo.nombre.c_str(),">id")==0){
                actual->id = compo.valor;
            }else  if(strcasecmp(compo.nombre.c_str(),">type")==0){
                actual->type = compo.valor;
            }else  if(strcasecmp(compo.nombre.c_str(),">fs")==0){
                actual->fs = compo.valor;
            }
            token = strtok(NULL, delimitador);
        }
        bool error = true;
        for (mount item : montados) {
            cout<<item.id<<endl;
            cout<<actual->id<<endl;
                if(item.id==actual->id){
                     actual->makefs(item);
                     error = false;
                     break;
                }
        }
        if(error){
            cout<<"Particion no montada o inexistente!"<<endl;
        }
       
    }else if (strcasecmp(vec.c_str(),"rep")==0){
        rep *actual=new rep();
        token = strtok(NULL, delimitador);
        while (token != NULL){

            string aux = token;
            info compo = ObtenerValor(aux);
            if(strcasecmp(compo.nombre.c_str(),">path")==0){
                 if(compo.valor=="$")compo.valor=rutaAux;

                actual->path = compo.valor;
            }else  if(strcasecmp(compo.nombre.c_str(),">name")==0){
                actual->name = compo.valor;
            }else  if(strcasecmp(compo.nombre.c_str(),">ruta")==0){
                actual->ruta = compo.valor;
            }else  if(strcasecmp(compo.nombre.c_str(),">id")==0){
                actual->id = compo.valor;
            }
            token = strtok(NULL, delimitador);
        }
        //actual->imprimir();
        for (mount item : montados) {
                if(item.id==actual->id){
                    actual->makerep(item.path,item.name);
                    break;
                }
                
        }
    }else if(strcasecmp(vec.c_str(),"login")==0){
        login *actual=new login();
        token = strtok(NULL, delimitador);
        while (token != NULL){
            string aux = token;
            info compo = ObtenerValor(aux);
            if(strcasecmp(compo.nombre.c_str(),">user")==0){
                actual->user = compo.valor;
            }else  if(strcasecmp(compo.nombre.c_str(),">pass")==0){
                actual->pass = compo.valor;
            }else  if(strcasecmp(compo.nombre.c_str(),">id")==0){
                actual->id = compo.valor;
            }
            token = strtok(NULL, delimitador);
        }
        bool error = true;
        for (mount item : montados) {
                if(item.id==actual->id){
                    if(actual->user=="root"&&actual->pass=="123"){
                        logeado=actual;
                        cout<<"Bienvenido "<<actual->user<<endl;
                    }else{
                        cout<<"usuario o contrasenia incorrectos!!!"<<endl;
                    }
                    error = false;
                    break;
                }
        }
        if(error){
            cout<<"Particion no montada o inexistente!"<<endl;
        }

    }else if(strcasecmp(vec.c_str(),"mkfile")==0){
        mkfile *actual=new mkfile();
        token = strtok(NULL, delimitador);
        while (token != NULL){
            string aux = token;
            info compo = ObtenerValor(aux);
            if(strcasecmp(compo.nombre.c_str(),">path")==0){
                if(compo.valor=="$")compo.valor=rutaAux;
                actual->path=compo.valor;
            }else  if(strcasecmp(compo.nombre.c_str(),">r")==0){
                actual->r =true;
            }else  if(strcasecmp(compo.nombre.c_str(),">size")==0){
                actual->size = stoi(compo.valor);
            }else if(strcasecmp(compo.nombre.c_str(),">cont")==0){
                if(compo.valor=="$")compo.valor=rutaAux;
                actual->cont=compo.valor;
            }
            token = strtok(NULL, delimitador);
        }
        if(logeado->id==""){
            cout<<"Error: No hay usuario logeado, Realizar login"<<endl;
        }else{
            bool error = true;
            for (mount item : montados) {
                    if(item.id==logeado->id){
                        actual->makefile(item);
                        error = false;
                        break;
                    }
            }
            if(error){
                cout<<"Particion no montada o inexistente!"<<endl;
            }
        }
    }else if(strcasecmp(vec.c_str(),"pause")==0){
        cout<<"Presione enter para continuar...";
        cin.get();
        cin.get();
        cout<<"Programa reanudado\n";
    }else if(strcasecmp(vec.c_str(),"mkdir")==0){
        mkfile *actual=new mkfile();
        actual->r=true;
        token = strtok(NULL, delimitador);
        while (token != NULL){
            string aux = token;
            info compo = ObtenerValor(aux);
            if(strcasecmp(compo.nombre.c_str(),">path")==0){
                if(compo.valor=="$")compo.valor=rutaAux;
                actual->path=compo.valor;
            }else  if(strcasecmp(compo.nombre.c_str(),">r")==0){
                actual->r =true;
            }else  if(strcasecmp(compo.nombre.c_str(),">size")==0){
                actual->size = stoi(compo.valor);
            }else if(strcasecmp(compo.nombre.c_str(),">cont")==0){
                if(compo.valor=="$")compo.valor=rutaAux;
                actual->cont=compo.valor;
            }
            token = strtok(NULL, delimitador);
        }
        if(logeado->id==""){
            cout<<"Error: No hay usuario logeado, Realizar login"<<endl;
        }else{
            bool error = true;
            for (mount item : montados) {
                    if(item.id==logeado->id){
                        actual->makefile(item);
                        error = false;
                        break;
                    }
            }
            if(error){
                cout<<"Particion no montada o inexistente!"<<endl;
            }
        }
    }
}

int main()
{
    /*
    char aux[]="/jiji/jaja.com";
    char delimitador[] = "/";
    char * token = strtok(aux,delimitador);
    while(token!=NULL){
        cout<<"<"<<token<<endl;
        token = strtok(NULL,delimitador);
    }
    */
    char rEx[400];
    bool bucle = true;
    cout<<"Proyecto 1-201701015"<<endl;
    do{
        cout<< "> ";
        scanf(" %[^\n]",rEx);
        //LLamamos a la funcion para leer comandos
        LeerComando(rEx);
     } while(bucle);
    //ExeCute >path=./exec.eea
    return 0;
}