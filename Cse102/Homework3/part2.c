#include <stdio.h>
#include <math.h>
int number_length(int number);
int find_digit(int number, int index);

int main()
{
	int number, length, index, th;
	printf("Enter a number (maximum 6 digits):\n");
	scanf("%d", &number);
	
	length=number_length(number);

	if ( (length<=6) && (length>=0) ) // to keep length between 1-6 digits.
	{
		printf("Your number has %d digits\n", length);	
	}
	else
	{
		printf("Error! You must give numbers between 1-6 digits.\n");
		return 0;
	}

	printf("When your number is written 100 times next to each other, which digit of this number would you like to see?:");
	scanf("%d", &index);

	th=find_digit(number, index);
	printf("%d.th digit of the big number sequence: %d\n", index, th);

	return 0;
}

int number_length(int number)
{
	return(log10(number)+1);
}

int find_digit(int number, int index)
{
	int length, k, d1, d2, d3, d4, d5, d6;
	length=number_length(number);
	k=(index%length); // k refers to remaining.
	if (k==0)	// I did this because when k equals to zero the whole code collapses. Actually if k is 0 it also means that it is equals to it's own length.
	{
		k=length;
	}
	if (length==6)
	{
		d1=(number/pow(10, 5)); //ex:123456/100000 = 1
		d2=(number/pow(10, 4)); //ex:123456/10000 = 12
		d3=(number/pow(10, 3)); //ex:123456/1000 = 123
		d4=(number/pow(10, 2)); //ex:123456/100 = 1234
		d5=(number/pow(10, 1));	//ex:123456/10 = 12345
		d6=(number/pow(10, 0));	//ex:123456/1 = 123456
		if(k==1)
			return(d1);	// for number 123456, examples are below...
		else if(k==2)
			return(d2%(d1*10));	// ex: 12%10 = 2
		else if(k==3)
			return(d3%(d2*10)); // ex: 123%120 = 3
		else if(k==4)
			return(d4%(d3*10)); // ex: 1234%1230 = 4
		else if(k==5)
			return(d5%(d4*10)); // ex: 12345%12340 = 5
		else if(k==6)
			return(d6%(d5*10)); // ex: 123456%123450 = 6
	}
	if (length==5)
	{
		d1=(number/pow(10, 4)); // same example can be writen for number 12345.
		d2=(number/pow(10, 3)); 
		d3=(number/pow(10, 2)); 
		d4=(number/pow(10, 1));
		d5=(number/pow(10, 0));	
		if(k==1)
			return(d1);
		if(k==2)
			return(d2%(d1*10));
		if(k==3)
			return(d3%(d2*10));
		if(k==4)
			return(d4%(d3*10)); 
		if(k==5)
			return(d5%(d4*10));
	}
	if (length==4)
	{
		d1=(number/pow(10, 3)); // same example can be writen for number 1234.
		d2=(number/pow(10, 2)); 
		d3=(number/pow(10, 1));
		d4=(number/pow(10, 0));	
		if(k==1)
			return(d1);
		if(k==2)
			return(d2%(d1*10));
		if(k==3)
			return(d3%(d2*10));
		if(k==4)
			return(d4%(d3*10));
	}
	if (length==3)
	{
		d1=(number/pow(10, 2)); // same example can be writen for number 123.
		d2=(number/pow(10, 1));
		d3=(number/pow(10, 0));	
		if(k==1)
			return(d1);
		if(k==2)
			return(d2%(d1*10));
		if(k==3)
			return(d3%(d2*10));		
	}
	if (length==2)
	{
		d1=(number/pow(10, 1)); // same example can be writen for number 12.
		d2=(number/pow(10, 0));
		if(k==1)
			return(d1);
		if(k==2)
			return(d2%(d1*10));
	}
	if (length==1)
		return(number);
}











