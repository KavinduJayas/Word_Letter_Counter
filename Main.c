#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define DEFAULT_LENGTH 10
#define MAX_WORD_SIZE 100

typedef struct _{
	char word[MAX_WORD_SIZE];
	unsigned int occurrence;
	struct _ *next;
}word_t;

typedef struct ___{
	FILE* filePointer;
	struct ___ *next;
}file_t;

int printUsage(char*);
int* countLetter(size_t*,unsigned int* ,file_t*);
void countWord(size_t*,size_t*,word_t** ,file_t*);
word_t* sortWordArray(word_t**,int,size_t*);
unsigned int* sortLetterArray(unsigned int*,int,size_t*,int*);
void printWordGraph(word_t**,int,size_t*);
void printLetterGraph(unsigned int*,int,size_t*);

int main(int argc, char* argv[]){
	unsigned int letterArray[36]={0};
	int *foundOrder=NULL;
	int scaled=0,wordMode=1,lengthFlag=1,indicater=1;
	int length=DEFAULT_LENGTH;
	size_t count=0;
	size_t countAll=0;
	file_t* files=NULL;
	word_t* wordArray=NULL;
	

    for(; indicater < argc && argv[indicater][0] == '-'; indicater++) {
        switch (argv[indicater][1]) {
			case '-': 
				if(!strcmp(argv[indicater],"--scaled")){
					scaled=1;
				}else{
					return printUsage(argv[0]);
				}
				break;
			case 'c':wordMode=0;break;
			case 'w':
				if(!wordMode){
					return printUsage(argv[0]);
				}
				break;
			case 'l':
				if(argv[indicater+1]!=NULL && lengthFlag){
					if(strlen(argv[indicater+1])>2){						
						return printUsage(argv[0]);
					}else if(strlen(argv[indicater+1])==2){			
						if(strcmp(argv[indicater+1],"10")){
							return printUsage(argv[0]);
						}
						indicater++;
					}else if(isdigit(argv[indicater+1][0])){						
							if((argv[indicater+1][0]-'0')>0 && (argv[indicater+1][0]-'0')<10){								
								length = (long int)(argv[indicater+1][0]-'0');						
								lengthFlag=0;								
								indicater++;								
							}else{
								return printUsage(argv[0]);
							}
					}else{
						return printUsage(argv[0]);
					}
				}else{
					return printUsage(argv[0]);
				}
				break;
			default:
				return printUsage(argv[0]);
        }   
    }   
	
	if(argv[indicater]==NULL){//no files given 
		return printUsage(argv[0]);
	}

	printf("\nWordMode=%i length=%i scaled=%i \n",wordMode,length,scaled);

	for(int i=indicater;argv[i]!=NULL;i++){//openning pointers to the files
			file_t* newFile=(file_t*)malloc(sizeof(file_t));
			newFile->filePointer=fopen(argv[i],"r");
			if(newFile->filePointer==NULL){
				return printUsage(argv[0]);
			}
			newFile->next=files;
			files=newFile;
			printf("\nfiles added at %p \n",files->filePointer);
	}

	if(wordMode){
		word_t* maxArray=NULL;
		countWord(&count,&countAll,&wordArray,files);
		printf("\ncount =%zu\n",count);
		printf("\nword insertion Done\n");
		maxArray=sortWordArray(&wordArray,length,&count);
		printWordGraph(&maxArray,length,&count);	
		printf("\nGraph Printed\n");
	}else{	
		unsigned int* maxArray=NULL;	
		foundOrder=countLetter(&countAll,letterArray,files);
		printf("\nletter insertion Done\n");	
		printf("\nchars sorted \n");
		sortLetterArray(letterArray,length,&countAll,foundOrder);
		printLetterGraph(maxArray,length,&countAll);		
	}
	return 0;
}

