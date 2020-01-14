#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define ROZ_DOM 1000
#define LICZNIK 3
#define  MIANOWNIK 2
#define INDEX_POCZ 0
#define INDEX_KON 1
#define KOL_NOWY_WIERSZ 2
#define MAGAZYN 28
#define STACK_SIZE 1000;

typedef struct{
    int ile_wierszy;
    char** plansza;
}Plansza;

Plansza init_TPlansza();
char** tworz_Plansze(int*);
char** nowa_Plansza();
void drukuj_Plansze(Plansza);
void czyszczenie(Plansza* P);

Plansza plansza_wizyt(Plansza Org);
void aktualizuj_kopie(Plansza*, Plansza);

typedef struct{
    char skrzynia;
    int kierunek;
}Polecenie;

typedef struct{
    int wrs;
    int kol;
    int litera;
    int kier;
}Pozycja;

Pozycja** tworz_magazyn(Plansza);
void wyczysc_magazyn(Pozycja **);
void kompletuj_magazyn(Plansza, Pozycja**, Polecenie);
void przepisz_pozycje(Pozycja* org,Pozycja* kopia);

typedef  struct{
    int height;
    int top;
    Pozycja* memory;
}Stack;

//Stos Pozycji
void init(Stack** S);
bool pusty_stos(Stack f);
bool pelny_stos(Stack f);
void pobierz_stos(Stack *S, Pozycja *p);
void poszerzStos(Stack** t);
void wstaw_stos(Stack *t, Pozycja p);
Pozycja top(Stack*);
void wyczysc_stos(Stack *t);

typedef struct{
    Pozycja* T;
    int pocz,kon,rozm;
}PQueue;

//Kolejka Pozycji
void wstaw_za(PQueue *Q, Pozycja L);
void pobierzK(PQueue *Q, Pozycja *R);
int nastepny(int i, PQueue Q);
bool czy_pustaK(PQueue Q);
bool czy_pelnaK(PQueue Q);
void init_kolejka(PQueue**k);
void czysc_kolejke(PQueue* A);

//Sokoban
bool dostepnaSkrzynia(Plansza*,Pozycja**,Polecenie);
bool czy_mozna_odwiedzic(char* wiersz,int kol,Pozycja* akt);
bool czy_mozna_przesunac(Plansza p_wizyt, int w, int kol);
void przesun_kom(int*,int*,int);
bool czy_sciezka(Plansza*, PQueue*);
void otrzymaj_pozycje(Pozycja* skrzynia, int);
Pozycja* znajdz_obiekt(Plansza, char);
bool sciagnij_Polecenie(Polecenie *ptr);
void przesun_skrzynie(Plansza*, Pozycja, Pozycja);
void przesun_ludzika(Plansza*, Pozycja, Pozycja);
void faza_przesuwania(Plansza*,Pozycja**,Stack*);
void cofanie(Plansza* T,Pozycja**,Stack*);

char **nowa_Plansza() {
    //zwraca macierz charow o domyslnym rozmiarze
    char **nowa = malloc(ROZ_DOM * sizeof(*nowa));
    for (int i = 0; i < ROZ_DOM; ++i)
        nowa[i] = malloc(ROZ_DOM * sizeof(**nowa));
    return nowa;
}

