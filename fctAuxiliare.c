#include "TArbore.h"

//initializare nod nou
TNod* initNod (char tip, char caracter)
{
	TNod *nod = (TNod *) malloc (sizeof(TNod));
	nod->tip = tip;
	nod->caracter = caracter;
	nod->urmfrt = NULL;
	nod->urmlista = NULL;

	return nod;
}

//returneaza diferenta codurilor ASCII a doua caractere
int compC (char car1, char car2)
{
	return car1 - car2;
}

//elibereaza memorie din arbore
void eliberare (TArbore arb)
{
	if (!arb)
		return;
	eliberare(arb->urmlista);
	eliberare(arb->urmfrt);
	free(arb);
}