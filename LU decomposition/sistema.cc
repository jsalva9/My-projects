void resol(double **a, double x[], double b[], int n, int perm[]){
    

	// Ly = b(barret)
    double y[n];
    y[0] = b[perm[0]];
    for (int i = 1; i < n; ++i) {
        double sum = 0;
        for (int j = 0; j < i; ++j) {
            sum += a[i][j]*y[j];
        }
        y[i] = b[perm[i]] - sum;
    }
    
    
    // Ux = y
    x[n-1] = y[n-1]/a[n-1][n-1];
    for (int i = n - 2; i >= 0; --i){
        double sum = 0;
        for (int j = i + 1; j < n; ++j) {
            sum += a[i][j]*x[j];
        }
        x[i] = (y[i] - sum)/a[i][i];
    }
    
    return;
}
