#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

typedef struct {
	
	char L[3][17];  //Registres	
	char F[9];	    //Fonction

}LFSR;

typedef struct {
	int pos;
	float p;
	int ** gen_suite;
}LFSR_I;

//int * L0;
//int * L1;
//int * L2;

int t_verite[3][8] = {{0,1,0,1,0,1,0,1}, {0,0,1,1,0,0,1,1},{0,0,0,0,1,1,1,1}};
int xor_pos[3][4] = {{15,14,11,8}, {15,14,8,4}, {15,13,12,10}};	

void checkArgs ( int argc)	{
	if(argc != 3){ 
		fprintf(stderr, "Flow: opérande manquant\nUsage: Usage  : ./Flow fichier n\n");
		exit(1);
	}
}

/* 
	Affichage d'une suite de N bits
*/
void printSequence ( char * t, int * s, int n)	{
	printf("\n%s = ",t);
	for (int i = 0; i < n; i++)
		printf ("%d", s[i]);
	//printf ("");	
}

/* 
	* Fonction de filtrage
	* prend trois bits en entrée et retourne un bit en sortie
*/
int filteringF(char x0, char x1, char x2){
	return (x0*1) + (x1*2) + (x2*4);
}

/* 
	* Fonction de décalage
	* Décaler chaque LFSR
	* \param Geffe : Initialisation des trois LFSR 
	* \param i : Position de LFSR
	* \return Bit de sortie de la fonction de filtrage aprés décalage
*/
int shift ( LFSR * Geffe, int i)	{
	char x0 = Geffe->L[0][15] - '0';
	char x1 = Geffe->L[1][15] - '0';
	char x2 = Geffe->L[2][15] - '0';

	//L0[i] = x0;
	//L1[i] = x1;
	//L2[i] = x2;

	int indice = filteringF ( x0, x1, x2);
	short tmp0 = (Geffe->L[0][15] - '0') ^ (Geffe->L[0][14] - '0') ^ (Geffe->L[0][11] - '0') ^ (Geffe->L[0][8] - '0'),
		  tmp1 = (Geffe->L[1][15] - '0') ^ (Geffe->L[1][14] - '0') ^ (Geffe->L[1][8] - '0') ^ (Geffe->L[1][4] - '0'),
		  tmp2 = (Geffe->L[2][15] - '0') ^ (Geffe->L[2][13] - '0') ^ (Geffe->L[2][12] - '0') ^ (Geffe->L[2][10] - '0');
	for (int i = 15; i > 0; i--)	{
		Geffe->L[0][i] = Geffe->L[0][i-1];
		Geffe->L[1][i] = Geffe->L[1][i-1];
		Geffe->L[2][i] = Geffe->L[2][i-1];
	}
	Geffe->L[0][0] = tmp0 + '0';
	Geffe->L[1][0] = tmp1 + '0';
	Geffe->L[2][0] = tmp2 + '0';

	return Geffe->F[indice] - '0';
}


/* 
	* Générer une suite de N bits pour un seul LFSR 
	* \param li_initial : initialisation de LFSR
	* \param n : nombre de bits à générer
	* \return : suite de N bits de la sortie de LFSR
*/
int * generate_li(char * li_initial, int n, int lfsr_i){

	int * LI = (int *) malloc (n * sizeof(int));

	for (int i = 0; i < n; i++)
	{
		LI[i] = li_initial[15] - '0';
		//L0
		short xor = (li_initial[xor_pos[lfsr_i][0]] - '0') ^ (li_initial[xor_pos[lfsr_i][1]] - '0') 
					^ (li_initial[xor_pos[lfsr_i][2]] - '0') ^ (li_initial[xor_pos[lfsr_i][3]] - '0');
		for (int i = 15; i > 0; i--)	{
			li_initial[i] = li_initial[i-1];
		}
		
		li_initial[0] = xor + '0';
		
	}
	return LI;
}

/*
	* Génération de la suite chiffrante S pour N bits
	* \param Geffe : 3 LFSR du générateur de type Geffe
	* \param n : nombre de bits à générer
	* \return la suite chiffrante de N bits

*/
int * generate(LFSR* Geffe, int n) { //char* bitL0, char* bitL1, char* bitL2){
	fprintf ( stdout, "Generation de la suite chiffrante ");
	int * s = (int *) malloc ( n * sizeof (int));
	//L0 = (int *) malloc ( n * sizeof (int));
	//L1 = (int *) malloc ( n * sizeof (int));
	//L2 = (int *) malloc ( n * sizeof (int));

	if ( !s)	{
		fprintf ( stderr, "Erreur lors de l'allocation de la clé S\n");
		exit(1);
	}
	for (int i = 0; i < n; i++)	{
		s[i] = shift ( Geffe, i);
	}
	return s;
}


