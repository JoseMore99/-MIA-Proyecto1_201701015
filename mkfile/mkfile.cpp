#include "mkfile.h"
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <string.h>
#include <math.h> 
#include "../estructuras.cpp"

using namespace std;
string ObtenerContenido(string path){
    string Contenido;
    ifstream ejecutable;
    ejecutable.open(path,ios::in);
    string line;
    if(ejecutable.fail()){
        exit(1);
    }
    while (getline(ejecutable, line)){
        Contenido=Contenido+line;
    }
    return Contenido;
}
Partition BP(Partition lista [], string nombe){//Buscar Particion (otra vez xd)
   Partition aux;
    for (int i = 0; i < 4; i++){
        if(lista[i].part_name==nombe)
            return lista[i];
    }
    return aux;
}

int ExisteCarArch(FILE *disco,TablaI raiz,char *token,SuperB reporte){
    int retorno = -1;
    for (int i = 0; i < 15; i++){
        if(raiz.i_block[i]!=-1){
            BloqueCarpetas carp;
            int pos = reporte.s_block_start+(raiz.i_block[i]*sizeof(BloqueArchivos));
            fseek(disco, pos, SEEK_SET);
            fread(&raiz,sizeof(BloqueCarpetas),1,disco);
            if(carp.b_content[0].b_name[0]=='.'){//Es carpeta
                for (int j = 1; j < 4; j++)
                {
                    if(carp.b_content[j].b_name==token){
                        retorno=carp.b_content[j].b_inodo;
                        return retorno;
                    }
                }
            }
        }
    }
    return retorno;
    
}

bool EsCarpeta( string valor){
    for(int i = 0; i < valor.length(); i++){
        char caracter = valor.at(i);
        if(caracter== '.'){
            return false;
        }
    }
    return true;
}
int PosInodo(FILE *disco,TablaI raiz,SuperB reporte){
    int retorno = -1;
    for (int i = 0; i < 15; i++){
        if(raiz.i_block[i]!=-1){
            BloqueCarpetas carp;
            int pos = reporte.s_block_start+(raiz.i_block[i]*sizeof(BloqueArchivos));
            fseek(disco, pos, SEEK_SET);
            fread(&raiz,sizeof(BloqueCarpetas),1,disco);
            if(carp.b_content[0].b_name[0]=='.'){//Es carpeta
                for (int j = 1; j < 4; j++)
                {
                    if(carp.b_content[j].b_name==""){
                        return i;
                    }
                }
            }
        }
    }
    for (int i = 0; i < 15; i++){
        if(raiz.i_block[i]==-1){
            return i;

        }}
    return retorno;
    
} 
int PosBloque(FILE *disco,TablaI raiz,SuperB reporte){
    int retorno = -1;
    for (int i = 0; i < 15; i++){
        if(raiz.i_block[i]!=-1){
            BloqueCarpetas carp;
            int pos = reporte.s_block_start+(raiz.i_block[i]*sizeof(BloqueArchivos));
            fseek(disco, pos, SEEK_SET);
            fread(&raiz,sizeof(BloqueCarpetas),1,disco);
            if(carp.b_content[0].b_name[0]=='.'){//Es carpeta
                for (int j = 1; j < 4; j++)
                {
                   if(carp.b_content[j].b_name==""){
                        return j;
                    }
                }
            }
        }
    }
    return retorno;
    
}

mkfile::mkfile(){
    this->r=false;
    this->cont="";
}


