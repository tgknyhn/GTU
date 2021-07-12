#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE        2002  
#define MAX_GAME_LENGTH 100
#define MAX_ROW_LENGTH  200   
#define MAX_LENGTH      30 
#define MAX_GENRE_NUM   13 
#define MAX_PLAT_NUM    11
#define ALL_DATA        7

//menu sends initialized variables into data() func, then uses these datas with related operations
void menu(char names[MAX_LINE][MAX_GAME_LENGTH], char genres[MAX_GENRE_NUM][MAX_LENGTH], char platforms[MAX_PLAT_NUM][MAX_LENGTH], float all_data[MAX_LINE][ALL_DATA]);
void data(char names[MAX_LINE][MAX_GAME_LENGTH], char genres[MAX_GENRE_NUM][MAX_LENGTH], char platforms[MAX_PLAT_NUM][MAX_LENGTH], float all_data[MAX_LINE][ALL_DATA]);
//checks if any data is already taken or not. (prevents overlapping)
void check_genres	(char temp[MAX_LENGTH], char genres[MAX_GENRE_NUM][MAX_LENGTH],    int *line_genre,    float *all_data_index);
void check_platforms(char temp[MAX_LENGTH], char platforms[ALL_DATA][MAX_LENGTH], int *line_platform, float *all_data_index);
//executes operation according to user input with given data
void operation0(char genres[MAX_GENRE_NUM][MAX_LENGTH]);
void operation1(char platforms[MAX_PLAT_NUM][MAX_LENGTH]);
void operation2(float all_data[MAX_LINE][ALL_DATA], char names[MAX_LINE][MAX_GAME_LENGTH]);
void operation3(float all_data[MAX_LINE][ALL_DATA], char names[MAX_LINE][MAX_GAME_LENGTH], char genres[MAX_GENRE_NUM][MAX_LENGTH], char platforms[MAX_PLAT_NUM][MAX_LENGTH]);
void operation4(float all_data[MAX_LINE][ALL_DATA]);
void operation5(float all_data[MAX_LINE][ALL_DATA], char names[MAX_LINE][MAX_GAME_LENGTH]);
void operation6(float all_data[MAX_LINE][ALL_DATA], char genres[MAX_GENRE_NUM][MAX_LENGTH],    int *index_genre);
void operation7(float all_data[MAX_LINE][ALL_DATA], char platforms[MAX_PLAT_NUM][MAX_LENGTH], int *index_platform);

int main()
{
	//initializing variables which will hold all the information
	float all_data  [MAX_LINE][ALL_DATA]  		= {0};
	char  names	    [MAX_LINE][MAX_GAME_LENGTH] 		= {0};
	char  genres    [MAX_GENRE_NUM][MAX_LENGTH] = {0}; 
	char  platforms [MAX_PLAT_NUM][MAX_LENGTH]  = {0};
	//fills variables
	data(names, genres, platforms, all_data);
	//prints menu
	menu(names, genres, platforms, all_data);
	return 0;
}

void menu(char names[MAX_LINE][MAX_GAME_LENGTH], char genres[MAX_GENRE_NUM][MAX_LENGTH], char platforms[MAX_PLAT_NUM][MAX_LENGTH], float all_data[MAX_LINE][ALL_DATA])
{
	char op;
	printf("0: Lists of the Genres                        \n");
	printf("1: Lists of the Platforms                     \n");
	printf("2: Lists of the Games Through the Years       \n");
	printf("3: All Informations of a Single Game          \n");
	printf("4: Average of the User Scores                 \n");
	printf("5: Georographical Information of a Single Game\n");
	printf("6: Frequence of the Genres                    \n");
	printf("7: Frequence of the Platforms                 \n");
	printf("8: Exit                                     \n\n");
	printf("Please select an operation:                   \n");
	scanf(" %c", &op);
	//point genres/platforms for op6/op7
	int index_genre    = 1;
	int index_platform = 1;
	if(op == '0' || op == '1' || op == '2' || op == '3' || op == '4' || op == '5' || op == '6' || op == '7' || op == '8'){
		switch(op){
			case '0':
				operation0(genres);
				menu(names, genres, platforms, all_data);
				break;
			case '1':
				operation1(platforms);
				menu(names, genres, platforms, all_data);
				break;
			case '2':
				operation2(all_data, names);
				menu(names, genres, platforms, all_data);
				break;
			case '3':
				operation3(all_data, names, genres, platforms);
				menu(names, genres, platforms, all_data);
				break;
			case '4':
				operation4(all_data);
				menu(names, genres, platforms, all_data);
				break;
			case '5':
				operation5(all_data, names);
				menu(names, genres, platforms, all_data);
				break;
			case '6':
				operation6(all_data, genres, &index_genre);
				printf("\n");
				menu(names, genres, platforms, all_data);
				break;
			case '7':
				operation7(all_data, platforms, &index_platform);
				printf("\n");
				menu(names, genres, platforms, all_data);
				break;
			case '8':
				printf("\nThanks for using :)\n");
				break;
		}
	}
	else{
		printf("\nTry Again :c\n\n");
		menu(names, genres, platforms, all_data);	
	}
}

