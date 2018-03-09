#include "TArbore.h"

//operatia care cauta un cuvant in tree
/* returneaza pozitia la care se poate incheia un alt
cuvant cu acelasi prefix ca si cuvantul dat */
TArbore find (TArbore arb, char *word, int *count)
{
	if (!arb)
		return NULL;

	int ok = 0, i = 0;
	TArbore aux = NULL, lista = NULL, to_return = arb;
	aux = arb->urmlista;	//prima litera din trie
	*count = 0;
	//parcurgerea arborelui
	while (i < strlen(word) && aux)
	{
		ok = 0;
		lista = aux;	//lista de copii
		//verifica daca litera curenta e in lista de copii
		while (lista)
		{
			if (!compC(lista->caracter, word[i]))
			{
				ok = 1;
				aux = lista;
				break;
			}
			lista = lista->urmfrt;
		}
		//daca nu s-a gasit litera, returneaza NULL
		if (ok == 0)
			return NULL;
		i++;
		/* salveaza pozitia la care se poate incheia un
		alt cuvant, reprezentand prefix pentru cuvantul dat */
		if (i < strlen(word) && (aux->tip == 'f' || 
			aux->urmlista->urmfrt))
		{
			*count = *count + i;
			to_return = aux;
		}
		/* daca s-au terminat literele din cuvant si inca
		nu s-a ajuns la un nod final, returneaza NULL */
		if (i == strlen(word) && aux->tip != 'f')
			return NULL;
		aux = aux->urmlista;
	}

	//daca sunt cuvinte ce sunt prefix pentru cuvantul dat
	if (aux)
		*count = -1;
	return to_return;
}

//afisare mesaj in functie de rezulatul functiei find
void findWord (TArbore arb, char *word, FILE *fout)
{
	if (!arb)
		return;

	int ok = 0;
	if (find(arb, word, &ok))
		fprintf(fout, "True\n");
	else
		fprintf(fout, "False\n");
}

//functia de stergere cuvant din trie
void removeW (TArbore arb, char *word)
{
	if (!arb)
		return;

	int ok = -1;
	TNod *found = find(arb, word, &ok);
	if (!found)
		return;
	int i = 0;
	TArbore aux = arb->urmlista, lista = NULL;
	/* daca word este un prefix al altui cuvant, ultima
	litera din el nu va mai fi sfarsit de cuvant */
	if (ok == -1)
	{
		while (i < strlen(word))
		{
			lista = aux;
			while (lista)
			{
				if (!compC(lista->caracter, word[i]))
				{
					aux = lista;
					break;
				}
				lista = lista->urmfrt;
			}
			i++;
			if (i == strlen(word) && aux->tip == 'f')
				aux->tip = 'n';
			aux = aux->urmlista;
		}
	}
	else
	{
		TArbore ant = NULL, antlista = found;
		/* porneste de la pozitia de la care se poate 
		sterge un cuvant fara a distruge alte cuvinte */
		aux = found->urmlista;
		i = ok;
		int contor = 0;
		while (i < strlen(word))
		{
			contor++;
			lista = aux;
			ant = NULL;
			while (lista)
			{
				if (!compC(lista->caracter, word[i]))
				{
					//stergere de la inceput de lista
					if (!ant)
					{
						/* face legatura urmatoarei litere din lista
						cu nodul anterior, pentru a nu se pierde
						cuvintele care incep cu literele precedente */
						if (contor == 1)
							antlista->urmlista = lista->urmfrt;
						aux = lista->urmlista;
						free(lista);
					}
					//stergere din interiorul listei
					else if (ant && lista->urmfrt)
					{
						aux = lista->urmlista;
						ant->urmfrt = lista->urmfrt;
						free(lista);
					}
					//stergere de la final de lista
					else
					{
						aux = lista->urmlista;
						ant->urmfrt = NULL;
						free(lista);
					}
					break;
				}
				ant = lista;	//actualizeaza anterior
				lista = lista->urmfrt;
			}
			i++;
		}
	}
}