char **tworz_Plansze(int *nr_wiersza) {
    *nr_wiersza = 0;
    /*
     * wywołanie: tworz_Plansze(&ptr) gdzie int ptr = 0;
     * pobiera znaki z wejscia po czym umieszcza je w nowoutworzonej macierzy
     * pierwsze dwa indeksy kazddego wiersza przechowuja zakres planszy w tym wierszu
     * w ostatniej kolumnie pierwszego wiersza jest przechowywana ilosc wierszy calej planszy
     * petla konczy sie gdy zostanie wykryta podwojny znak '\n' na wejsciu
     */
    char **plansza_wejsciowa = nowa_Plansza(), wejscie = getchar();
    bool podwojny_enter = 1;
    int max_wiersz = ROZ_DOM;

    while (wejscie != EOF && podwojny_enter) {

        int pocz_wiersza = 0, max_kol = ROZ_DOM, nr_kolumny = KOL_NOWY_WIERSZ;
        char *wiersz = plansza_wejsciowa[*nr_wiersza];

        while (wejscie != EOF && wejscie != '\n') {

            if (!pocz_wiersza && wejscie != ' ') {
                //wyznaczanie poczatku planszy w wierszu
                wiersz[INDEX_POCZ] = nr_kolumny;
                pocz_wiersza = 1;
            }

            wiersz[nr_kolumny] = wejscie;
            nr_kolumny++;

            if (max_kol <= nr_kolumny) {
                //przepełnienie wiersza
                int nowy_roz = LICZNIK * max_kol / MIANOWNIK + 1;
                wiersz = realloc(wiersz, nowy_roz * sizeof(char));
            }
            wejscie = getchar();
        }

        if (wejscie == '\n') {
            //zakonczenie wiersza & zmniejszenie zajmowanego rozmiaru
            wiersz[INDEX_KON] = nr_kolumny;
            wiersz = realloc(wiersz,nr_kolumny* sizeof(char));
            plansza_wejsciowa[*nr_wiersza] =  wiersz;
            //nr_wiersza bedzie wskazywal teraz na nastepny wiersz
            (*nr_wiersza)++;

            if ((*nr_wiersza) >= max_wiersz) {
                //wydluz kolumny
                int nowy_roz = LICZNIK / MIANOWNIK * (max_wiersz) + 1;
                plansza_wejsciowa = realloc(plansza_wejsciowa, nowy_roz * sizeof(char *));
                for (; (max_wiersz) < nowy_roz; ++max_wiersz)
                    plansza_wejsciowa[max_wiersz] = malloc(ROZ_DOM * sizeof(char));
            }

            char wejscie2 = getchar();
            if (wejscie == '\n' && wejscie2 == '\n')
                podwojny_enter = 0;
            else
                wejscie = wejscie2;
        }
    }
    if (max_wiersz > *nr_wiersza ){
        for(int i= *nr_wiersza; i<max_wiersz;++i)
            free(plansza_wejsciowa[i]);
    }
    return plansza_wejsciowa;
}

void drukuj_Plansze(Plansza P) {
    int ile_wierszy = P.ile_wierszy;
    for (int i = 0; i < ile_wierszy; i++) {
        char* wiersz = P.plansza[i];
        for (int j = 2; j < wiersz[INDEX_KON];)
            printf("%c", wiersz[j++]);
        printf("\n");
    }
}

void czyszczenie(Plansza *P ){
    int i = P->ile_wierszy-1;
    while(i>=0) {
        char *wiersz = P->plansza[i--];
        free(wiersz);
    }
    free(P->plansza);
}

void init(Stack **S) {
    *S = (Stack *) malloc(sizeof **S);
    (*S)->height = STACK_SIZE;
    (*S)->memory = (Pozycja *) malloc(((*S)->height) * sizeof(Pozycja));
    (*S)->top = 0;
}

bool pusty_stos(Stack f) {
    return f.top == 0;
}

bool pelny_stos(Stack f) {
    return f.top == f.height;
}

void pobierz_stos(Stack *S, Pozycja* p) {
    S->top--;
    *p = (S->memory[S->top]);
}

void poszerzStos(Stack **t) {
    (*t)->height = (*t)->height * LICZNIK / MIANOWNIK + 1;
    (*t)->memory = (Pozycja *) realloc((*t)->memory, (*t)->height * sizeof(Pozycja *));
}

void wstaw_stos(Stack *t, Pozycja p) {
    if (!pelny_stos(*t))
    t->memory[t->top++] = p;
}

Pozycja top(Stack *t) {
    return t->memory[t->top - 1];
}

void wyczysc_stos(Stack *t) {
    free(t->memory);
    free(t);
}

void wstaw_za(PQueue* Q, Pozycja L){
    if(!czy_pelnaK(*Q)) {
        Q->T[Q->kon] = L;
        Q->kon = nastepny(Q->kon, *Q);
    }
}

