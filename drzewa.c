#include <stdio.h>
#include <stdlib.h>
#include "drzewa.h"


int max(int a, int b);

Drzewo* nowy_wezel(TYP p){
    Drzewo* nowy = malloc(sizeof(*nowy));
    nowy->p = p;
    nowy->lsyn = NULL;
    nowy->psyn = NULL;
    return nowy;
}

void wyczysc(Twezel* d){
    if(d){
        wyczysc(d->lsyn);
        wyczysc(d-> psyn);
        free(d);
    }
}
 
int wysokosc(Drzewo* p){
    if(p)
       return 1 + max(wysokosc(p->lsyn),wysokosc(p->psyn));
    return 0;
}

int max(int a, int b)
    return (a>b?a:b);
}

int srednica(Drzewo* d) {
    int h1, h2;
    return pom_srednica(d, &h1, &h2);
}

int pom_srednica(Drzewo* d,int* h){
    if(d){
        int sr_lewa,sr_prawa;
        int h_lsyn,h_psyn;
        sr_lewa = pom_srednica(d->lsyn,&h_lsyn);
        sr_prawa = pom_srednica(d->psyn,&h_psyn);

        *h = 1+ max(h_lsyn,h_psyn);
        return max(max(sr_lewa,sr_prawa),2+h_lsyn+h_psyn);
    }
    *h = -1;
    return 0;
}