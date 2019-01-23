#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <vector>
using namespace std;
#include "matrix.h"
#include "lu.cc"
#include "sistema.cc"
#include "inversa.cc"


int main(int argc,char *argv[]) {
	
    ifstream fileIn;
    ofstream fileOut;
    
    // Fitxer d'entrada
    if (argc >= 2) fileIn.open(argv[1]); // obrim el fitxer per llegir les dades.
    else {cerr << "ERROR: no hi ha fitxer d'entrada" << endl; return 0;}
    
    if (!fileIn.is_open()) {
        cerr << "Error: no s'ha pogut obrir el fitxer " << argv[1]
            << " per llegir les dades" << endl;
        return 0;
    }
    
    int n;  // dimensi� del sistema.
    fileIn >> n;
    
    int m;  // nombre de components no nuls d'A.
    fileIn >> m; 
    
    double **A;		// declarem la matriu A.
	A = (double **) calloc(n, sizeof(double*));
	for (int i = 0; i < n; ++i) A[i] = (double *) calloc(n, sizeof(double));
    
    for (int k = 0; k < m; ++k) {
        
        int i, j;
        double comp;
        fileIn >> i >> j >> comp;
        
        // Introduïm els components no nuls de la matriu del sistema A.
        A[i][j] = comp;
    
    }
    
    int k;	// Nombre de components no nuls de b.
    fileIn >> k;
    
    double b[n];	// Declarem el vector de termes independents.
    
    for (int l = 0; l < k; ++l) {
    	double comp;
        int i;
    	fileIn >> i >> comp;
    	
    	// Introduïm els components al vector.
    	b[i] = comp;
	}
    
    // Tanquem el fitxer
    fileIn.close();
    
    // Definim el vector de permutacions.
    int perm[n];
	for (int i = 0; i < n; ++i) perm[i] = i;
	
	double tol = pow(10, -12);
	
    // C és una còpia de la matriu A per poder calcular el vector residu posteriorment.
    double **C;	
	C = (double **) calloc(n, sizeof(double*));
	for (int i = 0; i < n; ++i) C[i] = (double *) calloc(n, sizeof(double));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) C[i][j] = A[i][j];
    }

    /*cout << "C: " << endl;
    PrintMatrix(C, n);*/
    
    cout << endl << "Dimension n=" << n << endl << endl;
    
    // Intentem calcular la descomposició LU.
    int paritat = lu(A, n, perm, tol);
    if (paritat == 0) {
    	cout << "La matriu introduïda és una matriu singular, s'atura la factorització LU" << endl;
    	return 0;
	}
	
	// Construim les matrius L, U.
	double **L;
	L = (double **) calloc(n, sizeof(double*));
	for (int i = 0; i < n; ++i) L[i] = (double *) calloc(n, sizeof(double));
	
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) L[i][j] = 1;
            if (i < j) L[i][j] = 0;
            if (i > j) L[i][j] = A[i][j];
        }
    }
    /*cout << "L: " << endl;
    PrintMatrix(L, n);*/
    
    double **U;
	U = (double **) calloc(n, sizeof(double*));
	for (int i = 0; i < n; ++i) U[i] = (double *) calloc(n, sizeof(double));
   
	for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i > j) U[i][j] = 0;
            else U[i][j] = A[i][j];
        }
    }
    /*cout << "U: " << endl;
    PrintMatrix(U, n);*/
    // Càlcul del determinant.
    double det = paritat;
    for (int i = 0; i < n; ++i) {
    	det *= U[i][i];
	}
    cout << "Determinant of A: " << det << endl << endl;
    
    // Escrivim el vector de permutació.
    cout << "Permutation vector:" << endl << "[";
    for (int i = 0; i < n - 1; ++i) cout << perm[i] << ",";
    cout << perm[n - 1] << "]" << endl << endl;
    
    // Precisió de la descomposició LU.
    cout << "Checking the norm of the residue: |PA-LU|" << endl;
    
    double **LU;
	LU = (double **) calloc(n, sizeof(double*));
	for (int i = 0; i < n; ++i) LU[i] = (double *) calloc(n, sizeof(double));
	LU = Product(L, U, n);
	/*cout << "LU: " << endl;
	PrintMatrix(LU, n);*/
    

	// Calculem P aplicat a la matriu A.    
    double **PA;
	PA = (double **) calloc(n, sizeof(double*));
	for (int i = 0; i < n; ++i) PA[i] = (double *) calloc(n, sizeof(double));
    for (int i = 0; i < n; ++i) {
    	for (int j = 0; j < n; ++j) {
    		PA[i][j] = C[perm[i]][j];
		}
	}
    /*cout << "PA: " << endl;
	PrintMatrix(PA, n);*/
    
    double **R;
    R = (double **) calloc(n, sizeof(double*));
	for (int i = 0; i < n; ++i) R[i] = (double *) calloc(n, sizeof(double));
	R = Difference(PA, LU, n);
	
	double norma;
	norma = SupremeNorm(R, n);
	cout << "   1-norm: " << norma << endl;
	norma = Norm1(R, n);
	cout << "   supremum norm: " << norma << endl << endl;
	
    
	// Resolució del sistema mitjançant la descomposició LU.
	double x[n]; 	// vector d'incògnites.
	resol(A, x, b, n, perm);
    
    double *Ax;
    Ax = (double*) calloc(n, sizeof(double));
    Ax = MATxVEC(C,x,n);
    
    // Residu R = Ax-b.
    double res[n];
    for (int i = 0; i < n; ++i) res[i] = Ax[i] - b[i];
    cout << "Solving Ax=b. Checking the norm of the residue vector Ax-b:" << endl;
    cout << "   1-norm: " << norma1(res, n) << endl;
    cout << "   supremum norm: " << abs(res[norminf(res, n)]) << endl << endl;
    
    
    // Creem i obrim el fitxer "solucioSL.dat".
    ofstream ofs("solucioSL.dat");
	if (not ofs.is_open()) {
		cout << "Error obrint el fitxer!" << endl;
		exit(1);
	}
    
	// Escrivim al fitxer de sortda
	for (int i = 0; i < n; ++i) {
        ofs << i << " " << scientific << x[i] << endl;
        ofs << endl;
	}
	
	// Tanquem el fitxer de sortida
	ofs.close();
    
    // Copia de la matriu A, C2
    double **C2;
	C2 = (double **) calloc(n, sizeof(double*));
	for (int i = 0; i < n; ++i) C2[i] = (double *) calloc(n, sizeof(double));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) C2[i][j] = C[i][j];
    }
    
    // Declarem la matriu inversa de A, inv_a
    double **inv_a;
	inv_a = (double **) calloc(n, sizeof(double*));
	for (int i = 0; i < n; ++i) inv_a[i] = (double *) calloc(n, sizeof(double));
    
    // Determinant d'A
    double * det_a;
    inv_a = (double *) calloc(1, sizeof(double));
    det_a[0] = 1;
    
    cout << "We compute the inverse of A. Checking norm of A*inv(A) - I:" << endl;
    int par = inversa(C2, inv_a, n, det_a, tol);
    if (par == 0) cout << "La matriu �s singular" << endl;
    
    
    double **Q;
	Q = (double **) calloc(n, sizeof(double*));
	for (int i = 0; i < n; ++i) Q[i] = (double *) calloc(n, sizeof(double));
    
    // Calculem A*A^-1
    Q = Product(inv_a, C, n);
    
    // Calculem A*A^-1 - Id
    for (int i = 0; i < n; ++i) {
        Q[i][i] -= 1;
    }
    cout << "   1-norm: " << Norm1(Q, n) << endl;
    cout << "   supremum norm: " << SupremeNorm(Q, n) << endl << endl;
}
