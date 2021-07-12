#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
void board();																						 //prints letters. almost all the visual things happens here. 
char rand_letter();																					 //chooses random words for empty spaces
void rand_word          (char letters[21][21], char words[100][15]);								 //chooses 10 random words and sends them into randomly chosen one of the 8 function.
void rand_num_generator (int *word_nums, int count);												 //generates 10 different numbers between [0, count). Count is the parameter.
void horizontal_word    (char word[15], char letters[21][21]);										 //takes "word" parameter and puts it's letters into "letters" in direction of east.
void rev_horizontal_word(char word[15], char letters[21][21]);				  		    			 //takes "word" parameter and puts it's letters into "letters" in direction of west.
void vertical_word      (char word[15], char letters[21][21]);										 //takes "word" parameter and puts it's letters into "letters" in direction of south.
void rev_vertical_word  (char word[15], char letters[21][21]);										 //takes "word" parameter and puts it's letters into "letters" in direction of north.
void north_east_word    (char word[15], char letters[21][21]);									 	 //takes "word" parameter and puts it's letters into "letters" in direction of north-east.
void north_west_word    (char word[15], char letters[21][21]);										 //takes "word" parameter and puts it's letters into "letters" in direction of north-west.
void south_east_word    (char word[15], char letters[21][21]);										 //takes "word" parameter and puts it's letters into "letters" in direction of south-east.
void south_west_word    (char word[15], char letters[21][21]);										 //takes "word" parameter and puts it's letters into "letters" in direction of south-west.
int  check_empty        (int direction, int column, int row, int word_length, char letters[21][21]); //func is for to prevent from writing over into another letter. returns 1 if all the way of a word is empty.
int  check_word         (char word[15], int column, int row, int word_length, char letters[21][21]); //checks if the given "word" parameter exist in the inputted coords. If it exists then returns 1. 
int  check_same         (char words[100][15], char word[15]);                                        //words except in the "words.txt" file will lead to return 0 this func. If word exists in the file then returns 1.
int  found_words        (char word[15], int count1);												 //func prints an error message when user tries to enter a word which found it before. Also it doesn't count twice :))

int main(){
	srand(time(NULL)); //initializing a seed for rand() just once.
	board();
	return 0;
}

void board(){
	int flag             = 1;    //this is for check_word func.
	int count            = 0;	 //counts how many word has done so far. when it's equal to 10 game ends.
	char input[15]       = {0};  //holds user input
	char letters[21][21] = {0};  //holds letters
	char words[100][15]  = {0};	 //holds words in the "words.txt" file

	FILE *fp;
	fp = fopen("words.txt", "r");
	//scans all the txt file and puts into "words".
	for(int i=0; i<100; i++) 	 
		fscanf(fp, "%s", words[i]);

	//after this line "letters" char holds all 10 random words in random coords and direction 
	rand_word(letters, words); 
	
	//quits when all the 10 words is found
	while(count != 10){
		//lines between 47-62 prints the board. 
		printf("      ");
		for(int i=0; i<=9; i++)
			printf("%d  ", i);
		for(int i=10; i<=19; i++)
			printf("%d ", i);	
		printf("\n\n");
		for(int i=0; i<20; i++){	
			printf("%c)    ", i + 'A');
			for(int j=0; j<20; j++){
				//if char is already filled with a letter, prints it. otherwise puts a random letter. 
				if(letters[i][j] != '\0')
					printf("%c  ", letters[i][j]);
				else
					printf("   "); //if you want to see all the chosen words use this "printf("   ");" instead of in this line. 
			}
			printf("\n");
		}
		
		//it is necessery to initialize like that, otherwise errors occur.
		char word[15] = {0}; 
		char row      = 0;
		int  column   = 0;

		//first took as string then converted it into row, column and word.
		printf("'Exit' to quit.\nPlease enter the coordinate and the word:");
		scanf(" %[^\n]s", input);

		//as a result of taking as string, column may have 1 or 2 number.
		//in these conditions; all row, column and word are filled with user input.   
		if(input[2] == ' '){
			row = input[0];
			column = input[1] - 48; 			
			for(int i=0; i<strlen(input)-3; i++)
				word[i] = input[i+3];
		}
		else if(input[3] == ' '){
			row = input[0];
			column = (input[1] - 48)*10+(input[2] - 48);
			for(int i=0; i<strlen(input)-4; i++)
				word[i] = input[i+4];
		}

		//lines between 90-109 if anything is wrong with user input prints an error about it or if user wants to quit, quits the code.
		//also if everything is ok with user input then increases the count and holds the word to prevent from being used again.
		if(input[0] == 'E' && input[1] == 'x' && input[2] == 'i' && input[3] == 't'){
			printf("Thanks for playing\n");
			break;
		}
		else if(check_same(words, word) == 1){
			row = row - 65;
			flag = check_word(word, column, row, strlen(word), letters);
				if(flag == 1)
					count = found_words(word, count);
				else
					printf("******* YOUR COORDINATES ARE WRONG OR YOU'VE ALREADY FOUND THIS WORD :( *******\n");
			printf("You found %d words so far.\n%d words left.\n", count, (10-count));
		}
		else
			printf("\n******* THIS WORD DOES NOT EXIST IN TXT FILE :( *******\n");
		
		if(count == 10){
			printf("*******  CONGRATULATIONS!! YOU HAVE WON :))))  ******\n");
			break;
		}
	}
}

