#include <stdio.h>
#include <string.h>

void menu();
void read_news(char buffer[500], char file_path[11], int is_Only_Title);
void append_file(char *file_path, char c);
void read_magic_numbers(char buffer_magic[10], char buffer_news[500]);
double f_func(int x);
double g_func(double f(int x), int a);

void main()
{
	menu();
}

void menu(){
	char text[500]; 
	char buffer_magic[10];
	char op, yn;	//op for first menu's switch case variable and yn is for yes/no question
	char new_id;	//holds which news text wanted to read
	char readed_news; 	
	int flag = 0;	//this is for checking either new is readed before or not

	char new1[11] = "news/1.txt";
	char new2[11] = "news/2.txt";
	char new3[11] = "news/3.txt";
	char new4[11] = "news/4.txt";

	char *rni = "readed_news_id.txt";	
	FILE *rn;
	rn = fopen(rni, "r");

	printf("*************** DAILY PRESS ***************\n\nToday's news are listed for you:\n\n");
	
	printf("Title of 1. news:");
	read_news(text, new1, 1);

	printf("Title of 2. news:");
	read_news(text, new2, 1);
	
	printf("Title of 3. news:");
	read_news(text, new3, 1);
	
	printf("Title of 4. news:");
	read_news(text, new4, 1);
	
	printf("\nWhat do you want to do?\na.Read a new\nb.List the readed news\nc.Get decrypted information from the news\n");
	scanf(" %c", &op);

	switch(op){
		//in case a, after getting input from user opens another switch case to show inputted new. 
		case 'a':
			printf("Which news text do you want to read?:");
			scanf(" %c", &new_id);
			switch(new_id){
				//in every case, code scans "readed_news_id.txt" file until EOF to find selected id. If finds it, marks the flag. 
				case '1':
					flag = 0;
					while(fscanf(rn, "%c", &readed_news) != EOF)
						if(readed_news == '1'){
							flag = 1;
							printf("This new is readed. Do you want to read it again? Yes(1) / No(0):");
							scanf(" %c", &yn);
							if(yn == '1'){
								read_news(text, new1, 0);
								break;
							}
							else if(yn == '0')
								break;
						}
					//if couldn't find it then writes num of new into "readed_news_id.txt" file.
					if(flag == 0){
						read_news(text, new1, 0);
						append_file(rni, '1');
					}
					break;		
				//same...
				case '2':
					flag = 0;
					while(fscanf(rn, "%c", &readed_news) != EOF)
						if(readed_news == '2'){
							flag = 1;
							printf("This new is readed. Do you want to read it again? Yes(1) / No(0):");
							scanf(" %c", &yn);
							if(yn == '1'){
								read_news(text, new2, 0);
								break;
							}
							else if(yn == '0')
								break;
						}
					if(flag == 0){
						read_news(text, new2, 0);
						append_file(rni, '2');
					}
					break;
				//same...
				case '3':
					flag = 0;
					while(fscanf(rn, "%c", &readed_news) != EOF)
						if(readed_news == '3'){
							flag = 1;
							printf("This new is readed. Do you want to read it again? Yes(1) / No(0):");
							scanf(" %c", &yn);
							if(yn == '1'){
								read_news(text, new3, 0);
								break;
							}
							else if(yn == '0')
								break;
						}
					if(flag == 0){
						read_news(text, new3, 0);
						append_file(rni, '3');
					}
					break;
				//same...
				case '4':
					flag = 0;
					while(fscanf(rn, "%c", &readed_news) != EOF)
						if(readed_news == '4'){
							flag = 1;
							printf("This new is readed. Do you want to read it again? Yes(1) / No(0):");
							scanf(" %c", &yn);
							if(yn == '1'){
								read_news(text, new4, 0);
								break;
							}
							else if(yn == '0')
								break;
						}
					if(flag == 0){
						read_news(text, new4, 0);
						append_file(rni, '4');
					}
					break;
			}
			printf("\nDo you want to continue? Yes(y) / No(n):");
			scanf(" %c", &yn);
			if(yn == 'y')
				menu();
			else if(yn == 'n')
				printf("Good Bye!\n");
			break;
		//in case b, code scans "readed_news_id.txt" again but this time it prints every char until it is EOF.
		case 'b':
			printf("Readed news are listed below:\n");
			while(fscanf(rn, "%c", &readed_news) != EOF)
				for(char i='1'; i<='4'; i++)
					if(readed_news == i)
						printf("%c. new is readed.\n", i);
			printf("Do you want to continue? Yes(y) / No(n):");
			scanf(" %c", &yn);
			if(yn == 'y')
				menu();
			else if(yn == 'n')
				printf("Good Bye!\n\n");
			break;
		//in case c, code asks for a new id then gets into another switch case.
		case 'c':
			printf("Which news would you like to decrypt?:");
			scanf(" %c", &new_id);
			switch(new_id){
				//in every case, code first reads selected new's text. Then sends it's text buffer to another function to calculate secret number.
				case '1':
					read_news(text, new1, 0);
					printf("Number of tests performed=\n");
					read_magic_numbers(buffer_magic, text);
					printf("Do you want to continue? Yes(y) / No(n):\n");
					scanf(" %c", &yn); 
					if(yn == 'y')
						menu();
					else if(yn == 'n')
						printf("Good Bye!\n");
					break;

				case '2':
					read_news(text, new2, 0);
					printf("Number of sick people = ");
					read_magic_numbers(buffer_magic, text);
					printf("\nDo you want to continue? Yes(y) / No(n):");
					scanf(" %c", &yn);
					if(yn == 'y')
						menu();
					else if(yn == 'n')
						printf("Good Bye!\n");
					break;

				case '3':
					read_news(text, new3, 0);
					printf("\nNumber of deaths = ");
					read_magic_numbers(buffer_magic, text);
					printf("\nDo you want to continue? Yes(y) / No(n):");
					scanf(" %c", &yn);
					if(yn == 'y')
						menu();
					else if(yn == 'n')
						printf("Good Bye!\n");
					break;

				case '4':
					read_news(text, new4, 0);
					printf("\nExpected number of sick people = ");
					read_magic_numbers(buffer_magic, text);
					printf("\nDo you want to continue? Yes(y) / No(n):");
					scanf(" %c", &yn);
					if(yn == 'y')
						menu();
					else if(yn == 'n')
						printf("Good Bye!\n");
					break;
			}
			break;
	}
}

