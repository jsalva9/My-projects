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

subject to fora_estrada:
sum {k in unitats} x[21,k] = 0;

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

subject to suhita_a_castors:
x[7,0] = 1;

subject to furqui_no_castors:
x[9,0] = 0;

/*
subject to no_parelles_1 {k in unitats}:	# gina = 19, gim = 18
x[18,k]*x[19,k] = 0;

subject to no_parelles_2 {k in unitats}:	# helen = 0, rovi = 8
x[0,k]*x[8,k] = 0;

subject to no_parelles_3 {k in unitats}:	# berni = 5, samso = 23
x[5,k]*x[23,k] = 0;

subject to germans_cortes {k in unitats}:	# marti = 17, arnau = 20
x[17,k]*x[20,k] = 0;

subject to germans_llansa {k in unitats}:	# pau = 15, joan = 24
x[15,k]*x[24, k] = 0;
*/

subject to germanes_cobera:		# nani = 25, suhita a castors
x[25,0] = 0;

subject to germans_salva_1: 	# salva = 10, maria fuan a pios
x[10,3] = 0;

subject to germans_salva_2: 	# salva = 10, mateu a truk
x[10,4] = 0;

subject to germans_rabella: 	# quim = 1, mauri a pios
x[1,3] = 0;

subject to germans_franquesa: 	# berni = 5, aineta a ranguis
x[5,2] = 0;

subject to germans_rovira: 		# rovi = 8, marta a pios
x[8,3] = 0;

subject to germanes_salinas: 	# marta = 11, aina a pios
x[11,3] = 0;

subject to germanes_pallares: 	# bet = 12, gina a truk
x[12,4] = 0;

subject to paula_animadora:
x[22,4] = 1;

/*
subject to germanes_crego: 		# adriana = 14, eli a truk
x[14,4] == 0;
*/
 
subject to germans_estrada:		# marti = 21, clara a truk
x[21,4] = 0;

subject to germanes_moix:		# gina = 19, la seva germana a pios
x[19,3] = 0;


subject to no_animadors_1 {i in {0,1,2,3,23,4,5,6,7,8,9,10,11}}:		# helen = 0, quim = 1, berta = 2, hosta = 3, samso = 23,franci = 4, berni = 5, puig = 6, suhita = 7, rovi = 8, furqui = 9, salva = 10, salinas = 11
x[i,4] = 0;

subject to no_caps_de_pios {i in {0,1,2,3,23}}:		# helen = 0, quim = 1, berta = 2, hosta = 3, samso = 23
x[i,3] = 0;

subject to una_noia_per_unitat {k in unitats}:
(sum {j in caps} x[j,k]*noia[j]) >= 1;

subject to un_noi_per_unitat {k in unitats}: 
(sum {j in caps} x[j,k]*noi[j]) >= 1;

/*
subject to igualtat_1 {i in caps}:
(1/n_caps)*sum {l in caps} (pes[l]*(sum {k in unitats, j in caps : j != l} (x[l,k]*x[j,k]*(si_cocaps[l,j]-no_cocaps[l,j])))
+((1-pes[l])*(sum {k in unitats} x[l,k]*(si_unitats[l,k]-no_unitats[l,k]))))-(pes[i]*(sum {k in unitats, j in caps} x[i,k]*x[j,k]*(si_cocaps[i,j]-no_cocaps[i,j]))
+((1-pes[i])*(sum {k in unitats} x[i,k]*(si_unitats[i,k]-no_unitats[i,k])))) <= ctrl*(1/n_caps)*(sum {l in caps} (pes[l]*(sum {k in unitats, j in caps : j != l} (x[l,k]*x[j,k]*(si_cocaps[l,j]-no_cocaps[l,j])))
+((1-pes[l])*(sum {k in unitats} x[l,k]*(si_unitats[l,k]-no_unitats[l,k])))));

subject to igualtat_2 {i in caps}:
pes[i]*(sum {k in unitats, j in caps : j != i} (x[i,k]*x[j,k]*(si_cocaps[i,j]-no_cocaps[i,j])))
+((1-pes[i])*(sum {k in unitats} x[i,k]*(si_unitats[i,k]-no_unitats[i,k]))) >= ctrl;
*/