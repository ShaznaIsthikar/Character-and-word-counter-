//libraries
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

//defining a constant
#define MAXC 1024u

//data type to create linked list
typedef struct list {
    char word_t[70];
    int count;
    struct list *next;
}list_t;

//prototypes of user defined functions
void freelist (list_t *node);
void printoccur(list_t *node,float totalcount);
int search(list_t * head, char *word);
list_t * addword (list_t **head, char *word);
int count(list_t * node1, list_t * node2);
void printlist (list_t *node);
void printbar(list_t * head,int nobars,float totalcount,int scaled,int chwd);
void sortedInsert(list_t** head_ref, list_t* new_node);
list_t* newNode(list_t * node);
list_t * sortdescend(list_t *head);
char * lower(char *str);
void checkAlnum(char *ch);



int main (int argc, char ** argv){
	
	//basic functionality handling variables

	char word[MAXC];	//Initializing a char array to store words(characters) taken from the file
	FILE *inputfpr;	//initializing a list_t pointer type variable to the file
	list_t *head1= NULL;	//head node of the linked list containing all the words (or characters)
	list_t *head2= NULL;	//head node containing the linked list containing all the distinct words (or characters)
	list_t *head3= NULL;	//head node containing the linked list in sorted in descending order
	char cha;	//initializing a char variable
	char *ch;	//initializing a pointer char variable
	float totalcount=0;	//counting all the words in the file
	int check;	//variable to check whether the character is alphanumeric
	int isUnique;	//variable to know the input word is distint or not(1 if distinct else 0)
	int discount;	//variable to count the distinct words
	
	//argument handling variables
	
	int scaled=0;	//not scaled(default)
	int chwd=2;	//word frequency(default)
	int noinput=0;	//counting the non tag inputs
	int bars=10;	//no of bars deault
	int l=0;	//counting the nontags after l
	
	
	//checking for l arguments
	
	for(int i=1;i<argc;i++){
		if (strcmp(argv[i],"-l")==0){
			//if there is nothing after -l arg
			if (i==(argc-1)){
				printf("Not enough options for [-l]\n");
				printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
				exit(0);
			
			//checking whether the next word is a number or not	
			}else{	
				//checking for negative numbers
				if (strncmp(argv[i+1],"-",1)==0){
					for (int j=1; argv[i+1][j]; j++) {
						if (!isdigit(argv[i+1][j])) {
							//if there are non digit chracters after (-)
               					printf("Invalid options for [-l]\n");
							printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
							exit(0);	
               				}
            				}
            				//there are no non digit characters which means the integer after -l is negative
            				printf("Invalid option(negative) for [-l]\n");
					printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
					exit(0);
				}
				//checking whether the next one is a number or not
				for (int j=0; argv[i+1][j]; j++) {
					if (!isdigit(argv[i+1][j])) {
               				printf("Invalid options for [-l]\n");
						printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
						exit(0);	
               			}
            			}
            			//as the next number is a non negative integer converting it the string into an integer
              			bars=atoi(argv[i+1]);
              			
              			//when the input exceeds the int range they convert into negatives in those cases
            			if (bars<0){
            				printf("Invalid option(negative) for [-l]\n");
					printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
					exit(0);
            			}
            			
            			l++;	//to increment l by 1 to indicate that this non tag input is not an input file
			}	
		}
	}
	
	//when number of bars equals to 0 exiting the program
	if(bars==0){
		exit(0);
	}
	
	
	//checking for invalid tags
	
	for(int i=1;i<argc;i++){
		if (strncmp(argv[i],"-",1)==0){
			if((strcmp(argv[i],"-c")!=0)&&(strcmp(argv[i],"-w")!=0)&&(strcmp(argv[i],"-l")!=0)&&(strcmp(argv[i],"--scaled")!=0)){
				printf("Invalid option [%s]\n",argv[i]);
				printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
				exit(0);	
			}
		}
	}
	
	//exiting the program if -c and -w are both used
	 
	for (int i=0;i<argc;i++){
		for (int j=0;j<argc;j++){
			if ((strcmp(argv[i],"-c")==0)&&(strcmp(argv[j],"-w")==0)){
				printf("[-c] and [-w] cannot use together\n");
				printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
				exit(0);
			}
	
		}
	}
	
	
	//exiting the program if no input files are given
	
	for(int i=1;i<argc;i++){
		if (strncmp(argv[i],"-",1)!=0){
			noinput++;	//increasing the non tag inputs by 1
		
		}
	}

	if ((noinput==l)||(noinput==0)){	//if the number of non tag inputs are equal to the number of non tag inputs came after -l tag OR there are no non tag inputs
		printf("No input files were given\n");
		printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
		exit(0);	
	}

	
	//checking whether it is -c or -w and whether it is scaled or not and storing in relevant variables
	
	for(int i=1;i<argc;i++){
		//checking whether it is scaled
		if (strcmp(argv[i],"--scaled")==0){
			scaled=1;
		}
		//checking whether it is -c or -w
		if ((strcmp(argv[i],"-c")==0)||(strcmp(argv[i],"-w")==0)){
			//checking whether it is -c
			if(strcmp(argv[i],"-c")==0){
				chwd=1;
			}else{
				chwd=2;
			}
			
		}
	}
	
	
	//reading files
	
	for(int p=1;p<argc;p++){
		
		//if the previous argument is -l skipping to the next argument
		if(strcmp(argv[p-1],"-l")==0){
			p++;
			//if p equals to argc breaking the loop as in later statements accssesing memory out of region happens which gives a segmentation fault.
			if(p==argc){
				break;
			}
		}
		
		//if the argument is a non tag argument
		if (strncmp(argv[p],"-",1)!=0){
			
			inputfpr = fopen(argv[p], "r");	//Open file for read. 
		        
		        //if there is no such files        
        		if (inputfpr == NULL){
            			printf("Cannot open one or more given files\n");    //Program prints error message and closes if file is not found           
     				exit(0);
     			}
     			
   			//if the argument says -c
			if (chwd==1){
				
			  	while(cha=fgetc(inputfpr)){	 // Taking input single character at a time 	
  	
    					if(cha == EOF){	//breaking the loop when it reaches '\0'
    						break;
    					}
    					
    					//PREPROCESSING
					cha=tolower(cha);	//turning all the characters to lower case
  					check=isalnum(cha);	//checking whether the character is an alphabetic character or a numerical
  					
  					//BASIC FUNCTIONALITY
  					ch=&cha;	//declaring cha to the char pointer
  		
  					if (check != 0){	//doing the operations only if the character is alphanumeric
  		
  						isUnique =search(head2,ch);	//checking whether the character is in the distinct character list
  			
  						if (isUnique) {	//if the character is unique adding to the distinct character list
  							addword(&head2, ch);	
  			
  						}
  			
  						addword(&head1,ch);	//adding each character to the big list
  						totalcount++;	//counting all the characters in the file
  		
  					}
  				}
  				
				fclose(inputfpr);	//Closing the file
			
			//if the argument says -w	 
			}else{
			
				while (fscanf (inputfpr, "%s", word) != EOF){
				
					//PREPROCESSING
					
					lower(word);	//converting the word to lower case
					checkAlnum(word);	//converting the word to a string which only contains alphabetic and numeric characters
					//BASIC FUNCTIONALITY
					isUnique =search(head2,word) ;	//checking whether the input word is in the distinct word list
					if (isUnique){	// If word is unique then add it to distinct words list
			
        						
						if (word[0]!='\0'){	//checking whether the word is empty	
							
							addword(&head2, word);	//adding words to the list
						}
					}
		
					if (word[0]!='\0'){	//checking whether the word is empty		
						
						addword(&head1, word);	//adding words to the list
						totalcount++;	//increasing the total count by 1
					}
        			}
				fclose(inputfpr);	//Closing the file
			}
		}
	}
	
	
	discount=count(head1,head2);	//counting the frequency of each character

	freelist (head1);	//freeing the list
	
	head3=sortdescend(head2);	//sorting list of distinct words in descending order

	freelist (head2);	//freeing the list
	
	//if the given number of bars exceed the number of distinct words	
	if (bars>discount){
		bars=discount;
	}
	
	printbar(head3,bars,totalcount,scaled,chwd);	//printing the graph according to given requirements or the default data 

	freelist (head3);	//freeing the list
	
	return 0;
}