void pobierzK(PQueue* Q, Pozycja* R){
    *R = Q->T[Q->pocz];
    Q->pocz = nastepny(Q->pocz, *Q);
}

int nastepny(int i, PQueue Q){
    if(i == Q.rozm-1)
        return 0;
    return i+1;
}

bool czy_pustaK(PQueue Q){
    return Q.kon == Q.pocz;
}

bool czy_pelnaK(PQueue Q){
    return (Q.pocz == nastepny(Q.kon, Q));
}

void init_kolejka(PQueue** k){
    *k = (PQueue*) malloc(sizeof**k);
    (*k)->pocz = 0;
    (*k)->kon = 0;
    (*k)->rozm = STACK_SIZE;
    (*k)->T =(Pozycja*)  malloc((*k)->rozm* sizeof(Pozycja));
}

void czysc_kolejke(PQueue* A){
    free(A->T);
    free(A);
}

bool sciagnij_Polecenie(Polecenie* ptr){
    char c = getchar();
    while (c!= EOF && c != '\n' && c!='.'){
        if( c >= 'a' && c <='z')
            ptr->skrzynia = c;
        else if (c >= '0' && c<='9')
            ptr->kierunek = (int) c - 48;
        c = getchar();
    }
    return (c == '\n')?true:false;
}

Plansza init_TPlansza(){
    Plansza T;
    int ile_wierszy;
    T.plansza = tworz_Plansze(&ile_wierszy);
    T.ile_wierszy = ile_wierszy;
    return T;
}

Plansza plansza_wizyt(Plansza Org){
    int ile_w = Org.ile_wierszy;
    Plansza plansza_odwiedzonych;
    char** plansza = malloc(ile_w * sizeof(char*));

    for(int i=0;i<ile_w;i++) {

        char *wiersz_org = Org.plansza[i];
        int ile_kom = wiersz_org[INDEX_KON];
        int pocz = wiersz_org[INDEX_POCZ];
        plansza[i] = malloc(ile_kom * sizeof(char));

        char* wiersz = plansza[i];
        wiersz[INDEX_KON] = ile_kom;
        wiersz[INDEX_POCZ] = pocz;

        for(int j= 2;j<pocz;j++)
            plansza[i][j] = 3;

        for(;pocz<ile_kom;pocz++){
            char kom = wiersz_org[pocz];
            switch (kom){
                case '*':
                case '@':
                case '-':
                case '+':
                    wiersz[pocz] = 0;
                    break;
                default:
                    wiersz[pocz] = 3;
                    break;
            }
        }
        plansza[i] = wiersz;
    }

    plansza_odwiedzonych.plansza = plansza;
    plansza_odwiedzonych.ile_wierszy = ile_w;

    return plansza_odwiedzonych;
}

void aktualizuj_kopie(Plansza* kopia, Plansza Org)
{
    int i=0,ile_w = kopia->ile_wierszy;
    while(i<ile_w){
        char* wiersz = kopia->plansza[i],*wiersz_org = Org.plansza[i];
        int pocz = wiersz[INDEX_POCZ], kon = wiersz[INDEX_KON];
        for(;pocz<kon;pocz++){
            switch (wiersz_org[pocz]){
                case '@':
                case '*':
                case '-':
                case '+':
                    wiersz[pocz] = 0;
                    break;
                default:
                    wiersz[pocz] = 3;
            }
        }
        kopia->plansza[i++] = wiersz;
    }
}
void drukujPolecenie(Polecenie p) {
    printf("%c %d\n", p.skrzynia, p.kierunek);
}

