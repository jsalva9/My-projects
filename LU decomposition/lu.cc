// Descomposa A = LU, perm Ès el vector de permutacions.
int lu(double **a, int n, int perm[], double tol){
    
    int p = 0; 		// Ser‡† el nombre de permutacions durant la descomposicio LU.
    
    // Passos del m√®tode de Gauss. Busquem zeros a la columna k per sota la fila k.
    for (int k = 0; k < n - 1; ++k) {
        
        
        // Cerquem pivot (pivotatge parcial esglaonat).
        double max = -1; 
        int r = -1;
    	for (int i = k; i < n; ++i) {
    		double s = abs(a[i][k]);
    		for (int j = k + 1; j < n; ++j) {
    			if (s < abs(a[i][j])) s = abs(a[i][j]);		// Calcula s_i, el m‡xim valor de la fila i.
			}
    		s = abs(a[i][k]/s);
    		if (max < s) {
    			max = s;
    			r = i;
    		}
		}

		if (abs(a[r][k]) < tol) return 0;
		if (k != r) {
			swap(a[k], a[r]);					// Canvi: fila r --- fila k.
	        swap(perm[k], perm[r]);				// Guardem el canvi al vector de permutacions.
	        ++p;								// Hem fet una permutaciÛ de files.
		}

        
        // Posem zeros a la fila i.
        for (int i = k + 1; i < n; ++i) {
        	double m = a[i][k]/a[k][k];		// C‡lcul del multiplicador.
        	a[i][k] = m;					// Ja tenim el 0 a la posiciÛ ik. Guardem el multiplicador en aquesta posiciÛ.
        	
				// Modifiquem tots els altres valors de la fila i
				for (int j = k + 1; j < n; ++j) {
					a[i][j] -= m*a[k][j];
				}
		}
    }
    
	// Retornem 1, -1 en funciÛ de la paritat del nombre de permutacions de files.
    if (p%2 == 0) return 1;
    return - 1;
    
}
