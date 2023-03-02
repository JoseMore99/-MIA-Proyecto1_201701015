
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


