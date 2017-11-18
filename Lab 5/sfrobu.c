//rewrite sfrobs to use system calls instead of <stdio.h>

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>

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
	// printf("done frobcomparing");
}

int frobcmpUppercase (const char* a, const char* b)
{
	/*byte by byte lexicographic comparison on NON-SPACE bytes; 
	assume arrays are frobnicated, so XORing it unfrobnicates it
	should return equivalent of running memcmp on unfrobnicated arrays*/
	while ((*a != ' ') && (*b != ' ')) /*whiLe a and b are not spaces*/
	{
		/*xor with 42 bc memfrob frobnicates each byte by xor with 42*/
		if ((toupper(*a^42)) < (toupper(*b^42)))/*less than, return -1*/
		{
			return -1; 
		}
		if ((toupper(*a^42)) > (toupper(*b^42))) /*greater than, return 1*/
		{
			return 1; 
		}
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
	// printf("done frobcomparing");
}

/*compare function for qsort*/
int sortcmpFunc(void const *a, void const *b) 
{
	return frobcmp(*(char**)a, *(char**)b); 
}

int sortcmpFuncUpper(void const *a, void const *b) 
{
	return frobcmpUppercase(*(char**)a, *(char**)b); 
}

int main(int argc, char** argv) 
{
	// printf("I AM STARTING"); 
	int numArgC = argc; 
	struct stat datafstat; //apart of fstat
	char charRead; //characters being read in 
	unsigned char *charStored; //pointer to characters being stored
	//char **ptToWord; //double pointer; point to beginning of each word
	size_t fileSize = 0; 
	char *tempMem; 
	int initAlloc = 50;
	//doesn't work with 100 or 0 for some reason, but 50 works


	if (fstat(0, &datafstat) < 0)
	{
		fprintf(stderr, "fstat failure.");
		exit(1); 
	}

	if (S_ISREG(datafstat.st_mode)) //checks if it is a regular file
	{
		fileSize = datafstat.st_size;  //size in bytes 
		//printf("filesize: %d", fileSize); 
		charStored = (char*)malloc(sizeof(char)*fileSize); 
		if (charStored == NULL)
			{
				fprintf(stderr, "Memory allocation failure.");
				exit(1); 
			}
	}
	else //if not a regular file, allocate not file size
	{	
		fileSize = initAlloc; 
		charStored = (char*)malloc(sizeof(char)*fileSize); 
	}

	int index = 0;
	int numWord = 0;
	int readBytes = read(0, &charRead, 1); 

	if (readBytes < 0)
	{
		fprintf(stderr, "Error reading file.");
		exit(1); 
	}  

	while(readBytes > 0)
	{	
		if (index == fileSize)
		{
			fileSize *= 2; 
			tempMem = (char*)realloc(charStored,sizeof(char)*fileSize); 
			if (tempMem == NULL)
			{
				fprintf(stderr, "Memory reallocation failure."); 
				free(charStored); 
				exit(1); 
			}
			else 
				if (tempMem != NULL)
				{
					charStored = tempMem; 
				}
		}

		if (charRead != ' ')
		{
			charStored[index++] = charRead; 
		}
		else 
			if (charRead == ' ')
			{
				numWord++; 
				charStored[index++] = charRead; 
			}
	
		readBytes = read(0, &charRead, 1); 
	}

	if (readBytes == 0)
	{
		if (index == fileSize)
		{
				fileSize += 1; 
			tempMem = realloc(charStored,sizeof(char)*fileSize); 
			if (tempMem == NULL)
			{
				fprintf(stderr, "Memory reallocation failure."); 
				free(charStored); 
				exit(1); 
			}
			else 
				if (tempMem != NULL)
				{
					charStored = tempMem; 
				}
		}
		if ((index > 0) && (charStored[index-1] != ' '))
		{
			//printf("ADDING SPACE TO END"); 
			charStored[index] = ' '; 
			numWord++; 
		}
	}

	
	char *ptCharStored = charStored; 
	char** ptToWord = (char**)malloc(sizeof(char*)*numWord); 
	if (ptToWord == NULL)
	{
		fprintf(stderr, "Memory allocation failure."); 
		exit(1); 
	}

	int i; 
	for (i = 0; i < numWord; i++)
	{
		// printf("I am getting words"); 
		ptToWord[i] = ptCharStored; 
		while ((*ptCharStored != ' ') && (i+1 != numWord))
		{
			ptCharStored++; 
		}
		if (*ptCharStored == ' ')
		{
			ptCharStored++; 
		}
	}

	// int w; 
	// for ( w = 0;w < index; w++)
	// {
	// 	printf("print uppercase letter: %c\n", toupper(charStored[w])); 
	// } 

	if (argc == 2)
	{
		if ((argv[1][0] == '-') && (argv[1][1] == 'f'))
		{
			qsort(ptToWord, numWord, sizeof(char*), sortcmpFuncUpper); 
		}
	}
	else
	{
		qsort(ptToWord, numWord, sizeof(char*), sortcmpFunc); 
	}

	/*print out*/
	int j; 
	char *ptrToPrint; 
	for (j = 0; j < numWord; j++)
	{	
		 // printf("printing");
		ptrToPrint = ptToWord[j];
		write(1, ptrToPrint, 1); 
		while (*ptrToPrint != ' ')
		{		
			ptrToPrint++; 
			write(1, ptrToPrint, 1);
			// printf("I HAVE WRITTEN");  
		}
	}

	/*free*/
	free(ptToWord);
	free(charStored); 

	exit(0); 

}