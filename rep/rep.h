#ifndef REP_H
#define REP_H

#include <iostream>
#include <dirent.h>
using namespace std;


class rep
{
public:
    rep();
    string id;
    string ruta;
    string path;
    string name;

void makerep(string localizar,string parti);

};


#endif