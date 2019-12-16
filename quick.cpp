#include <iostream>
#include <random>
#include <omp.h>
using namespace std;

void swap(int* a, int* b){
  int tmp = *a;
  *a = *b;
  *b = tmp;
}
/*

La fonction SommePrefixe qui permet de determiner les tableaux somme left et somme right de taille
nb threads + 1 tels que somme left[i] (respectivement somme right[i]) est le nombre d'elements plus petits
ou egaux (respectivement plus grands) que le pivot et qui ont ete trouves par les threads qui precedent le
thread i (somme left[0] = somme right[0]=0). La signature a respecter est la suivante
*/
void SommePrefixe(int* s, int* r,int* somme_left, int* somme_right, int nb_threads) {
  somme_left[0] = 0;
  somme_right[0] = 0;

  for (int i = 1; i < nb_threads + 1; i++) {
    somme_left[i] = somme_left[i - 1] + s[i - 1];
    somme_right[i] = somme_right[i - 1] + r[i - 1];
  }
}
/*

La fonction Partitionnement qui permet de repartir les elements a droite ou a gauche du pivot. Chaque
thread devra eectuer le partitionnement d'un morceau du tableau global. La signature a respecter est la
suivante
*/
void Partitionnement(int* tab, int n, int pivot, int nb_threads, int* s, int* r) {
  #pragma omp parallel for
  for (int i = 0; i < n; i++) {
    int id = omp_get_thread_num();
    if (tab[i] <= pivot) {
      s[id] += 1;
    }
    else {
      r[id] += 1;
    }
  }
}
/*

La fonction Rearrangement qui permet de reconstruire correctement un nouveau tableau tel que tous les
elements plus petits ou egaux au pivot soient a gauche dans le tableau et les autres a droite. La signature a
respecter est la suivante
*/
void Rearrangement(int* somme_left, int* somme_right, int* tab, int* res,int n, int nb_threads){
  int a = 0;
  int b = somme_left[nb_threads];
  #pragma omp parallel for
  for (int i = 0; i < n; i++) {
    int id = omp_get_thread_num();
    for (int i = 0; i < somme_left[i + 1]; i++) {
      res[a] = tab[i];
      a += 1;
    }
    for (int i = 0; i < somme_right[i + 1]; i++) {
      res[b] = tab[i];
      b += 1;
    }
  }
}
void QuickSort(int* A, int q, int r) {
  if (q<r) {
    int x = A[q];
    int s = q;
    for (int i=q+1; i<r; i++) {
      if (A[i]<=x) {
        s++;
        swap(A+s,A+i);
      }
    }
    swap(A+q,A+s);
    QuickSort(A,q,s);
    QuickSort(A,s+1,r);
  }
}

/*
Verification du tri
*/
bool sorted(int *tab ,int n){
  for (size_t t = 1; t < n; t++) {
    if (tab[t-1]>tab[t]) return false;
  }
  return true;
}
int main(int argc, char** argv) {

  if (argc<3) {
    cout << "Mauvaise utilisation du programme :" << endl;
    cout << "./Tri [taille du tableau] [nbthreads] " << endl;
    return 1;
  }


  int n = atoi(argv[1]);
  int* tab = new int[n];
  int n_ths = atoi(argv[2]);
  omp_set_num_threads(n_ths);

  srand(time(NULL));
  for (int i=0; i<n; i++)
  tab[i] = rand()%100;

  int pivot =   50; //TODO A CHANGER !
  cout  << "PIVOT : "<<pivot<<endl ;

  for (int i=0; i<n; i++)
  cout << tab[i] << " ";
  cout << endl;

  int * s = new int[n_ths];
  int * r = new int[n_ths];
  Partitionnement(tab, n, pivot, n_ths, s, r);
  int * somme_left = new int[n_ths + 1];
  int * somme_right = new int[n_ths + 1];
  SommePrefixe(s, r, somme_left, somme_right, n_ths);
  cout << "somme_left  = ";
  for (int i = 0; i < n_ths + 1; i++) {
    cout << somme_left[i] << " ";
  }
  cout << endl;
  cout << "somme_right = ";
  for (int i = 0; i < n_ths + 1; i++) {
    cout << somme_right[i] << " ";
  }
  cout << endl;

  //QuickSort(tab,0,n);
  cout  << "s = " ;
  for (int i = 0; i < n_ths; i++) {
    cout  << s[i]<<" " ;

  }
  cout  <<endl;
  cout  << "r = " ;
  for (size_t t = 0; t < n_ths; t++) {
    cout  << r[t]<<" " ;
  }
  cout  <<endl;

  for (int i=0; i<n; i++)
  cout << tab[i] << " ";
  cout << endl;
  return 0;
}
