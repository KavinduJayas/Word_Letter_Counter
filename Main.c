#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
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
letterFrequency_t countLetter(int ,letterFrequency_t* ,file_t*);
wordFrequency_t countWord(int ,wordFrequency_t* ,file_t*);
void sortWord(wordFrequency_t*);
void sortLetter(letterFrequency_t*);
void printWordGraph(wordFrequency_t*);
void printLetterGraph(letterFrequency_t*);

int main(int argc, char* argv[]){
	int scaled=0,wordMode=1,length=10,lengthFlag=1,totalFileSize=0;
	file_t* files=NULL;
	wordFrequency_t* wordFrequency=NULL;
	letterFrequency_t* letterFrequency=NULL;

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
			case 'c':wordMode=0;break;
			case 'w':
				if(wordMode=0){
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

	for(int i=indicater;argv[indicater]!=NULL;i++){//openning pointers to the files and recording the collective  file size
			file_t* newFile=(file_t*)malloc(sizeof(file_t));
			newFile->filePointer=fopen(argv[indicater],'r');
			newFile->next=files;
			files=newFile;
			totalFileSize+=fileSize(newFile);
			
	}

	if(wordMode){
		countWord(totalFileSize,wordFrequency,files);
		sortWord(wordFrequency);
	}else{
		countLetter(totalFileSize,letterFrequency,files);
		sortLetter(letterFrequency);
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
	fseek(fp, 0, SEEK_SET);    
}

wordFrequency_t countWord(int totalFileSize,wordFrequency_t* wordFrequency,file_t* files){
	char c;
	wordFrequency=(wordFrequency_t*)malloc(totalFileSize);
	for(file_t* current=files;current->next!=NULL;current=current->next){
		while ((c=fgetc(current->filePointer))!=EOF){
			if(isalnum(c) || c==' '){//pre-processing
				
			}
		}
		
	}
}