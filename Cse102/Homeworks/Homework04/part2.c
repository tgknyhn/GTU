#include <stdio.h>
#include <math.h>
int isprime(int a);

int main()
{
	int num;
	printf("Please enter an integer:");
	scanf("%d", &num);

	for(int i=2; i<num; i++){ //loop will take all the numbers starting from 2'till the given number and will check if num is dividable by it. 
		if(isprime(i)==0)
			printf("%d is a prime number.\n", i);
		else
			printf("%d is not a prime number, it is dividible by %d\n", i, isprime(i));
	}
	return 0;
}

int isprime(int a) //this will return 0 if the given number is prime number. Else will return the least divisor of the number.
{
	int flag=1, divisor;
	for(int i=2; i<=sqrt(a); i++) //loop takes all numbers between 2 and sqrt of the given number.
		if(a%i==0){	//however, if one of the numbers is divisor of the given number then marks the flag as 0 and 
			flag=0; //takes that divisor into an int variable then quits the loop.
			divisor=i;
			break;
		}
	if(flag==0)
		return(divisor);
	else
		return(0);
}
