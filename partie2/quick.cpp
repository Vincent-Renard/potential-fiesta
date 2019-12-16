#include <iostream>

#include <omp.h>
#include <math.h>
#include "fonctions.h"

using namespace std;

/*
Verification du tri
*/
bool sorted(int *tab ,int n){
  for (size_t t = 1; t < n; t++) {
    if (tab[t-1]>tab[t]) return false;
  }
  return true;
}

void QuickSort(int *tab,int debut,int taille,int nb_threads) {
  if (debut<taille) {
    int pivot = tab[debut];

    int * tmp = new int[taille - debut];
    for (int i = 0; i < taille - debut; i++) {
      tmp[i] = tab[debut + i];
    }

    int * s = new int[nb_threads];
    int * r = new int[nb_threads];
    Partitionnement(tmp, taille - debut, pivot, nb_threads, s, r);

    int * somme_left = new int[nb_threads + 1];
    int * somme_right = new int[nb_threads + 1];
    SommePrefixe(s, r, somme_left, somme_right, nb_threads);

    int * res = new int[taille - debut];
    Rearrangement(somme_left, somme_right, tmp, res, taille - debut, nb_threads);

    for (int i = 0; i < taille - debut; i++) {
      tab[debut + i] = tmp[i];
    }

    int ss = debut + somme_left[nb_threads] - 1;
    swap(tab+debut,tab+ss);
    QuickSort(tab,debut,ss,nb_threads);
    QuickSort(tab,ss+1,taille,nb_threads);

    delete[] tmp;
    delete[] s;
    delete[] r;
    delete[] somme_left;
    delete[] somme_right;
    delete[] res;
  }
}

void QuickSort(int *tab,int taille,int nb_threads) {
  QuickSort(tab,0,taille,nb_threads);
  cout << (sorted(tab,taille) ? "OK":"KO")<< '\n';
}

int main(int argc, char** argv) {

    if (argc<2) {
        cout << "Mauvaise utilisation du programme :" << endl;
        cout << "./Etape1 [taille du tableau] [nombre de threads]" << endl;
        return 1;
    }

    omp_set_nested(1);

    int n = atoi(argv[1]);
    int nthreads = atoi(argv[2]);

    int *tab = new int[n];
    srand(time(NULL));
    //srand(64);
    for (int i = 0; i < n; i++)
        tab[i] = rand() % n;
        cout << "dans le main les données initiales :" << endl;
        for (int i = 0; i < n; i++)
            cout << tab[i] << " ";
        cout << endl;

    QuickSort(tab, n, nthreads);
    for (size_t i = 0; i < n; i++) {
      cout << tab[i] << " ";
    }
    cout << endl;

    /*
    int pivot = tab[4];

    cout << "pivot=" << pivot << endl;
    int* s = new int[nthreads];
    int* r = new int[nthreads];
    Partitionnement(tab,n,pivot,nthreads,s,r);

    cout << "dans le main les données avec partitionnement :" << endl;
    for (int i = 0; i < n; i++)
        cout << tab[i] << " ";
    cout << endl;

    for (int i=0; i<nthreads; i++)
        cout << s[i] << " ";
    cout << endl;
    for (int i=0; i<nthreads; i++)
        cout << r[i] << " ";
    cout << endl;


    int* somme_left = new int[nthreads+1];
    int* somme_right = new int[nthreads+1];

    SommePrefixe(s,r,somme_left,somme_right, nthreads);

    cout << "la somme des préfixes (gauche et droit)" << endl;

    for (int i=0; i<nthreads+1; i++)
        cout << somme_left[i] << " ";
    cout << endl;

    for (int i=0; i<nthreads+1; i++)
        cout << somme_right[i] << " ";
    cout << endl;

   int* res = new int[n];
    Rearrangement(somme_left, somme_right, tab, res, n, nthreads);

    cout << "dans le main les données après réarrangement :" << endl;
    for (int i = 0; i < n; i++)
        cout << res[i] << " ";
    cout << endl;
*/


    delete[] tab;
    //delete[] s;
    //delete[] r;
    //delete[] somme_left;
    //delete[] somme_right;
    return 0;
}