TNod* fprefix (TNod *nod, int length, char *word, char *prefix)
{
	int i = 0, ok = 0;
	TNod *new;

	while (i < strlen(word))
	{
		new = nod;
		ok = 0;
		while (new)
		{
			if (!compC(new->caracter, word[i]))
			{
				nod = new->urmlista;
				prefix[i] = new->caracter;
				ok = 1;
				break;
			}
			new = new->urmfrt;
		}
		if (!ok)
			break;
		i++;
	}
	prefix[i] = '\0';

	return nod;
}

//cauta cel mai lung prefix al cuvantului
void findl_prefix (TArbore arb, char *word, FILE *fout, int length)
{
	if (!arb)
		return;

	TArbore aux = arb->urmlista;
	char *prefix = (char *) malloc (length+ 1);
	if (!prefix)
		return;
	prefix[0] = '\0';
	
	//cauta caracter cu caracter in trie
	fprefix (aux, strlen(word), word, prefix);
	
	//daca a gasit un prefix, il afiseaza
	if (prefix[0] != '\0')
		fprintf(fout, "%s\n", prefix);
	else
		fprintf(fout, "None\n");
	free(prefix);
}

//cauta toate cuvintele cu un prefix
void findaw_prefix (TArbore arb, char *word, FILE *fout, int length)
{
	if (!arb)
		return;

	TArbore aux = arb->urmlista;
	char *prefix = (char *) malloc (length+1);
	if (!prefix)
		return;
	
	//cauta prefixul in trie
	aux = fprefix (aux, strlen(word), word, prefix);

	if ((strlen(word) != strlen(prefix)) || !aux)
		fprintf(fout, "None\n");
	else
	{
		//subarborele prefixului dat
		TArbore newarb = initNod('n', '\0');
		if (!newarb)
			return;
		newarb->urmlista = aux;
		int yes = 0;
		//daca prefixul este si cuvant, il afiseaza
		if (find(arb, prefix, &yes))
			fprintf(fout, "%s ", prefix);
		//afiseaza toate cuvintele cu prefixul dat
		afisare(newarb, prefix, fout);
		fprintf(fout, "\n");
		free(newarb);
	}

	free(prefix);
	
}

//functia de inserare alfabetica in trie
void InserAlfab (AArbore arb, char *word)
{
	if (!(*arb))
		return;

	int i = 0, ok = 0;
	TArbore aux = (*arb), nod = NULL;
	TArbore lista = NULL, ant = NULL;

	while (i < strlen(word))
	{
		//daca nu exista nicio litera in trie
		if (!aux->urmlista)
		{
			//initializeaza un nou nod
			if (i < strlen(word) - 1)
				nod = initNod('n', word[i]);
			else
				nod = initNod('f', word[i]);
			aux->urmlista = nod;
			aux = aux->urmlista;
		}
		else
		{
			lista = aux->urmlista;
			ant = NULL;
			ok = 0;
			while (lista)
			{
				/* daca s-a gasit pozitia inaintea caruia
				sa se insereze */
				if (compC(lista->caracter, word[i]) > 0)
				{
					//verifica daca este final de cuvant
					if (i < strlen(word) - 1)
						nod = initNod('n', word[i]);
					else
						nod = initNod('f', word[i]);
					//inserare la inceput de lista
					if (!ant)
					{
						nod->urmfrt = aux->urmlista;
						aux->urmlista = nod;
						ok = 1;
						aux = aux->urmlista;
						break;
					}
					//inserare in interiorul listei
					else
					{
						nod->urmfrt = lista;
						ant->urmfrt = nod;
						ok = 1;
						aux = ant->urmfrt;
						break;
					}
				}
				//daca exista deja in trie, marcheaza
				//nodul ca fiind sfarsit de cuvant
				else if (!compC(lista->caracter, word[i]))
				{
					ok = 1;
					if (i == strlen(word) - 1)
						lista->tip = 'f';
					aux = lista;
					break;
				}
				//actualizare anterior
				ant = lista;
				lista = lista->urmfrt;
			}

			//inserare la sfarsit de lista, daca nu s-a
			//gasit caracterul in trie
			if (!ok && compC(ant->caracter, word[i]) < 0)
			{
				if (i < strlen(word) - 1)
					nod = initNod('n', word[i]);
				else
					nod = initNod('f', word[i]);
				ant->urmfrt = nod;
				aux = ant->urmfrt;
			}
		}
		i++;
	}
}