void data(char names[MAX_LINE][MAX_GAME_LENGTH], char genres[MAX_GENRE_NUM][MAX_LENGTH], char platforms[ALL_DATA][MAX_LENGTH], float all_data[MAX_LINE][ALL_DATA])
{
	//temp chars to scan file
	char lines		   [MAX_LINE][MAX_ROW_LENGTH] = {0};
	char temp_genre    [MAX_LINE][MAX_LENGTH]     = {0};
	char temp_platform [MAX_LINE][MAX_LENGTH]     = {0};
	char temp_data2	   [MAX_LINE][ALL_DATA]       = {0};
	char temp_data3	   [MAX_LINE][ALL_DATA]       = {0};
	char temp_data4	   [MAX_LINE][ALL_DATA]  	  = {0};
	char temp_data5	   [MAX_LINE][ALL_DATA] 	  = {0};
	char temp_data6	   [MAX_LINE][ALL_DATA] 	  = {0};
	char temp_data7	   [MAX_LINE][ALL_DATA] 	  = {0};
	//points index of genre/platform to fill genres/platforms char properly
	int line_genre 	   = 0;
	int line_platform  = 0;
	//hold index of genres/platforms char
	float all_data_index_genre;
	float all_data_index_platform;
	//holds temp char indexes 
	int comma 	       = 0;
	int index_genre    = 0;
	int index_platform = 0;
	int index_data2	   = 0;
	int index_data3	   = 0;
	int index_data4	   = 0;
	int index_data5	   = 0;
	int index_data6	   = 0;
	int index_data7	   = 0;
	//for strtod func
	char *data_text;

	FILE *fp;
	fp = fopen("Video_Games.txt", "r");

	for(int i=0; i<MAX_LINE; i++){
		//resetting indexes each line
		comma 		   = 0;
		index_genre    = 0;
		index_platform = 0;
		index_data2	   = 0;
		index_data3	   = 0;
		index_data4	   = 0;
		index_data5	   = 0;
		index_data6	   = 0;
		index_data7	   = 0;

		fscanf(fp, "%s", lines[i]);
		for(int j=0; j<strlen(lines[i]); j++){
			if(lines[i][j] == ','){
				comma++;
				j++;
			}
			//according to comma count, fills the temp chars with related information 
			if(comma == 0)
				names[i][j] = lines[i][j]; //filling names
			else if(comma == 1){
				temp_genre[i][index_genre] = lines[i][j];
				index_genre++;
			}
			else if(comma == 2){
				temp_platform[i][index_platform] = lines[i][j];
				index_platform++;
			}
			else if(comma == 3){
				temp_data2[i][index_data2] = lines[i][j];
				index_data2++;
			}
			else if(comma == 4){
				temp_data3[i][index_data3] = lines[i][j];
				index_data3++;
			}
			else if(comma == 5){
				temp_data4[i][index_data4] = lines[i][j];
				index_data4++;
			}
			else if(comma == 6){
				temp_data5[i][index_data5] = lines[i][j];
				index_data5++;
			}
			else if(comma == 7){
				temp_data6[i][index_data6] = lines[i][j];
				index_data6++;
			}
		}
		//filling genres/platforms
		check_genres   (temp_genre[i],    genres,    &line_genre,    &all_data_index_genre);		
		check_platforms(temp_platform[i], platforms, &line_platform, &all_data_index_platform);
		//filling data float
		all_data[i][0] = all_data_index_genre;
		all_data[i][1] = all_data_index_platform;
		all_data[i][2] = strtod(temp_data2[i], &data_text);
		all_data[i][3] = strtod(temp_data3[i], &data_text);
		all_data[i][4] = strtod(temp_data4[i], &data_text);
		all_data[i][5] = strtod(temp_data5[i], &data_text);
		all_data[i][6] = strtod(temp_data6[i], &data_text);
	}
	fclose(fp);
}

