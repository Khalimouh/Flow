#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
	char L[3][17];  //Registres
	char F[9];	    //Fonction
}LFSR;

void checkArgs ( int argc)	{
	if(argc != 3)	{ 
		fprintf(stderr, "Flow: opérande manquant\nUsage: Usage  : ./Flow fichier n\n");
		exit(1);
	}	
}

void timer (void)	{
	for (int i = 0; i < 3; i++)	{
		fflush (stdout);
		printf (".");
		fflush ( stdout);
		sleep ( 1);
	}
    printf ("\n");
}

void printSequence ( int * s, int n)	{
	printf ("Affichage des n premiers bit de la sequence chiffrante :\n\t");
	for (int i = 0; i < n; i++)
		printf ("%d ", s[i]);
	printf ("\n");	
}

int filteringF(char x0, char x1, char x2){
	return (x0*1) + (x1*2) + (x2*4);
}

int shift ( LFSR * Geffe)	{
	int indice = filteringF ( (Geffe->L[0][15] - '0'), (Geffe->L[1][15] - '0'), (Geffe->L[2][15] - '0'));
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


int * generate(LFSR* Geffe, int n) { //char* bitL0, char* bitL1, char* bitL2){
	fprintf ( stdout, "Generation de la suite chiffrante ");
	int * s = (int *) malloc ( n * sizeof (int));
	if ( !s)	{
		fprintf ( stderr, "Erreur lors de l'allocation de la clé S\n");
		exit(1);
	}
	for (int i = 0; i < n; i++)	{
		s[i] = shift ( Geffe);
	}
//	timer ();
	return s;
}


void initialiserLFSR(const char* path,LFSR* gen){
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

/**
 *	Usage  : ./Flow fichier n
 */

int main(int argc, char const *argv[])
{
	checkArgs ( argc);
	//Ouvir le fichier et initialiser le LFSR
	
	LFSR* Geffe = (LFSR*) malloc(sizeof(LFSR));
	initialiserLFSR(argv[1],Geffe);
	
	int * s = generate ( Geffe, atoi ( argv[2]));
	printSequence ( s, atoi ( argv[2]));
  	
  	free(Geffe);
  	free ( s);
	return 0;
}