void mkfile::makefile(mount montado){

    FILE *disco;
    disco=fopen(montado.path.c_str(),"rb+");
    fseek(disco, 0, SEEK_SET);
    if (disco==NULL)
        exit(1);
    Mbr aux;
    fread(&aux, sizeof(Mbr), 1, disco);
    Partition bul= BP(aux.mbr_partition,montado.name);
    fseek(disco, bul.part_start, SEEK_SET);
    SuperB reporte;
    fread(&reporte,sizeof(SuperB),1,disco);

    char delimitador[] = "/";
    string vec;
    string str = this->path;
    char a[str.length()+1];
    strcpy(a, str.c_str());
    char * token = strtok(a,delimitador);
    int NumeroInodo=0;
    while (token != NULL){
        TablaI raiz;
        int pos = reporte.s_inode_start+(NumeroInodo*sizeof(TablaI));
        fseek(disco, pos, SEEK_SET);
        fread(&raiz,sizeof(TablaI),1,disco);
        //Existe la Carpeta o archivo?
        int comprobar = ExisteCarArch(disco,raiz,token,reporte);
        if(comprobar==-1){//no existe
            //buscamos pocision en el bloque y de no haberla creamos un bloque 
            int inodoActual=PosInodo(disco,raiz,reporte);// Posicion del inodo al apuntador bloue bloque
            int bloqueActual=PosBloque(disco,raiz,reporte);//pocision del bloque con espacio libre
            string contenido=token;
            if(bloqueActual==-1){//si no hay un bloque disponible, se crea
                BloqueCarpetas extender;
                string Actual=".";
                string Anterior="..";
                string libre="";
                
                strcpy(extender.b_content[0].b_name,Actual.c_str());
                strcpy(extender.b_content[1].b_name,Anterior.c_str());
                strcpy(extender.b_content[2].b_name,contenido.c_str());
                strcpy(extender.b_content[3].b_name,libre.c_str());
                pos= reporte.s_block_start+(reporte.s_first_blo*sizeof(BloqueArchivos));
                extender.b_content[2].b_inodo=reporte.s_first_ino;
                fseek(disco, pos, SEEK_SET);
                fwrite(&extender,sizeof(BloqueArchivos),1,disco);
                raiz.i_block[inodoActual]=reporte.s_first_blo;
                reporte.s_first_blo++;
                reporte.s_blocks_count++;
                reporte.s_free_blocks_count--;

                bloqueActual=2;
                pos= reporte.s_inode_s+(NumeroInodo*sizeof(TablaI));
            }else{//Si hay bloque disponible,solo se edita
                BloqueCarpetas reescribir;
                pos= reporte.s_block_start+(raiz.i_block[inodoActual]*sizeof(BloqueArchivos));
                fseek(disco, pos, SEEK_SET);
                fread(&reescribir,sizeof(BloqueArchivos),1,disco);
                strcpy(reescribir.b_content[bloqueActual].b_name,contenido.c_str());
                reescribir.b_content[bloqueActual].b_inodo=reporte.s_first_ino;
                fseek(disco, pos, SEEK_SET);
                fwrite(&reescribir,sizeof(BloqueArchivos),1,disco);
            }

            //Creamos La carpeta o el archivo
            TablaI nuevoInodo;
            if(EsCarpeta(token)){
                nuevoInodo.i_uid=1;
                nuevoInodo.i_gid = 1;
                nuevoInodo.i_s = sizeof(BloqueCarpetas);
                time_t now;
                now = time(NULL);
                nuevoInodo.i_ctime= now;
                nuevoInodo.i_atime= now;
                nuevoInodo.i_mtime= now;
                for (int i = 0; i < 15; i++)
                {
                    nuevoInodo.i_block[i]=-1;
                }
                nuevoInodo.i_type='0';
                nuevoInodo.i_perm=664;
                nuevoInodo.i_block[0]=reporte.s_first_blo;
                pos= reporte.s_inode_start+(reporte.s_first_ino*sizeof(TablaI));
                NumeroInodo=reporte.s_first_ino;
                fseek(disco, pos, SEEK_SET);
                fwrite(&nuevoInodo,sizeof(TablaI),1,disco);
                reporte.s_first_ino++;
                reporte.s_inodes_count++;
                reporte.s_free_inodes_count--;
                //creamos el Bloque de carpeta
                BloqueCarpetas nuevoBC;
                string Actual=".";
                string Anterior="..";
                string libre="";
                
                strcpy(nuevoBC.b_content[0].b_name,Actual.c_str());
                strcpy(nuevoBC.b_content[1].b_name,Anterior.c_str());
                strcpy(nuevoBC.b_content[2].b_name,libre.c_str());
                strcpy(nuevoBC.b_content[3].b_name,libre.c_str());
                int postemp= reporte.s_block_start+(reporte.s_first_blo*sizeof(BloqueArchivos));
                fseek(disco, postemp, SEEK_SET);
                fwrite(&nuevoBC,sizeof(BloqueArchivos),1,disco);
                reporte.s_first_blo++;
                reporte.s_blocks_count++;
                reporte.s_free_blocks_count--;
            }else{
                nuevoInodo.i_uid=1;
                nuevoInodo.i_gid = 1;
                nuevoInodo.i_s = sizeof(BloqueCarpetas);
                time_t now;
                now = time(NULL);
                nuevoInodo.i_ctime= now;
                nuevoInodo.i_atime= now;
                nuevoInodo.i_mtime= now;
                for (int i = 0; i < 15; i++)
                {
                    nuevoInodo.i_block[i]=-1;
                }
                nuevoInodo.i_type='1';
                nuevoInodo.i_perm=664;
                nuevoInodo.i_block[0]=reporte.s_first_blo;
                pos= reporte.s_inode_start+(reporte.s_first_ino*sizeof(TablaI));
                NumeroInodo=reporte.s_first_ino;
                fseek(disco, pos, SEEK_SET);
                fwrite(&nuevoInodo,sizeof(TablaI),1,disco);
                reporte.s_first_ino++;
                reporte.s_inodes_count++;
                reporte.s_free_inodes_count--;
                //creamos el Bloque de carpeta
                BloqueArchivos nuevoBA;
                string libre="";
                
                if(this->cont!=""){
                    libre = ObtenerContenido(this->cont);
                }else{
                    int contador=0;
                    int contador2=0;
                    while(contador<this->size){
                        libre = libre+to_string(contador2);
                        if(contador2==9){
                            contador2=0;
                        }
                        contador2++;
                        contador++;
                    }
                }

                strcpy(nuevoBA.b_content,libre.c_str());
                int postemp= reporte.s_block_start+(reporte.s_first_blo*sizeof(BloqueArchivos));
                fseek(disco, postemp, SEEK_SET);
                fwrite(&nuevoBA,sizeof(BloqueArchivos),1,disco);
                reporte.s_first_blo++;
                reporte.s_blocks_count++;
                reporte.s_free_blocks_count--;
            }
        }else{//si existe
            NumeroInodo=comprobar;
        }
        token = strtok(NULL, delimitador);
    }
    fseek(disco, bul.part_start, SEEK_SET);
    cout<<reporte.s_inodes_count<<endl;
    fwrite(&reporte,sizeof(SuperB),1,disco);
    fclose(disco);
}