//dostepnaSkrzynia(...) zwraca nam info czy mozemy do niej dojsc
//oraz czy mozemy ja przesunac
// na samym końcu (flag==true) => *R zostanie zmienione zeby wskazywala nowa pozycje skrzyni
bool dostepnaSkrzynia(Plansza * p_wizyt, Pozycja ** Mgz, Polecenie R)
{
    bool flag = false;
    char szukanaSkrzynia = R.skrzynia;
    int litera = szukanaSkrzynia - 96;
    Pozycja *skrzynia = Mgz[litera], *ludzik = Mgz[0];
    int w = skrzynia->wrs, k = skrzynia->kol,kier = R.kierunek;

    //czy za skrzynia nic nie stoi
    przesun_kom(&w,&k,kier);
    flag = czy_mozna_przesunac(*p_wizyt,w,k);

    if(flag) {
        //ustawienie na docelowe miejsce
        przesun_kom(&w,&k,10-kier);
        przesun_kom(&w,&k,10-kier);
        flag = czy_mozna_przesunac(*p_wizyt, w, k);
    }

    if(flag) {
        //miejsce do ktorego poszukiwana jest sciezka
        p_wizyt->plansza[w][k] = 2;

        PQueue *Kolejka;
        init_kolejka(&Kolejka);
        wstaw_za(Kolejka, *ludzik);

        flag = czy_sciezka(p_wizyt, Kolejka);

        czysc_kolejke(Kolejka);
    }

    if(flag){
        if(!Mgz[27])
            Mgz[27] = malloc(sizeof(Pozycja));
        Mgz[27]->kol = k;
        Mgz[27]->wrs = w;
        Mgz[27]->litera = R.skrzynia - 96;
        Mgz[27]->kier = R.kierunek;
    }

    return flag;
}

//sprawdza czy 'za' kostka nie ma sciany
bool czy_mozna_przesunac(Plansza p_wizyt, int w, int kol){
    if( w>= 0 && w < p_wizyt.ile_wierszy){
        if(kol >= p_wizyt.plansza[w][INDEX_POCZ] && kol < p_wizyt.plansza[w][INDEX_KON]) {
            char kom = p_wizyt.plansza[w][kol];
            return kom == 0;
        }
    }
    return false;
}

//sprawdza czy 'przed' jest korytarz
bool czy_mozna_odwiedzic(char* wiersz,int kol,Pozycja* akt){
    int kon = wiersz[INDEX_KON],pocz = wiersz[INDEX_POCZ];
    if(kol < kon && kol >= pocz && wiersz[kol]!= 1 && wiersz[kol]!= 3){
        akt->kol = kol;
        return true;
    }
    return false;
}

//przesuwa pozycje w danym kierunku
void otrzymaj_pozycje(Pozycja* skrzynia, int kier){

    //skrzynia wskazuje gdzie jest skrzynia aktualnie
    int w = skrzynia->wrs,k = skrzynia->kol;
    przesun_kom(&w,&k,kier);
    skrzynia->wrs = w;
    skrzynia->kol = k;
}

//BFS znajdujacy sciezke
bool czy_sciezka(Plansza* odwiedzone, PQueue* K){
    Pozycja akt;
    int k,w,ile_wierszy = odwiedzone->ile_wierszy;
    bool cel = false;

    do{
        //wyrzucany adres kommorki
        //zakladam poprawnosc Pozycji na planszy
        pobierzK(K, &akt);
        w = akt.wrs;
        k = akt.kol;

        char* wiersz = odwiedzone->plansza[w],*wiersz_wyzej = NULL,*wiersz_nizej = NULL;

        if(w > 0){
            akt.wrs = w-1;
            wiersz_wyzej = odwiedzone->plansza[w-1];
            if(czy_mozna_odwiedzic(wiersz_wyzej,k,&akt)) {
                wstaw_za(K, akt);
            }
        }

        if(w < ile_wierszy-1) {
            akt.wrs = w+1;
            wiersz_nizej = odwiedzone->plansza[w + 1];
            if(czy_mozna_odwiedzic(wiersz_nizej,k,&akt)){
                wstaw_za(K, akt);
            }
        }

        akt.wrs = w;
        if(czy_mozna_odwiedzic(wiersz,k+1,&akt)){
            wstaw_za(K, akt);
        }

        if(czy_mozna_odwiedzic(wiersz,k-1,&akt)) {
            wstaw_za(K, akt);
        }

        if(wiersz[k]==2)
            cel = true;
        else
            wiersz[k]=1;

        odwiedzone->plansza[w] = wiersz;
    }while(!cel && !czy_pustaK(*K));
    return cel;
}

