#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

char decrypt_numbers(int number);
void decrypt_and_print (char *file_path);
void deep_decryp_and_print(char *file_path);
void track_machine();
void refresh_position(int *X, int *Y, double *D, double *R);
void encrypt_messages(char *file_path);
void menu();

int main()
{ 
	srand(time(NULL));
	printf("1-) Decrypt and print encrypted_p1.img\n");	// I could've write it all in one printf but it wouldn't look nice so I did like that.
	printf("2-) Decrypt and print encrypted_p2.img\n");
	printf("3-) Switch on the tracking machine\n");
	printf("4-) Encrypt the message\n5-) Switch off.\n\n");
	printf("Please make your choice:");
	menu();
}

void menu()
{
	char op;
	char *encrypted_p1="encrypted_p1.img";
	char *encrypted_p2="encrypted_p2.img";
	char *decrypted_p4="decrypted_p4.img";
	
	scanf(" %c", &op); // I used char instead of int because when I use int and give character into %d it breaks the scanf func. Also the blank before %c is for prevent from getting white space as input. 
	
	switch(op)	//Gives error unless you don't give numbers between 1-5. 
	{
		case '1':
			decrypt_and_print(encrypted_p1);
			break;		
		case '2':
			deep_decryp_and_print(encrypted_p2);
			break;
		case '3':
			track_machine();
			break;
		case '4':
			encrypt_messages(decrypted_p4);
			break;
		case '5':
			printf("\nStopping the code...\n");
			break;
		default:
		{
			printf("\n~~~~Error! Enter a number between 1-5: ~~~~\n\n");
			main();
			break;
		}
	}
}

void decrypt_and_print (char *file_path)
{
	char num_char;
	int num_int;
	
	FILE *img;
	img=fopen(file_path, "r");
	
	fscanf(img, "%c", &num_char);	//I get numbers as chars so it is easier to deal with than convert them into int's by substracting from '0'. 
	while(!feof(img))
 	{
		fscanf(img, "%c", &num_char);
		if(num_char=='\n')	//When line ends instead of converting to int it prints \n and continues to loop 
			printf("%c", num_char);
		else 
		{
			num_int = num_char - '0';
			printf("%c", decrypt_numbers(num_int));	//printing symbols by sending numbers as int to the function.
		}	
	}
	fclose(img);
	main();
}

char decrypt_numbers(int number)
{
	switch(number) 
	{
		case 0:
			return(' ');
		case 1:
			return('-');
		case 2:
			return('_');
		case 3:
			return('|');
		case 4:
			return('/');
		case 5:
			return('\\');
		case 6:
			return('O');
	}
}


void deep_decryp_and_print(char *file_path)
{
	char num_char;
	int num1, num2, num3, result;

	FILE *img;
	img=fopen(file_path, "r");

	fscanf(img, "%c", &num_char);	//Got first 3 values and convert them into int's so I can get values in the loop one by one each time.
	num1 = num_char - '0';

	fscanf(img, "%c", &num_char);
	num2 = num_char - '0';

	fscanf(img, "%c", &num_char);
	num3 = num_char - '0';

	while(!feof(img)) 
	{
		result = (num1+num2+num3)%7;
		printf("%c", decrypt_numbers(result));

		fscanf(img, "%c", &num_char);

		if(num_char == '\n') //when a line ends had to get first 3 values before getting into loop again. 
		{
			printf("\n");
			fscanf(img, "%c", &num_char); // 1.
			num2 = num_char - '0';

			fscanf(img, "%c", &num_char); // 2.
			num3 = num_char - '0';

			fscanf(img, "%c", &num_char); // 3.
		}
		num1 = num2; 
		num2 = num3;
		num3 = num_char - '0'; // Deletes num1 and puts new value into num3 as int each time. 
	}
	printf("\n");
	fclose(img);
	main();
}

void track_machine() 
{
	int x=0, y=0; //initial position of 'E'
	char op;
	double D, R;
	D = 0;	// Displacement is 0 at start
	R = 8.49;	// It is equal to square root of 72 also equals to distance between 6,6 - 0,0.
	while(1==1) 
	{
		for(int i=1; i<=11; i++)  //prints 11 dots in both x and y direction. 
		{
			for(int j=1; j<=11; j++) 
			{
				if(i==x && j==y)	//at first 'E' will appear at 0,0 then every time refresh_position is called it'll locate randomly.
					printf("E ");
				else if(i==6 && j==6)
					printf("O ");
				else
					printf(". ");
			}
			printf("\n");
		}
		printf("Enemies X position:%d\nY position:%d\nDisplacement:%.2f\nDistance to our camp:%.2f\nCommand waiting...:\n", x, y, D, R);
		scanf(" %c", &op);

		while( (op!='R') && (op!='E') )
		{
			printf("~~~~ERROR! Please enter either E to return to the menu or R to refresh position.~~~~\n");
			scanf(" %c", &op);
		}
		if(op=='R')
			refresh_position(&x, &y, &D, &R);
		else if(op=='E') 
		{
			main();
			break;
		}	
	}
}

void refresh_position(int *X, int *Y, double *D, double *R)
{
	int x=*X, y=*Y; // these x and y int's keeps previous position of 'E' so I can calculate Displacement and Distance each time R command is given.

    do {
    	*X = rand()%11+1;
    	*Y = rand()%11+1; 	
    } while((*X == 6) && (*Y == 6));
    
    *D = sqrt( ((*X-x)*(*X-x)) + ((*Y-y)*(*Y-y)) );
    *R = sqrt( ((*X-6)*(*X-6)) + ((*Y-6)*(*Y-6)) );

}

void encrypt_messages(char *file_path)
{
	int num1=0, num2=0, num3, result;
	char num_char;

	FILE *decrypt;	// For getting values
	FILE *encrypt;	// For printing values I got after I put them in to the loop for converting from symbols to int's and using the formula.  
	decrypt=fopen(file_path, "r");
	encrypt=fopen("encrypt_p4.img", "w");
	
	while(!feof(decrypt)) 
	{
		fscanf(decrypt, "%c", &num_char);
		while(num_char == '\n') // first looking for to see if it is end of any line or not. 
		{
			fprintf(encrypt, "%c", num_char);
			fscanf(decrypt, "%c", &num_char);
			num1 = 0; // First two values must be 0 according to formula so when it starts to a new line num1 and num2 must be equal to 0.
			num2 = 0;
			if(feof(decrypt)) // Since I look for line endings with while loop it lasts forever if it cames to the "eof". I prevented it happen by doing so.
				break;
		}
		switch(num_char) // Converting symbols into int's. 
		{
			case ' ':
				num3 = 0;
				break;
			case '-':
				num3 = 1;
				break;
			case '_':
				num3 = 2;
				break;
			case '|':
				num3 = 3;
				break;			
			case '/':
				num3 = 4;
				break;			
			case '\\':
				num3 = 5;
				break;			
			case 'O':
				num3 = 6;
				break;			
		}	

		result = (num1+num2+num3)%7;
		fprintf(encrypt, "%d", result);
		num1 = num2;
		num2 = num3;
	}
	main();

	fclose(decrypt);
	fclose(encrypt);
}