//printing the bar graph

void printbar(list_t * head,int nobars,float totalcount,int scaled,int chwd){

	list_t *current=head;	//declaring a new list_t variable and asigning head to it
	
	//exiting the program if the head is NULL
	if(current==NULL){
		printf("No data to process\n");
		exit(0);
	}
	
	
	//SETTING THE PRINTING SPACE
	
	printf("\n");

	int len;	//a variable to store string lengths
	int maxlen=0;	//a variable to store a maximum string length

	//setting the space to print the word(finding the maximum word length of the word)
	
	for(int i=0;i<nobars;i++){
	
		len=strlen(current->word_t);	//storing the string lengths in the variable length
		
		//finding the maximum length
		if (maxlen<len){
			maxlen=len;
		}
		
		current=current->next;
	}

	int maxbarlen;	//a variable to store the maximum bar length
	float barlen;	//a variable to store the bar lengths
	int count;	//a variable to store the frequency of the words(or characters)
	int horizontal;	//a variable to store the length of horizontal line
	
	current=head;	//assigning the head to current
	
	//checking whether the percentage is 4 digit or 3 digit
	if(((int)current->count/totalcount*100)>9){
		maxbarlen=71-maxlen;
		horizontal=maxbarlen+6;
	}else{
		maxbarlen=72-maxlen;
		horizontal=maxbarlen+5;
	}
	
	float maxcount=current->count;	//a variable to store the maximum frequency
	
	for(int i=0;i<nobars;i++){	//itterating till i becomes the no of bars
	
		count=current->count;	//assigning the frequency to the variable count
		
		//making the barlength according to whether it is scled or not
		if (scaled){
			barlen=(count/maxcount)*maxbarlen;
		}else{
			barlen=(count/totalcount)*maxbarlen;
		}

		barlen=(int)barlen;	//making the barlen an int
		len=strlen(current->word_t);	//storing the string lengths
		
		for(int index=0;index<3;index++){	//printing the bar lengths of width 3 lines
	
			if(index==1){	//printing the name of the strings and their percentages in the middle line
				
				printf(" ");
				printf ("%s",current->word_t);	//printing the name of the string

				for (int j=0;j<(maxlen-len);j++){
					printf(" ");
				}
				printf(" ");
				printf("\u2502");	//printing the vertical line
				for(int k=0;k<barlen;k++){
					printf("\u2591");	//printing the box to make the bar
				}
				printf ("%.2f%c",current->count/totalcount*100,37);	//printing the percentage of the words
				
			}else{
				for(int j=0;j<(maxlen+2);j++){
					printf(" ");
				}
				printf("\u2502");	//printing the vertical line
				for(int k=0;k<barlen;k++){
				printf("\u2591");	//printing the box to make the bar
				}
			
			}
			printf("\n");
		}	
			
		current=current->next;	//itterating to the next node
		
		for(int j=0;j<(maxlen+2);j++){
			printf(" ");
		}
		printf("\u2502\n");	//printing the vertical line
	
	}
	
	for(int j=0;j<(maxlen+2);j++){
		printf(" ");
	}
	
	//printing the horizontal line
	
	printf("\u2514");	//printing the L
			
	for(int k=0;k<horizontal;k++){
		printf("\u2500");	//horizontal line
	}
	printf("\n");
			
}

    
//insering the created nodes in the descending order
  
