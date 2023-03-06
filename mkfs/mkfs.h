#ifndef MKFS_H
#define MKFS_H

#include <iostream>
#include <dirent.h>
#include "../mount/mount.h"

using namespace std;


class mkfs
{
public:
    mkfs();
    string id;
    string type;
    string fs;

void makefs(mount montado);

};


#endif