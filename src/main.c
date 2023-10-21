#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <openssl/md5.h>

#include "hash.h"
#include "t3c_struct.h"

#define MAX_BUFFER_SIZE 1024



/********************************/
//Comparaison
//args: 2 hashs. si correspondance, retourne vrai

//Permet de convertir le hash en string pour la comparaison
char *hexEncode(const unsigned char *hash, size_t length) {
	char *hexString = malloc(length * 2 + 1);
	for (size_t i = 0; i < length; i++) {
		sprintf(hexString + i * 2, "%02x", hash[i]);
	}
	return hexString;
}

bool comparaison(char *input_hash, unsigned char *rainbow_hash){
	bool cracked = false;
	char *hexString = hexEncode(rainbow_hash, MD5_DIGEST_LENGTH);
	if(strcmp(input_hash, hexString)==0) cracked = true;
	free(hexString);
	return cracked;
}

/********************************/

void checkInit(int argc, char** argv, FILE *output_file){
	if(argc==3 && isatty(fileno(stdin))){
		perror("Error too few arguments");
		exit(1);
	}
	if (output_file == NULL) {
		perror("Error opening rainbow table");
		exit(1);
	}
}

void modeG(char buffer[MAX_BUFFER_SIZE], int count, int argc, char ** argv){

	FILE *input_stream = fopen(argv[3], "r");
	if(input_stream==NULL) input_stream = stdin;
	FILE *output_file = fopen(argv[2], "w");
	//Vérification de la commande
	checkInit(argc, argv, output_file);
	
	
	unsigned char hash[MD5_DIGEST_LENGTH*2 + 1];
	printf("INFO Writing table...\n");
	while (fgets(buffer, MAX_BUFFER_SIZE, input_stream) != NULL) { // Lit ligne par ligne l'entrée standard
		size_t length = strlen(buffer);
		if (length > 0 && buffer[length - 1] == '\n') { //remplace \n par \0 dans le buffer
			buffer[length - 1] = '\0';
		}
		generation_hash_MD5(hash, buffer);//Génération du hash
		hash_to_file(output_file, hash, buffer); //Ecriture de la rainbow table
		count++;

		if (count % 1000000 == 0){
			printf("INFO %dM hashes written...\n", count/1000000);//Affichage utilisateur
		}
	}

	printf("INFO %d hashes written.\n", count);

	fclose(output_file);
}

void modeL(char buffer[MAX_BUFFER_SIZE], int count, int argc, char ** argv){

	FILE * input_hashs = fopen(argv[3], "r");
	if(input_hashs==NULL) input_hashs = stdin;
	FILE *rainbow = fopen(argv[2], "r");
	//Vérification de la commande
	checkInit(argc, argv, rainbow);
	
	t3c *liste = create();

	printf("INFO Loading dict file: %s\n", argv[2]);
	while (fgets(buffer, MAX_BUFFER_SIZE, rainbow) != NULL) { 

		char *hash_str = strtok(buffer, ":");
		char *mdp_clair = strtok(NULL, ":");

		if (mdp_clair != NULL && hash_str != NULL) {
			// Convertir la chaîne hexadécimale en tableau d'octets
			unsigned char hash[MD5_DIGEST_LENGTH];
			for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
				sscanf(hash_str + 2 * i, "%02hhx", &hash[i]);
			}

			// Ajouter à la liste
			addt3c(&liste, mdp_clair, hash);

			count++;
			if (count % 1000000 == 0) {
				printf("INFO Loading %dM hashes...\n", count/1000000);
			}

		}
	}
	printf("INFO %d hashes loaded.\n", count);



	fclose(rainbow);
	//Fin du chargement en mémoire

	//Recherche de correspondance entre les input hashs et les rainbow hashs
	char buffer2[MAX_BUFFER_SIZE];
	while (fgets(buffer2, MAX_BUFFER_SIZE, input_hashs) != NULL) {
		size_t length = strlen(buffer2);
		if (length > 0 && buffer2[length - 1] == '\n') { //remplace \n par \0 dans le buffer
			buffer2[length - 1] = '\0';
		}

		t3c *tmp = liste;
		while (tmp != NULL) {
			if(comparaison(buffer2, tmp->hash)){
				printf("MATCH ");

				for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
					printf("%02x", tmp->hash[i]);
				}
				printf(" %s\n", tmp->mdp_clair);
				break;
			}
			tmp = tmp->next;
		}
	}

	// Libération de la mémoire allouée par la liste
	freeList(liste);
}


int main(int argc, char *argv[]){
	char buffer[MAX_BUFFER_SIZE];
	int count = 0;
	if(strcmp(argv[1],"-G")==0){//Mode Génération
		modeG(buffer, count, argc, argv);

	}else if(strcmp(argv[1], "-L")==0){ //Mode de comparaison
		modeL(buffer, count, argc, argv);
	}
	else printf("Mode inconnu\n");
	return 0;

}