//all the func below is briefly explained in the initializing part. 

char rand_letter(){
	char letter;
	letter = rand()%26 + 'a';
	return letter;
}

void rand_word(char letters[21][21], char words[100][15]){
	int op;
	int temp;	
	int word_nums[11]; 
	rand_num_generator(word_nums, 100); 

	//words[temp] holds a random word and op is chosen randomly each time.
	for(int i=0; i<10; i++){
		temp = word_nums[i];
		op = rand()%8;
		switch(op){
			case 0:
				horizontal_word(words[temp], letters);
				break;
			case 1:
				rev_horizontal_word(words[temp], letters);
				break;
			case 2:
				vertical_word(words[temp], letters);
				break;
			case 3:
				rev_vertical_word(words[temp], letters);
				break;
			case 4:
				north_east_word(words[temp], letters);
				break;
			case 5:
				north_west_word(words[temp], letters);
				break;
			case 6:
				south_east_word(words[temp], letters);
				break;
			case 7:
				south_west_word(words[temp], letters);
				break;
		}
	}
}

void rand_num_generator(int *word_nums, int count){
	//this func is actually for not choosing the same number twice. 
	//if rand() func chooses a number which chose before then chooses all the 10 numbers again until all of them is different. 
	int flag = 1;
	while(flag == 1){
		flag = 0;
		for(int i=0; i<10; i++)
			word_nums[i] = rand()%count;
		for(int i=9; i>0; i--)
			for(int j=i-1; j>=0; j--)
				if(word_nums[i] == word_nums[j])
					flag = 1;
	}
}

/* each 8 func below does the same thing but in different directions.
 * first, chooses random row and column locations
 * then, with chech_empty() func. it searchs "letters" func as length of the word. 
 * if it is empty then marks the flag as 0, if not chooses another locations for that word until finds a place which is empty.
 * when it finds, quits the loop and puts the word's letters into "char letters[21][21]".
*/

