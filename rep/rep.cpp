#include "rep.h"
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <string.h>
#include "../estructuras.cpp"

using namespace std;

rep::rep(){
}


void rep::makerep(string localizar){
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
                    while(auxiliar.part_next!=-1){
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
                        apunta = auxiliar.part_next;
                        fseek(disco, apunta, SEEK_SET);
                        fread(&auxiliar, sizeof(Ebr), 1, disco);
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
        int porcentaje=0;
        for (int i = 0; i < 4; i++){
           if(aux.mbr_partition[i].part_name[0]!='\0'){
                archi<<"node"+to_string(contador)+"[label=\"";
                if (aux.mbr_partition[i].part_type=='P')
                {
                    porcentaje = (aux.mbr_partition[i].part_s/aux.mbr_tamano)*100;
                    archi<<"Primaria\\n";
                    archi<<porcentaje;
                    archi<<"%";
                    archi<<" del disco\"];"<<endl;
                }else{
                    extendida=contador;
                    porcentaje = (aux.mbr_partition[i].part_s/aux.mbr_tamano)*100;
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
                    while(auxiliar.part_next!=-1){
                        archi<<"nodeL"+to_string(contadorL)+"[label=\"";
                        archi<<"Ebr";
                        archi<<"\"];"<<endl;
                        archi<<"nodel"+to_string(contadorL)+"[label=\"";
                        porcentaje = (auxiliar.part_s/aux.mbr_tamano)*100;
                        archi<<"Logica\\n";
                        archi<<porcentaje;
                        archi<<"%";
                        archi<<" del disco\"];"<<endl;
                        contadorL++;
                        apunta = auxiliar.part_next;
                        fseek(disco, apunta, SEEK_SET);
                        fread(&auxiliar, sizeof(Ebr), 1, disco);
                    }
                    archi<<"rank=same{";
                    for (int i = 0; i < contadorL; i++)
                    {
                        archi<<"nodel";
                        archi<<contadorL;
                        archi<<"->";
                    }
                    archi<<"nodel";
                    archi<<contadorL;
                    archi<<"}"<<endl;
                    
                    archi<<"};"<<endl;
                }
                contador++;
            }
        }
        archi<<"rank=same{";
        for (int i = 0; i < contador; i++)
        {
            archi<<"node";
            archi<<contador;
            archi<<"->";
        }
        archi<<"node";
        archi<<contador;
        archi<<"}"<<endl;
        archi<<"}\n}"<<endl;
        archi.close();
        string cmd = "dot -Tjpg ./Reportd.dot -o "+this->path;
        cout<<cmd<<endl;
        system(cmd.c_str());
        cout<<"Reporte de particiones creado"<<endl;
    }
}