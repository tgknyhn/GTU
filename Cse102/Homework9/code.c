#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARR_SIZE 500
#define N 		 9

void   menu();
//part1
void   part1                	     (int arr[], int n, int max_value, int index, double *mean);
void   generate_hofstadters_sequence (int arr[], int n);
int    find_max                      (int arr[], int index, int max_value);
int    sum_array				     (int arr[]);
double std_array					 (int arr[], double *mean, int n, int index);
//part2
void   part2						 (int visited[N][N], int x, int y, int orientation);
void   install_pipes 			     (int visited[N][N], int x, int y, int orientation);
//part3
void   part3						 (char *str);
char*  remove_duplicates             (char *str);

int main()
{
	menu();
	return 0;
}

void menu()
{
	/* -- to hold information in variables first initialized them here -- */

	//variables for part1
	int arr[ARR_SIZE] = {0};
	int n             = 1;
	int index         = 1;
	int max_value     = 1;
	double mean       = 0;
	//variables for part2
	int visited[N][N] = {0};
	int x 			  = 0;
	int y 			  = 0;
	int orientation	  = 0;
	//variable for part3
	char str[ARR_SIZE] = {0};

	char op;
	printf("Welcome to Homework 8, please choose one of the parts to continue\n");
	printf("*****************\n");
	printf("1) Execute Part 1\n");
	printf("2) Execute Part 2\n");
	printf("3) Execute Part 3\n");
	printf("4) Exit			 \n");

	scanf(" %c", &op);
	if(op == '1' || op == '2' || op == '3' || op == '4')
	{
		switch(op)
		{
			case '1':
				part1(arr, n, max_value, index, &mean);
				break;
			case '2':
				part2(visited, x, y, orientation);
				break;
			case '3':
				part3(str);
				break;
			case '4':
				printf("Good Bye!\n");
				exit(0);
				break;
		}
	}
	else
	{
		printf("Invalid input. Please try again.\n");
		menu();
	}
}

//shows menu for part1 and executes functions according to user input.
//this part will show up until '5' is inputted.
void part1(int arr[], int n, int max_value, int index, double *mean)
{		
	char op;
	printf("Please make your choice:	   \n");
	printf("************************	   \n");
	printf("1) Fill Array				   \n");
	printf("2) Find Biggest Number	       \n");
	printf("3) Calculate Sum			   \n");
	printf("4) Calculate Standart Deviation\n");
	printf("5) Exit						   \n");
	
	scanf(" %c", &op);
	if(op == '1' || op == '2' || op == '3' || op == '4' || op == '5')
	{	
		switch(op)
		{
			case '1':
				printf("Please enter range:");
				scanf(" %d", &n);
				generate_hofstadters_sequence(arr, n);
				part1(arr, n, max_value, index, mean);
				break;
			case '2':
				printf("Max value is => %d\n", find_max(arr, n, max_value));
				part1(arr, n, max_value, index, mean);
				break;
			case '3':
				printf("Sum=>%d\n", sum_array(arr));
				//initializing again to prevent overlapping
				arr[0] = 0; 
				arr[1] = 1;
				part1(arr, n, max_value, index, mean);
				break;
			case '4':
				printf("Standart Deviation =>%f\n", std_array(arr, mean, n, index));
				printf("Mean =>%f\n", *mean);
				//initializing again to prevent overlapping
				index  = 1;
				arr[0] = 0;
				part1(arr, n, max_value, index, mean);
				break;
			case '5':
				printf("\nReturning to the main menu\n\n");
				menu();
				break;
		}
	}
	else
	{
		printf("Invalid input. Please try again.\n");
		part1(arr, n, max_value, index, mean);
	}
}

// calls itself until n(range) becomes 1 and holds equalities in related array indexes
void generate_hofstadters_sequence(int *arr, int n)
{
	if(n == 0)
		arr[1] = 0;
	else if(n == 1)
		arr[n] = 1;
	else if(n == 2)
	{
		generate_hofstadters_sequence(arr, n-1);
		arr[n] = 1;
	}
	else
	{
		generate_hofstadters_sequence(arr, n-1);
		arr[n] = arr[n-arr[n-1]]+arr[n-arr[n-2]];
	}
}

//calls itself until index becomes 1 and then checks every time if max_value is bigger then arr[index] or not. 
//If not then changes its value with arr[index].
int find_max(int arr[], int index, int max_value)
{
	if(index == 1)
		return max_value;

	max_value = find_max(arr, index-1, max_value);

	if(max_value < arr[index])
		max_value = arr[index];	

	return max_value;
}

