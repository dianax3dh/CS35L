#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>

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

	char cRead[1]; //character array where read content be stored 
	int readBytes = read(0, cRead, 1); 
	int k; 
	int flag = 0; 
	while (readBytes > 0) //transliterating byte 
	{
		for (k = 0; k < strlen(from); k++)
		{
			if (cRead[0] == from[k])
			{		
				flag = 1; 
				cRead[0] = to[k]; 
				write(1, &cRead, 1);
				break;  
			}
			// else 
			// 	if (cRead != from[k])
			// 	{
			// 		write(1, &cRead, 1); 
			// 		break;  
			// 	}
		}
		if (flag == 0)
		{
			write(1, &cRead, 1); 
		}
		flag = 0; 
		readBytes = read(0, cRead, 1); 
	}

	return 0; 
}