void check_genres(char temp[MAX_LENGTH], char genres[MAX_GENRE_NUM][MAX_LENGTH], int *line_genre, float *all_data_index)
{
	int count = 0;
	int flag  = 0;
	//scans genres char if the "genre" in the temp char is already taken or not.
	for(int i=0; i<MAX_GENRE_NUM; i++){
		count = 0;
		for(int j=0; j<strlen(temp); j++)
			if(temp[j] == genres[i][j])
				count++;
		if(count == strlen(temp)){
			*all_data_index = i;
			flag = 1;
			break;
		}
	}
	//if it is not taken then fills the genres char with that temp char, then increases index for the next genre.
	if(flag == 0){
		for(int i=0; i<strlen(temp); i++)
			genres[*line_genre][i] = temp[i];
		//holds index for next op
		*all_data_index = *line_genre;
		*line_genre     = *line_genre + 1;
	}
}

void check_platforms(char temp[MAX_LENGTH], char platforms[MAX_PLAT_NUM][MAX_LENGTH], int *line_platform, float *all_data_index)
{
	int count = 0;
	int flag =  0;
	//scans platforms char if the "platform" in the temp char is already taken or not.
	for(int i=0; i<MAX_PLAT_NUM; i++){
		count = 0;
		for(int j=0; j<strlen(temp); j++)
			if(temp[j] == platforms[i][j])
				count++;	
		if(count == strlen(temp)){
			*all_data_index = i;
			flag = 1;
			break;
		}
	}
	//if it is not taken then fills the platforms char with that temp char, then increases index for the next platform.
	if(flag == 0){
		for(int i=0; i<strlen(temp); i++)
			platforms[*line_platform][i] = temp[i];
		//holds index for next op
		*all_data_index = *line_platform;
		*line_platform  = *line_platform + 1;
	}
}

//prints genres
void operation0(char genres[MAX_GENRE_NUM][MAX_LENGTH])
{
	printf("\n");
	for(int i=1; i<MAX_GENRE_NUM; i++)
		printf("%s\n", genres[i]);
	printf("\n");
}

//prints platforms
void operation1(char platforms[MAX_PLAT_NUM][MAX_LENGTH])
{
	printf("\n");
	for(int i=1; i<MAX_PLAT_NUM; i++)
		printf("%s\n", platforms[i]);
	printf("\n");
}

//prints game names according to inputted year.
void operation2(float all_data[MAX_LINE][ALL_DATA], char names[MAX_LINE][MAX_GAME_LENGTH])
{
	int flag = 0;
	int year = 0;
	char boo = 0;

	printf("\n");
	printf("Enter a year:");
	scanf(" %d", &year);
	printf("Until (0) or Since (1)  %d:", year);
	scanf(" %c", &boo);
	//if 0, scans all names then prints only before inputted year.
	if(boo == '0'){
		printf("\n");
		for(int i=1; i<2001; i++)
			if(all_data[i][2] < year){
				flag = 1;
				printf("%s\n", names[i]);
			}
		printf("\n");
		if(flag == 0){
			printf("Invalid result. Try again.\n");
			operation2(all_data, names);
		}
	}
	//if 1, scans all names then prints only after inputted year
	else if(boo == '1'){
		printf("\n");
		for(int i=1; i<2001; i++)
			if(all_data[i][2] > year){
				flag = 1;
				printf("%s\n", names[i]);
			}
		printf("\n");	
		if(flag == 0){
			printf("Invalid result. Try again.\n");
			operation2(all_data, names);
		}
	}
	else{
		printf("Invalid result. Try again.\n");
		operation2(all_data, names);
	}
}

