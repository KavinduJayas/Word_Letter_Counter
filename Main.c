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

typedef struct ___{
	FILE* filePointer;
	struct ___ *next;
}file_t;


int printUsage(char*);
int fileSize(FILE*);
char* preProcessor(char*);
letterFrequency_t countLetter(char*,char);
wordFrequency_t countWord(char*,char*);
void printWordGraph(wordFrequency_t*);
void printLetterGraph(letterFrequency_t*);

int main(int argc, char* argv[]){
	char *allString =NULL,*words,*letters;	
	int scaled=0,word=1,length=10,lengthFlag=1,totalFileSize=0;
	file_t* files=NULL;
	wordFrequency_t* wordFrequency = (wordFrequency_t*)malloc(sizeof(wordFrequency_t)*10);
	letterFrequency_t* letterFrequency = (letterFrequency_t*)malloc(sizeof(letterFrequency_t)*10);

    int indicater=1;
    for (indicater = 1; indicater < argc && argv[indicater][0] == '-'; indicater++) {
        switch (argv[indicater][1]) {
			case '-': 
				if(strcmp(argv[indicater],"--scaled")==0){
					scaled=1;
				}else{
					return printUsage(argv[0]);
				}
				break;
			case 'c':word=0;break;
			case 'w':
				if(word=0){
					return printUsage(argv[0]);
				}
				break;
			case 'l':
				if(argv[indicater+1]!=NULL && lengthFlag){
					if(strlen(argv[indicater+1]>2)){
						return printUsage(argv[0]);
					}else if(strlen(argv[indicater+1])==2){
						if(isdigit(argv[indicater+1][0]) && isdigit(argv[indicater+1][1])){
							if(strtoi(argv[indicater+1]>=0 && strtoi(argv[indicater+1]<=10))){
								length = strtoi(argv[indicater+1]);
								lengthFlag =0;
							}else{
								return printUsage(argv[0]);
							}
						}else{
							return printUsage(argv[0]);
						}
					}else if(isdigit(argv[indicater+1][0])){
							if(strtoi(argv[indicater+1]>=0 && strtoi(argv[indicater+1]<=10))){
								length = strtoi(argv[indicater+1]);
								lengthFlag=0;
							}else{
								return printUsage(argv[0]);
							}
					}else{
						return printUsage(argv[0]);
					}
				}else{
					return printUsage(argv[0]);
				}
			default:
				return printUsage(argv[0]);
        }   
    }   

	if(argv[indicater]=NULL){//no files given 
		return printUsage(argv[0]);
	}

	for(int i=indicater;argv[indicater]!=NULL;i++){
			file_t* newFile=(file_t*)malloc(sizeof(file_t));
			newFile->filePointer=fopen(argv[indicater],'r');
			newFile->next=files;
			files=newFile;
			totalFileSize+=fileSize(newFile);
	}
	if(word){
		allString = (char*)malloc(fileSize(totalFileSize));

		allString = preProcessor(allString);
		for(int i=0;i<10;i++){
			wordFrequency[i]=countWord(allString,frequentWords[i]);
		}
	}
	
	
	

	for(int i=0;i<10;i++){
		letterFrequency[i]=countLetter(allString,frequentLetters[i]);
	}
	
	printWordGraph(wordFrequency);
	printLetterGraph(letterFrequency);

	return 0;
}

int printUsage(char* fileName){
	printf("Usage: %s [-ilw] [file...]\n", fileName);
	return 0;
}


int fileSize(FILE* fp){
	fseek(fp, 0, SEEK_END);
	return ftell(fp);    
}
