
#include <iostream>
#include <time.h>
using namespace std;

struct Ebr
{
   char part_status;
   char part_fit;
   int part_start;
   int part_s;
   int part_next;
   char part_name[16];
};

struct Partition
{
   char part_status;
   char part_type;
   char part_fit;
   int part_start;
   int part_s;
   char part_name[16];
};


struct Mbr
{
   int mbr_tamano;
   time_t mbr_fecha_creacion;
   int mbr_signature;
   char dsk_fit;
   Partition mbr_partition[4];
};

struct SuperB
{
   int s_filesystem_type;
   int s_inodes_count;
   int s_blocks_count;
   int s_free_blocks_count;
   int s_free_inodes_count;
   time_t s_mtime;
   time_t s_umtime;
   int s_mnt_count;
   int s_magic;
   int s_inode_s;
   int s_block_s;
   int s_first_ino;
   int s_first_blo;
   int s_bm_inode_start;
   int s_bm_block_start;
   int s_inode_start;
   int s_block_start;
};

struct TablaI
{
   int i_uid;
   int i_gid;
   int i_s;
   time_t i_atime;
   time_t i_ctime;
   time_t i_mtime;
   int i_block[15];
   char i_type;
   int i_perm;
};

struct content
{
   char b_name[12];
   int b_inodo;
};


struct BloqueCarpetas
{
   content b_content [4];
};

struct BloqueArchivos
{
   char b_content [64];
};
struct BloqueApuntadores
{
   int b_content [16];
};


