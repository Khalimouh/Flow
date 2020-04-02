#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct {
	char L[3][17];
	char F[8];
}LFSR;

void DebugLFSR(LFSR* Geffe){
	for (int i = 0; i < 3; ++i)
	{
		printf("%s\n", Geffe->L[i]);
	}
  	
  	printf("%s\n", Geffe->F);
}

int FonctionF(char x0, char x1, char x2){
	return (x0*1) + (x1*2) + (x2*4);
}


void initialiserLFSR(const char* path,LFSR* gen){
	FILE* file = fopen(path,"r");
	assert(file);
	char* buffer;
	ssize_t read;
	size_t len = 0;
	int i = 0;
   	printf("Initialisation du LFSR\n");
    while ((read = getline(&buffer, &len, file)) != -1) {
       read = getline(&buffer, &len, file);
       //printf("%s", buffer);
       strcpy(gen->L[i],buffer);
       //gerer derniere ligne
       if(i == 3) strncpy(gen->F,buffer,8);
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
	if(argc != 3){ fprintf(stderr, "Usage: Usage  : ./Flow fichier n\n"); exit(0);}
	//Ouvir le fichier et initialiser le LFSR
	LFSR* Geffe = (LFSR*) malloc(sizeof(LFSR));
	initialiserLFSR(argv[1],Geffe);
	//DebugLFSR(Geffe);
	//printf("%d\n", FonctionF(1, 1, 1));
  	free(Geffe);
	return 0;
}
