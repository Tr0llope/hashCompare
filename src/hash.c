#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h> //nécessite libssl-dev

#include "hash.h"

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

