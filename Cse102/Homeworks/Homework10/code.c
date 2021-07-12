#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void menu();
//part1
void part1		  (int street, int avenue);
int  numPathsHome (int street, int avenue);
//part2
struct Hospital
{
	char *name;
	char citiesServed[3];
} results[4];
void part2				   (char _cities[6], char _locations[5][4], int _numHospitals, struct Hospital results[4]); // changed [4][3] to [4][4] because of \0 character is missing when it is 3. also array acts unreliable.
int  canOfferCovidCoverage (char _cities[6], char _locations[5][4], int _numHospitals, struct Hospital results[4]); // changed [4][4] to [5][4] to see if the array reached to 0 or not.
int  checkCities	  	   (char _cities[6], struct Hospital results[4]);
//part3
struct card
{
	const char *face;
	const char *suit;
} deck[52] = {NULL};
void part3(struct card deck[52]);
void randNum(int totalnum, int *randNum);


int main()
{
	menu();
	return 0;
}

void menu()
{
	srand(time(NULL));
	//variables for part1
	int street = 0;
	int avenue = 0;
	//variables for part2
	char cities[6]       = {'A', 'B', 'C', 'D', 'E', 'F'};
	char locations[5][4] = {"ABC", "ACD", "BF", "CEF", "0"}; 
	int  numHospitals	 = {0};

	char op;
	printf("Welcome to Homework 9, please choose a part to continue\n");
	printf("*********\n");
	printf("1) Part 1\n");
	printf("2) Part 2\n");
	printf("3) Part 3\n");
	printf("4) Exit  \n");

	scanf(" %c", &op);

	if(op == '1' || op == '2' || op == '3' || op == '4')
	{
		switch(op)
		{
			case '1':
				part1(street, avenue);
				break;
			case '2':
				part2(cities, locations, numHospitals, results);
				break;
			case '3':
				part3(deck);
				break;
			case '4':
				printf("Good Bye!\n");
				break;
		}
	}
	else
	{
		printf("Invalid input! Try again.\n");
		menu();
	}
}

// executes first task. works until (n) is inputted from the user. 
void part1(int street, int avenue)
{
	char op;
	//street
	printf("Enter the street number: ");
	scanf(" %d", &street);
	if(street < 1 || street > 5)
	{
		printf("Invalid input! The street number must be between 1-5.\n");
		part1(street, avenue);
		return;
	}
	printf("street:%d\n", street);
	//avenue
	printf("Enter the avenue number: ");
	scanf(" %d", &avenue);
	if(avenue < 1 || avenue > 5)
	{
		printf("Invalid input! The avenue number must be between 1-5.\n");
		part1(street, avenue);
		return;
	}
	printf("avenue:%d\n", avenue);
	//result
	printf("Number of optimal paths to take back home: %d\n", numPathsHome(street, avenue));
	printf("To continue (y)\nTo return to the menu (n)\n");
	scanf(" %c", &op);
	//exe part1 or return to menu
	if(op == 'y')
		part1(street, avenue);

	else if(op == 'n')
		menu();
}


/*	Purpose 		 => To find how many paths are there with the same least step.
	Finish condition => With returning 1 when one of the street or avenue number becomes 1 (because there will be only one way to the home [edges in the graph below])
	Recursion 		 => Decreases street number by one until it becomes one but avenue number stays same
					 => Decreases avenue number by one until it becomes one but street number stays same
					 => 1	5	15	35	70 (shows total number of paths according to coordinates)
					 	1	4	10	20	35
					 	1	3	6	10	15
					 	1	2	3	4	5
					 	1	1	1	1	1
					 => Codes simply recurse this graph. Once it reaches street num 1 then starts to sum up and return until inputted number comes.
					 => Then decreases avenue by one and does the same thing again until avenue becomes 1
					 => Ex: street: 4 | avenue: 4 the sum until the final return will be like  = (2->3->4) + (2->3) + (2->3)
					 																   		   = 4 + 6 (sum of (2,4) and (3,3))
					 																	  	   = 10 (3,4)
					 																	  	   (this time reverse)	  
																						  	   = (2->3) + (2->3)
					 																	       = 6
					 																	  	   = (2->3->4) + 6 (sum of (4,2) and (3,3))
					 																	  	   = 10 (4,3)
					 																	  	   (final sum)
					 																	  	   = (3, 4) + (4, 3)
					 																	  	   =   10   +   10 => 20	  
*/
int numPathsHome(int street, int avenue)
{
	int sum1 = 0;
	int sum2 = 0;
	if(street == 1 || avenue == 1)
		return 1;

	sum1 = numPathsHome(street-1, avenue);
	sum2 = numPathsHome(street, avenue-1);
	return sum1 + sum2;
}

