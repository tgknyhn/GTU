#include <stdio.h>

int main()
{
	int num, cv; //num for first switch case and cv for second switch case.
	float eur, dol, tur, money;
	printf("*****Welcome to ABC Exchange Office*****\n\n");
	printf("Enter your amount:\n");
	scanf("%f", &money);
	printf("\n");

	printf("Please select your currency\n1. Turkish Lira\n2. Euro\n3. Dollar\n\n");
	scanf("%d", &num);
	switch(num)
	{
		case 1:
			printf("You have %f Turkish Liras\n", money);
			printf("Choose which currency you want to convert\n");
			scanf("%d", &cv);
			printf("\n");
			switch(cv)	// I didn't put chosen case again because it's meaningless to convert same thing again so code'll give error when you choose 1. 
			{
				case 2:
					eur=(money/6.69);
					printf("You have %f Euro\n", eur);
					break;
				case 3:
					dol=(money/6.14);
					printf("You have %f Dollar\n", dol);
					break;
				default:
					printf("Your selection is invalid!\n");
					return 0;
			}
			break;
		
		case 2:
			printf("You have %f Euros\n", money);
			printf("Choose which currency you want to convert\n");
			scanf("%d", &cv);
			printf("\n");
			switch(cv)
			{
				case 1:
					tur=(money*6.69);
					printf("You have %f Turkish Lira\n", tur);
					break;
				case 3:
					dol=( (money*6.69)/6.61 );	// I could've calculate this right at the beggining of the code but I think this way it's simpler. 
					printf("You have %f Dollar\n", dol); // First it will convert euro to turkish lira then dollar.
					break;
				default:
					printf("Your selection is invalid!\n");
					return 0;
			}
			break;
		
		case 3:
			printf("You have %f Dollars\n", money);
			printf("Choose which currency you want to convert\n");
			scanf("%d", &cv);
			printf("\n");
			switch(cv)
			{
				case 1:
					tur=(money*6.61);
					printf("You have %f Turkish Lira\n", tur);
					break;
				case 2:
					eur=( (money*6.61)/6.69 ); // same convert here but just the opposite.
					printf("You have %f Euro\n", eur);
					break;
				default:
					printf("Your selection is invalid!\n");
					return 0;
			}
			break;
		
		default:
			printf("Your selection is invalid!\n");
			return 0;
	}
	return 0;
}