#include <iostream>
#include <mpi.h>
#define ROOT 0

using namespace std;

void swap(int* a, int* b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void QuickSort_seq(int* A, int q, int r) {
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
        QuickSort_seq(A,q,s);
        QuickSort_seq(A,s+1,r);
    }
}

void QuickSort(int* tab, int n) {
  int pid, nprocs,q,r,n_local;
  int *count,*displ;

  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;
  MPI_Request request;
  MPI_Status status;
  q=n/nprocs ; r= n%nprocs;

  if (pid == ROOT){
    count = new int[nprocs];
    displ= new int[nprocs];
    for (size_t proc_i = 0; proc_i < nprocs; proc_i++) {
      count[proc_i] = proc_i < r ? q+1 : q;
      displ[proc_i] = proc_i < r ? (q+1)*proc_i : q*proc_i +r;

    }

  }
  n_local=pid < r ? q+1 : q;
  int *tab_local = new int[n_local];
  MPI_Scatterv(tab, count, displ, MPI_INT,tab_local , n_local, MPI_INT, ROOT, MPI_COMM_WORLD);
  cout <<pid<< " [ ";
      for (int i=0; i<n_local; i++)
          cout << tab_local[i] << " ";
      cout << endl;
}


int main(int argc, char** argv) {
  int pid, nprocs,q,r,n;
  int* tab;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Request request;
  MPI_Status status;

    if (argc<2) {
        cout << "Mauvaise utilisation du programme :" << endl;
        cout << "./quick [taille du tableau] " << endl;
        return 1;
    }

     n = atoi(argv[1]);




    if (pid == ROOT ){
    tab = new int[n];

    srand(time(NULL));
    cout << "[ ";
    for (int i=0; i<n; i++) {tab[i] = rand()%100;  cout << tab[i] << " "; }
      cout << " ]"<<endl;



    }

    QuickSort(tab,n);


    MPI_Finalize() ;
    return 0;
}
