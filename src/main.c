#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/md5.h> //nécessite libssl-dev

//cat rockyou.txt | ./exec -G rainbow_table.txt
//cat hashes_input.txt | ./exec -L rainbow_table.txt

#define MAX_BUFFER_SIZE 1024
#define MD5_DIGEST_LENGTH 16

/*******************************/
//Hashage
void generation_hash_MD5(unsigned char hash[MD5_DIGEST_LENGTH], char *string){
	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx, string, strlen(string));
	MD5_Final(hash, &ctx);
}

void printHash(unsigned char hash[MD5_DIGEST_LENGTH]){
	printf("Hash: ");
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
		printf("%02x", hash[i]);
	}
	printf("\n");
}

// Ecrit dans le fichier de sortie: mdp_clair:mdp_hash
void hash_to_file(FILE *file, unsigned char mdp_hash[MD5_DIGEST_LENGTH], char * mdp_clair){
	fprintf(file, "%s:", mdp_clair);
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
		fprintf(file, "%02x", mdp_hash[i]);
	}
	fprintf(file, "\n");
}

/********************************/
//Structure
typedef struct T3c {
	char *mdp_clair;
	unsigned char hash[MD5_DIGEST_LENGTH];
	struct T3c *next;
} t3c;

t3c* create() {
	return NULL;
}

void addt3c(t3c **tete, char *mdp_clair, unsigned char *hash) {
	t3c *new_elem = malloc(sizeof(t3c));
	new_elem->mdp_clair = strdup(mdp_clair);
	memcpy(new_elem->hash, hash, MD5_DIGEST_LENGTH);
	new_elem->next = *tete;
	*tete = new_elem;
}

// Fonction pour afficher toute la liste chaînée
void print_t3c(t3c *tete) {
	t3c *tmp = tete;
	while (tmp != NULL) {
		printf("Mot de passe : %s\n", tmp->mdp_clair);
		printf("Hash : ");
		for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
			printf("%02x", tmp->hash[i]);
		}
		printf("\n");

		tmp = tmp->next;
	}
}

// Fonction pour libérer la mémoire allouée par la liste chaînée
void freeList(t3c *tete) {
	t3c *tmp = tete;
	while (tmp != NULL) {
		t3c *next = tmp->next;
		free(tmp->mdp_clair);
		free(tmp);
		tmp = next;
	}
}

/********************************/
//Comparaison
//args: 2 hashs. si correspondance, retourne vrai

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

int main(int argc, char *argv[]){
	//printf("mode: %s\n", argv[1]);
	char buffer[MAX_BUFFER_SIZE];
	int count = 0;

	if(strcmp(argv[1],"-G")==0){//Mode Génération stdin(dico) | ./main.c mode rainbow ou ./main.c mode rainbow dico

		//on ouvre le fichier de sortie
		FILE *output_file = fopen(argv[2], "w");
		if (output_file == NULL) {
			perror("Error opening output file");
			exit(EXIT_FAILURE);
		}
		unsigned char hash[MD5_DIGEST_LENGTH*2 + 1];
		printf("INFO Writing table...\n");
		while (fgets(buffer, MAX_BUFFER_SIZE, stdin) != NULL) { // Lit ligne par ligne l'entrée standard
			size_t length = strlen(buffer);
			if (length > 0 && buffer[length - 1] == '\n') { //remplace \n par \0 dans le buffer
				buffer[length - 1] = '\0';
			}
			generation_hash_MD5(hash, buffer);//Génération du hash
			hash_to_file(output_file, hash, buffer); //Ecriture de la rainbow table
			count++;

			if (count % 1000000 == 0){
				printf("INFO %d hashes written...\n", count);//Affichage utilisateur
			}
		}

		printf("INFO %d hashes written.\n", count);

		fclose(output_file);
	}
	else if(strcmp(argv[1], "-L")==0){ //Mode de comparaison stdin(hashs) | ./main.c mode rainbow ou ./main.c mode rainbow hashs
		//test 2: fonction de comparaison entre stdin et struct t3c
		//on ouvre la rainbow table
		FILE *rainbow = fopen(argv[2], "r");
		if (rainbow == NULL) {
			perror("Error opening output file");
			exit(EXIT_FAILURE);
		}

	/*char buffer2[MAX_BUFFER_SIZE];
		while (fgets(buffer2, MAX_BUFFER_SIZE, stdin) != NULL) {
			size_t length = strlen(buffer2);
			if (length > 0 && buffer2[length - 1] == '\n') { //remplace \n par \0 dans le buffer
				buffer2[length - 1] = '\0';
			}
			printf("%s\n", buffer2);
		}*/
















		t3c *liste = create();

		printf("INFO Loading dict file: %s\n", argv[2]);
		while (fgets(buffer, MAX_BUFFER_SIZE, rainbow) != NULL) { 

			// Splitter la ligne en mot de passe clair et hash (vous devrez adapter cela à votre format)
			char *mdp_clair = strtok(buffer, ":");
			char *hash_str = strtok(NULL, ":");

			if (mdp_clair != NULL && hash_str != NULL) {
				// Convertir la chaîne hexadécimale en tableau d'octets
				unsigned char hash[MD5_DIGEST_LENGTH];
				for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
					sscanf(hash_str + 2 * i, "%02x", &hash[i]);
				}

				// Ajouter à la liste
				addt3c(&liste, mdp_clair, hash);

				count++;
				if (count % 1000000 == 0) {
					printf("INFO Loading %d hashes...\n", count);
				}

			}
		}
		printf("INFO %d hashes loaded.\n", count);



		fclose(rainbow);
		//Fin du chargement en mémoire

		//Recherche de correspondance entre les input hashs et les rainbow hashs
		char buffer2[MAX_BUFFER_SIZE];
		while (fgets(buffer2, MAX_BUFFER_SIZE, stdin) != NULL) {
			size_t length = strlen(buffer2);
			if (length > 0 && buffer2[length - 1] == '\n') { //remplace \n par \0 dans le buffer
				buffer2[length - 1] = '\0';
			}
			//printf("%s\n", buffer2);

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
	else {
		printf("Mode inconnu\n");
	}

	return 0;

}
