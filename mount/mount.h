#ifndef MOUNT_H
#define MOUNT_H

#include <iostream>
#include <dirent.h>

using namespace std;


class mount
{
public:
    mount();
    string id;
    string path;
    string name;
    int numeroParticion();
};



#endif