//lungimea medie a cuvintelor care incep cu un prefix dat
void mean (TArbore arb, char *word, FILE *fout, int length)
{
	if (!arb)
		return;

	TArbore aux = arb->urmlista;
	char *prefix = (char *) malloc (length+1);
	if (!prefix)
		return;
	
	//cauta prefixul in trie
	aux = fprefix (aux, strlen(word), word, prefix);

	//daca nu a gasit prefiul in arbore, afiseaza 0
	if ((strlen(word) != strlen(prefix)) || !aux)
		fprintf(fout, "0\n");
	else
	{
		//subarborele prefixului dat
		TArbore newarb = initNod('n', '\0');
		newarb->urmlista = aux;
		float length = 0, nr = 0;
		mean_length(newarb, prefix, &length, &nr);
		fprintf(fout, "%.3f\n", length/nr);
		free(newarb);
	}

	free(prefix);
}

//calculeaza lungimea tuturor cuvintelor din trie
//si numarul total de cuvinte
void mean_length (TArbore nod, char *word, float *length, float *nr)
{
	//adauga un nou caracter
	word[strlen(word) + 1] = '\0';
	word[strlen(word)] = nod->caracter;
	//daca este sfarsit de cuvant, actualizeaza
	//lungimea totala si numarul total de cuvinte
	if (nod->tip == 'f')
	{
		*length += strlen(word);
		*nr = *nr + 1;
		if (!nod->urmlista)
			word[strlen(word) - 1] = '\0';
	}
	//parcurge toate listele din trie
	if (nod->urmlista)
	{
		mean_length(nod->urmlista, word, length, nr);
		//elimina caracterul adaugat cand 
		//se intoarce din recursivitate
		if (strlen(word))
			word[strlen(word) - 1] = '\0';
	}
	//parcurge lista unei liste din trie
	if (nod->urmfrt)
	{
		mean_length(nod->urmfrt, word, length, nr);
		//elimina caracterul adaugat cand 
		//se intoarce din recursivitate
		if (!nod->urmfrt && strlen(word))
			word[strlen(word) - 1] = '\0';
	}
}

//afiseaza cuvintele din trie
void afisare (TArbore nod, char *word, FILE *fout)
{
	//adauga un nou caracter
	word[strlen(word) + 1] = '\0';
	word[strlen(word)] = nod->caracter;
	//daca este sfarsit de cuvant, afiseaza cuvantul
	if (nod->tip == 'f')
	{
		fprintf(fout, "%s ", word);
		if (!nod->urmlista)
			word[strlen(word) - 1] = '\0';
	}
	//parcurge toate listele din trie
	if (nod->urmlista)
	{
		afisare(nod->urmlista, word, fout);
		//elimina caracterul adaugat cand 
		//se intoarce din recursivitate
		if (strlen(word))
			word[strlen(word) - 1] = '\0';
	}
	//parcurge lista unei liste din trie
	if (nod->urmfrt)
	{
		afisare(nod->urmfrt, word, fout);
		//elimina caracterul adaugat cand 
		//se intoarce din recursivitate
		if (!nod->urmfrt && strlen(word))
			word[strlen(word) - 1] = '\0';
	}
}
