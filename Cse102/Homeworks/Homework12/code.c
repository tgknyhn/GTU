#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_LINE 1000000

//** Structs **

//Linked List
struct numbers
{
	int number;
	struct numbers *next;
};
typedef struct numbers number_t;

//Different numbers
struct diff_nums
{
	int n1, n2;
};
typedef struct diff_nums diff_t;

//Time difference
struct clocks
{
	clock_t start;
	clock_t end;
};


//** Functions **

//Main
void menu();
double* part1(int* byte_arr, int* byte_ll, float* arr_stat, float* ll_stat, double* time);
diff_t* part2(int* count);
//Part1(array)
int*   read_array (char *file_path, int *numbers_array, int *index);
float* stats_array(int *numbers_array, float *stats_arr, int index);
//Part1(linkedlist)
number_t* read_linkedlist (char *file_path, number_t *head, int *count_ll);
number_t* insert_number   (int number);
float*    stats_linkedlist(number_t *head, float *stats_ll, int count_ll);
//Part2
int* random_num(int num_to_create, int max_value, int *rand_nums);


int main()
{
	srand(time(NULL)); 	//initializing seed

    menu();
    return 0;
}

void menu()
{
    int count    = 0;
    int byte_arr = 0;
    int byte_ll  = 0;
    
    diff_t *diff = (diff_t*)calloc(1000, sizeof(diff_t)); 
    float  *arr_stat = (float*) calloc(4, sizeof(float));
    float  *ll_stat  = (float*) calloc(4, sizeof(float));
    double *time     = (double*)calloc(4, sizeof(double));

    printf("Welcome to Homework 11. Enter the part number you want to execute.\n");
    printf("1. Part1\n");
    printf("2. Part2\n");
    printf("enter : ");

    char op; 
    scanf(" %c", &op);
    printf("\n");

    if(op == '1' || op == '2')
    {   
        switch(op)
        {

            case '1':
    			time = part1(&byte_arr, &byte_ll, arr_stat, ll_stat, time);
    			printf("      (average of 4 func call)                                              \n");
			    printf("   Time spent in OS for each func   || Total bytes that has been used       \n");
			    printf("  --------------------------------  || ------------------------------       \n");
			    printf(" *read_array      (..) = %fs  || Array      = %d bytes                      \n", time[0], byte_arr);
			    printf(" *read_linkedlist (..) = %fs  || Linkedlist = %d bytes                      \n", time[2], byte_ll);
			    printf(" *stats_array     (..) = %fs  ||                                            \n", time[1]);
			    printf(" *stats_linkedlist(..) = %fs  ||                                            \n", time[3]);
			    printf("----------------------------------------------------------------------------\n");
			    printf("                   Statistics of both array and linkedlist                  \n");
			    printf("                   ---------------------------------------				    \n");
			    printf("                Min 		Max 		Mean 		Std deviation           \n");
			    printf("Array       =>  %.2f 	     %.2f 	     %.3f		  %.3f                  \n", arr_stat[0], arr_stat[1], arr_stat[2], arr_stat[3]);
			    printf("Linked list =>  %.2f 	     %.2f 	     %.3f	 	  %.3f                  \n", ll_stat[0], ll_stat[1], ll_stat[2], ll_stat[3]);
                break;
            case '2':
                diff = part2(&count);
                diff = (diff_t*)realloc(diff, sizeof(diff_t)*(count+1)); //giving unnecessarily allocated memory back.  

                printf("\n");
                printf("Oooops! Some values are inserted wrong during the process... These are the wrong ones :)\n");
                
                printf("Array       =>");
                for(int i=0; i<count; i++)
                	printf(" %5.d ", diff[i].n2);
                printf("\n");
                
                printf("Linked List =>");
                for(int i=0; i<count; i++)
                	printf(" %5.d ", diff[i].n1);
                printf("\n");

                break;
        }
    }
    else
    {
        printf("Invalid input, try again.\n");
        main();
        return;       
    }

    //asking user for next process
    char op_menu;
    int flag = 0;
    printf("1) Go to menu\n");
    printf("2) Exit\n");
    printf("Enter: ");
    
    scanf(" %c", &op_menu);
    if(op_menu == '1'){
    	menu();
    }
    else if(op_menu == '2')
    	printf("Good Bye!\n");
    return;
}

