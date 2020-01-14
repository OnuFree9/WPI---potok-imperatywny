#ifndef DRZEWA_H
#define DRZEWA_H


typedef int TYP;

typedef struct Twezel{
    TYP p;
    struct Twezel* lsyn;
    struct Twezel* psyn;
}Drzewo,Twezel;


void wyczysc(Twezel* d);
Drzewo* nowy_wezel(TYP p);
int wysokosc(Drzewo* p);
int srednica(Drzewo* d);
int pom_srednica(Drzewo* d,int* h);



#endif