void countWord(size_t* count,size_t* countAll,word_t** wordArray,file_t* files){
	
	char c,*word=(char*)malloc(MAX_WORD_SIZE);	
	int position=0,inArray = 0;
	strcpy(word,"");

	for(file_t* current = files;current!=NULL;current=current->next){//iterating through the file-array
		while((c=fgetc(current->filePointer))!=EOF){//iterating through a file			
			c=tolower(c);
			if(isalnum(c)){//pre-processing	
				word[position]=c;	
				position++;
						
			}else if(c==' ' && position ){		
				if(wordArray!=NULL){
					for(word_t* temp=*wordArray; temp!=NULL;temp=temp->next){
						if(!(strcmp(temp->word,word))){				        
							(temp->occurrence)++;	
							inArray=1;
						}
					}
				}
				
				if(!(inArray)){
					word_t* newWord=(word_t*)malloc(sizeof(word_t));
					strcpy(newWord->word,word);
					newWord->occurrence=1;					
					newWord->next=*wordArray;
					*wordArray = newWord;
					(*count)++;
				}	
				inArray=0;
							
				for(int j=0;word[j]!='\0';j++){
					word[j]='\0';
				}
				position=0;
				(*countAll)++;
			}			
		}
		
	}
	
}

int* countLetter(size_t* countAll,unsigned int* letterArray,file_t* files){

	char c;	
	int count=1;	
	static int foundOrder[36]={0}; 

	for(file_t* current=files;current!=NULL;current=current->next){//iterating through the file-array
		while((c=fgetc(current->filePointer))!=EOF){//iterating through a file
			if(isalnum(c)){//pre-processing
				if(isdigit(c)){
					if(letterArray[c-'0'+26]==0){
						letterArray[c-'0'+26]=count;
						count++;
					}
					letterArray[c-'0'+26]++;
				}else{
					c=tolower(c);
					if(letterArray[c-'a']==0){
						letterArray[c-'a']=count;
						count++;
					}
					letterArray[c-'a']++;
				}				
				(*countAll)++;
			}
		}
		
	}
	return foundOrder;
}

int printUsage(char* fileName){
	printf("Usage: %s [-ilw] [file...]\n", fileName);
	return 0;
}

word_t* sortWordArray(word_t** wordArray,int length,size_t* count){
	if(*count<length){
		length=*count;
	}
	word_t* maxArray=(word_t*)malloc(sizeof(word_t)*length);
	word_t* max=NULL;
	word_t* maxPrev=NULL;
	for(int i=0;i<length;i++){
		max=*wordArray;
		maxPrev=NULL;
		for(word_t* current=*wordArray;current->next!=NULL;current=current->next){
			if(current->next->occurrence >= max->occurrence){
				max=current->next;		
				maxPrev=current;				
			}

		}
		if(maxPrev!=NULL){
			maxPrev->next=max->next;
		}

		maxArray[i].occurrence=max->occurrence;
		strcpy(maxArray[i].word,max->word);
		maxArray[i].next=NULL;

		if(max==*wordArray){
			*wordArray=(*wordArray)->next;
		}
		free(max);

	}	
	return maxArray;
}

unsigned int* sortLetterArray(unsigned int* letterArray,int length,size_t* countAll,int* foundOrder){

	int max=0;
	
	if(*countAll<length){
		length=*countAll;
	}

	unsigned int* maxArray=(unsigned int*)malloc(sizeof(unsigned int)*length);

	for(int i=0;i<length;i++){
		for(int j=0;j<36;j++){
			if(letterArray[max]<letterArray[j]){
				max = j;
			}
			if(letterArray[max] == letterArray[j]){
				if(foundOrder[max]>foundOrder[j]){
					max=j;
				}
			}
			maxArray[i]=letterArray[max];
			letterArray[max]=0;

		}
	}
	return maxArray;
}


void printWordGraph(word_t** maxArray,int length,size_t* count){
	if(*count<length){
		length=*count;
	}
	word_t* current = *maxArray;
	for(int i=0;i<length;i++){
		printf("  Word = %s , occurrence = %i \n",current->word,current->occurrence);
		current=current->next;
	}
}

void printLetterGraph(unsigned int* maxArray,int length,size_t* countAll){
	for(int i=0;i<length;i++){
		printf("occurrence = %i \n",maxArray[i]);
	}
}
