#include <stdio.h>
int sum(int n, int flag);
int mult(int n, int flag);

int main(){
	int n, flag, flag2;
	printf("Enter an integer:");
	scanf("%d", &n);

	printf("Please enter '0' for sum, '1' for multiplication:\n");
	scanf("%d", &flag);
	
	printf("Please enter '0' to work on even numbers, '1' to work on odd numbers:\n");
	scanf("%d", &flag2);
	
	if(flag!=0 && flag!=1){ //will give error if first flag is not either 1 or 0.
		printf("Unsported operation.");
		return 0;
	}
	if(flag2!=0 && flag2!=1){ //will giive error if the second flag is not either 1 or 0. Instead of asking for the flags again I finished the code.
		printf("Invalid value for odd/even selection.");
		return 0;
	}
	
	switch(flag)
	{
		case 0:{
			printf("=%d", sum(n, flag2)); //Only sum is printed here. Loops are printed in the function.
			break;
		}
		case 1:{
			printf("=%d", mult(n, flag2));	//Only result of the multiplication is printed here. Loops are printed in the function.
			break;
		}
	}
}

int sum(int n, int flag)
{
	if (flag==0){	
		int sum=2;	//if I didn't declare this int before the loop starts, it will be look like 2+4+6+=12. Meaning plus sign would be in the front.
		printf("2"); //to prevent this happen I put first value for both cases before the loop then put the plus sign before the values. 
		for(int i=3; i<=n; i++)
			if(i%2==0){	 
				printf("+%d", i);	//this will print "i" value if the "i" value is even.
				sum+=i;	//Calculated sum'till end of the loop then printed it in the main. 
			}
		return(sum);
	}
	
	else if (flag==1){ //same things here as well..
		int sum=1;
		printf("1");
		for(int i=2; i<=n; i++)
			if(i%2==1){
				printf("+%d", i);
				sum+=i;
			}
		return(sum);
	}
}

int mult(int n, int flag)
{
	if (flag==0){ //literally just changed the plus sign into mult sign c:
		int mult=2;
		printf("2");
		for(int i=3; i<=n; i++) 
			if(i%2==0){
				printf("*%d", i);
				mult*=i;
			}
		return(mult);
	}

	else if (flag==1){ 
		int mult=1;
		printf("1");
		for(int i=2; i<=n; i++)
			if(i%2==1){
				printf("*%d", i);
				mult*=i;
			}
		return(mult);
	}
}