// *** PART 1 ***
double* part1(int* byte_arr, int* byte_ll, float* arr_stat, float* ll_stat, double* time)
{
    char   path[]        = "list.txt";

	//array's elements
    int    index         = -1;  
    int   *numbers_array = (int*)malloc(sizeof(int));
    float *stats_arr     = (float*)calloc(4, sizeof(float));
    
    //linkedlist's elements
    float *stats_ll = (float*)calloc(4, sizeof(float));
    int    count_ll =  0; 
    number_t *head = NULL;


    struct clocks c1, c2, c3, c4;
    int    call_order[4] = {0, 1, 0, 1};
    double time_avg[4]   = {0};   
    //calling functions each time in different order for 4 times
    //also measuring time spent in OS for each function.
    for(int i=0; i<4; i++)
    {
    	if(call_order[i] == 0)
    	{
			c1.start = clock();    
		    numbers_array = read_array(path, numbers_array, &index);
		    c1.end   = clock();

			c3.start = clock();
		    head = read_linkedlist(path, head, &count_ll);
		    c3.end   = clock();
    	}
    	else if(call_order[i] == 1)
    	{
			c3.start = clock();
		    head = read_linkedlist(path, head, &count_ll);
		    c3.end   = clock();

			c1.start = clock();    
		    numbers_array = read_array(path, numbers_array, &index);
		    c1.end   = clock();
    	}
    	time_avg[0] = time_avg[0] + (double)(c1.end-c1.start)/CLOCKS_PER_SEC;
    	time_avg[2] = time_avg[2] + (double)(c3.end-c3.start)/CLOCKS_PER_SEC;
    }
    for(int i=0; i<4; i++)
    {
    	if(call_order[i] == 0)
		{
			c2.start = clock();
		    stats_arr = stats_array(numbers_array, stats_arr, index);
		    c2.end   = clock();

			c4.start = clock();
		   	stats_ll = stats_linkedlist(head, stats_ll, count_ll);
		    c4.end   = clock();  	
		}
    	else if(call_order[i] == 1)
    	{
			c4.start = clock();
		   	stats_ll = stats_linkedlist(head, stats_ll, count_ll);
		    c4.end   = clock();  	

			c2.start = clock();
		    stats_arr = stats_array(numbers_array, stats_arr, index);
		    c2.end   = clock();
    	}
    	time_avg[1] = time_avg[1] + (double)(c2.end-c2.start)/CLOCKS_PER_SEC;
    	time_avg[3] = time_avg[3] + (double)(c4.end-c4.start)/CLOCKS_PER_SEC;
    }

    for(int i=0; i<4; i++)
    	time_avg[i] = (double)time_avg[i]/4; 

    //will send them to menu
    *byte_ll  = index*4;
    *byte_arr = count_ll*8;
    for(int i=0; i<4; i++)
    {
    	arr_stat[i] = stats_arr[i];
    	ll_stat[i]  = stats_ll[i];
    	time[i]     = time_avg[i];
    }

    //freeing dynamically allocated heap elements
    free(numbers_array);
    free(stats_arr);
    free(stats_ll);

    number_t *temp;
    while((temp = head) != NULL)
    {
    	head = head->next;
    	free(temp);
    }

    return time;
}

//Array - Reads numbers
int *read_array(char *file_path, int *numbers_array, int *index)
{   
    FILE *fp;
    fp = fopen(file_path, "r");
 

    char temp[MAX_LINE];
    int  num   = 1;
    int  count = 0;
    *index = -1;

    numbers_array[0] = num;

    fscanf(fp, "%[^\n]s", temp);
    do
    {
    	(*index)++;
    	
    	num = strtol(temp+count, NULL, 10);

    	numbers_array = (int*)realloc(numbers_array, sizeof(int)*(*index+2));
    	numbers_array[*index] = num;
    	
    	while(num != 0)
    	{
    		num = num/10;
    		count++;
    	}
    	count++;
    } while(numbers_array[*index] != 0);
    fclose(fp);
    return numbers_array;
}

//Array - Calculates mean, std, etc..
float *stats_array(int *numbers_array, float *stats_arr, int index)
{
	//initializing
	for(int i=0; i<4; i++)
		stats_arr[i] = 0;
	//max
	for(int i=0; i<index; i++)
		if(stats_arr[1] < (float)numbers_array[i])
			stats_arr[1] = (float)numbers_array[i];

	stats_arr[0] = stats_arr[1];

	//min
	for(int i=0; i<index; i++)
		if(stats_arr[0] > (float)numbers_array[i])
			stats_arr[0] = (float)numbers_array[i];

	//mean
	double mean = 0;
	for(int i=0; i<index; i++)
		mean = mean + (double)numbers_array[i];

	mean = (double)(mean/index);
	stats_arr[2] = mean;

	//std deviation
	double std = 0;
	for(int i=0; i<index; i++)
		std = std + (mean - numbers_array[i])*(mean - numbers_array[i]);

	std = sqrt((double)(std/(index+1)));
	stats_arr[3] = std;

	return stats_arr;
}

