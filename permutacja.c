void rekperm(int k,int n,int perm[],int czyDostepna[]){
    if(k==n){
      for(int i=0;i<n;++i)
        printf("%d",perm[i]);
      printf("\n");
    }
    else{
        for(int l=0;l<n;l++){
          if(czyDostepna[l]==1){
            perm[k] = l+1;
            czyDostepna[l] = 0;
            rekperm(k+1,n,perm,czyDostepna);
            czyDostepna[l] = 1;
          }
        }
    }
}

void permutacja(int n){
  int* tab = (int*) malloc(n*sizeof *tab);
  int* czyDostepna =  (int*) malloc(n*sizeof *tab);
  for(int i=0;i<n;++i)
    czyDostepna[i]=1;
  rekperm(0,n,tab,czyDostepna);
  free(tab);
  free(czyDostepna);
}

int main(int argc,char* argv[]){
  int t = atoi(argv[argc-1]);
  //printf("%d\n",t);
  permutacja(t);
  return 0;
}
