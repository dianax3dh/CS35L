#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

int main(int argc, char **argv)
{
	if (argc != 3) //first arg: tr, second arg: FROM, third arg: TO
	{
		fprintf(stderr, "Please enter 3 arguments." );
		exit (1); 
	}
	const char* from = argv[1]; //getting 1st & 2nd args
	const char* to = argv[2]; 

	if (strlen(from) != strlen(to)) //checking length
	{
		fprintf(stderr, "String lengths do not match.");
		exit(1); 
	}

	int i; 
	int j; 
	for (i = 0; i < strlen(from); i++) //checking for dups
	{
		for (j = i + 1; j < strlen(from); j++)
		{
			if (from[i] == from[j])
			{
				fprintf(stderr, "Cannot have duplicate." );
				exit(1); 
			}
		}
	}

	int c  = getchar(); //getting the input from stdin
	int k; 
	int flag = 0; 
	while (c != EOF) //transliterating byte 
	{
		for (k = 0; k < strlen(from); k++)
		{
			if (c == from[k])
			{
				flag = 1;
				putchar(to[k]); 
				break; 
			}
			// else 
			// 	if (c != from[k])
			// 	{
			// 		putchar(c);
			// 		//break;  
			// 	}
		}
		if (flag == 0)
		{
			putchar(c); 
		}		
		flag = 0; //reset flag after it has been set to 1
		c = getchar(); 
	}

	return 0; 
}