/*
	* Initialisation d'un Générateur de type Geffe
	* \param path chemin de fichier des entrées
	* \param gen 3 LFSR pour initialiser
*/
void initialiserLFSR_file(const char* path,LFSR* gen){
	FILE* file = fopen(path,"r");
	if ( !file)	{
		printf ("Erreur à l'ouverture du fichier\n");
		exit(1);
	}
	char * buffer = NULL;
	ssize_t read;
	size_t len = 1;
	int i = 0;
   	printf("Initialisation du LFSR ");
    while ((read = getline(&buffer, &len, file)) != -1) {
		read = getline(&buffer, &len, file);
		// Gerer les valeurs k0, k1, k2
		if ( i < 3)	{  
		   strcpy(gen->L[i],buffer);
		   gen->L[i][16] = 0;
	   }
       //gerer derniere ligne (L)
       if(i == 3)	{
		   strncpy(gen->F,buffer,8);
		   gen->F[8] = 0;
	   }
       i++;
    }
//    timer ();
	free(buffer);
	fclose(file);
}

LFSR * initialiserLSFR(char * F, int * l0, int * l1, int * l2){
	LFSR* Geffe = (LFSR*) malloc(sizeof(LFSR));
	strncpy(Geffe->F, F, 8);
	for( int i = 0; i < 16 ; i++){
		Geffe->L[0][i] = l0[i] +'0';
	}
	for( int i = 0; i < 16 ; i++){
		Geffe->L[1][i] = l1[i]+'0';
	}
	for( int i = 0; i < 16 ; i++){
		Geffe->L[2][i] = l2[i] +'0';
	}

	return Geffe;

}

/*
	* Calculer la probabilité d'équivalence entre
		deux suite de bits
	* \return 
*/
float prob_equivalence(int * s, int * r, int n){
	int cpt = 0;
	for (int i = 0; i < n; i++)
	{
		if(s[i] == (r[i]))
		cpt++;
	}
	return (float)cpt/n;
}

/*
	* Générer toutes les possibilités pour N bits
	* \param path chemin de fichier pour générer
	* \param nbits nombre de bits
*/

void generate_bits(const char* path, int nbits){
	FILE  * file = fopen (path, "w");
	__int64_t n = pow(2,nbits);
	__int64_t tmp = 0;
	for (__int64_t c = 0; c < n; c++)
	{
		for (int i = sizeof(char)*nbits -1; i > -1; i--)
		{
			tmp = c & (1<<i);
			fprintf(file, "%c", (tmp == 0 ? '0': '1'));
		}
		fprintf(file, "\n");
	}

	fclose(file);
}

/* 
	* Vérifier pour chaque possibilité la probabilité d'equivalence et afficher si
	* \param path : chemin de fichier qui contient toutes les possiblités de N bits
	* \param s : suite chiffante de N bits à tester
	* \param n : la taille de la suite de bits
*/
void verifier_cor(const char * path,int * s, int n, LFSR_I lfsr){
	FILE* file = fopen(path,"r");
	char tab_tmp [16];
	if ( !file)	{
		printf ("Erreur à l'ouverture du fichier\n");
		exit(1);
	}
	char * buffer = NULL;
	size_t len = 1;
	lfsr.gen_suite = NULL;
	int taille = 0;
	lfsr.gen_suite = malloc(sizeof(int*));
	*lfsr.gen_suite = malloc(16*sizeof(int));
	taille++;
   	//printf("Attaque par correlation\n");
    while ((getline(&buffer, &len, file)) != -1) {
    	strncpy(tab_tmp, buffer,16);
    	int * result = generate_li(tab_tmp, n, lfsr.pos);
		float p = prob_equivalence(s, result, n);
		if(p > 0.74 && p  < 0.85){
		  lfsr.gen_suite =realloc(lfsr.gen_suite,taille);
		  lfsr.gen_suite[taille] = malloc(16*sizeof(int));
			for(int i = 15; i >= 0; i--){
					lfsr.gen_suite[taille-1][i] = result[i]; 
					//printf("%d", lfsr.gen_suite[taille-1][i]);
			}
			taille = sizeof(lfsr.gen_suite)/sizeof(int);
			//printf("\n");
			
		}
    }
	free(buffer);
	fclose(file);
}



