#include "mkfs.h"
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <string.h>
#include <math.h> 
#include "../estructuras.cpp"

using namespace std;

Partition BuscarP(Partition lista [], string nombe){
   Partition aux;
    for (int i = 0; i < 4; i++){
        if(lista[i].part_name==nombe)
            return lista[i];
    }
    return aux;
}


mkfs::mkfs(){
    this->fs="2fs";
}


void mkfs::makefs(mount montado){
        FILE *disco;
    disco=fopen(montado.path.c_str(),"rb+");
    fseek(disco, 0, SEEK_SET);
    if (disco==NULL)
        exit(1);
    Mbr aux;
    fread(&aux, sizeof(Mbr), 1, disco);
    if (fs=="2fs")
    {
        Partition bul= BuscarP(aux.mbr_partition,montado.name);
        int n=(bul.part_s - sizeof(SuperB)) / (1 + 3 + sizeof(TablaI) + 3 * sizeof(BloqueArchivos));
        n = floor(n);
        time_t now;
        now = time(NULL);
        SuperB bloque;
        bloque.s_filesystem_type=2;
        bloque.s_inodes_count=0;
        bloque.s_blocks_count=0;
        bloque.s_free_inodes_count=n;
        bloque.s_free_blocks_count=3*n;
        bloque.s_mtime=now;
        bloque.s_umtime=now;
        bloque.s_magic=0xEF53;
        bloque.s_inode_s=sizeof(TablaI);
        bloque.s_block_s=sizeof(BloqueArchivos);
        bloque.s_first_ino=bul.part_start+sizeof(SuperB)+n+3*n;
        bloque.s_first_blo=bul.part_start+sizeof(SuperB)+n+3*n+n*sizeof(TablaI);
        bloque.s_bm_inode_start=bul.part_start+sizeof(SuperB);
        bloque.s_bm_block_start=bul.part_start+sizeof(SuperB)+n;
        bloque.s_inode_start=bul.part_start+sizeof(SuperB)+n+3*n;
        bloque.s_block_start= bul.part_start+sizeof(SuperB)+n+3*n+n*sizeof(TablaI);
        
        bloque.s_inodes_count=2;
        bloque.s_blocks_count=2;
        bloque.s_free_blocks_count-=2;
        bloque.s_free_inodes_count-=2;
        fseek(disco, bloque.s_bm_inode_start, SEEK_SET);
        char unidad =1;
        fwrite(&unidad,1,2,disco);
        fseek(disco, bloque.s_bm_block_start, SEEK_SET);
        fwrite(&unidad,1,2,disco);

        time_t now2;
        now2 = time(NULL);
        TablaI raiz;
        raiz.i_uid=1;
        raiz.i_gid = 1;
        raiz.i_s = sizeof(BloqueCarpetas);
        raiz.i_ctime= now2;
        raiz.i_atime= now2;
        raiz.i_mtime= now2;
        for (int i = 0; i < 15; i++)
        {
            raiz.i_block[i]=-1;
        }
        raiz.i_type='0';
        raiz.i_perm=664;
        raiz.i_block[0]=bloque.s_first_blo;
        fseek(disco, bloque.s_inode_start, SEEK_SET);
        fwrite(&raiz,sizeof(TablaI),1,disco);
        
        BloqueCarpetas origen;
        string contenido = "users.txt";
        string Actual=".";
        string Anterior="..";
        
        strcpy(origen.b_content[0].b_name,Actual.c_str());
        strcpy(origen.b_content[1].b_name,Anterior.c_str());
        strcpy(origen.b_content[2].b_name,contenido.c_str());
        origen.b_content[2].b_inodo=bloque.s_first_ino;
        fseek(disco, bloque.s_block_start, SEEK_SET);
        fwrite(&origen,sizeof(BloqueCarpetas),1,disco);
        bloque.s_first_blo+=sizeof(BloqueCarpetas);
        bloque.s_first_ino+=sizeof(TablaI);
       
        TablaI users;
        users.i_uid=1;
        users.i_gid = 1;
        users.i_s = sizeof(BloqueCarpetas);
        now2 = time(NULL);
        users.i_ctime= now2;
        users.i_atime= now2;
        users.i_mtime= now2;
        for (int i = 0; i < 15; i++)
        {
            users.i_block[i]=-1;
        }
        users.i_type='1';
        users.i_perm=664;
        users.i_block[0]=bloque.s_first_blo;
        fseek(disco, bloque.s_first_ino, SEEK_SET);
        fwrite(&users,sizeof(TablaI),1,disco);
        bloque.s_first_ino+=sizeof(TablaI);
        
        BloqueArchivos usuarios;
        contenido = "1, G, root\n1,U, root, root, 123\n";
        strcpy(usuarios.b_content,contenido.c_str());
        fseek(disco, bloque.s_first_blo, SEEK_SET);
        fwrite(&usuarios,sizeof(BloqueArchivos),1,disco);
        
        bloque.s_first_blo+=sizeof(BloqueArchivos);

        fseek(disco, bul.part_start, SEEK_SET);
        fwrite(&bloque,sizeof(SuperB),1,disco);
        fclose(disco);
    }


}