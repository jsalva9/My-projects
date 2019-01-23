#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
typedef vector<double> Row;
typedef vector<Row> Matrix;



// Retorna la suma A+B, amb A i B matrius nxn
double **Sum(double** A, double** B, int n){
    double **S;
    S = (double **) calloc(n, sizeof(double*));
    for (int i = 0; i < n; ++i) S[i] = (double *) calloc(n, sizeof(double));

    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
                S[i][j] = A[i][j] + B[i][j];
            }
    }
    return S;
}

// Retorna la diferència A-B, amb A i B matrius nxn
double **Difference(double** A, double** B, int n){
    double **D;
    D = (double**) calloc(n, sizeof(double*));
    for (int i = 0; i < n; ++i) D[i] = (double *) calloc(n, sizeof(double));

    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
                D[i][j] = A[i][j] - B[i][j];
            }
    }
    return D;
}


// multiplica dues matrius nxn
double **Product(double** A, double** B, int n){
    double **AB;
    AB = (double**) calloc(n, sizeof(double*));
    for (int i = 0; i < n; ++i) AB[i] = (double *) calloc(n, sizeof(double));

    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
            for (int k = 0 ; k < n; ++k){
                AB[i][j] += A[i][k]*B[k][j];
            }
        }
    } 
    return AB;
}

// Norma del suprem matricial
double SupremeNorm(double** A, int n){
    double max = 0;
    for (int i = 0; i < n; ++i){
        double sum = 0;
        for (int j = 0; j < n; ++j){
            sum += abs(A[i][j]);    
        }
        if (sum > max) max = sum;
    }
    return max;
}

// Norma sub 1 matricial
double Norm1(double** A, int n){
    double max = 0;
    for (int j = 0; j < n; ++j){
        double sum = 0;
        for (int i = 0; i < n; ++i){
            sum += abs(A[i][j]);    
        }
        if (sum > max) max = sum;
    }
    return max;
    
}

// Escriu una matriu
void PrintMatrix(double** A, int n){
    for (int i = 0 ; i < n; ++i){
        for (int j = 0; j < n; ++j){
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl << endl << endl;
    return;
}

// Calcula matriu x vector
double *MATxVEC(double **M, double x[], int n) {
    double *fx;
    fx = (double*) calloc(n, sizeof(double));
    double num = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            num += M[i][j]*x[j];
        }
        fx[i] = num;
        num = 0;
    }
    return fx;
}

// Norma sub 1 vectorial
double norma1(double v[], int n) {
    double sum = 0; 
    for (int i = 0; i < n; ++i) {
        sum += abs(v[i]);
    }
    return sum;
}
    


// Norma sub infinit vectorial (retorna la posició)
int norminf(double v[], int n) {
    double max = abs(v[0]);
    int pos = 0;
    for (int i = 1; i < n; ++i) {
        if ((v[i]) > max) {
            max = abs(v[i]);
            pos = i;
        }
    }
    return pos;
}

