#ifndef HASH_H
#define HASH_H

#define MD5_DIGEST_LENGTH 16

void generation_hash_MD5(unsigned char hash[MD5_DIGEST_LENGTH], char *string);

void printHash(unsigned char hash[MD5_DIGEST_LENGTH]);

void hash_to_file(FILE *file, unsigned char mdp_hash[MD5_DIGEST_LENGTH], char * mdp_clair);

#endif