//Linked List - Reads numbers
number_t *read_linkedlist(char *file_path, number_t *head, int *count_ll)
{	
	*count_ll = 0;
    head      = NULL;

	FILE *fp;
	fp = fopen(file_path, "r");
 
	char temp_ll[MAX_LINE] = {0};
	int  num   = 1;
	int  count_file = 0;

    number_t *temp = (number_t*)malloc(sizeof(number_t));

	fscanf(fp, "%[^\n]s", temp_ll);
   	while(1 == 1)
	{
    	num = strtol(temp_ll+count_file, NULL, 10);

    	if(num == 0)
    		break;
		temp = insert_number(num);
		temp->next = head;
		head = temp; 

		while(num != 0)
    	{
    		num = num/10;
    		count_file++;
    	}
    	count_file++;
    	(*count_ll)++;
	}
	fclose(fp);
	return head;
}

//Linked List - Calculates mean, std, etc..
float *stats_linkedlist(number_t *head, float *stats_ll, int count_ll)
{
	//initializing
	for(int i=0; i<4; i++)
		stats_ll[i] = 0.0;
	number_t *temp = head;
	//max
	while(temp != NULL)
	{
		if(stats_ll[1]<(float)temp->number)
			stats_ll[1] = (float)temp->number;
		temp = temp->next;
	}

	temp = head;
	stats_ll[0] = stats_ll[1];
	
	//min
	while(temp != NULL)
	{
		if(stats_ll[0]>(float)temp->number)
			stats_ll[0] = (float)temp->number;
		temp = temp->next;
	}

	temp = head;
	
	//mean
	double mean = 0;
	while(temp != NULL)
	{
		//printf("%d ", temp->number);
		mean = mean + (double)temp->number;
		temp = temp->next;
	}

	mean = (double)(mean/count_ll);
	stats_ll[2] = mean;

	temp = head;

	//std deviation
	double std = 0;
	while(temp != NULL)
	{
		std = std + (mean-(double)temp->number)*(mean-(double)temp->number);
		temp = temp->next;
	}

	std = sqrt( (double)std/(count_ll+1) );
	stats_ll[3] = std;

	return stats_ll;
}

//Linked List - Creates a node, inserts number and returns the node
number_t *insert_number(int number)
{
	number_t *new = (number_t*)malloc(sizeof(number_t));
	new->number = number;

	return new;
}


// *** PART 2 *** 
diff_t *part2(int *count)
{
	int num_of_diff_nums;
	int num_to_create;
	int up_bound;
	printf("How many numbers you wanna create?\n");
	printf("Enter: ");
	scanf ("%d", &num_to_create);
	printf("What is the upper bound?\n");
	printf("Enter: ");
	scanf ("%d", &up_bound);

	num_of_diff_nums = (rand()%num_to_create/4)+1; //decides how many numbers are gonna be different
	//allocating and inserting random numbers
	int *rand_diff  = (int*)calloc(num_of_diff_nums, sizeof(int)); //different numbers value
	int *rand_index = (int*)calloc(num_of_diff_nums, sizeof(int)); //different numbers indexes
	int *rand_nums  = (int*)calloc(num_to_create, sizeof(int));
	rand_diff  = random_num(num_of_diff_nums, up_bound, rand_diff);
	rand_index = random_num(num_of_diff_nums, num_to_create, rand_index);
	rand_nums  = random_num(num_to_create, up_bound, rand_nums);

	//allocating array and inserting random numbers
	int *num_arr = (int*)calloc(num_to_create, sizeof(int));
	for(int i=num_to_create-1; i>=0; i--)
		num_arr[i] = rand_nums[num_to_create-(i+1)];

	//allocating linkedlist
	number_t *head = NULL;
	number_t *temp = NULL;
	for(int i=0; i<num_to_create; i++)
	{
		temp = insert_number(rand_nums[i]);
		for(int j=0; j<num_of_diff_nums; j++)
			if(rand_index[j] == i)
				temp->number = rand_diff[j];
		temp->next = head;
		head = temp;
	}

	//finding differences and allocating struct for them (also inserting numbers to that structs)
	diff_t *diff = (diff_t*)calloc(1, sizeof(diff_t));
	temp = head;
	*count = 0;	
	for(int i=0; i<num_to_create; i++)
	{
		if(temp->number != num_arr[i])
		{
			(*count)++; 
			diff = (diff_t*)realloc(diff, sizeof(diff_t)*(*count+1));
			diff[*count-1].n1 = temp->number;
			diff[*count-1].n2 = num_arr[i]; 
		}
		temp = temp->next;
	}

	temp = head;
	//printing array and linked list
	printf(" 	Array 	Linked List\n");
	for(int i=0; i<num_to_create; i++)
	{
		printf("%d)	 %d 	   %d\n", i+1, num_arr[i], temp->number);
		temp = temp->next;
	}

    return diff;
}

int *random_num(int num_to_create, int max_value, int *rand_nums)
{
	for(int i=0; i<num_to_create; i++)
		rand_nums[i] = (rand() % (max_value-1))+1;

	return rand_nums;
}
