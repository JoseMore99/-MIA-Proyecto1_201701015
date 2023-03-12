#include "rep.h"
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <string.h>
#include "../estructuras.cpp"

using namespace std;

string bloques;

string ObtenerBloque(int inicio,int numero,FILE *disco,char type){
    string archi="";
    string apuntadores="";
    int postemp= inicio+(numero*sizeof(BloqueArchivos));
    BloqueCarpetas aux2;
    fseek(disco, inicio, SEEK_SET);
    fread(&aux2,sizeof(BloqueArchivos),1,disco);
    //cout<<aux2.b_content[2].b_name<<endl;
    if(type=='0'){
        BloqueCarpetas aux;
        fseek(disco, postemp, SEEK_SET);
        fread(&aux,sizeof(BloqueArchivos),1,disco);
        archi=archi+"nodeB"+to_string(numero)+"[label=\"";
        archi=archi+"bloque Carpeta "+to_string(numero)+"\\n";
        for (int i = 0; i < 4; i++)
        {
            archi=archi+aux.b_content[i].b_name+"--"+to_string(aux.b_content[i].b_inodo)+"\\n";
            if(aux.b_content[i].b_inodo!=0){
                apuntadores=apuntadores+"nodeB"+to_string(numero)+"-> nodeI"+to_string(aux.b_content[i].b_inodo)+";\n";
            }
        }
        
        archi=archi+"\"];\n";
    }else if(type=='1'){
        BloqueArchivos aux;
        fseek(disco, postemp, SEEK_SET);
        fread(&aux,sizeof(BloqueArchivos),1,disco);
        archi=archi+"nodeB"+to_string(numero)+"[label=\"";
        archi=archi+"bloque Archivo "+to_string(numero)+"\\n";
        archi=archi+aux.b_content+"\\n";
        archi=archi+"\"];\n";
    }
    archi = archi+apuntadores;
    return archi;
}

Partition BuscarPar(Partition lista [], string nombe){
   Partition aux;
    for (int i = 0; i < 4; i++){
        if(lista[i].part_name==nombe)
            return lista[i];
    }
    return aux;
}

rep::rep(){
}