//arr[0] is sum since it is initialized with 0.
//arr[1] is index counter since it is initialized with 1.
//it calls itself until arr[1] cell is equals to total number of items then returns arr[0] which is sum.
int sum_array(int arr[])
{
	int index = 1;
	
//	sum    = sum    + arr[index]
	arr[0] = arr[0] + arr[arr[1]];

// 	index  = 1     + index
	arr[1] = index + arr[1];

	if(arr[arr[1]] != 0)
		sum_array(arr);

	return arr[0]; 
}

/*
* arr[0] is sum
* arr[1] is equal to total num of items(n) + 1
* returns standart deviation as 0 if array size is less then 2
* if not, then increases index until arr[index] equals to '\0' 
* and calculates std, which is equal to sum of difference *mean-arr[index]. 
* finally returns standart deviation when first func call continues again.
*/
double std_array(int arr[], double *mean, int n, int index)
{
	double std = 0;
	arr[0] = arr[0] + arr[index];

	if(arr[2] == 0 || arr[1] == 0)
	{
		*mean = arr[1];
		return 0.0;
	}
	else
	{
		if(arr[index] == 0)
			*mean = arr[0]/n*1.0;

		else if(arr[index] != 0)
			std_array(arr, mean, n, index+1);

		std =  std + (*mean - (arr[index]*1.0))*(*mean - (arr[index]*1.0));

		if(index == 1)
			return sqrt(std/(n-1));
	}
}

/*
* executes the pipe function when user inputted '1'
* and initializes some array values if user wants to executes again.
* this part will show up until '0' is inputted.
*/
void part2(int visited[N][N], int x, int y, int orientation)
{
	//I could've first define visited array in this func but 
	//since I define all variables in menu I didn't want to bring one just because can't initialize without a loop.	 
	
	//initializing visited array with '0' to make it work properly when '1' inputted more than once.
	for(int i=0; i<9; i++)
		for(int j=0; j<9; j++)
			visited[i][j] = 0;

	char op;
	printf("\nPress 1 to execute code or 0 to quit: \n");
	
	scanf(" %c", &op);
	if(op == '0' || op == '1')
	{
		switch(op)
		{
			case '0':
				printf("\nReturning to the main menu\n\n");
				menu();
				break;
			case '1':
				//initializing some values.
				visited[8][8] = 1; //shows which number printed into a pipe(ex: 0,0 => 1 | 2,1 => 2 | 4,2 => 3) and goes until 64 which is last empty space
				visited[8][7] = 1; //counts how many proper solution has found and printed
				visited[8][6] = 2; //shows same thing as visited[8][8] does but this is for finding orientation set of found solution
				visited[8][3] = 1; //it is a flag which allows to print "A proper orientation set is" part. Without this it prints this message each time an orientation is found(which total of 64*10).
				printf("It didn't crash. Please wait until it finds 10 proper solution.\n");
				install_pipes(visited, 0, 0, 1);
				break;
		}
	}
	else
	{
		printf("Invalid input. Please try again.\n");
		part2(visited, x, y, orientation);
	}
}

/*				    x   y
* Orientation1 => (+2, +1) 
* Orientation2 => (+1, -2) 
* Orientation3 => (-2, -1) 
* Orientation4 => (-1, +2) 
* Orientation5 => (+1, +2) 
* Orientation6 => (+2, -1) 
* Orientation7 => (-1, -2) 
* Orientation8 => (-2, +1) 
*/