//zwraca wskaznik do Pozycji obiektu
Pozycja* znajdz_obiekt(Plansza p, char obiekt)
{
    //znajduje pozycje skrzynki/ludzika i ją zwraca
    Pozycja* pos = malloc(sizeof(*pos));
    // flag == 1 nie znaleziono obiektu
    bool flag = 1;
    char** PLA = p.plansza;
    for(int i=0;i<p.ile_wierszy && flag;++i){

        char* wiersz = PLA[i];
        int start =wiersz[INDEX_POCZ], kon =wiersz[INDEX_KON];

        for(;start<kon && flag;start++){
            //mozliwe obiekty = {'@','*'} || ({'a',..,'z'} || {'A',...'Z'})
            if(obiekt != '@') {
                if (wiersz[start] == obiekt || wiersz[start] == (obiekt - 32)) {
                    pos->wrs = i;
                    pos->kol = start;
                    flag = 0;
                }
            }
            else{
                if (wiersz[start] == obiekt || wiersz[start] == '*') {
                    pos->wrs = i;
                    pos->kol = start;
                    flag = 0;
                }
            }
        }
    }

    if(flag){
        free(pos);
        pos = NULL;
    }
    return pos;
}

//tablica wskaznikow na pozycje skrzyn oraz ludzika
Pozycja **tworz_magazyn(Plansza T) {
    Pozycja **nowy = malloc(MAGAZYN*sizeof(*nowy));
    for(int i=1;i<MAGAZYN;++i)
        nowy[i]=NULL;
    nowy[0] = znajdz_obiekt(T, '@');
    nowy[0]->litera = '@';
    return nowy;
}

//czysci magazyn
void wyczysc_magazyn(Pozycja** M){
    for(int i=0;i<MAGAZYN;++i)
        free(M[i]);
    free(M);
}

//inicjalizuje dana pozycje przy pierwszym pojawieniu sie polecenia
//dla danej skrzynki
void kompletuj_magazyn(Plansza glowna, Pozycja** Mgz, Polecenie R){
    char obiekt = R.skrzynia;
    int literka = obiekt - 96;
    Pozycja* skrzynia = Mgz[literka];
    if(!skrzynia){
        skrzynia = znajdz_obiekt(glowna, obiekt);
        Mgz[literka] = skrzynia;
        Mgz[literka] -> litera = literka;
    }
}

//przekopiowuj pozycje
void przepisz_pozycje(Pozycja* org,Pozycja* kopia)
{
    kopia->kol = org->kol;
    kopia->wrs = org->wrs;
    kopia->litera = org-> litera;
}

//przesuwa dane parametry w danym kierunku
void przesun_kom(int *wrs,int* kol,int kier){
    if(kier==2)
        (*wrs)++;
    else if(kier==4)
        (*kol)--;
    else if(kier==6)
        (*kol)++;
    else if(kier==8)
        (*wrs)--;
}

void przesun_skrzynie(Plansza* P, Pozycja stara_pozycja, Pozycja nowa_pozycja)
{

    //zakladana jest poprawnosc juz danych pozycji <= sprawdzane przy dostepnaSkrzynia
    int s_wrs = stara_pozycja.wrs,s_kol = stara_pozycja.kol;
    int n_wrs = nowa_pozycja.wrs,n_kol = nowa_pozycja.kol;

    char mala_kom, duza_kom;
    char kom = P->plansza[s_wrs][s_kol], kom_2 = P->plansza[n_wrs][n_kol];

    if(kom < 97){
        duza_kom = kom;
        mala_kom = duza_kom + 32;
    }
    else{
        mala_kom = kom;
        duza_kom = mala_kom - 32;
    }

    if (kom_2 == '-')
        P->plansza[n_wrs][n_kol] = mala_kom;
    else //kom2 == '+'
        P->plansza[n_wrs][n_kol] = duza_kom;

    if(kom == duza_kom)
        P->plansza[s_wrs][s_kol] = '+';
    else
        P->plansza[s_wrs][s_kol] = '-';
}