void sortedInsert(list_t** head_ref, list_t* new_node){ 
	
	list_t* current;	//initializing pointer variables in list_t type	 
	
	// if the head node is NULL or if the head node does not have the highest frequency
	if (*head_ref == NULL || (*head_ref)->count <= new_node->count) { 
		
		//making the new node the head node
		new_node->next = *head_ref; 
		*head_ref = new_node;
		 
	} else { 

		//inserting the newnode in the relevant place
		current = *head_ref; 
		while (current->next != NULL && current->next->count >= new_node->count) { 
			current = current->next; 
		} 
		new_node->next = current->next; 
		current->next = new_node; 
	} 
} 


//creating a new node

list_t* newNode(list_t * node){ 
	
	list_t* new_node = (list_t*)malloc(sizeof(list_t));	//allocate memory to the new node 

	//inserting relevant data to the node
	strcpy(new_node->word_t, node->word_t);
	new_node->count = node->count; 
	new_node->next = NULL; 

	return new_node; 
} 


//looping the distinct word list and creating new nodes for each word and sending it to the function sortedInsert to insert nodes in descending order

list_t * sortdescend(list_t *head){

	list_t * temp,* node;	//initializing pointer variables in list_t type
	temp=NULL;
	
	//looping till the distinct word list becomes NULL
	 for (node=head;node!=NULL;node=node->next){
	 	
	 		list_t * new_node = newNode(node);	//creating a new node for each node in the distinct word list
	 		 
	 		sortedInsert(&temp,new_node);	//inserting the new nodes in a descending order
	 		 
	 }
	 return temp;
}