void read_news(char buffer[500], char file_path[11], int is_Only_Title){
	FILE *fp;
	fp = fopen(file_path, "r");
	//these 4 line codes is for determining how many characters are there in the text.
	//without knowing it printing with "%s" also prints characters after EOF which is an error.
	int sz;
	fseek(fp, 0L, SEEK_END);
	sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	//if/else if code below scans all the text if "is_Only_Title" is 0 or only title if it is 1. 
	//since we know how many characters are there in the text before using fscanf, it is possible to create a loop with char limit which is "sz". 
	if(is_Only_Title==0)
		for(int i=0; i<sz; i++){
			fscanf(fp, "%c", &buffer[i]);
			printf("%c", buffer[i]);
		}
	else if(is_Only_Title==1){
		fscanf(fp, "%[^\n]s", buffer);
		printf("%s\n", buffer);
	}	
	fclose(fp);
}

void append_file(char *file_path, char c){	
	//just prints a char each time. Nothing fancy.
	FILE *fp;
	fp = fopen(file_path, "a");
	fprintf(fp, "%c\n", c);
	fclose(fp);
}

void read_magic_numbers(char buffer_magic[10], char buffer_news[500]){
	int count=0; //counts how many characters are there in buffer_news to prevent printing unnecessary things.
	int j=0; //is for buffer_magic array, increases each time as a number is holded by buffer_magic.
	int magic_num, sum=0, result; // for calculating secret number. 

	for(int i=0; i<strlen(buffer_news)-1; i++)
		if(buffer_news[i] == '#')
			count++;
	for(int i=0; i<strlen(buffer_news)-1; i++)
		if(buffer_news[i] == '#'){
			buffer_magic[j] = buffer_news[i+1];
			j++;
		}
	//I can't use strlen(buffer_news) here because it differs each time this func is called. This is why I first created a count int first.
	for(int i=0; i<count; i++){
		magic_num = buffer_magic[i] - '0';
		result = g_func(f_func, magic_num);
		sum = sum + result;
	}
	printf("%d\n", sum);
}

double f_func(int x){
	double result;
	result = (x*x*x)-(x*x)+2;
	return result;
}

double g_func(double f(int x), int a){
	double result;
	result = f(a)*f(a);
	return result;	
}

























