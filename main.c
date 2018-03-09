#include "TArbore.h"

int main(int argc, char *argv[])
{
	FILE *fin = fopen(argv[1], "rt");

	if (!fin)
		return 1;

	FILE *fout = fopen(argv[2], "wt");

	if (!fout)
	{
		fclose(fin);
		return 1;
	}

	char *oper, *word, *new, ch;
	int Max = 0;
	oper = (char *) malloc (22);

	int N = 0, i = 0;
	TArbore arb = initNod('n', '\0');
	if (!arb)
	{
		free(oper);
		return 1;
	}

	fscanf(fin, "%d", &N);
	for(i = 0; i < N; i++)
	{
		//citeste operatia
		fscanf(fin, "%s", oper);
		//citeste spatiile pana la cuvant
		ch = fgetc(fin);
		while (ch == ' ')
			ch = fgetc(fin);
		//daca nu exista argument
		if ((ch == '\r' || ch == '\n') && 
		!strcmp(oper, "find_all_with_prefix"))
		{
			if (Max)
				new = (char *) malloc (Max+1);
			else
				return 1;
			if (!new)
			{
				free(oper);
				eliberare(arb);
				return 1;
			}
			new[0] = '\0';
			afisare(arb, new, fout);
			fprintf(fout, "\n");
			free(new);
		}
		else if ((ch == '\r' || ch == '\n') && 
		!strcmp(oper, "mean_length"))
		{
			float length = 0;
			float nr = 0;
			if (Max)
				new = (char *) malloc (Max+1);
			else
				return 1;
			if (!new)
			{
				free(oper);
				eliberare(arb);
				return 1;
			}
			new[0] = '\0';
			mean_length(arb, new, &length, &nr);
			fprintf(fout, "%.3f\n", length/nr);
			free(new);
		}
		//daca exista argument
		else
		{
			//formeaza cuvantul, citind litera cu litera
			//si realocand vectorul de caractere
			word = (char *) malloc (1);
			if (!word)
			{
				free(oper);
				eliberare(arb);
				return 1;
			}

			word[0] = '\0';
			int count = 1;
			fseek(fin, -1, SEEK_CUR);
			ch = fgetc(fin);
			while ((ch >= 'a' && ch <='z') || 
				(ch >= '0' && ch <= '9'))
			{
				count++;
				word = (char *) realloc (word, count);
				if (!word)
				{
					free(oper);
					eliberare(arb);
					return 1;
				}
				word[count - 2] = ch;
				word[count - 1] = '\0';
				ch = fgetc(fin);
			}

			//in functie de operatia citita apeleaza
			//functia corespunzatoare
			if (strlen(word) > Max)
				Max = strlen(word);
			if (!strcmp(oper, "add"))
				InserAlfab(&arb, word);
			else if (!strcmp(oper, "find_longest_prefix"))
				findl_prefix(arb, word, fout, Max);
			else if (!strcmp(oper, "find_all_with_prefix"))
				findaw_prefix(arb, word, fout, Max);
			else if (!strcmp(oper, "find"))
				findWord(arb, word, fout);
			else if (!strcmp(oper, "mean_length"))
				mean(arb, word, fout, Max);
			else if (!strcmp(oper, "remove"))
				removeW(arb, word);

			free(word);
		}
	}

	free(oper);
	eliberare (arb);
	fclose(fin);
	fclose(fout);

	return 0;
}
