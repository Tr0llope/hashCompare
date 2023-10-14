#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 255
int main(int argc, char *argv[]){
	//test 1: lire depuis l'entrée standard stdin et écrire dans un fichier
	
	//on ouvre le fichier de sortie
	FILE *output_file = fopen(argv[1], "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }
    
    char buffer[MAX_BUFFER_SIZE];
    while (fgets(buffer, MAX_BUFFER_SIZE, stdin) != NULL) {
    	fprintf(output_file, buffer);
    }
    
    fclose(output_file);

    return 0;
		
}
