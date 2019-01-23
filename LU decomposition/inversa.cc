int inversa(double **a, double **inv_a, int n, double *det_a, double tol){
    
    // Declarem el vector de permutacions.
    int perm[n];
    for (int i = 0 ; i < n; ++i) perm[i] = i;
    
    // Apliquem la descomposició lu.
    int par = lu(a, n, perm, tol);
    if (par == 0) {
    	cout << "La matriu introduïda és una matriu singular, s'atura la factorització LU" << endl;
    	return 0;
	}
    
    // Càlcul del determinant.
    double det = double(par);
    for (int i = 0; i < n; ++i) {
    	det *= a[i][i];
	}
    *det_a = det;
    
    double e[n];
    for (int i = 0; i < n; ++i) e[i] = 0;
    double x[n];
    for (int j = 0; j < n; ++j){
        
        e[j] = 1;
        resol(a, x, e, n, perm);
        for(int i = 0; i < n; ++i) inv_a[i][j] = x[i];
        
        for (int i = 0; i < n; ++i) x[i] = 0;
        e[j] = 0;
    }
    
    return 1;
}
