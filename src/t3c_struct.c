#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "t3c_struct.h"

t3c* create() {
	return NULL;
}

void addt3c(t3c **tete, char *mdp_clair, unsigned char *hash){
	t3c *new_elem = malloc(sizeof(t3c));
	new_elem->mdp_clair = strdup(mdp_clair);
	memcpy(new_elem->hash, hash, MD5_DIGEST_LENGTH);
	new_elem->next = *tete;
	*tete = new_elem;
}

// Fonction pour afficher toute la liste chaînée
void print_t3c(t3c *tete){
	t3c *tmp = tete;
	while (tmp != NULL){
		printf("Mot de passe : %s\n", tmp->mdp_clair);
		printf("Hash : ");
		for (int i = 0; i < MD5_DIGEST_LENGTH; i++){
			printf("%02x", tmp->hash[i]);
		}
		printf("\n");

		tmp = tmp->next;
	}
}

// Fonction pour libérer la mémoire allouée par la liste chaînée
void freeList(t3c *tete){
	t3c *tmp = tete;
	while (tmp != NULL){
		t3c *next = tmp->next;
		free(tmp->mdp_clair);
		free(tmp);
		tmp = next;
	}
}
