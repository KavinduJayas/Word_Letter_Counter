#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define DEFAULT_LENGTH 10
#define MAX_WORD_SIZE 100

typedef struct _{
	char word[MAX_WORD_SIZE];
	unsigned long int occurrence;
	struct _ *next;
}word_t;

typedef struct __{
	char letter;
	unsigned long int occurrence;
}char_t;

typedef struct ___{
	FILE* filePointer;
	struct ___ *next;
}file_t;

int printUsage(char*);
int* countLetter(size_t*,float*,unsigned int* ,file_t*);
void countWord(size_t*,float*,word_t** ,file_t*);
word_t* sortWordArray(word_t**,int,size_t*);
char_t* sortLetterArray(unsigned int*,int,float*,int*);
void printGraph(void*,int,size_t*,float*,int,int);


int main(int argc, char* argv[]){
	unsigned int letterArray[36]={0};
	int *foundOrder=NULL;
	int scaled=0,wordMode=1,lengthFlag=1,indicater=1,wordFlag=0;
	int length=0;
	size_t count=0;
	float countAll=0;
	file_t* files=NULL;
	word_t* wordArray=NULL;
	

    for(; indicater < argc && argv[indicater][0] == '-'; indicater++) {
        switch (argv[indicater][1]) {
			case '-': 
				if(!strcmp(argv[indicater],"--scaled")){
					scaled=1;
				}else{
					printf("Invalid option [%s]\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[indicater],argv[0]);
					return 0;
				}
				break;
			case 'c':
				wordMode=0;
				if(wordFlag){
					printf("[-c] and [-w] cannot use together\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
					return 0;
				}
				break;
			case 'w':
				if(!wordMode){
					printf("[-c] and [-w] cannot use together\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
					return 0;
				}
				wordFlag=1;
				break;
			case 'l':
				if(argv[indicater+1]!=NULL && lengthFlag){
					/*
					if(strlen(argv[indicater+1])>2){	
						printf("Invalid options for [-l]\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);				
						return 0;
					}else if(strlen(argv[indicater+1])==2){			
						if(strcmp(argv[indicater+1],"10")){
						printf("Invalid options for [-l]\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);				
						return 0;
						}
						indicater++;
					}else if(isdigit(argv[indicater+1][0])){						
							if((argv[indicater+1][0]-'0')>0 && (argv[indicater+1][0]-'0')<10){								
								length = (long int)(argv[indicater+1][0]-'0');						
								lengthFlag=0;								
								indicater++;								
							}else{
								printf("Invalid options for [-l]\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);				
								return 0;
							}
					}else{
						printf("Invalid options for [-l]\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);				
						return 0;
					}*/
					for(int i=0;i<strlen(argv[indicater+1]);i++){
						if(isdigit(argv[indicater+1][i])){
							length=length*10 + argv[indicater+1][i]-'0';
							lengthFlag=0;
							
						}else{
							printf("Invalid options for [-l]\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);		
							return 0;
						}
    				}
					indicater++;
				}else{
					printf("Invalid options for [-l]\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);				
					return 0;
				}

				break;
			default:
				printf("Invalid option [%s]\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[indicater],argv[0]);
				return 0;
        }   
    }   
	
	if(argv[indicater]==NULL){//no files given 
		printf("Cannot open one or more given files\n");
		return 0;
	}

	if(lengthFlag){
		length=DEFAULT_LENGTH;
	}

	//printf("\nWordMode=%i length=%i scaled=%i \n",wordMode,length,scaled);

	for(int i=indicater;argv[i]!=NULL;i++){//openning pointers to the files
			file_t* newFile=(file_t*)malloc(sizeof(file_t));
			newFile->filePointer=fopen(argv[i],"r");
			if(newFile->filePointer==NULL){
				printf("Cannot open one or more given files\n");
				return 0;
			}
			newFile->next=files;
			files=newFile;			
	}

	if(wordMode){
		countWord(&count,&countAll,&wordArray,files);		
		printGraph(sortWordArray((void*)&wordArray,length,&count),length,&count,&countAll,wordMode,scaled);	
	
	}else{		
		foundOrder=countLetter(&count,&countAll,letterArray,files);	
		printGraph((void*)sortLetterArray(letterArray,length,&countAll,foundOrder),length,&count,&countAll,wordMode,scaled);		
	}
	return 0;
}

void countWord(size_t* count,float* countAll,word_t** wordArray,file_t* files){
	
	char c,*word=(char*)malloc(MAX_WORD_SIZE);	
	int position=0,inArray = 0;
	strcpy(word,"");

	for(file_t* current = files;current!=NULL;current=current->next){//iterating through the file-array
		while((c=fgetc(current->filePointer))!=EOF){//iterating through a file			
			c=tolower(c);
			if(isalnum(c)){//pre-processing	
				word[position]=c;	
				position++;
						
			}else if((c==' ' || c=='\n' )&& (position !=0) ){		
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
		if(c==EOF && (position!=0)){
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

int* countLetter(size_t* count,float* countAll,unsigned int* letterArray,file_t* files){

	char c;		
	static int foundOrder[36]={0}; 

	for(file_t* current=files;current!=NULL;current=current->next){//iterating through the file-array
		while((c=fgetc(current->filePointer))!=EOF){//iterating through a file
			if(isalnum(c)){//pre-processing
				if(isdigit(c)){
					if(letterArray[c-'0'+26]==0){
						foundOrder[c-'0'+26]=*count;
						(*count)++;
					}
					letterArray[c-'0'+26]++;
				}else{
					c=tolower(c);
					if(letterArray[c-'a']==0){
						foundOrder[c-'a']=*count;
						(*count)++;
					}
					letterArray[c-'a']++;
				}				
				(*countAll)++;
			}
		}
		
	}
	return foundOrder;
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

char_t* sortLetterArray(unsigned int* letterArray,int length,float* countAll,int* foundOrder){

	int max=0;
	
	if(*countAll<length){
		length=*countAll;
	}

	char_t* maxArray=(char_t*)malloc(sizeof(char_t)*length);

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
		}
		if(max>25){
			maxArray[i].letter=max+'0'-26;
		}else{
			maxArray[i].letter=max+'a';
		}
		maxArray[i].occurrence=letterArray[max];
		letterArray[max]=0;
		max=0;		
	}
	return maxArray;
}


void printGraph(void* maxArray,int length,size_t* count,float* countAll,int wordMode,int scaled){
	float frequency,printFrequency;
	int barLength=0;
	if(*count<length){
		length=*count;
	}
	unsigned long int maxWordLength=1;

	if(wordMode){
		for(int i=0;i<length;i++){
			if(strlen((*((word_t*)maxArray+i)).word)>maxWordLength){
				maxWordLength=strlen((*((word_t*)maxArray+i)).word);
			}
		}
	}

	printf("\n");
	for(int i=0;i<length;i++){
		if(wordMode){
			frequency=(float)((*((word_t*)maxArray+i)).occurrence)/(*countAll);
		
		}else{
			frequency=(float)((*((char_t*)maxArray+i)).occurrence)/(*countAll);
			/*if((long int)((*((char_t*)maxArray+i)).occurrence)%(long int)(*countAll)>(*countAll)/2){
				frequency++;
			}*/
		}
		/*if((frequency-(int)frequency)>=0.5){
			frequency++;
		}*/
		
		/*             ONE WORD            */
		printFrequency=frequency;
		

		if(scaled){
			if(wordMode){
				frequency /=(float)((*((word_t*)maxArray)).occurrence)/(*countAll);
			}else{
				frequency /=(float)((*((char_t*)maxArray)).occurrence)/(*countAll);
			}	
		}	
		/*}else{
			printFrequency = frequency;
		}
		*/
		barLength =(int)(frequency*(71-maxWordLength));
		
		for(int j=0;j<(maxWordLength+2);j++){
			printf(" ");
		}
		printf("\u2502");
		for(int j=0;j<barLength;j++){
			printf("\u2591");
		}

		printf("\n");
		if(wordMode){
			printf(" %s",(*((word_t*)maxArray+i)).word);
			for(int j=0;j<(maxWordLength-strlen((*((word_t*)maxArray+i)).word)+1);j++){
			printf(" ");
			}
		}else{
			printf(" %c ",(*((char_t*)maxArray+i)).letter);
		}
		
		printf("\u2502");
		for(int j=0;j<barLength;j++){
			printf("\u2591");
		}
		printf("%.2f%%",printFrequency*100);
		printf("\n");



		for(int j=0;j<(maxWordLength+2);j++){
			printf(" ");
		}
		printf("\u2502");
		for(int j=0;j<barLength;j++){
			printf("\u2591");
		}
		printf("\n");	
		for(int j=0;j<(maxWordLength+2);j++){
			printf(" ");
		}
		printf("\u2502\n");

	}

	/*      DEFAULT LINE        */

	for(int i=0;i<(maxWordLength+2);i++){
		printf(" ");
	}	
	printf("\u2514");

	for(int i=0;i<(77-maxWordLength);i++){
		printf("\u2500");
	}
	printf("\n");

}
