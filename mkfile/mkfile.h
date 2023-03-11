#ifndef MKFILE_H
#define MKFILE_H

#include <iostream>
#include <dirent.h>
#include "../mount/mount.h"


using namespace std;


class mkfile
{
public:
    mkfile();
    string path;
    bool r;
    string cont;
    int size;

void makefile(mount montado);

};


#endif