#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define DEFAULT_LENGTH 10
#define MAX_WORD_SIZE 100
#define SCREEN_WIDTH 80

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

int* countLetter(size_t*,float*,unsigned int* ,file_t*);//counts letters from a set of files
void countWord(size_t*,float*,word_t** ,file_t*);//counts words from a set of files
word_t* sortWordArray(word_t**,int,size_t*);//sorts an array of word_t
char_t* sortCharArray(unsigned int*,int,float*,int*);//sorts an array of char_t
void printGraph(void*,int,size_t*,float*,int,int);//prints the graph


int main(int argc, char* argv[]){
	unsigned int letterArray[36]={0};
	int *foundOrder=NULL;
	int scaled=0,wordMode=1,lengthFlag=1,indicater=1,wordFlag=0;
	int length=0;
	size_t count=0;
	float countAll=0;
	file_t* files=NULL;
	word_t* wordArray=NULL;
	

    for(; indicater < argc && argv[indicater][0] == '-'; indicater++) {//handing arguments
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
				if(wordFlag){//if [-w] is already used
					printf("[-c] and [-w] cannot use together\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
					return 0;
				}
				break;
			case 'w':
				if(!wordMode){//if [-c] is already used
					printf("[-c] and [-w] cannot use together\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
					return 0;
				}
				wordFlag=1;
				break;
			case 'l':
				if(argv[indicater+1]!=NULL && lengthFlag){				
					for(int i=0;i<strlen(argv[indicater+1]);i++){
						if(isdigit(argv[indicater+1][i])){//converting the string to an integer
							length=length*10 + argv[indicater+1][i]-'0';
							lengthFlag=0;
							
						}else{//if the argument is not a number
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
	
	if(argv[indicater]==NULL){//if no files given 
		printf("Cannot open one or more given files\n");
		return 0;
	}

	if(lengthFlag){//if the length is not changed
		length=DEFAULT_LENGTH;
	}

	for(int i=indicater;argv[i]!=NULL;i++){//openning pointers to the files
			file_t* newFile=(file_t*)malloc(sizeof(file_t));
			newFile->filePointer=fopen(argv[i],"r");
			if(newFile->filePointer==NULL){//if the file couldn't be opened
				printf("Cannot open one or more given files\n");
				return 0;
			}
			newFile->next=files;
			files=newFile;			
	}

	if(wordMode){//operating in word mode
		countWord(&count,&countAll,&wordArray,files);		
		printGraph(sortWordArray((void*)&wordArray,length,&count),length,&count,&countAll,wordMode,scaled);	
	
	}else{		//operating in character mode
		foundOrder=countLetter(&count,&countAll,letterArray,files);	
		printGraph((void*)sortCharArray(letterArray,length,&countAll,foundOrder),length,&count,&countAll,wordMode,scaled);		
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
						
			}else if((c==' ' || c=='\n' || c=='\t' )&& (position !=0) ){//end of a word	
				if(wordArray!=NULL){
					for(word_t* temp=*wordArray; temp!=NULL;temp=temp->next){
						if(!(strcmp(temp->word,word))){				        
							(temp->occurrence)++;	
							inArray=1;
						}
					}
				}
				
				if(!(inArray)){//if the word is new
					word_t* newWord=(word_t*)malloc(sizeof(word_t));
					strcpy(newWord->word,word);
					newWord->occurrence=1;					
					newWord->next=*wordArray;
					*wordArray = newWord;
					(*count)++;
				}	
				inArray=0;
							
				for(int j=0;word[j]!='\0';j++){//cleaning the variable
					word[j]='\0';
				}
				position=0;
				(*countAll)++;
			}			
		}
		if(c==EOF && (position!=0)){//last word of a file
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
	static int foundOrder[36]={0};//order of the characters found

	for(file_t* current=files;current!=NULL;current=current->next){//iterating through the file-array
		while((c=fgetc(current->filePointer))!=EOF){//iterating through a file
			if(isalnum(c)){//pre-processing
				if(isdigit(c)){//if c is a number
					if(letterArray[c-'0'+26]==0){//if the character is new
						foundOrder[c-'0'+26]=*count;
						(*count)++;//increasing the number of unique characters
					}
					letterArray[c-'0'+26]++;//increasing the occurrence
				}else{//if c is a letter
					c=tolower(c);
					if(letterArray[c-'a']==0){//if the charcter is new
						foundOrder[c-'a']=*count;
						(*count)++;//increasing the number of unique characters
					}
					letterArray[c-'a']++;//increasing the occurrence
				}				
				(*countAll)++;//no of all characters
			}
		}
		
	}
	return foundOrder;
}

word_t* sortWordArray(word_t** wordArray,int length,size_t* count){
	if(*count<length){//if requested length is greater than the number of unique words
		length=*count;
	}
	word_t* maxArray=(word_t*)malloc(sizeof(word_t)*length);
	word_t* max=NULL;
	word_t* maxPrev=NULL;

	for(int i=0;i<length;i++){
		max=*wordArray;
		maxPrev=NULL;
		for(word_t* current=*wordArray;current->next!=NULL;current=current->next){
			if(current->next->occurrence >= max->occurrence){//finding the maximum occurrence
				max=current->next;		
				maxPrev=current;				
			}

		}
		if(maxPrev!=NULL){//removing maxx occurrence from the list
			maxPrev->next=max->next;
		}

		maxArray[i].occurrence=max->occurrence;//inserting the max value to maxArray
		strcpy(maxArray[i].word,max->word);
		maxArray[i].next=NULL;

		if(max==*wordArray){//head of the array has maxx occurrence
			*wordArray=(*wordArray)->next;
		}
		free(max);//freeing max occurrence

	}
	return maxArray;
}

char_t* sortCharArray(unsigned int* letterArray,int length,float* countAll,int* foundOrder){

	int max=0;
	
	if(*countAll<length){//if requested length is greater than the number of unique chars
		length=*countAll;
	}

	char_t* maxArray=(char_t*)malloc(sizeof(char_t)*length);

	for(int i=0;i<length;i++){
		for(int j=0;j<36;j++){
			if(letterArray[max]<letterArray[j]){//finding the max
				max = j;
			}
			if(letterArray[max] == letterArray[j]){//if the frequencies match
				if(foundOrder[max]>foundOrder[j]){//first inserted char is enterd first 
					max=j;
				}
			}
		}
		if(max>25){//if its number
			maxArray[i].letter=max+'0'-26;
		}else{//if its a letter
			maxArray[i].letter=max+'a';
		}
		maxArray[i].occurrence=letterArray[max];//inserting the max value to the array
		letterArray[max]=0;//removing the max value
		max=0;		
	}
	return maxArray;
}


void printGraph(void* maxArray,int length,size_t* count,float* countAll,int wordMode,int scaled){
	float frequency,printFrequency;
	float barLength=0;
	int label=5;

	if(*count<length){//if requested length is greater than the number of unique chars
		length=*count;
	}
	unsigned long int maxWordLength=1;

	if(wordMode){
		for(int i=0;i<length;i++){
			if(strlen((*((word_t*)maxArray+i)).word)>maxWordLength){//finding the word with the maximum length
				maxWordLength=strlen((*((word_t*)maxArray+i)).word);
			}
		}
	}

	printf("\n");
	for(int i=0;i<length;i++){//iterating through the max array
		if(wordMode){//finding the frequency according to the mode
			frequency=(float)((*((word_t*)maxArray+i)).occurrence)/(*countAll);
		
		}else{
			frequency=(float)((*((char_t*)maxArray+i)).occurrence)/(*countAll);
		}

		printFrequency=frequency;
		

		if(scaled){//finding the frequency for scaled mode(to be used for the length of the bar)
			if(wordMode){
				frequency /=(float)((*((word_t*)maxArray)).occurrence)/(*countAll);
			}else{
				frequency /=(float)((*((char_t*)maxArray)).occurrence)/(*countAll);
			}	
		}	
		
		if((int)(printFrequency*10) >0 ){//if the frequency is 2 digits
			label=6;
		}
		if(((int)printFrequency)==1){//if the frequency is 3 digits
			label=7;
		}

		barLength=(int)(frequency*(SCREEN_WIDTH-3-label-maxWordLength));
	

		/*             ONE WORD            */
		for(int j=0;j<(maxWordLength+2);j++){
			printf(" ");
		}
		printf("\u2502");
		for(int j=0;j<barLength;j++){//first line of the bar
			printf("\u2591");
		}


		printf("\n");
		if(wordMode){
			printf(" %s",(*((word_t*)maxArray+i)).word);//printing the word
			for(int j=0;j<(maxWordLength-strlen((*((word_t*)maxArray+i)).word)+1);j++){//alligning with the longest word
				printf(" ");
			}
		}else{
			printf(" %c ",(*((char_t*)maxArray+i)).letter);//printing the character
		}		
		printf("\u2502");
		for(int j=0;j<barLength;j++){//second line of the bar
			printf("\u2591");
		}
		printf("%.2f%%",printFrequency*100);//printing the frequency
		printf("\n");



		for(int j=0;j<(maxWordLength+2);j++){
			printf(" ");
		}
		printf("\u2502");
		for(int j=0;j<barLength;j++){//third line of the bar
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
	printf("\u2514");//"L" shape

	for(int i=0;i<(SCREEN_WIDTH-maxWordLength-3);i++){//line at the end
		printf("\u2500");
	}
	printf("\n");

}