//executes second task. works until (n) is inputted from the user. 
void part2(char _cities[6], char _locations[5][4], int _numHospitals, struct Hospital results[4])
{
	char op;
	//getting input
	printf("Enter the maximum number of hospitals that can be constructed:");
	scanf(" %d", &_numHospitals);
	if(_numHospitals<0 || _numHospitals>4)
	{
		printf("Invalid input! Please enter a number between 0-4\n");
		part2(_cities, _locations, _numHospitals, results);
		return;
	}
	results[4].citiesServed[0] = '0'; //to stop printing after getting solution
	//printing the result
	if(canOfferCovidCoverage( _cities, _locations, _numHospitals, results) == 1)
		for(int i=0; i<_numHospitals; i++)
			printf("Hospital - %d\nHospital Locations:%s\n", i+1, results[i].citiesServed);
	else
		printf("No, some cities are not covered.\n");
	//initializing some struct variables to make function work properly again
	for(int i=0; i<4; i++)
		results[i].citiesServed[1] = '0';
	//exe part2 or return to menu
	printf("To continue (y)\nTo return to the menu (n)\n");
	scanf(" %c", &op);
	if(op == 'y')
		part2(_cities, _locations, _numHospitals, results);
	else if(op == 'n')
		menu();
}

/* Purpose 			=> To check if the inputted number of hospitals covers all cities or not.
   Finish condition => First ending conditin is when the location array becomes to '0'. It means we looked for all possibilities.
  					=> Second ending condition is when inputted number becomes to 0. It is used as index.
   Recursion		=> First, code increases _locations[5][4] array by 1 until it's first element becomes '0'. When it becomes '0' and returns 0 
					=> function continues and starts to copy all locations to "result.citiesServed" array. when it copies a location then decreases "_numHospitals" int
					=> and calls func again. until it becomes 0. Then checkes if the "result.citiesServed" captures all the cities or not. If so then returns 1
					=> and in the part2 func it prints the result. If not then function works until _locations[0][0] becomes '0' and if still there is no solution then returns 0.
*/
int canOfferCovidCoverage(char _cities[6], char _locations[5][4], int _numHospitals, struct Hospital results[4])
{
	//first finish condition
	if(_locations[0][0] == '0')
		return 0;
	//second finish condition
	if(_numHospitals == 0)
	{
		if(checkCities(_cities, results) == 1)
		{
			results[4].citiesServed[0] = '1';
			return 1;
		}
		else
			return 0;
	}

	//first call
	canOfferCovidCoverage(_cities, _locations+1, _numHospitals, results);	

	//copying _locations into result.citiesServed by 1 each time
	if(_locations[_numHospitals-2][0] != '0' && results[4].citiesServed[0] != '1')
		strcpy(results[_numHospitals-1].citiesServed, _locations[_numHospitals-1]);
	
	//second call
	canOfferCovidCoverage(_cities, _locations, _numHospitals-1, results);
}

//checks if the cities are covered or not. If so returns one else returns 0
int checkCities(char _cities[6], struct Hospital results[4])
{
	char temp[15] = {0};
	int count = 0;
	int flag = 0;

	//marks flag if there are same city locations
	for(int i=0; i<4; i++)
		for(int j=i+1; j<4; j++)
			if(results[i].citiesServed[1] == results[j].citiesServed[1] && results[i].citiesServed[1] != '\0')
				flag = 1;

	//using temp to check easier
	strcat(temp, results[0].citiesServed);
	strcat(temp, results[1].citiesServed);
	strcat(temp, results[2].citiesServed);
	strcat(temp, results[3].citiesServed);

	//increases count by 1 if a city is provided
	for(int i=0; i<6; i++)
		for(int j=0; j<strlen(temp); j++)
			if(_cities[i] == temp[j])
			{
				count++;
				break;
			}

	if(count == 6 && temp[strlen(temp)-1] != '0' && flag == 0)
		return 1;
	else
		return 0;
}

//executes third task. works until (n) is inputted from the user. 
void part3(struct card deck[52])
{
	char op;
	//deck
	char *faces[] = {"Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};
	char *suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
	//rand nums for shuffling
	int randNum_face[52];
	int randNum_suit[52];
	//getting rand nums
	for(int i=0; i<4; i++)
	{
		randNum(13, randNum_face);
		for(int j=0; j<13; j++)
			deck[j+(13*i)].face = faces[randNum_face[j]];
	}
	for(int i=0; i<13; i++)
	{
		randNum(4, randNum_suit);
		for(int j=0; j<4; j++)
			deck[j+(4*i)].suit = suits[randNum_suit[j]];
	}
	//printing struct
	for(int i=0; i<26; i++)
		printf("%9.9s of %s 	%9.9s of %s\n", deck[i].face, deck[i].suit, deck[i+26].face, deck[i+26].suit);
	//exe part3 or return to menu
	printf("To continue (y)\nTo return to the menu (n)\n");
	scanf(" %c", &op);
	if(op == 'y')
		part3(deck);
	else if(op == 'n')
		menu();
}

//generates rand numbers but every number is different. If finds equal numbers then generates until it can't find one.
void randNum(int totalnum, int *randNum)
{
	int flag = 1;
	while(flag == 1)
	{
		flag = 0;
		for(int i=0; i<totalnum; i++)
			randNum[i] = rand()%totalnum;
		for(int i=totalnum-1; i>0; i--)
			for(int j=i-1; j>=0; j--)
				if(randNum[i] == randNum[j])
					flag = 1;
	}
}