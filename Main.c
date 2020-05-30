#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _{
	char word[50];
	float frequency;
}wordFrequency_t;

typedef struct __{
	char letter;
	float frequency;
}letterFrequency_t;

FILE* readFile(char*);
float countLetter(char);
float countWord(char*);
void printWordGraph(wordFrequency_t*);
void printWordGraph(letterFrequency_t*);

int main(){


	return 0;
}

FILE* readFile(char* file){
	FILE *fp;

	fp = fopen(file, "r"); // read mode

	if (fp == NULL)
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}

} 