//conting the frequencies of distinct words

int count(list_t * node1, list_t * node2){

	int discount=0;	//a variable to count the number of distinct words in the list
	
    	list_t * current1 = node1;	//declaring a new list_t variable and asigning node1 to it
    	list_t * current2 = node2;	//declaring a new list_t variable and asigning node2 to it
    
    	while (current2 != NULL){	//itterate till the node becomes NULL  
    		discount++;	//increasing the count of distinct words by 1
    		list_t * temp = current2;	//declaring a new list_t variable and asigning current2 to it
    	
    		int count = 0;	//a varible to find the frequency of words(or characters)
    		for (current1 = node1;current1 != NULL;current1 = current1->next){	//itterate till the node becomes NULL  
        		if (strcmp((current1->word_t), (temp->word_t)) == 0){
        	   		count++;	//increasing the count by 1
        		}
        	}
    	
    		temp->count=count;
    		current2 = current2->next;	//ittering to the next node
    	}
    	return discount;
}


//checking whether the word is in the distinct word list

int search(list_t * head, char *word){
	
	list_t * current = head;	//declaring a new list_t variable and asigning head to it
	while (current != NULL){	//itterate till the node becomes NULL  
       	if (strcmp((current->word_t), word) == 0)
       		return 0; 
        	current = current->next;	//ittering to the next node
    	} 
    	return 1;
    	 
}


//freeing the allocated memories of linked lists

void freelist (list_t *node){

	while (node){	//itterate till the node becomes NULL    
		
		list_t *temp = node;	//declaring a new list_t variable
        	node = node->next;	//ittering to the next node
      		free (temp);	//freeing the temp node
    }
}


//adding word to the linked lists

list_t * addword (list_t **head, char *word){
	  	
   	
   	list_t  *node = malloc (sizeof *node);	//alocate memory for a new node
   	list_t *temp = *head;	//creating a new node to insert it to the list
   	node->next = NULL;
   	   	
   	strcpy (node->word_t, word);	//copying the word to the node->word_t 
   	
   	//joining the newnode to the end of the list
   	
   	if (!*head){	//checking whether the head node is null
   		return (*head = node);
   	}else{
   		while (temp->next){	//checking whether the temp next is null
   			temp = temp->next;	//itterate to the next node
		}
		return (temp->next = node);
        }
}


//removing all the characters from the word except for the aphabetical characters and numbers

void checkAlnum(char *ch){

	for(char *p = ch; *p; ++p){	//ittering till the string reaches the NULL character
		if(isalnum(*p)){	//checking whether it is a aphabetical characters or number
        		*ch++ = *p;
        	}
        }		
    	*ch = '\0';
}


//Converting the string into lowercase by ittering each character using a pointer

char * lower(char *str){

	while (*str){	//Ittering till we meet the null character
		*str = tolower(*str);
		str++;	//shifting the pointer by 1
	}

  	return str;
 }