void przesun_ludzika(Plansza* glowna, Pozycja stara_pozycja, Pozycja nowa_pozycja) {

    int s_wrs = stara_pozycja.wrs,s_kol = stara_pozycja.kol;
    int n_wrs = nowa_pozycja.wrs,n_kol = nowa_pozycja.kol;

    if(glowna-> plansza[s_wrs][s_kol] == '@')
       glowna->plansza[s_wrs][s_kol] = '-';
    else
        glowna->plansza[s_wrs][s_kol] = '+';

    if(glowna->plansza[n_wrs][n_kol]== '+')
        glowna->plansza[n_wrs][n_kol] = '*';
    else
        glowna->plansza[n_wrs][n_kol] = '@'; //must be '-'
}

void faza_przesuwania(Plansza* T,Pozycja** Mgz,Stack* stos_pozycji){
    int skrz = Mgz[27]->litera;
    int kier = Mgz[27]->kier;

    Pozycja* stara_pozycja = Mgz[0];
    Pozycja* nowa_pozycja = Mgz[27],posrednia;
    przesun_ludzika(T, *stara_pozycja, *nowa_pozycja);

    wstaw_stos(stos_pozycji, *stara_pozycja);
    wstaw_stos(stos_pozycji, *nowa_pozycja);

    stara_pozycja = nowa_pozycja;

    //przesuwanie szkrzyni
    // printf("[%d]\n",skrz);
    przepisz_pozycje(Mgz[skrz],&posrednia);
    nowa_pozycja = Mgz[skrz];
    otrzymaj_pozycje(nowa_pozycja, kier);
    przesun_skrzynie(T, posrednia, *nowa_pozycja);
    wstaw_stos(stos_pozycji, posrednia);

    //przeswuwanie ludzika za skrzynia
    przesun_ludzika(T, *stara_pozycja, posrednia);

    //aktualizacja Magazynu
    przepisz_pozycje(&posrednia,Mgz[0]);
    Mgz[skrz]=nowa_pozycja;
}

void cofanie(Plansza *T, Pozycja ** Mgz , Stack * stos_pozycji)
{
    Pozycja ludzik,ludzik2,skrzynia;
    pobierz_stos(stos_pozycji, &skrzynia);
    pobierz_stos(stos_pozycji, &ludzik);
    int skr = skrzynia.litera;

    //przesuwanai ludzika i skrzyni!!!
    przesun_ludzika(T, *Mgz[0], ludzik);
    przesun_skrzynie(T, *Mgz[skr], skrzynia);
    przepisz_pozycje(&skrzynia,Mgz[skr]);

    pobierz_stos(stos_pozycji, &ludzik2);
    przesun_ludzika(T, ludzik, ludzik2);
    przepisz_pozycje(&ludzik2,Mgz[0]);
}

int main() {
    Plansza T  = init_TPlansza(),wizyty = plansza_wizyt(T);
    Polecenie R;
    Stack* stos_pozycji;
    init(&stos_pozycji);

    drukuj_Plansze(T);

    Pozycja** Mgz = tworz_magazyn(T);
    while(sciagnij_Polecenie(&R))
    {
        if(R.kierunek != 0)
        {
            kompletuj_magazyn(T, Mgz, R);
            aktualizuj_kopie(&wizyty, T);

            if (dostepnaSkrzynia(&wizyty, Mgz, R))
                faza_przesuwania(&T,Mgz,stos_pozycji);
        }

        //cofanie <= nie pusty stos
        else if(!pusty_stos(*stos_pozycji))
            cofanie(&T,Mgz,stos_pozycji);

        drukuj_Plansze(T);
    }

    //sprzatanie
    wyczysc_stos(stos_pozycji);
    wyczysc_magazyn(Mgz);
    czyszczenie(&wizyty);
    czyszczenie(&T);
    return 0;
}

