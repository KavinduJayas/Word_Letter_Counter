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

FILE* openFile(char*);
int fileSize(FILE*);
char* preProcessor(char*);
letterFrequency_t countLetter(char*,char);
wordFrequency_t countWord(char*,char*);
void printWordGraph(wordFrequency_t*);
void printLetterGraph(letterFrequency_t*);

int main(int argc, char* argv[]){
	char* allString =NULL;	
	FILE* fp=NULL;
	wordFrequency_t* wordFrequency = (wordFrequency_t*)malloc(sizeof(wordFrequency_t)*10);
	letterFrequency_t* letterFrequency = (letterFrequency_t*)malloc(sizeof(letterFrequency_t)*10);

	fp = openFile(argv[3]);

	allString = (char*)malloc(fileSize(fp));
	allString = preProcessor(allString);
	
	wordFrequency[i]=
	printWordGraph(wordFrequency);
	printLetterGraph(letterFrequency);

	return 0;
}

FILE* openFile(char* file){
	FILE *fp;

	fp = fopen(file, "r"); // read mode

	if (fp == NULL)
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}

} 

int fileSize(FILE* fp){
	fseek(fp, 0, SEEK_END);
	return ftell(fp);    
}
