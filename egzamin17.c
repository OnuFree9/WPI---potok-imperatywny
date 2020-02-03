#include <stdio.h>
#include "egzaminy.h"
#include "stack.h"

//Zadanie w ktorym dostajemy tablice z ciagiem bitonicznym
// i mamy zwrocic ile mam takich ciagow (i,j,k) : 0 <=  i < k < j <= n -1
// A[i] < A[k] > A[j]

int IleGorek(int* A,int n){
    int l=0,p= n-1;
    while(l < p)
    {
        int s = (l+p)/2;
        if(A[s]  <  A[s+1])
            l = s+1;
        else
            p=s;
    }

    const int l_szczyt = l;
    l = 0, p = n-1;
    while(l<p)
    {
        int s = (l+p+1)/2;
        if(A[s-1] < A[s])
            l = s;
        else
            p = s-1;
    }
    const int p_szczyt = l;
    int licznik = (p_szczyt - l_szczyt + 1)*l_szczyt*(n-p-1);

    int akt = A[l_szczyt-1], __pom = p_szczyt + 1;


    int tmp = __pom;
    for(int i = l_szczyt-1;i>=0;i--)
    {
        akt = A[i],l = tmp, p = n-1;
        while(l < p) {
            int s = (l + p) / 2;
            if (A[s] > akt)
                l = s + 1;
            else
                p = s;
        }
        tmp = l;
        licznik += (i* (n-1-tmp));
    }

    return licznik;
}

//policzone o(n^2)
int IleGorek2(int* A,int n){
    int l=0,p= n-1;
    while(l < p)
    {
        int s = (l+p)/2;
        if(A[s]  <  A[s+1])
            l = s+1;
        else
            p=s;
    }

    const int l_szczyt = l;
    l = 0, p = n-1;
    while(l<p)
    {
        int s = (l+p+1)/2;
        if(A[s-1] < A[s])
            l = s;
        else
            p = s-1;
    }
    const int p_szczyt = l;
    int licznik = (p_szczyt - l_szczyt + 1)*l_szczyt*(n-p-1);

    for(int i = l_szczyt-1;i>=0;--i)
    {
        int akt = A[i], j=p_szczyt+1;
        for(; j < n && A[j] > akt;++j);
        licznik += (i* (n-1-j));
    }

    return licznik;
}


//Zad2 Korzen
// znalezc korzen;
int Korzen(Twezel G[],int n){

    int i=0;
    for(i=0;i<n;++i)
        G[i].glebokosc = 0;
    for(i = 0;i<n;++i)
    {
        Tlista* tmp = G[i].sasiedzi;
        while(tmp){
            G[tmp->w].glebokosc = -1;
            tmp = tmp->nast;
        }
    }

    while(i<n && G[i++].glebokosc != 0);

    rec_Korzen(G,i,0);

    return i;
}

void rec_Korzen(Twezel G[],int num,int wys){

    G[num].glebokosc = wys;

    Tlista* tmp = G[num].sasiedzi;
    while(tmp){
        rec_Korzen(G,tmp->w,wys+1);
        tmp=tmp->nast;
    }
}


void testZad1() {
    int f1[] = {1, 3, 4, 6, 6, 2};
    int f2[] = {0, 2, 3, 3, 2, 1};
    int f3[] = {0, 2, 3, 5, 2, 1};

    //sprarwdzanie
    printf(" %d  %d\n", IleGorek(f1, 6), IleGorek2(f1, 6));
    printf(" %d  %d\n", IleGorek(f2, 6), IleGorek2(f2, 6));
    printf(" %d  %d\n", IleGorek(f3, 6), IleGorek2(f3, 6));

}

int bliskieLiscie(Drzewo *d) {
    int s = -1;
    return rec_bliskieLiscie(d,&s);
}

int min(int a,  int b){
    return (a>b)?b:a;
}

int rec_bliskieLiscie(Drzewo *d,int* wys){
    if(d){
        int wysl,wysp,lbliskie,pbliskie, bliskie = 0;
        lbliskie = rec_bliskieLiscie(d->lsyn,&wysl);
        pbliskie = rec_bliskieLiscie(d->psyn,&wysp);

        *wys = 1+ min(wysl,wysp); //wysokosc do najmniejszego liscia

        if(lbliskie && pbliskie)
            bliskie = min(min(lbliskie,pbliskie),2+wysl+wysp);
        else if(lbliskie)
            bliskie = min(lbliskie,wysl+2+wysp);
        else if(pbliskie)
            bliskie = min(pbliskie,wysl+wysp+ 2);
        else
            if(*wys > 1 && wysl && wysp)
                bliskie = wysl + wysp + 2;

        return bliskie;
    }
    *wys = 0;
    return  0;
}

