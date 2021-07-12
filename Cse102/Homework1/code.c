#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void find_root();
void find_newtonian_root();
int find_multiple_closest(int a, int b);

int main()
{
	printf("PART1\n");
	find_root();
	
	printf("PART2\n");
	find_newtonian_root();

	printf("PART3\n");
	int num1;
	int num2;
	int sonuc;

	printf("Enter the first integer:");
	scanf("%d", &num1);

	printf("Enter the second integer:");
	scanf("%d", &num2);

	sonuc=find_multiple_closest(num1, num2);

	if (num1==sonuc)
	{
		printf("Closest number to %d that is multiple of %d is itself", num1, num2);
	}
	else
	{
		printf("Closest number to %d that is multiple of %d is %d", num1, num2, sonuc);
	}
	
	return 0;
}

void find_root()
{
	int a, b, c;
    double dsc, i;
    float root1, root2;
    
    printf("Please enter the first coefficient:");
    scanf("%d", &a);

    printf("Please enter the second coefficient:");
    scanf("%d", &b);

    printf("Please enter the third coefficient:");
    scanf("%d", &c);

    dsc=(b*b)-(4*a*c); //finding discriminant
    i=sqrt(dsc);	//I used "i" to describe square root of discriminant for easy writing

    root1=(-b-i)/(2*a);	
    root2=(-b+i)/(2*a);
    
    if (dsc<0)	// I could've used "i" here as well but since they re not equal to zero it doesn't matter which one I use. 
    {
        printf("Your equation  %+dx^2%+dx%+d does not have any real roots.\n", a, b, c); 
    }
    else
    {
        printf("Your equation %+dx^2%+dx%+d have real roots {%.1f,%.1f}\n", a, b, c, root1, root2); // for some quadratic equations it was rounding the roots so.. 
    }																								//..I used float to pretend it and described my precise with %.1f.
}


void find_newtonian_root()
{
	int a, b, c, temp1, temp2;
    float i, dfx, fx, x, root1, root2, temp;
    double dsc, l;

    printf("Please enter the first coefficient:");
    scanf("%d", &a);

    printf("Please enter the second coefficient:");
    scanf("%d", &b);

    printf("Please enter the third coefficient:");
    scanf("%d", &c);

    printf("Please enter the initial:");
    scanf("%f", &i);

    dsc=(b*b)-(4*a*c); //finding discriminant
    l=sqrt(dsc);	//this time I used "l" to describe square root of discriminant for easy writing
 
    root1=(-b-l)/(2*a); 
    root2=(-b+l)/(2*a);

    fx=(a*i*i)+(b*i)+(c); // quadratic equation 	
    dfx=(2*a*i)+(b); // derivative of q.e. 		
    x=i-(fx/dfx);  // newton's law 		

    temp1=abs(root1-x);		//I didn't want to write it in long way so I put the root differences into temps.
    temp2=abs(root2-x);	

    if (temp1<temp2)	//by doing so I can find which root I found is the closest to "x" then use it for showing difference part
    {
    	temp=root1;
    }
    else
    {
    	temp=root2;
    }   

    printf("Your equation is  %+dx^2%+dx%+d and the iterations are:\n", a, b, c);	
    printf("Step        x    	 fx		Differece\n");
    
    i=x;					//I'm repeating my equations to find new values five times in a row
    fx=(a*i*i)+(b*i)+(c); 
    dfx=(2*a*i)+(b); 

    printf("x1        %.4f      %.4f		%.4f\n", x, fx, x-temp); 

    x=i-(fx/dfx);   
    i=x;
    fx=(a*i*i)+(b*i)+(c); 
    dfx=(2*a*i)+(b); 
        
    printf("x2        %.4f      %.4f		%.4f\n", x, fx, x-temp); 

    x=i-(fx/dfx);   
    i=x;
    fx=(a*i*i)+(b*i)+(c); 
    dfx=(2*a*i)+(b); 
        
    printf("x3        %.4f      %.4f		%.4f\n", x, fx, x-temp); 

    x=i-(fx/dfx);    
    i=x;
    fx=(a*i*i)+(b*i)+(c); 
    dfx=(2*a*i)+(b); 
        
    printf("x4        %.4f      %.4f		%.4f\n", x, fx, x-temp); 

    x=i-(fx/dfx);  
    i=x;
    fx=(a*i*i)+(b*i)+(c); 
    dfx=(2*a*i)+(b); 
        
    printf("x5        %.4f      %.4f		%.4f\n", x, fx, x-temp); 
}

int find_multiple_closest(int a, int b)
{
	int bolum, low, high, temp;	

	bolum=a/b;	//First I found division so I can find closest low value because int will round it.
				//by adding one and multiplying it with the "b" I can find closest high value.
	low=bolum*b;
	high=(bolum+1)*b;

	if ((a-low)<(high-a))	//This will return value itself if difference of high and low-
	{						//-values are equal otherwise will return the closest one.
		return(low);
	}
	else if ((high-a)<(a-low))
	{
		return(high);
	}

	else if ((a-low)==(high-a))
	{
		return(a);
	}
}