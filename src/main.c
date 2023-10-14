#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

#define MAX_BUFFER_SIZE 1024
#define MD5_DIGEST_LENGTH 16

void generation_hash_MD5(unsigned char hash[MD5_DIGEST_LENGTH], char *string)
{
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

int main(int argc, char *argv[]){
	
	//on ouvre le fichier de sortie
	FILE *output_file = fopen(argv[1], "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }
    
    char buffer[MAX_BUFFER_SIZE];
    unsigned char hash[MD5_DIGEST_LENGTH*2 + 1];
    while (fgets(buffer, MAX_BUFFER_SIZE, stdin) != NULL) {
    	size_t length = strlen(buffer);
        if (length > 0 && buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0';
        }
    	generation_hash_MD5(hash, buffer);//Génération du hash
    	hash_to_file(output_file, hash, buffer);
    }
    
    fclose(output_file);

    return 0;
		
}
