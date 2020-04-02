#include <stdio.h>
#include <stdlib.h>
//#include <assert.h>
#include <string.h>

typedef struct {
	char L[3][17];  //Registres
	char F[9];	    //Fonction
}LFSR;

void DebugLFSR(LFSR* Geffe){
	for (int i = 0; i < 3; ++i)	{
		printf("%s\n", Geffe->L[i]);
	}
  	printf("%s\n", Geffe->F);
}

int FonctionF(char x0, char x1, char x2){
	return (x0*1) + (x1*2) + (x2*4);
}

void decale ( LFSR* Geffe)	{
	short tmp0 = (Geffe->L[0][0] - '0') ^ (Geffe->L[0][1] - '0') ^ (Geffe->L[0][4] - '0') ^ (Geffe->L[0][7] - '0'),
		  tmp1 = (Geffe->L[1][0] - '0') ^ (Geffe->L[1][1] - '0') ^ (Geffe->L[1][7] - '0') ^ (Geffe->L[1][11] - '0'),
		  tmp2 = (Geffe->L[2][0] - '0') ^ (Geffe->L[2][2] - '0') ^ (Geffe->L[2][3] - '0') ^ (Geffe->L[2][5] - '0');
	for (int i = 15; i > 0; i--)	{
		Geffe->L[0][i] = Geffe->L[0][i-1];
		Geffe->L[1][i] = Geffe->L[1][i-1];
		Geffe->L[2][i] = Geffe->L[2][i-1];
	}
	Geffe->L[0][0] = tmp0 + '0';
	Geffe->L[1][0] = tmp1 + '0';
	Geffe->L[2][0] = tmp2 + '0';
}


void Decalage(LFSR* Geffe, char* bitL0, char* bitL1, char* bitL2){
	//Sortie du bit de poids faible
	*bitL0 = Geffe->L[0][0]; 
	*bitL1 = Geffe->L[1][0];
	*bitL2 = Geffe->L[2][0];
	//Décalage
	//decale ( Geffe);

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
   	printf("Initialisation du LFSR\n");
    while ((read = getline(&buffer, &len, file)) != -1) {
       read = getline(&buffer, &len, file);
       strcpy(gen->L[i],buffer);
       gen->L[i][16] = 0;
       //gerer derniere ligne
       if(i == 3)	{
		   strncpy(gen->F,buffer,8);
		   gen->F[8] = 0;
	   }
       i++;
    }

	free(buffer);
	fclose(file);
}

/**
 *	Usage  : ./Flow fichier n
 */

int main(int argc, char const *argv[])
{
	if(argc != 3)	{ 
		fprintf(stderr, "Usage: Usage  : ./Flow fichier n\n");
		exit(0);
	}
	//Ouvir le fichier et initialiser le LFSR
	
	LFSR* Geffe = (LFSR*) malloc(sizeof(LFSR));
	initialiserLFSR(argv[1],Geffe);
	
	DebugLFSR(Geffe);
	decale(Geffe);
	
	printf ("\n\nNouveau test\n");
	DebugLFSR(Geffe);
	//printf("%d\n", FonctionF(1, 1, 1));
  	
  	free(Geffe);
	return 0;
}
