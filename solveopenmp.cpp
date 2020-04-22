#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <cmath>
using namespace std;


int readFile(double* &X, double* &Soln, double &n, double &error, const char* filename) {
    FILE *fp;
   
    fp = fopen(filename, "r");
 
    if(fp == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }
 
    fscanf(fp, "%lf", &n);
    fscanf(fp, "%lf", &error);
    X = (double *)malloc(sizeof(double)*(n+1));
    // get X
    for(int i =0; i < n ; i++) {
        fscanf(fp, "%lf", &X[i]);
    }

    Soln = (double*) malloc(sizeof(double)*n*(n+1));
    int count = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n+1; j++) {
            int addr = i*(n+1) + j;
            fscanf(fp, "%lf", &Soln[addr]);
        }
    }

    // for(int i = 0; i < n; i ++) {
    //     for(int j = 0; j < n+1; j ++) {
    //         cout<<Soln[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }

    fclose(fp);
    return 0;
}


void pureOpenMP(double* X, double* Soln, int n , float error, int n_th) {
    double start, end;
    double* new_X = (double*) malloc(sizeof(double)*n);
    int flags[n];
    int num_itrs = 0;
    while(true) {
        num_itrs++;
        for(int i = 0; i < n; i++)  flags[i] = 0;

        #pragma omp parallel for num_threads(n_th) shared(flags, X, Soln, new_X)
        for(int i = 0; i < n; i++) {
            double c = Soln[i*(n+1) + n];
            double denom = 0;

            for(int j = 0; j < n; j++) {
                if(i != j) {
                    double a = Soln[i*(n+1) + j];
                    denom += a * X[j];
                }
            }

            new_X[i] = (c - denom) / Soln[i*(n+1) + i];
            double new_err = abs(new_X[i] - X[i]) / new_X[i];
            if(new_err > error) {
                flags[i] = 1;
            }
        }

        bool stop = true;
        for(int i = 0; i < n; i++) {
            if(flags[i] == 1) {
                stop = false;
                break;
            }
        }

        for(int i = 0; i < n; i++) {
            X[i] = new_X[i];
        }

        if(stop == true) {
            break;
        }
    }

    // end = omp_get_wtime();
    // double time_taken = end - start; // in seconds 
    // printf("Problem Size: %d \t Number of threads: %d\n", n, n_th);
    // printf("Time taken for inner loop: %lf\n", time_taken);
    printf("Number of iterations: %d\n", num_itrs);
    FILE * pFile;
    int m = n;
    char buffer [50];
    sprintf(buffer, "%d.sol", m);
    pFile = fopen (buffer,"w");
    for (int i = 0; i < n; i++)
    {
        fprintf (pFile, "%lf\n",X[i]);
    }
    fclose (pFile);
    // Print X
    // for(int i = 0; i < n; i++) {
    //    cout<<X[i]<<endl;
    // }
}

int main(int argc, char *argv[]) {
    char* filename = argv[1];
    int n_th = 4;
    double n, error;
    double* X;
    double* Soln;
    readFile(X, Soln, n, error, filename);
    pureOpenMP(X, Soln, n, error, n_th);
}

