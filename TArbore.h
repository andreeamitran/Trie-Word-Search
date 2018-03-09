#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TRIE
#define TRIE

typedef struct arbore
{
	char tip;
	char caracter;
	struct arbore *urmfrt;
	struct arbore *urmlista;
} TNod, *TArbore, **AArbore;

TNod* initNod (char tip, char caracter);
int compC (char car1, char car2);
TArbore find (TArbore arb, char *word, int *yes);
TNod* fprefix (TNod *nod, int length, char *word, char *prefix);
void InserAlfab (AArbore arb, char *word);
void afisare (TArbore nod, char *word, FILE *fout);
void findl_prefix (TArbore arb, char *word, FILE *fout, int length);
void findaw_prefix (TArbore arb, char *word, FILE *fout, int length);
void findWord (TArbore arb, char *word, FILE *fout);
void mean (TArbore arb, char *word, FILE *fout, int length);
void mean_length (TArbore nod, char *word, float *length, float *nr);
void removeW (TArbore arb, char *word);
void eliberare (TArbore arb);

#endif