/*
* this func contains 2 part.
* first part looks for all possible ways to cover field and when it founds one, then deletes last pipes and searchs for another solution.
* second part takes a solution and visits its all route by starting 1 until 64.
* and prints which orientation is used each time(does this 10 time then goes back to part2)
*/
void install_pipes(int visited[N][N], int x, int y, int orientation)
{
	visited[x][y] = visited[8][8];

	// ******* PART 1 *******
	//(pipe number   > 63 && printed count < 11)
	if(visited[8][8] > 63 && visited[8][7] < 11)
	{
		//(     flag     == 1 && pipe number(2)< 65)
		if(visited[8][3] == 1 && visited[8][6] < 65)
		{
			printf("A proper orientation set is:\n{ ");
		}
		if(visited[8][6] < 65)
		{ 
			// if any condition is satisfied, before calling function again increases or decreases x and y values according to related orientation and sets orientation with its equivelent number. 

			//  visited[ (      x      )+2 ][ (      y      )+1 ] ==     2 ==> 64  && (      x      )+2<8	 (      x      )+2>=0    (      y      )+1<8	(      y      )+1>=0)
			if( visited[ (visited[8][4])+2 ][ (visited[8][5])+1 ] == visited[8][6] && (visited[8][4])+2<8 && (visited[8][4])+2>=0 && (visited[8][5])+1<8 && (visited[8][5])+1>=0) 
			{	
				orientation = 1;				
				//    x       =       x       + 2;
				visited[8][4] = visited[8][4] + 2;
				//    y       =       y       + 1
				visited[8][5] = visited[8][5] + 1;
			}
			else if( visited[ (visited[8][4])+1 ][ (visited[8][5])-2 ] == visited[8][6] && (visited[8][4])+1<8 && (visited[8][4])+1>=0 && (visited[8][5])-2<8 && (visited[8][5])-2>=0) 
			{	
				orientation = 2;
				visited[8][4] = visited[8][4] + 1;
				visited[8][5] = visited[8][5] - 2;
			}
			else if( visited[ (visited[8][4])-2 ][ (visited[8][5])-1 ] == visited[8][6] && (visited[8][4])-2<8 && (visited[8][4])-2>=0 && (visited[8][5])-1<8 && (visited[8][5])-1>=0) 
			{	
				orientation = 3;
				visited[8][4] = visited[8][4] - 2;
				visited[8][5] = visited[8][5] - 1;
			}
			else if( visited[ (visited[8][4])-1 ][ (visited[8][5])+2 ] == visited[8][6] && (visited[8][4])-1<8 && (visited[8][4])-1>=0 && (visited[8][5])+2<8 && (visited[8][5])+2>=0)
			{	
				orientation = 4;
				visited[8][4] = visited[8][4] - 1;
				visited[8][5] = visited[8][5] + 2;
			}
			else if( visited[ (visited[8][4])+1 ][ (visited[8][5])+2 ] == visited[8][6] && (visited[8][4])+1<8 && (visited[8][4])+1>=0 && (visited[8][5])+2<8 && (visited[8][5])+2>=0)
			{	
				orientation = 5;
				visited[8][4] = visited[8][4] + 1;
				visited[8][5] = visited[8][5] + 2;
			}
			else if( visited[ (visited[8][4])+2 ][ (visited[8][5])-1 ] == visited[8][6] && (visited[8][4])+2<8 && (visited[8][4])+2>=0 && (visited[8][5])-1<8 && (visited[8][5])-1>=0)
			{	
				orientation = 6;
				visited[8][4] = visited[8][4] + 2;
				visited[8][5] = visited[8][5] - 1;
			}
			else if( visited[ (visited[8][4])-1 ][ (visited[8][5])-2 ] == visited[8][6] && (visited[8][4])-1<8 && (visited[8][4])-1>=0 && (visited[8][5])-2<8 && (visited[8][5])-2>=0)
			{	
				orientation = 7;
				visited[8][4] = visited[8][4] - 1;
				visited[8][5] = visited[8][5] - 2;
			}
			else if( visited[ (visited[8][4])-2 ][ (visited[8][5])+1 ] == visited[8][6] && (visited[8][4])-2<8 && (visited[8][4])-2>=0 && (visited[8][5])+1<8 && (visited[8][5])+1>=0)
			{	
				orientation = 8;
				visited[8][4] = visited[8][4] - 2;
				visited[8][5] = visited[8][5] + 1;
			}
			printf("O%d ", orientation);
			//setting flag as 0 to prevent overwriting.
			visited[8][3] = 0;
			//increaseing pipe number(until 64)
			visited[8][6] = visited[8][6] + 1;
			//calling again with new x and y values.
			install_pipes(visited, x, y, 1);
			return;
		}
		else
		{
			printf("}\n\n");
	    	
	    	//you can just delete the comment brackets to see each solved field with their pipe nums. they look a bit similar(of course not equal) but it's because it gets first 10 solution. 
	    	//as code progress further the difference between first and last one gets bigger. 
		    /*
		    for(int i=0; i<8; i++)
			{
				for(int j=0; j<8; j++)
					printf("%d 	", visited[i][j]);
				printf("\n");	
			}
			printf("\n\n");
			*/

			//when an orientation set is completely found initializes again all necessary values for another set.
			visited[8][3] = 1;
			visited[8][4] = 0;
			visited[8][5] = 0;
			visited[8][6] = 2;

			//increasing found solution num.
			visited[8][7]++; 
			
			//returning to menu
			if(visited[8][7] == 11)
			{
				part2(visited, 0, 0, 1);
				return;
			}
		}
	}
	// ******** PART 2 *********
	else
	{ 	
		//if every condition is satisfied, then increases pipe number by one and calls func with new x and y values.
		//after last pipe initialized and doesn't mather if it reached 64 or not if there is no possible move left
		//then sets its x and y value to 0, decreases pipe number by one and tries another orientation.
		if(orientation == 1 && visited[x+2][y+1] == 0 && (x+2>=0 && x+2<8 && y+1>=0 && y+1<8))
		{
			visited[8][8]++;
			install_pipes(visited, x+2, y+1, 1);
			visited[8][8]--;
			visited[x+2][y+1] = 0;
		}
		orientation = 2;
		if(orientation == 2 && visited[x+1][y-2] == 0 && (x+1>=0 && x+1<8 && y-2>=0 && y-2<8))
		{
			visited[8][8]++;
			install_pipes(visited, x+1, y-2, 1);
			visited[8][8]--;
			visited[x+1][y-2] = 0;
		}
		orientation = 3;
		if(orientation == 3 && visited[x-2][y-1] == 0 && (x-2>=0 && x-2<8 && y-1>=0 && y-1<8))
		{
			visited[8][8]++;
			install_pipes(visited, x-2, y-1, 1);
			visited[8][8]--;
			visited[x-2][y-1] = 0;
		}
		orientation = 4;
		if(orientation == 4 && visited[x-1][y+2] == 0 && (x-1>=0 && x-1<8 && y+2>=0 && y+2<8))
		{			
			visited[8][8]++;
			install_pipes(visited, x-1, y+2, 1);
			visited[8][8]--;
			visited[x-1][y+2] = 0;
		}
		orientation = 5;
		if(orientation == 5 && visited[x+1][y+2] == 0 && (x+1>=0 && x+1<8 && y+2>=0 && y+2<8))
		{
			visited[8][8]++;
			install_pipes(visited, x+1, y+2, 1);
			visited[8][8]--;
			visited[x+1][y+2] = 0;
		}
		orientation = 6;
		if(orientation == 6 && visited[x+2][y-1] == 0 && (x+2>=0 && x+2<8 && y-1>=0 && y-1<8))
		{
			visited[8][8]++;
			install_pipes(visited, x+2, y-1, 1);
			visited[8][8]--;
			visited[x+2][y-1] = 0;
		}
		orientation = 7;
		if(orientation == 7 && visited[x-1][y-2] == 0 && (x-1>=0 && x-1<8 && y-2>=0 && y-2<8))
		{
			visited[8][8]++;
			install_pipes(visited, x-1, y-2, 1);
			visited[8][8]--;
			visited[x-1][y-2] = 0;
		}
		orientation = 8;
		if(orientation == 8 && visited[x-2][y+1] == 0 && (x-2>=0 && x-2<8 && y+1>=0 && y+1<8))
		{
			visited[8][8]++;
			install_pipes(visited, x-2, y+1, 1);
			visited[8][8]--;
			visited[x-2][y+1] = 0;
		}
	}
}

