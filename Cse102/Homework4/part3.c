#include <stdio.h>
int andop(int a, int b);

int main(){

	int bin1, bin2, temp, count1=1, count2=1, isbin;
	printf("First Integer: ");
	scanf("%d", &bin1);
	printf("Second Integer: ");
	scanf("%d", &bin2);
	
	temp=bin1;
	while(temp!=0){ //this loop divides first integer by 10 'till it equals to zero
		isbin=temp%10;	//I get every last digit before dividing by 10 and check if it is either 1 or 0. If not it gives error and asks for new integers.
		temp/=10;
		count1++; //Also by adding 1 each time to count int, comparison of lengths between first bin and second bin is possible.

		if(isbin>1){
			printf("Integers should be binary, please enter 2 new integers.\n");
			main();
			return 0;
		}
	}

	temp=bin2; // I use temp because didn't want to change real value of inputted numbers.
	while(temp!=0){	//same thing here as well.
		isbin=temp%10;
		temp/=10;
		count2++;
		if(isbin>1){
			printf("Integers should be binary, please enter 2 new integers.\n");
			main();
			return 0;
		}
	}

	while(count1!=count2){ //If lengths of the inputted numbers are not equal this loop gives error and asks for new integers.
		printf("Integers should have the same length, please enter 2 new integers.\n");
		main();
		return 0;
	}

	printf("%d AND %d = %d", bin1, bin2, andop(bin1, bin2)); 
}

int andop(int a, int b){
	int sum, temp, temp2=0, lastdig, and=0, count=0, mult=1;
	sum=a+b;	//sum of two binary numbers is key here 'cause with AND operator the digit which must be "1", also is "1" in each two binary numbers.
	temp=sum;	//this means in ex: 1100 AND 1011 = 1000 also 1100 + 1011 = 2111. If a digit is equals to "2" then it means that digit is also equal to "1" in AND op. 
	while(temp!=0)	//"while" loop here looks every digit 'till temp is equal to zero. 
	{
		lastdig=temp%10;
		if(lastdig==2){	//if any digit equals to "2" then it will get in to another loop. In this loop temp2 increases by 1 each time until equals to count. 
			while(temp2!=count){	//"count" int counts which digit we are right now. by doing so we can know where we should put our digit "1".
				mult*=10;
				temp2++;
			}
			and=and+mult; //after putting digit 1, we must hold our sum in a variable. this "and" int meets our need. 
		}
		temp/=10;
		count++;
	}
	return(and); // the value returning now equals to bin1 AND bin2 
}