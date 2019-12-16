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

int pid_dest(int n,int i_global){
  int pid, nprocs,n_local,pidest;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;

  n_local = pid < n%nprocs ? n/nprocs +1 : n/nprocs;

  if ( i_global< n%nprocs * (n_local +1) )
  pidest=(i_global/(n_local +1));
  else
  pidest=n%nprocs - 1 +(i_global-(n%nprocs * n_local +1)/n_local);
  return pidest;
}


void print_tab(int* tab, int n){
  int pid;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  cout <<"("<<pid<<") : [ ";
  for (int i=0; i<n; i++)
  cout << tab[i] << " ";
  cout << "]" <<endl;
}


void QuickSort(int* tab, int n) {
  int pid, nprocs,q,r,n_local,pivot;
  int sum_left_local,sum_right_local;
  int *count,*displ;
  int *sum_left, *sum_right;
  int *sum_left_total, *sum_right_total;


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
  // Distribution tableaux locaux
  MPI_Scatterv(tab, count, displ, MPI_INT,tab_local , n_local, MPI_INT, ROOT, MPI_COMM_WORLD);

  if (pid==ROOT){
    pivot = tab[0];
  }

  // Distribution/Récupération pivot
  MPI_Bcast(&pivot, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
  cout<<"pid "<<pid<<" "<< pivot<<endl;


  sum_left_local = 0;
  sum_right_local = 0;

  // Calcul local
  for (int i=1; i<n_local; i++) {
    if (tab_local[i] <= pivot) {
      sum_left_local += 1;
      swap(tab_local+sum_left_local,tab_local+i);
    }
    else {
      sum_right_local += 1;
    }
  }
  swap(tab_local,tab_local+sum_left_local);

  if (tab_local[0] <= pivot) {
    sum_left_local += 1;
  }
  else {
    sum_right_local += 1;
  }

  // Récupération des tableaux locaux
  MPI_Gatherv(tab_local, n_local,MPI_INT, tab ,count,  displ ,MPI_INT, ROOT, MPI_COMM_WORLD);

  sum_left = new int[nprocs];
  sum_right = new int[nprocs];

  // Récupération gauche
  MPI_Gather(&sum_left_local, 1, MPI_INT, sum_left, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
  // Récupération droite
  MPI_Gather(&sum_right_local, 1, MPI_INT, sum_right, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

  if (pid  == ROOT) {
      print_tab(tab,n);
      print_tab(sum_left, nprocs);
      print_tab(sum_right, nprocs);

      sum_left_total = new int[nprocs + 1];
      sum_right_total = new int[nprocs + 1];

      // Accumulation
      sum_left_total[0] = 0;
      sum_right_total[0] = 0;
      for (int i = 1; i < nprocs + 1; i++) {
        sum_left_total[i] = sum_left_total[i - 1] + sum_left[i - 1];
        sum_right_total[i] = sum_right_total[i - 1] + sum_right[i - 1];
      }

      print_tab(sum_left_total, nprocs + 1);
      print_tab(sum_right_total, nprocs + 1);

      // Réarrangement
      cout << pivot << endl;
      int * res = new int[n];
      int k = 0;
      for (int i = 0; i < nprocs; i++) {
        for (int j = sum_left_total[i]; j < sum_left_total[i + 1]; j++) {
          res[k] = tab[sum_right_total[i] + j];
          k++;
        }
      }
      for (int i = 0; i < nprocs; i++) {
        for (int j = sum_right_total[i]; j < sum_right_total[i + 1]; j++) {
          res[k] = tab[sum_left_total[i + 1] + j];
          k++;
        }
      }
      print_tab(res, n);
  }
}


int main(int argc, char** argv) {
  int pid, nprocs,n;
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
