#include <stdio.h>
#include <math.h>
int draw_triangle(int side1, int side2, int side3);
void type_triangle(int side1, int side2, int side3);
int perimeter_triangle(int side1, int side2, int side3);
double area_triangle(int side1, int side2, int side3, int perimeter);

int main()
{
	int a, b, c, draw, perimeter;
	double area;
	printf("The length of the first side:\n");
	scanf("%d", &a);
	printf("The length of the second side:\n");
	scanf("%d", &b);
	printf("The length of the third side:\n");
	scanf("%d", &c);
	printf("\n");

	draw=draw_triangle(a, b, c);
	perimeter=perimeter_triangle(a, b, c);
	area=area_triangle(a, b, c, perimeter);

	if (draw==1)
	{
		printf("According to the triangle inequality theorem, this triangle can be drawn.\n\n");
		type_triangle(a, b, c);
		printf("The perimeter of the triangle:%d\n", perimeter);
		printf("The area of the triangle:%lf\n", area);
	}
	else
	{
		printf("According to the triangle inequality theorem, this triangle cannot be drawn.\n");
	}

	return 0;
}

int draw_triangle(int side1, int side2, int side3)
{
	return((side1+side2)>side3 && (side1+side3)>side2 && (side2+side3)>side1);	
}

void type_triangle(int side1, int side2, int side3)
{
	if (side1==side2 && side2==side3)
	{
		printf("It is a equilateral triangle\n");
	}
	else if(side1!=side2 && side2!=side3 && side1!=side3) //I had to use the last equation because there is a situation which side1 and side3 are equal.
	{
		printf("It is a scalene triangle\n");
	}
	else if( (side1==side2 && side2!=side3) || (side1==side3 && side3!=side2) || (side2==side3 && side3!=side1) )
	{
		printf("It is a isosceles triangle\n");
	}
}

int perimeter_triangle(int side1, int side2, int side3)
{
	return(side1+side2+side3);
}

double area_triangle(int side1, int side2, int side3, int perimeter)
{
	double area, a, p, temp;
	temp=perimeter;	//I used temp because when I use perimeter as an integer it was rounding the division. In an other word I changed it's variable type into double.
	p=(temp/2);
	a=(p*(p-side1)*(p-side2)*(p-side3)); // to see it more clearly first I wrote down the equation without taking square root then took it.
	area=sqrt(a);
	return(area);
}