/*
* asks for a string from user and sends the string to remove_duplicates func.
* after removing prints the string again
* this part will show up until '0' is inputted.
*/
void part3(char *str)
{
	//initializing str with 0 to prevent overwriting.
	*str = 0;

	printf("Please enter a string(just enter 0 to exit): ");
	scanf(" %[^\n]s", str);
	
	if(str[0] == '0' && str[1] == '\0')
	{
		printf("\nReturning to the main menu\n\n");	
		menu();
	}
	else
	{
		remove_duplicates(str);
		printf("%s\n", str);
		part3(str);
	}
}

/*
* if the length of the string is equal to 1, returns it.
* if not, then compares two adjacent characters from the start
* if they are equal then sets second adjacent character to '0' as a char variable.
* then with strtod() func, seperates left and right of the '0' char. 
* after strtod func is done this time sets '0' char to '\0'. (if char is not set to '\0' then strcat func doesn't overwrite string. ex: aabb => a0bb => a0bb(bb) )
* after setting '\0' strcat overwrites into the second adjacent char. (ex: aabbcc => a0bbcc => a\0bbcc => abbcc)
* if they are not equal, then calls the func with increased index. 
* func recurse until the second adjacent character is equal to '\0'.
*/
char* remove_duplicates(char *str)
{
	//for strtod() func
	char *ptr;

	if(strlen(str) == 1)
		return str;

	if(*(str+1) != '\0')
	{
		if(*str == *(str+1))
		{
			*(str+1) = '0';
			strtod((str+1), &ptr);
			*(str+1) = '\0';
			strcat(str, ptr);
			return remove_duplicates(str);
		}
		else
		{
			return remove_duplicates(str+1);
		}
	}
	else
		return 0;
}