// ** 1 **
void horizontal_word(char word[15], char letters[21][21]){
	int column;
	int row;
	int flag = 1;
	while(flag == 1){
		column = rand()%(21-strlen(word));
		row    = rand()%19;
		flag = check_empty(1, column, row, strlen(word), letters);
	}
	for(int i=0; i<strlen(word); i++)
		letters[row][column+i] = word[i];
}
// ** 2 **
void rev_horizontal_word(char word[15], char letters[21][21]){
	int column;
	int row;
	int flag = 1;
	while(flag == 1){	
		column = rand()%(21-strlen(word));
		row= rand()%19;
		flag = check_empty(2, column, row, strlen(word), letters);
	}
	for(int i=0; i<strlen(word); i++)
		letters[row][column+i] = word[strlen(word)-1-i];
}
// ** 3 **
void vertical_word(char word[15], char letters[21][21]){
	int column;
	int row;
	int flag = 1;
	while(flag == 1){	
		column = rand()%19;
		row= rand()%(21-strlen(word));
		flag = check_empty(3, column, row, strlen(word), letters);
	}
	for(int i=0; i<strlen(word); i++)
		letters[row+i][column] = word[i];	
}
// ** 4 **
void rev_vertical_word(char word[15], char letters[21][21]){
	int column;
	int row;
	int flag = 1;
	while(flag == 1){	
		column = rand()%19;
		row= rand()%(21-strlen(word));
		flag = check_empty(4, column, row, strlen(word), letters);
	}
	for(int i=0; i<strlen(word); i++)
		letters[row+i][column] = word[strlen(word)-1-i];	
}
// ** 5 **
void north_east_word(char word[15], char letters[21][21]){
	int column;
	int row;
	int flag = 1;
	while(flag == 1){
		column = rand()%(21-strlen(word))+strlen(word)-1;
		row= rand()%(21-strlen(word));
		flag = check_empty(5, column, row, strlen(word), letters);
	}
	for(int i=0; i<strlen(word); i++)
		letters[row+i][column-i] = word[strlen(word)-1-i];
}
// ** 6 **
void north_west_word(char word[15], char letters[21][21]){
	int column;
	int row;
	int flag = 1;
	while(flag == 1){
		column = rand()%(21-strlen(word));
		row= rand()%(21-strlen(word));
		flag = check_empty(6, column, row, strlen(word), letters);
	}
	for(int i=0; i<strlen(word); i++)
		letters[row+i][column+i] = word[strlen(word)-1-i];
}
// ** 7 **
void south_east_word(char word[15], char letters[21][21]){
	int column;
	int row;
	int flag = 1;
	while(flag == 1){
		column = rand()%(21-strlen(word));
		row= rand()%(21-strlen(word));
		flag = check_empty(7, column, row, strlen(word), letters);
	}
	for(int i=0; i<strlen(word); i++)
		letters[row+i][column+i] = word[i];
}
// ** 8 **
void south_west_word(char word[15], char letters[21][21]){
	int column;
	int row;
	int flag = 1;
	while(flag == 1){
		column = rand()%(21-strlen(word))+strlen(word)-1;
		row    = rand()%(21-strlen(word));
		flag = check_empty(8, column, row, strlen(word), letters);	
	}
	for(int i=0; i<strlen(word); i++)
		letters[row+i][column-i] = word[i];
}

// looks in every direction. if there isn't any word flag is 0. otherwise flag is 1. returns flag.
int check_empty(int direction, int column, int row, int word_length, char letters[21][21]){
	int flag = 0;
	switch (direction){
		case 1:
			for(int i=0; i<word_length; i++)
				if(letters[row][column+i] != '\0')
					flag = 1;
			break;
		case 2:
			for(int i=0; i<word_length; i++)
				if(letters[row][column+i] != '\0')
					flag = 1;
			break;
		case 3:
			for(int i=0; i<word_length; i++)
				if(letters[row+i][column] != '\0')
					flag = 1;
			break;
		case 4:
			for(int i=0; i<word_length; i++)
				if(letters[row+i][column] != '\0')
					flag = 1;
			break;
		case 5:
			for(int i=0; i<word_length; i++)
				if(letters[row+i][column-i] != '\0')
					flag = 1;
			break;
		case 6:
			for(int i=0; i<word_length; i++)
				if(letters[row+i][column+i] != '\0')
					flag = 1;
			break;
		case 7:
			for(int i=0; i<word_length; i++)
				if(letters[row+i][column+i] != '\0')
					flag = 1;
			break;
		case 8:
			for(int i=0; i<word_length; i++)
				if(letters[row+i][column-i] != '\0')
					flag = 1;
			break;
	}
	return flag;
}

