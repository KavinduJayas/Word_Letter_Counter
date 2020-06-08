#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define DEFAULT_LENGTH 10
#define MAX_WORD_SIZE 50

typedef struct _{
	char word[MAX_WORD_SIZE];
	int frequency;
}wordFrequency_t;

typedef struct __{
	char letter;
	int frequency;
}letterFrequency_t;

typedef struct ___{
	FILE* filePointer;
	struct ___ *next;
}file_t;

int compareChar(const void*,const void*);
int compareWord(const void*,const void*);
int printUsage(char*);
int fileSize(FILE*);
void countLetter(size_t*,letterFrequency_t* ,file_t*);
void countWord(size_t*,wordFrequency_t* ,file_t*);
void sortWord(wordFrequency_t*);
void printWordGraph(wordFrequency_t*,long int,size_t*);
void printLetterGraph(letterFrequency_t*,long int,size_t*);

int main(int argc, char* argv[]){
	int scaled=0,wordMode=1,lengthFlag=1,totalFileSize=0,indicater=1;
	long int length=DEFAULT_LENGTH;
	size_t *count =(size_t*)malloc(sizeof(long int));
	*count=0;
	file_t* files=NULL;
	wordFrequency_t* wordArray=NULL;
	letterFrequency_t* letterArray=NULL;	

    for(indicater = 1; indicater < argc && argv[indicater][0] == '-'; indicater++) {
		printf("\nHandling Arguments\n");
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
				if(wordMode){
					return printUsage(argv[0]);
				}
				break;
			case 'l':
				if(argv[indicater+1]!=NULL && lengthFlag){
					if(strlen(argv[indicater+1])>2){
						return printUsage(argv[0]);
					}else if(strlen(argv[indicater+1])==2){
						if(isdigit(argv[indicater+1][0]) && isdigit(argv[indicater+1][1])){
							if(atoi(argv[indicater+1])>=0 && atoi(argv[indicater+1])<=10){
								length = atoi(argv[indicater+1]);
								lengthFlag =0;
							}else{
								return printUsage(argv[0]);
							}
						}else{
							return printUsage(argv[0]);
						}
					}else if(isdigit(argv[indicater+1][0])){
							if(atoi(argv[indicater+1])>=0 && atoi(argv[indicater+1])<=10){
								length = atoi(argv[indicater+1]);
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

	if(argv[indicater]==NULL){//no files given 
		return printUsage(argv[0]);
	}

	printf("\nArgument handling Done\n");

	printf("\nWordMode=%i length=%li scaled=%i \n",wordMode,length,scaled);

	printf("\n%i %s\n",indicater,argv[indicater]);

	for(int i=indicater;argv[i]!=NULL;i++){//openning pointers to the files and recording the collective  file size
			file_t* newFile=(file_t*)malloc(sizeof(file_t));
			newFile->filePointer=fopen(argv[i],"r");
			newFile->next=files;
			files=newFile;
			printf("\nfiles added at %x \n",files->filePointer);
			totalFileSize+=fileSize(newFile->filePointer);
			fseek(newFile->filePointer, 0, SEEK_SET);
			printf("\nTotal size = %i \n",totalFileSize);
	}

	if(wordMode){
		wordArray=(wordFrequency_t*)malloc(totalFileSize);
		countWord(count,wordArray,files);
		printf("\nword insertion Done\n");
		qsort(wordArray,*count,sizeof(wordFrequency_t),compareWord);
		printf("\nwords sorted\ncount=%lu \n",*count);
		for(int i=0;i<*count;i++){
		printf("  Word = %s , Frequency = %i \n",wordArray[i].word,wordArray[i].frequency);
		}
		printWordGraph(wordArray,length,count);
		printf("\nGraph Printed\n");
	}else{
		letterArray=(letterFrequency_t*)malloc(totalFileSize);
		countLetter(count,letterArray,files);
		printf("\nletter insertion Done\n");
		qsort(letterArray,*count,sizeof(letterFrequency_t),compareChar);
		printf("\nchars sorted \n");
		printLetterGraph(letterArray,length,count);
		
	}
	return 0;
}

int compareChar(const void* x,const void* y){
	int l= ((letterFrequency_t*)x)->frequency;
	int r= ((letterFrequency_t*)y)->frequency;

	if(l==r){
		return ((letterFrequency_t*)y)->letter-((letterFrequency_t*)x)->letter;
	}else{
		return r-l;
	}
}

int compareWord(const void* x,const void* y){
	int l= ((wordFrequency_t*)x)->frequency;
	int r= ((wordFrequency_t*)y)->frequency;

	if(l==r){
		return strcmp(((wordFrequency_t*)y)->word,((wordFrequency_t*)x)->word);
	}else{
		return r-l;
	}
}

int printUsage(char* fileName){
	printf("Usage: %s [-ilw] [file...]\n", fileName);
	return 0;
}


int fileSize(FILE* fp){
	fseek(fp, 0, SEEK_END);
	return ftell(fp);
	    
}

void countWord(size_t* count,wordFrequency_t* wordArray,file_t* files){
	char c,*word=(char*)malloc(MAX_WORD_SIZE);	
	int position=0,isInArray=0;
	strcpy(word,"");
	file_t* current = files;

	for(;current->next!=NULL;current=current->next){//iterating through the file-array
		while((c=fgetc(current->filePointer))!=EOF){//iterating through a file
			//printf("\nfiles reading at %x \n",current->filePointer);
			//printf("%c", c);
			if(isalnum(c)){//pre-processing
				word[position]=c;				
			}else if(c==' ' && position){
				for(int i=0;i<*count;i++){
					if(strcmp(wordArray[*count].word,word)){
						wordArray[*count].frequency++;
						isInArray=1;
					}
				}
				printf("\nword=%s \n",word);
				if(!isInArray){
					strcpy(wordArray[*count].word,word);
					(*count)++;
				}	
				isInArray=0;			
				strcpy(word,"");
				position=0;
			}			
		}
		
	}
}

void countLetter(size_t* count,letterFrequency_t* letterArray,file_t* files){
	char c;	
	int isInArray=0;
	for(file_t* current=files;current->next!=NULL;current=current->next){//iterating through the file-array
		while((c=fgetc(current->filePointer))!=EOF){//iterating through a file
			if(isalnum(c)){//pre-processing
				for(int i=0;i<*count;i++){
					if(letterArray[i].letter==c){//if the letter has already been entered 
						letterArray[i].frequency+=1;
						isInArray=1;
					}
				}
				if(!isInArray){//new letter
					letterArray[*count].letter=c;		
					letterArray[*count].frequency=0;
					(*count)++;				
				}
				isInArray=0;			
				
			}
		}
		
	}
}

void printWordGraph(wordFrequency_t *wordArray,long int length,size_t* count){
	if(*count<length){
		length=*count;
	}
	for(int i=0;i<length;i++){
		printf("  Word = %s , Frequency = %i \n",wordArray[i].word,wordArray[i].frequency);
	}
}

void printLetterGraph(letterFrequency_t *letterArray,long int length,size_t* count){
	if(*count<length){
		length=*count;
	}
	for(int i=0;i<length;i++){
		printf("  Letter = %c , Frequency = %i \n",letterArray[i].letter,letterArray[i].frequency);
	}
}
