#ifndef STRUCT_H
#define STRUCT_H

#define MD5_DIGEST_LENGTH 16

typedef struct T3c {
	char *mdp_clair;
	unsigned char hash[MD5_DIGEST_LENGTH];
	struct T3c *next;
} t3c;

t3c* create();

void addt3c(t3c **tete, char *mdp_clair, unsigned char *hash);

void print_t3c(t3c *tete);

void freeList(t3c *tete);

#endif
