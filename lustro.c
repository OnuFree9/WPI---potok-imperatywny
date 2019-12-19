void lustro(List* t) {
    pomoc_lustro(t,t);
}

List* pomoc_lustro(List* obecny,List* pocz){
    if(obecny){
        List* nastepny = pomoc_lustro(obecny->next,pocz);
        nastepny->wsk = obecny;
        return nastepny->next;
    }
    else
        return pocz;
}