// again looks in every direction. if finds the same word in the parameter changes its letters witch uppercase ones and marks flag as 1. otherwise only marks the flag as 0 and returns it.
int check_word(char word[15], int column, int row, int word_length, char letters[21][21]){
	int flag = 0;
	int count = 0;
	char temp_word[15] = {0};
	for(int i=1; i<=8; i++){
		count = 0;
		switch(i){
			case 1:			
				for(int i=0; i<word_length; i++)
					if(word[i] == letters[row][column+i])
						count++;
				if(count == word_length)
					flag = 1;
				if(flag == 1){
					for(int i=0; i<word_length; i++)
						letters[row][column+i] = letters[row][column+i] - 32;
				}
				break;
			case 2:
				for(int i=0; i<word_length; i++)	
					if(word[i] == letters[row][column-i])
						count++;	
				if(count == word_length)
					flag = 1;
				if(flag == 1){
					for(int i=0; i<word_length; i++)
						letters[row][column-i] = letters[row][column-i] - 32;
				}
				break;
			case 3:
				for(int i=0; i<word_length; i++)
					if(word[i] == letters[row+i][column])
						count++;
				if(count == word_length)
					flag = 1;
				if(flag == 1){
					for(int i=0; i<word_length; i++)
						letters[row+i][column] = letters[row+i][column] - 32;
				}
				break;
			case 4:
				for(int i=0; i<word_length; i++)
					if(word[i] == letters[row-i][column])
						count++;
				if(count == word_length)
					flag = 1;
				if(flag == 1){
					for(int i=0; i<word_length; i++)
						letters[row-i][column] = letters[row-i][column] - 32;
				}
				break;
			case 5:
				for(int i=0; i<word_length; i++)
					if(word[i] == letters[row-i][column+i])
						count++;
				if(count == word_length)
					flag = 1;
				if(flag == 1){
					for(int i=0; i<word_length; i++)
						letters[row-i][column+i] = letters[row-i][column+i] - 32;
				}
				break;
			case 6:
				for(int i=0; i<word_length; i++)
					if(word[i] == letters[row-i][column-i])
						count++;
				if(count == word_length)
					flag = 1;
				if(flag == 1){
					for(int i=0; i<word_length; i++)
						letters[row-i][column-i] = letters[row-i][column-i] - 32;
				}
				break;
			case 7:
				for(int i=0; i<word_length; i++)
					if(word[i] == letters[row+i][column+i])
						count++;
				if(count == word_length)
					flag = 1;
				if(flag == 1){
					for(int i=0; i<word_length; i++)
						letters[row+i][column+i] = letters[row+i][column+i] - 32;
				}
				break;
			case 8:
				for(int i=0; i<word_length; i++)
					if(word[i] == letters[row+i][column-i])
						count++;
				if(count == word_length)
					flag = 1;
				if(flag == 1){
					for(int i=0; i<word_length; i++)
						letters[row+i][column-i] = letters[row+i][column-i] - 32;
				}
				break;
		}
		if(flag == 1){
			return flag;
			break;
		}
	}
	return flag;
}

// *line 19* 
int check_same(char words[100][15], char word[15]){
	int count = 0;
	for(int i=0; i<100; i++){
		count = 0;
		for(int j=0; j<strlen(words[i]); j++)
			if(word[j] == words[i][j])
				count++;
		if(count == strlen(words[i]))
				return 1;	
	}
	return 0;
}

// *line 20*
int found_words(char word[15], int count1){
	int count2 = 0;
	int flag = 0;
	char found[10][15] = {0};

	for(int i=0; i<10; i++){
		count2 = 0;
		for(int j=0; j<strlen(word); j++)
			if(word[j] == found[i][j])
				count2++;
		if(count2 == strlen(word))
			flag = 1;
	}

	if(flag == 1)
		printf("***** YOU HAVE ALREADY FOUND THIS WORD *****");
	else if(flag == 0){
		for(int i=0; i<strlen(word); i++)
			found[count1][i] = word[i];
		count1++;
	}
	return count1;
}