void rep::makerep(string localizar,string parti){
    if(this->name=="mbr"){
        ofstream archi;
        archi.open("Report.dot",ios::out);
        if(archi.fail()){
        cout<<"Ocurrio un error inesperado"<<endl;
        return;
        }
        archi<<"digraph g {\ngraph [];\nnode [\nfontsize = \"16\"\nshape = \"record\"\n];\nedge [];"<<endl;
        FILE *disco;
        disco=fopen(localizar.c_str(),"rb+");
        if(disco==NULL)
            exit(1);
        Mbr aux;
        fread(&aux,sizeof(Mbr),1,disco);
        archi<<" a0 [shape=none label=<<TABLE border=\"0\">\n";
        archi<<"<TR><TD bgcolor=\"yellow\">Reporte MBR</TD>"<<endl;
        archi<<"<TD bgcolor=\"yellow\"></TD></TR>"<<endl;
        archi<<"<TR><TD>MBR tamanio</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<aux.mbr_tamano<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>MBR Fecha Creacion</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<ctime(&aux.mbr_fecha_creacion)<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>MBR signature</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<aux.mbr_signature<<endl;
        archi<<"</TD></TR>"<<endl;
        for (int i = 0; i < 4; i++)
        {
           if(aux.mbr_partition[i].part_name[0]!='\0'){
                if (aux.mbr_partition[i].part_type=='P')
                {
                    archi<<"<TR><TD bgcolor=\"yellow\">Particion</TD>"<<endl;
                    archi<<"<TD bgcolor=\"yellow\"></TD></TR>"<<endl;
                    archi<<"<TR><TD>status</TD>"<<endl;
                    archi<<"<TD>"<<endl;
                    archi<<aux.mbr_partition[i].part_status<<endl;
                    archi<<"</TD></TR>"<<endl;
                    archi<<"<TR><TD>type</TD>"<<endl;
                    archi<<"<TD>"<<endl;
                    archi<<aux.mbr_partition[i].part_type<<endl;
                    archi<<"</TD></TR>"<<endl;
                    archi<<"<TR><TD>fit</TD>"<<endl;
                    archi<<"<TD>"<<endl;
                    archi<<aux.mbr_partition[i].part_fit<<endl;
                    archi<<"</TD></TR>"<<endl;
                    archi<<"<TR><TD>start</TD>"<<endl;
                    archi<<"<TD>"<<endl;
                    archi<<aux.mbr_partition[i].part_start<<endl;
                    archi<<"</TD></TR>"<<endl;
                    archi<<"<TR><TD>size</TD>"<<endl;
                    archi<<"<TD>"<<endl;
                    archi<<aux.mbr_partition[i].part_s<<endl;
                    archi<<"</TD></TR>"<<endl;
                    archi<<"<TR><TD>name</TD>"<<endl;
                    archi<<"<TD>"<<endl;
                    archi<<aux.mbr_partition[i].part_name<<endl;
                    archi<<"</TD></TR>"<<endl;
                }else{
                    archi<<"<TR><TD bgcolor=\"brown:violet\">Particion Extendida</TD>";
                    archi<<"<TD bgcolor=\"brown:violet\"></TD></TR>";
                    int apunta =aux.mbr_partition[i].part_start;
                    Ebr auxiliar;
                    fseek(disco, apunta, SEEK_SET);
                    fread(&auxiliar, sizeof(Ebr), 1, disco);
                    while(true){
                        archi<<"<TR><TD bgcolor=\"violet\">Particion Logica</TD>"<<endl;
                        archi<<"<TD bgcolor=\"violet\"></TD></TR>"<<endl;
                         archi<<"<TR><TD>status</TD>"<<endl;
                        archi<<"<TD>"<<endl;
                        archi<<auxiliar.part_status<<endl;
                        archi<<"</TD></TR>"<<endl;
                        archi<<"<TR><TD>next</TD>"<<endl;
                        archi<<"<TD>"<<endl;
                        archi<<auxiliar.part_next<<endl;
                        archi<<"</TD></TR>"<<endl;
                        archi<<"<TR><TD>fit</TD>"<<endl;
                        archi<<"<TD>"<<endl;
                        archi<<auxiliar.part_fit<<endl;
                        archi<<"</TD></TR>"<<endl;
                        archi<<"<TR><TD>start</TD>"<<endl;
                        archi<<"<TD>"<<endl;
                        archi<<auxiliar.part_start<<endl;
                        archi<<"</TD></TR>"<<endl;
                        archi<<"<TR><TD>size</TD>"<<endl;
                        archi<<"<TD>"<<endl;
                        archi<<auxiliar.part_s<<endl;
                        archi<<"</TD></TR>"<<endl;
                        archi<<"<TR><TD>name</TD>"<<endl;
                        archi<<"<TD>"<<endl;
                        archi<<auxiliar.part_name<<endl;
                        archi<<"</TD></TR>"<<endl;
                        if(auxiliar.part_next!=-1){
                            apunta = auxiliar.part_next;
                            fseek(disco, apunta, SEEK_SET);
                            fread(&auxiliar, sizeof(Ebr), 1, disco);
                        }else{
                            break;
                        }
                        
                    }
                }
                
           }
        }
        archi<<" </TABLE>>];\n";
        
        archi<<"}";
        archi.close();
        string cmd = "dot -Tjpg ./Report.dot -o "+this->path;
        cout<<cmd<<endl;
        system(cmd.c_str());
        cout<<"Reporte de particiones creado"<<endl;
    }else if(this->name=="disk"){
        ofstream archi;
        archi.open("Reportd.dot",ios::out);
        if(archi.fail()){
        cout<<"Ocurrio un error inesperado"<<endl;
        return;
        }
        FILE *disco;
        disco=fopen(localizar.c_str(),"rb+");
        if(disco==NULL)
            exit(1);
        Mbr aux;
        fread(&aux,sizeof(Mbr),1,disco);
        archi<<"digraph G {\n subgraph cluster1 {fillcolor=\"blue\" label=\"";
        archi<<this->path;
        archi<<"\" fontcolor=\"white\" style=\"filled\""<<endl;
        int contador = 0;
        int contadorL = 0;
        int extendida=0;
        archi<<"node [shape=box fillcolor=\"yellow\" style=\"filled\"]"<<endl;
        archi<<"node"+to_string(contador)+"[label=\"";
        archi<<"Mbr";
        archi<<"\"];"<<endl;
        contador++;
        double porcentaje=0;
        for (int i = 0; i < 4; i++){
           if(aux.mbr_partition[i].part_name[0]!='\0'){
                archi<<"node"+to_string(contador)+"[label=\"";
                if (aux.mbr_partition[i].part_type=='P')
                {
                    porcentaje = (aux.mbr_partition[i].part_s*100/aux.mbr_tamano);
                    archi<<"Primaria\\n";
                    archi<<porcentaje;
                    archi<<"%";
                    archi<<" del disco\"];"<<endl;
                }else{
                    extendida=contador;
                    porcentaje = (aux.mbr_partition[i].part_s*100/aux.mbr_tamano);
                    archi<<"Extendida\\n";
                    archi<<porcentaje;
                    archi<<"%";
                    archi<<"del disco\"];"<<endl;
                    int apunta =aux.mbr_partition[i].part_start;
                    Ebr auxiliar;
                    fseek(disco, apunta, SEEK_SET);
                    fread(&auxiliar, sizeof(Ebr), 1, disco);
                    archi<<"subgraph cluster2 {fillcolor=\"cyan\" label=\"Logicas";
                    archi<<"\"style=\"filled\""<<endl;
                    archi<<"node [shape=box fillcolor=\"green\" style=\"filled\" ]"<<endl;
                    while(true){
                        archi<<"nodel"+to_string(contadorL)+"[label=\"";
                        archi<<"Ebr";
                        archi<<"\"];"<<endl;
                        contadorL++;
                        archi<<"nodel"+to_string(contadorL)+"[label=\"";
                        porcentaje = (auxiliar.part_s*100/aux.mbr_tamano);
                        archi<<"Logica\\n";
                        archi<<porcentaje;
                        archi<<"%";
                        archi<<" del disco\"];"<<endl;
                        contadorL++;
                        if(auxiliar.part_next!=-1){
                            apunta = auxiliar.part_next;
                            fseek(disco, apunta, SEEK_SET);
                            fread(&auxiliar, sizeof(Ebr), 1, disco);
                        }else{
                            break;
                        }
                        
                    }
                    archi<<"rank=same{";
                    for (int i = 0; i < contadorL-1; i++)
                    {
                        archi<<"nodel";
                        archi<<i;
                        archi<<"->";
                    }
                    archi<<"nodel";
                    archi<<contadorL-1;
                    archi<<"}"<<endl;
                    
                    archi<<"};"<<endl;
                }
                contador++;
            }
        }
        archi<<"rank=same{";
        for (int i = 0; i < contador-1; i++)
        {
            archi<<"node";
            archi<<i;
            archi<<"->";
        }
        archi<<"node";
        archi<<contador-1;
        archi<<"}"<<endl;
            archi<<"node";
            archi<<extendida;
            archi<<"->";
            archi<<"nodel0"<<endl;
        archi<<"}\n}"<<endl;

        archi.close();
        string cmd = "dot -Tjpg ./Reportd.dot -o "+this->path;
        cout<<cmd<<endl;
        system(cmd.c_str());
        cout<<"Reporte de particiones creado"<<endl;
    }else if(this->name=="sb"){
       ofstream archi;
        archi.open("Reportsb.dot",ios::out);
        if(archi.fail()){
        cout<<"Ocurrio un error inesperado"<<endl;
        return;
        }
        archi<<"digraph g {\ngraph [];\nnode [\nfontsize = \"16\"\nshape = \"record\"\n];\nedge [];"<<endl;
        FILE *disco;
        disco=fopen(localizar.c_str(),"rb+");
        if(disco==NULL){
            cout<<"ocurrio un error inesperado"<<endl;
            exit(1);
        }
        Mbr aux;
        fread(&aux,sizeof(Mbr),1,disco);
        
        Partition esesta= BuscarPar(aux.mbr_partition,parti);
        cout<<"aquie inicia "<<esesta.part_start<<endl;
        
        fseek(disco, esesta.part_start, SEEK_SET);
        SuperB reporte;
        fread(&reporte,sizeof(SuperB),1,disco);

        archi<<" a0 [shape=none label=<<TABLE border=\"0\">\n";
        archi<<"<TR><TD bgcolor=\"green\">Reporte SUPER BLOQUE</TD>"<<endl;
        archi<<"<TD bgcolor=\"green\"></TD></TR>"<<endl;
        archi<<"<TR><TD>s_filesystem_type</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<reporte.s_filesystem_type<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>s_inodes_count</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<reporte.s_inodes_count<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>s_blocks_count</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<reporte.s_blocks_count<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>s_free_blocks_count</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<reporte.s_free_blocks_count<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>s_free_inodes_count</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<reporte.s_free_inodes_count<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>s_mtime</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<ctime(&reporte.s_mtime)<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>s_umtime</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<ctime(&reporte.s_umtime)<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>s_mnt_count</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<reporte.s_mnt_count<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>s_magic</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<reporte.s_magic<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>s_inode_s</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<reporte.s_inode_s<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>s_block_s</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<reporte.s_block_s<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>s_firts_ino</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<reporte.s_first_ino<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>s_first_blo</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<reporte.s_first_blo<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>s_bm_inode_start</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<reporte.s_bm_inode_start<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>s_bm_block_start</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<reporte.s_bm_inode_start<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>s_inode_start</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<reporte.s_inode_start<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<"<TR><TD>s_block_start</TD>"<<endl;
        archi<<"<TD>"<<endl;
        archi<<reporte.s_block_start<<endl;
        archi<<"</TD></TR>"<<endl;
        archi<<" </TABLE>>];\n";
        
        archi<<"}";
        archi.close();
        string cmd = "dot -Tjpg ./Reportsb.dot -o "+this->path;
        cout<<cmd<<endl;
        system(cmd.c_str());
        cout<<"Reporte de Super Bloque creado"<<endl;
    }else if(this->name=="inode"){
        ofstream archi;
        archi.open("Reportin.dot",ios::out);
        if(archi.fail()){
        cout<<"Ocurrio un error inesperado"<<endl;
        return;
        }
        FILE *disco;
        disco=fopen(localizar.c_str(),"rb+");
        if(disco==NULL)
            exit(1);
        Mbr aux;
        fread(&aux,sizeof(Mbr),1,disco);
        archi<<"digraph G {\n label=\"";
        archi<<this->path;
        archi<<"\""<<endl;
        int contador = 0;
        archi<<"node [shape=box fillcolor=\"aquamarine\" style=\"filled\"]"<<endl;
        Partition esesta= BuscarPar(aux.mbr_partition,parti);
        cout<<"aquie inicia "<<esesta.part_start<<endl;
        
        fseek(disco, esesta.part_start, SEEK_SET);
        SuperB reporte;
        fread(&reporte,sizeof(SuperB),1,disco);
        int pos = reporte.s_inode_start;
        TablaI raiz;
        while(contador<reporte.s_inodes_count){
            fseek(disco, pos, SEEK_SET);
            fread(&raiz,sizeof(TablaI),1,disco);
            archi<<"node"+to_string(contador)+"[label=\"";
            archi<<"inodo "<<contador<<"\\n";
            archi<<"i_uid:"<<raiz.i_uid<<"\\n";
            archi<<"i_s:"<<raiz.i_s<<"\\n";
            archi<<"i_atime:"<<ctime(&raiz.i_atime);
            archi<<"i_ctime:"<<ctime(&raiz.i_ctime);
            archi<<"i_mtime:"<<ctime(&raiz.i_mtime);
            for (int i = 0; i < 15; i++){
                archi<<"i_block["<<i<<"]:"<<raiz.i_block[i]<<"\\n";
            }
            
            archi<<"i_type:"<<raiz.i_type<<"\\n";
            archi<<"i_perm:"<<raiz.i_perm;
            archi<<"\"];"<<endl;
            contador++;
            pos += sizeof(TablaI);
        }
        
       
        archi<<"rank=same{";
        for (int i = 0; i < contador-1; i++)
        {
            archi<<"node";
            archi<<i;
            archi<<"->";
        }
        archi<<"node";
        archi<<contador-1;
        archi<<"}"<<endl;
        archi<<"}"<<endl;

        archi.close();
        string cmd = "dot -Tjpg ./Reportin.dot -o "+this->path;
        cout<<cmd<<endl;
        system(cmd.c_str());
        cout<<"Reporte de inodos creado"<<endl;
    }else if(this->name=="block"){
        ofstream archi;
        archi.open("Reportblo.dot",ios::out);
        if(archi.fail()){
        cout<<"Ocurrio un error inesperado"<<endl;
        return;
        }
        FILE *disco;
        disco=fopen(localizar.c_str(),"rb+");
        if(disco==NULL)
            exit(1);
        Mbr aux;
        fread(&aux,sizeof(Mbr),1,disco);
        archi<<"digraph G {\n label=\"";
        archi<<this->path;
        archi<<"\""<<endl;
        int contador = 0;
        archi<<"node [shape=box fillcolor=\"burlywood1\" style=\"filled\"]"<<endl;
        Partition esesta= BuscarPar(aux.mbr_partition,parti);
        cout<<"aquie inicia "<<esesta.part_start<<endl;
        
        fseek(disco, esesta.part_start, SEEK_SET);
        SuperB reporte;
        fread(&reporte,sizeof(SuperB),1,disco);
        int pos = reporte.s_block_start;
        BloqueCarpetas raiz;
        BloqueArchivos doc;

        while(contador<reporte.s_inodes_count){
            fseek(disco, pos, SEEK_SET);
            fread(&raiz,sizeof(BloqueCarpetas),1,disco);
            //cout<<raiz.b_content[0].b_name<<" jsjs\n";
            if(raiz.b_content[0].b_name[0]=='.'){
                 archi<<"node"+to_string(contador)+"[label=\"";
                archi<<"bloque Carpeta "<<contador<<"\\n";
                for (int i = 0; i < 4; i++)
                {
                   archi<<raiz.b_content[i].b_name<<"--"<<raiz.b_content[i].b_inodo<<"\\n";
                }
                
                archi<<"\"];"<<endl;
                contador++;
                pos += sizeof(BloqueCarpetas);
            }else{
                fseek(disco, pos, SEEK_SET);
                fread(&doc,sizeof(BloqueArchivos),1,disco);
                archi<<"node"+to_string(contador)+"[label=\"";
                archi<<"bloque Archivo "<<contador<<"\\n";
                archi<<doc.b_content<<"\\n";
                archi<<"\"];"<<endl;
                contador++;
                pos += sizeof(BloqueCarpetas);
            }
           
        }
        
       
        archi<<"rank=same{";
        for (int i = 0; i < contador-1; i++)
        {
            archi<<"node";
            archi<<i;
            archi<<"->";
        }
        archi<<"node";
        archi<<contador-1;
        archi<<"}"<<endl;
        archi<<"}"<<endl;
        fclose(disco);
        archi.close();
        string cmd = "dot -Tjpg ./Reportblo.dot -o "+this->path;
        cout<<cmd<<endl;
        system(cmd.c_str());
        cout<<"Reporte de bloques creado"<<endl;
    }else if(this->name=="bm_inode"){
        ofstream archi;
        archi.open(this->path,ios::out);
        if(archi.fail()){
        cout<<"Ocurrio un error inesperado"<<endl;
        return;
        }
        FILE *disco;
        disco=fopen(localizar.c_str(),"rb+");
        if(disco==NULL)
            exit(1);
        Mbr aux;
        fread(&aux,sizeof(Mbr),1,disco);
        int contador = 0;
        Partition esesta= BuscarPar(aux.mbr_partition,parti);
        fseek(disco, esesta.part_start, SEEK_SET);
        SuperB reporte;
        fread(&reporte,sizeof(SuperB),1,disco);
        int pos = reporte.s_bm_inode_start;
        int fin = reporte.s_bm_block_start;
        char valor;
        int saltos=1;
        cout<<pos<<endl;
        cout<<fin<<endl;
        while(pos<fin){
            if(saltos ==20){
                archi<<"\n";
                saltos=1;
            }
            fseek(disco, pos, SEEK_SET);
            fread(&valor,sizeof(char),1,disco);
            cout<<int(valor)<<endl;
            if(valor==0){
                archi<<'0';
            }else{
                archi<<'1';
            }
            pos++;
            saltos++;
        }
        
       
        archi.close();
        cout<<"Reporte de bitmap de inodos creado"<<endl;
    }else if(this->name=="bm_bloc"){
        ofstream archi;
        archi.open(this->path,ios::out);
        if(archi.fail()){
        cout<<"Ocurrio un error inesperado"<<endl;
        return;
        }
        FILE *disco;
        disco=fopen(localizar.c_str(),"rb+");
        if(disco==NULL)
            exit(1);
        Mbr aux;
        fread(&aux,sizeof(Mbr),1,disco);
        int contador = 0;
        Partition esesta= BuscarPar(aux.mbr_partition,parti);
        fseek(disco, esesta.part_start, SEEK_SET);
        SuperB reporte;
        fread(&reporte,sizeof(SuperB),1,disco);
        int pos = reporte.s_bm_block_start;
        int fin = reporte.s_inode_start;
        char valor;
        int saltos=1;
        cout<<pos<<endl;
        cout<<fin<<endl;
        while(pos<fin){
            if(saltos ==20){
                archi<<"\n";
                saltos=1;
            }
            fseek(disco, pos, SEEK_SET);
            fread(&valor,sizeof(char),1,disco);
            if(valor==0){
                archi<<'0';
            }else{
                archi<<'1';
            }
            pos++;
            saltos++;
        }
        
       
        archi.close();
        cout<<"Reporte de bitmap de inodos creado"<<endl;
    }else if(this->name=="tree"){
        ofstream archi;
        archi.open("Reportree.dot",ios::out);
        if(archi.fail()){
        cout<<"Ocurrio un error inesperado"<<endl;
        return;
        }
        FILE *disco;
        disco=fopen(localizar.c_str(),"rb+");
        if(disco==NULL)
            exit(1);
        Mbr aux;
        fread(&aux,sizeof(Mbr),1,disco);
        archi<<"digraph G {\n label=\"";
        archi<<this->path;
        archi<<"\""<<endl;
        int contador = 0;
        archi<<"node [shape=box fillcolor=\"aquamarine\" style=\"filled\"]"<<endl;
        Partition esesta= BuscarPar(aux.mbr_partition,parti);
        
        fseek(disco, esesta.part_start, SEEK_SET);
        SuperB reporte;
        fread(&reporte,sizeof(SuperB),1,disco);
        int pos = reporte.s_inode_start;
        
        TablaI raiz;
        while(contador<reporte.s_inodes_count){
            bloques ="";
            fseek(disco, pos, SEEK_SET);
            fread(&raiz,sizeof(TablaI),1,disco);
            archi<<"nodeI"+to_string(contador)+"[label=\"";
            archi<<"inodo "+to_string(contador)+"\\n";
            archi<<"i_uid:"<<raiz.i_uid<<"\\n";
            archi<<"i_s:"<<raiz.i_s<<"\\n";
            archi<<"i_atime:"<<ctime(&raiz.i_atime);
            archi<<"i_ctime:"<<ctime(&raiz.i_ctime);
            archi<<"i_mtime:"<<ctime(&raiz.i_mtime);
            for (int i = 0; i < 15; i++){
                archi<<"i_block["<<i<<"]:"<<raiz.i_block[i]<<"\\n";
            }
            
            archi<<"i_type:"<<raiz.i_type<<"\\n";
            archi<<"i_perm:"<<raiz.i_perm;
            archi<<"\"];"<<endl;

            for (int i = 0; i < 15; i++){
                if(raiz.i_block[i]!=-1){
                    archi<<"nodeI"+to_string(contador)+"-> nodeB"+to_string(raiz.i_block[i])<<endl;
                    bloques = bloques + ObtenerBloque(reporte.s_block_start,raiz.i_block[i],disco,raiz.i_type);
                }
            }
            archi<<bloques<<endl;

            contador++;
            pos += sizeof(TablaI);
        }
        
        archi<<"}"<<endl;
        fclose(disco);
        archi.close();
        string cmd = "dot -Tjpg ./Reportree.dot -o "+this->path;
        cout<<cmd<<endl;
        system(cmd.c_str());
        cout<<"Reporte de inodos creado"<<endl;
    }

}