//prints all information about inputted game
void operation3(float all_data[MAX_LINE][ALL_DATA], char names[MAX_LINE][MAX_GAME_LENGTH], char genres[MAX_GENRE_NUM][MAX_LENGTH], char platforms[MAX_PLAT_NUM][MAX_LENGTH])
{
	int  line           =  0;
	int  flag           =  0;
	char count          =  0;
	char game[MAX_GAME_LENGTH] = {0};

	printf("Please enter the name of the game:");
	scanf("%s", game);
	//scans all games to find inputted name
	for(int i=1; i<MAX_LINE-1; i++){
		count = 0;
		for(int j=0; j<strlen(game); j++)
			if(game[j] == names[i][j])
				count++;
		if(count == strlen(game)){
			line = i;
			flag = 1;
			break;
		}
	}
	//holds index of genre/platofrm
	int index_genre    = (int)all_data[line][0];
	int index_platform = (int)all_data[line][1];;
	//if name could've found then prints its informations
	if(flag == 1){
		printf("\n");
		printf("Name:		 	%s\n", names[line]);
		printf("Genre:		 	%s\n", genres[index_genre]);
		printf("Platform:		%s\n", platforms[index_platform]);
		printf("Year:			%.0f\n", all_data[line][2]);
		printf("Sales in NA:		%.2f\n", all_data[line][3]);
		printf("Sales in EU:	 	%.2f\n", all_data[line][4]);
		if(all_data[line][5] == 0.0)	
			printf("Total Sales: 		Not Available\n");
		else
			printf("Total Sales:	 	%.2f\n", all_data[line][5]);			
		printf("User Score:	 	%.2f\n", all_data[line][6]);
		printf("\n");
	}
	//if not then asks again
	if(flag == 0){
		printf("Invalid name. Try again.\n");
		operation3(all_data, names, genres, platforms);
	}
}

//prints average of the user scores of all games
void operation4(float all_data[MAX_LINE][ALL_DATA])
{
	float average = 0;
	float sum     = 0;
	//scans all user scores and holds their sum
	for(int i=1; i<MAX_LINE-1; i++)
		sum+=all_data[i][6];
	average = sum/2000;

	printf("\n");
	printf("Average: %.6f\n", average);
	printf("\n");
}

//prints in which region(EU/NA) the inputted game is more popular.
void operation5(float all_data[MAX_LINE][ALL_DATA], char names[MAX_LINE][MAX_GAME_LENGTH])
{
	char game[MAX_GAME_LENGTH] = {0};
	int line  = 0;
	int flag  = 0;
	int count = 0;

	printf("Please enter the name of the game:");
	scanf("%s", game);
	//scans all games to find inputted name
	for(int i=1; i<MAX_LINE-1; i++){
		count = 0;
		for(int j=0; j<strlen(game); j++)
			if(game[j] == names[i][j])
				count++;
		if(count == strlen(game)){
			line = i;
			flag = 1;
			break;
		}
	}
	//holds popularity
    float NA = all_data[line][3];
	float EU = all_data[line][4];
	//if name is found then prints
	if(flag == 1){
		printf("\n");
		if(NA > EU)
			printf("This game was more popular in North America\n");
		else if(EU > NA)
			printf("This game was more popular in Europe\n");
		else if(EU == NA)
			printf("The popularity of this game was equal in both region.\n");
		printf("\n");
	}
	//if not then asks again
	else if(flag == 0){
		printf("Invalid name. Try again.\n");
		operation5(all_data, names);
	}
}

//prints frequence of genres (recursive)
void operation6(float all_data[MAX_LINE][ALL_DATA], char genres[MAX_GENRE_NUM][MAX_LENGTH], int *index_genre)
{
	int count = 0;
	//scans all data and increases count each time it founds equal index
	for(int i=1; i<MAX_LINE-1; i++)
		if((int)all_data[i][0] == *index_genre)
			count++;
	
	//if case is just to make code look a bit better.(in line 7 it prints an additional tab)
	if(*index_genre == 7)
		printf("%s  		%d\n", genres[*index_genre], count);
	else
		printf("%s  	%d\n", genres[*index_genre], count);
	
	//will call function until comes to last genre.
	*index_genre = *index_genre + 1;
	if(*index_genre != MAX_GENRE_NUM)
		operation6(all_data, genres, index_genre);
}

//prints frequence of platforms (recursive)
void operation7(float all_data[MAX_LINE][ALL_DATA], char platforms[MAX_PLAT_NUM][MAX_LENGTH], int *index_platform)
{
	int count = 0;
	//scans all data and increases count each time it founds equal index
	for(int i=1; i<2001; i++)
		if((int)all_data[i][1] == *index_platform)
			count++;

	printf("%s  	%d\n", platforms[*index_platform], count);
	
	//will call function until comes to last platform.
	*index_platform = *index_platform + 1;
	if(*index_platform != MAX_PLAT_NUM)
		operation7(all_data, platforms, index_platform);
}
