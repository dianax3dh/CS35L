#include <stdio.h>
#include <stdlib.h> /*malloc, realloc, free, qsort */
/*frobcmp accepts two arguements, both are const char that*/ 
/*point to array of non-space bytes, followed by space bytes*/
/*if < return -1*/
/*if == return 0*/
/*if > return 1*/


int frobcmp (const char* a, const char* b)
{
	/*byte by byte lexicographic comparison on NON-SPACE bytes; 
	assume arrays are frobnicated, so XORing it unfrobnicates it
	should return equivalent of running memcmp on unfrobnicated arrays*/
	while ((*a != ' ') && (*b != ' ')) /*whiLe a and b are not spaces*/
	{
		/*xor with 42 bc memfrob frobnicates each byte by xor with 42*/
		if ((*a^42) < (*b^42))/*less than, return -1*/
		{
			return -1; 
		}
		if ((*a^42) > (*b^42)) /*greater than, return 1*/
		{
			return 1; 
		}
		// if ((*a^42) == (*b^42)) equal to, return 0
		// {
		// 	return 0; 
		// }

		a++; /*iterate*/
		b++; /*iterate*/
	}

	/*frobnicated arrays here*/ 
	/*a and b have spaces; check prefixes*/
	if ((*a == ' ') && (*b == ' '))/*if a and b are the same*/
	{
		return 0; 
	}
	if ((*a == ' ') && (*b != ' ')) /*if a < b*/
	{
		return -1; 
	}
	else 
		if ((*a != ' ') && (*b == ' ')) /*if a > b*/
		{
			return 1; 
		}
}

/*compare function for qsort*/
int sortcmpFunc(void const *a, void const *b) 
{
	return frobcmp(*(char**)a, *(char**)b); 
}

int main() 
{
	int sizeofallocation = 100; 
	int indexOfWord = 0; /*index*/
	int numOfWords = 0; /*count the # of words*/
	char *arrMem; 
	arrMem = (char*)malloc(sizeof(char)*sizeofallocation); 

	if (arrMem == NULL) /*memory allocation error, exit 1*/
	{
		fprintf(stderr, "Memory allocation failure."); 
		exit(1); 
	}
	

	char inputChar = getchar(); 
	/* getChar will read char by char until the end of the file,
	so while not the end of the file it will do......*/
	/*get all of the characters from stdin into one big array*/
	char *tempMem; 
	while (inputChar != EOF) 
	{
		/*if memory is full, reallocate*/
		if (indexOfWord == sizeofallocation)
		{
			sizeofallocation *= 2; 
			//arrMem = realloc(arrMem,sizeof(char)*sizeofallocation); /*reallocate, double the size*/ 
			//use temp incase realloc fails and there is memory leak
			tempMem = realloc(arrMem,sizeof(char)*sizeofallocation); 
			if (tempMem  == NULL)
			{
				fprintf(stderr, "Memory reallocation failure.");
				free(arrMem); 
				exit(1); 
			}
			else
				if (tempMem != NULL)
				{
					arrMem = tempMem; 
				}
			// if (arrMem == NULL) /*check again for any allocation error*/
			// {
			// 	fprintf(stderr, "Memory reallocation failure."); 
			// 	exit(1); 
			// }
		}

		if (inputChar != ' ')
		/*if charcter is not a space, add to the array*/
		{
			// printf("inputting chars"); 
			// printf("Char being put in is: %c\n", inputChar); 
			// printf("Index of word is now: %d", indexOfWord); 
			arrMem[indexOfWord++] = inputChar; 
		}		
		else
			if (inputChar == ' ')
			/*if there is a space and it is not the first element; 
			incase first word begins after the second position*/
			{	
				// printf("inputting spaces"); 
				numOfWords++; 
				arrMem[indexOfWord++] = inputChar; 
			}
		// printf("Num Words: %d", numOfWords);
		inputChar = getchar(); /*continue to get char*/
	}

	if (inputChar == EOF) /*if end of file check for space*/
	{
		// printf("reached EOF"); 
		
		if (indexOfWord == sizeofallocation)
		{
			sizeofallocation += 1; 
			tempMem = realloc(arrMem,sizeof(char)*sizeofallocation); 
			if (tempMem  == NULL)
			{
				fprintf(stderr, "Memory reallocation failure.");
				free(arrMem); 
				exit(1); 
			}
			else
				if (tempMem != NULL)
				{
					arrMem = tempMem; 
				}
			// arrMem = realloc(arrMem,sizeof(char)*sizeofallocation); 
			
			// if (arrMem == NULL) check again for any allocation error
			// {
			// 	fprintf(stderr, "Memory reallocation failure."); 
			// 	exit(1); 
			// }
				// printf("Index of word is now: %d", indexOfWord); 
		}	
		
		if ((indexOfWord > 0) && (arrMem[indexOfWord-1] != ' '))
		{
			arrMem[indexOfWord] = ' '; /*add space to end of file*/
			numOfWords++; 
		}		
	}

	if (ferror(stdin))  /*error reading from stdin*/ 
	{  
		fprintf(stderr, "Error reading input." );
		exit(1); 
	}

	char *ptArrMem = arrMem;  /*needed to free arrMem later*/
	char** getWords = (char**)malloc(sizeof(char*)*numOfWords); 
	/*memory allocation for new "array" of pointers that point to the words*/
	if (getWords == NULL) /*check for malloc error*/
	{
		fprintf(stderr, "Memory allocation failure."); 
		exit(1); 
	}
	// printf("Created the getWords array"); 

	int i;
	for (i = 0; i < numOfWords; i++)
	{
		getWords[i] = ptArrMem;
		while(*ptArrMem != ' ' && (i+1 != numOfWords)) /*while not a space and not last word*/
		{
			ptArrMem++; /*increment arrMem array*/
		}
		if (*ptArrMem == ' ') //DONT ++
		{
			ptArrMem++; /*go past the space*/
		}
		// printf("printed stuff to getWords");
	}

	qsort(getWords, numOfWords, sizeof(char*), sortcmpFunc); 

	/*print out*/
	int j; 
	char *ptrToPrint; 
	for (j = 0; j < numOfWords; j++)
	{	
		// printf("printing");
		ptrToPrint = getWords[j];
		putchar(*ptrToPrint); 
		while (*ptrToPrint != ' ' && *ptrToPrint != EOF)
		{		
			ptrToPrint++; 
			putchar(*ptrToPrint); 
		}
	}

	if (ferror(stdout))
	{
		fprintf(stderr, "Error in printing out."); 
		exit(1); 
	}

	/*free*/
	free(getWords);
	free(arrMem); 

	exit(0); 

}