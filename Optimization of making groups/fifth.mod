/* INICI: cal introduir els paràmetres de música, cotxe, muntanya. També el relleu*/
/* FINAL: */

# nombre de persones i unitats
param n_caps;
param n_unitats;

param ctrl; # parametre per controlar la diferència de cadascú a la mitjana.

set caps := 0..(n_caps-1);
set unitats := 0..(n_unitats-1);
param si_unitats {caps,unitats};
param no_unitats {caps,unitats};
param si_cocaps {caps,caps};
param no_cocaps {caps,caps};
param pes {caps};
param noia {caps};
param noi {i in caps} :=
	if (noia[i] == 0) then 1
	else 0;

param musica {caps};
param muntanya {caps};
param cotxe {caps};
param carnet {caps};

param min_musica {unitats};
param min_muntanya {unitats};
param min_cotxe {unitats};
param min_carnet {unitats};
param min_director {unitats};

param y {caps,unitats}; # taula de caps del curs passat. els nous caps tenen un -1
param relleu {unitats}; # 1 si cal que hi hagi relleu, 0 si no cal que hi hagi relleu

param max_caps {unitats};
param min_caps {unitats};


# les variables son binaries. Es una matriu de pertinenca {caps, unitats}
var x {i in caps, k in unitats} binary;

# funcio objectiu
maximize f : 
sum {i in caps} (pes[i]*(sum {k in unitats, j in caps : j != i} (x[i,k]*x[j,k]*(si_cocaps[i,j]-no_cocaps[i,j])))
+((1-pes[i])*(sum {k in unitats} x[i,k]*(si_unitats[i,k]-no_unitats[i,k]))));


# restriccions
subject to fitasup {k in unitats}: 
(sum {j in caps} x[j,k]) <= max_caps[k];

subject to fitainf {k in unitats}:
(sum {j in caps} x[j,k]) >= min_caps[k];
 
subject to una_unitat_per_cap {i in caps : i != 21}:
(sum {k in unitats} x[i,k]) = 1;		# es pot canviar per: <= 1. el programa fara fora a gent, si cal.

subject to una_mica_de_musica {k in unitats}:
(sum {j in caps} x[j,k]*musica[j]) >= min_musica[k];

subject to una_mica_de_muntanya {k in unitats}:
(sum {j in caps} x[j,k]*muntanya[j]) >= min_muntanya[k];

subject to un_carnet_de_cotxe {k in unitats}:
(sum {j in caps} x[j,k]*cotxe[j]) >= min_cotxe[k];

subject to un_relleu_potser {k in unitats}:
(sum {j in caps} x[j,k]*y[j,k]) >= relleu[k];

subject to carnets_per_unitat {k in unitats}:
(sum {j in caps} x[j,k]*carnet[j]) >= min_carnet[k];

subject to directors_per_unitat {k in unitats}:
(sum {j in caps} x[j,k]*(carnet[j] div 2)) >= min_director[k];

subject to una_noia_per_unitat {k in unitats}:
(sum {j in caps} x[j,k]*noia[j]) >= 1;

subject to un_noi_per_unitat {k in unitats}: 
(sum {j in caps} x[j,k]*noi[j]) >= 1;

subject to igualtat_1 {i in caps}:
(1/n_caps)*sum {l in caps} (pes[l]*(sum {k in unitats, j in caps : j != l} (x[l,k]*x[j,k]*(si_cocaps[l,j]-no_cocaps[l,j])))
+((1-pes[l])*(sum {k in unitats} x[l,k]*(si_unitats[l,k]-no_unitats[l,k]))))-(pes[i]*(sum {k in unitats, j in caps} x[i,k]*x[j,k]*(si_cocaps[i,j]-no_cocaps[i,j]))
+((1-pes[i])*(sum {k in unitats} x[i,k]*(si_unitats[i,k]-no_unitats[i,k])))) <= ctrl*(1/n_caps)*(sum {l in caps} (pes[l]*(sum {k in unitats, j in caps : j != l} (x[l,k]*x[j,k]*(si_cocaps[l,j]-no_cocaps[l,j])))
+((1-pes[l])*(sum {k in unitats} x[l,k]*(si_unitats[l,k]-no_unitats[l,k])))));

subject to igualtat_2 {i in caps}:
pes[i]*(sum {k in unitats, j in caps : j != i} (x[i,k]*x[j,k]*(si_cocaps[i,j]-no_cocaps[i,j])))
+((1-pes[i])*(sum {k in unitats} x[i,k]*(si_unitats[i,k]-no_unitats[i,k]))) >= ctrl;