/* 
	* Calculer correlation entre chaque sortie de LFSR 
	et la sortie de la fonction F
*/
LFSR_I * calculer_correlation_f(char * F){
	LFSR_I  * lfsr;

	lfsr = (LFSR_I *) malloc ( 3 * sizeof (LFSR_I));

	printf("F = %s", F);
	int F_int[8];
	for(int i = 0;i< 8; i++){
		F_int[i] = F[i] -'0';
	}
	for(int i= 0; i < 3; i++){
		lfsr[i].pos = i;
		float p = prob_equivalence(F_int, t_verite[i], 8);
		lfsr[i].p = p ;
		printf("L[%d] = %.2f\t", i,p);
	}
	printf("\n---------------------------------------------\n");

	return lfsr;
}

void calculer_correlation_t_f(){
	generate_bits("gen_8bits",8);
	FILE * file = NULL;
	file = fopen("gen_8bits", "r");
	if (!file)	{
		printf ("Erreur à l'ouverture de fichier gen_8bits\n");
	}
	char * buffer = NULL;
	size_t len = 1;
	while (getline(&buffer, &len, file) != -1){
		calculer_correlation_f(buffer);
	}
	free(buffer);
	fclose(file);
}

void decrypt(char * F, int * s, int n){
	LFSR_I * tab_lfsr;
	LFSR* Geffe = (LFSR*) malloc(sizeof(LFSR));
	tab_lfsr =  calculer_correlation_f(F);
	// Vérifier corrélation de chaque LFSR
	for (int i = 0; i < 3; ++i)
	{
		if(tab_lfsr[i].p == 0.5) {
			printf("Attaque par recherche exhaustive n'est pas implementé\n LFSR [%d] = %.2f",i,tab_lfsr[i].p);
			exit(1);
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		verifier_cor("gen_16bits", s, n, tab_lfsr[i]);

	}
	int size_lfsr_0 = sizeof(tab_lfsr[0].gen_suite)/sizeof(int*);
	int size_lfsr_1 = sizeof(tab_lfsr[1].gen_suite)/sizeof(int*);
	int size_lfsr_2 = sizeof(tab_lfsr[2].gen_suite)/sizeof(int*);
	for (int i = 0; i < size_lfsr_0; i++)
	{
		for (int j = 0; j < size_lfsr_1; j++)
		{
			for (int k = 0; k < size_lfsr_2; k++)
			{	
				Geffe = initialiserLSFR(F, tab_lfsr[0].gen_suite[i],tab_lfsr[1].gen_suite[j],
								tab_lfsr[2].gen_suite[k]);
				float p = prob_equivalence(s, generate(Geffe, n), n);
				if(p == 1.00){
						printf("Felicitation\n ");
						printSequence("K0", tab_lfsr[0].gen_suite[i],n);
						printSequence("K1", tab_lfsr[1].gen_suite[j],n);
						printSequence("K2", tab_lfsr[2].gen_suite[k],n);
						exit(0);
					}
				}
		}
	}



}
/**
 *	Usage  : ./Flow fichier n
 */

int main(int argc, char const *argv[])
{
	checkArgs ( argc);
	//Ouvir le fichier et initialiser le LFSR
	int n = atoi ( argv[2]);
	/*
	LFSR* Geffe = (LFSR*) malloc(sizeof(LFSR));
	initialiserLFSR_file(argv[1],Geffe);
	
	int * s = generate ( Geffe, n);
	printSequence ("S ",  s, n);
	//printSequence ("L0", L0, n);
	//printSequence ("L1", L1, n);
	//printSequence ("L2", L2, n);
  	printf("\n\n");
  	//printf("%.2f\n", prob_equivalence(s, L0, n));
  	//printf("%.2f\n", prob_equivalence(s, L1, n));
  	//printf("%.2f\n", prob_equivalence(s, L2, n));
  	free(Geffe);
  	free ( s);
	
	//generate_bits(argv[1], n);
	
	*/
	/* int s [100]= {0,0,1,0,1,0,1,0,1,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,
				0,1,0,0,1,0,0,0,0,1,0,1,0,1,1,0,0,1,1,0,1,0,1,0,
				0,0,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,1,0,1,1};
	*/
	//int s [28] = {0,0,1,0,1,0,1,0,1,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,1,1};
	int s [64] = {0,0,1,0,1,0,1,0,1,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,
	 			1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,0,1,0,0,0,0,1,0,1,0,1,1,0,0,1,1,0,1,0};
	char * F = "10001110";
	
	//verifier_cor(argv[1], s, n);
	//calculer_correlation_t_f();
	decrypt(F, s, n);
	return 0;
}
