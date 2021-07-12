#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_WORD_SIZE   20
#define MAX_POINT_NUM   20
#define MAX_LINE_NUM    2020 // for p20 p19 -> l2019
#define MAX_POLYGON_NUM 20

struct Point
{
	float x, y; 
} pt[MAX_POINT_NUM];

struct Line
{	
	struct Point pt_line[2];
} l[MAX_LINE_NUM];

struct Polygon
{
	struct Point pt_polygon[20];
	struct Line l_polygon[20]; 
} pg[MAX_POLYGON_NUM];


void  commands();
float Distance_pp   (struct Point pt[MAX_POINT_NUM], int point1, int point2);
float Distance_pl   (struct Point pt[MAX_POINT_NUM], struct Line l[MAX_LINE_NUM], int point1, int point2);
float Length_Line   (struct Line l[MAX_LINE_NUM], int point);
float Length_Polygon(struct Polygon pg[MAX_POLYGON_NUM], int point, int polygons_num[20]);
float Area_polygon  (struct Polygon pg[MAX_POLYGON_NUM], int point, int polygons_num[20]);
float Angle_lines   (struct Line l[MAX_LINE_NUM], int line1, int line2);

int main()
{
	commands();
	return 0;
}

void commands()
{
	float result_length  = 0;
	//holds x and y value of a point
	float data_x     	 = 0.0;
	float data_y     	 = 0.0;
	//holds temp point location
	int   temp           = 0;
	int   temp1			 = 0;
	int   temp2			 = 0;
	//how many lines are read so far
	int   eof_num  		 = 0;
	int   data_num 		 = 0;
	int	  action_num	 = 0;
	//
	int   point_location   = 0;
	int   count_polygon    = 1;
	int   count_point      = 2;
	int   count_line       = 0;
	int   count_index      = 1;
	int   count_num        = 0;
	int   polygons_num[20] = {0};  
	//
	char  output_txt[15];
	char  line_by_letter;
	char  line_by_word[MAX_WORD_SIZE];

	FILE *fpi;
	fpi = fopen("commands.txt", "r");

	fscanf(fpi, "%s", line_by_word); //holds "data" text
	fscanf(fpi, "%d", &data_num); //holds "data" number	

	// DATA STARTS
	while(eof_num != data_num)
	{ 
		count_num = 0;
		char line_by_letter = '0';
		fscanf(fpi, "%s", line_by_word); 
		
		//lines or polygons by using points
		if(line_by_word[0] == 'P')
		{
			temp1 = (int)strtod(line_by_word+1, NULL);
			fscanf(fpi, "%s", line_by_word);
			temp2 = (int)strtod(line_by_word+1, NULL);

			fscanf(fpi, "%s", line_by_word);
			//ex. P1 P2 L12
			if(line_by_word[0] == 'L')
			{
				l[(temp1*10)+temp2].pt_line[0].x = pt[temp1].x;
				l[(temp1*10)+temp2].pt_line[0].y = pt[temp1].y;

				l[(temp1*10)+temp2].pt_line[1].x = pt[temp2].x;
				l[(temp1*10)+temp2].pt_line[1].y = pt[temp2].y;
			}
			//ex. P1 P2 P3 P4 PG1
			else if(line_by_word[0] == 'P')
			{
				count_num++;
				pg[count_polygon].pt_polygon[0].x = pt[temp1].x;
				pg[count_polygon].pt_polygon[0].y = pt[temp1].y;

				count_num++;
				pg[count_polygon].pt_polygon[1].x = pt[temp2].x;
				pg[count_polygon].pt_polygon[1].y = pt[temp2].y;

				if(line_by_word[1] != 'G')
				{
					while(line_by_word[1] != 'G')
					{
						count_num++;
						temp = (int)strtod(line_by_word+1, NULL);
						pg[count_polygon].pt_polygon[count_point].x = pt[temp].x;
						pg[count_polygon].pt_polygon[count_point].y = pt[temp].y;
						count_point++;
						fscanf(fpi, "%s", line_by_word);
					}
				}
				else if(line_by_word[1] == 'G')
				{
					count_num++;
					temp = (int)strtod(line_by_word+1, NULL);
					pg[count_polygon].pt_polygon[2].x = pt[temp].x;
					pg[count_polygon].pt_polygon[2].y = pt[temp].y;
				}
				count_polygon++;
				polygons_num[count_index] = count_num;
				count_index++;
				
				fscanf(fpi, "%c", &line_by_letter);
				if(line_by_letter == '\n')
				eof_num++;
			}
			//checking "\n" 
			fscanf(fpi, "%c", &line_by_letter);
			if(line_by_letter == '\n')
			eof_num++;
		}

		//skipping comments
		else if(line_by_word[0] == '/' && line_by_word[1] == '/')
		{
			while(line_by_letter != '\n')
				fscanf(fpi, "%c", &line_by_letter);
			eof_num++;
		}
		
		//polygons by using lines
		else if(line_by_word[0] == 'L')
		{
			count_line = 0;
			while(line_by_word[0] != 'P')
			{
				temp = (int)strtod(line_by_word+1, NULL);
				pg[count_polygon].l_polygon[count_line] = l[temp];
				//printf("x=>%f y=>%f pg=>%d line=>%d\n", pg[count_polygon].l_polygon[count_line].pt_line[0].x, pg[count_polygon].l_polygon[count_line].pt_line[0].y, count_polygon, count_line);
				fscanf(fpi, "%s", line_by_word);
				count_num++;
				count_line++;
			}
				polygons_num[count_index] = count_num;
				count_index++;
			fscanf(fpi, "%c", &line_by_letter);
			if(line_by_letter == '\n')
				eof_num++;
		}
		
		//points
		else
		{
			data_x = strtod(line_by_word, NULL);

			fscanf(fpi, "%s %c%d", line_by_word, &line_by_letter, &point_location);
			data_y = strtod(line_by_word, NULL);

			pt[point_location].x = data_x;
			pt[point_location].y = data_y;
			
			eof_num++;
		}			
	}
	// DATA ENDS
	eof_num = 0;
	if(line_by_word[0] == 'o' && line_by_word[1] == 'u')
		strcpy(output_txt, line_by_word);
	else
	{
		fscanf(fpi, "%s", line_by_word); //holds "actions" text
		fscanf(fpi, "%s", output_txt);   //holds "output.txt" text	
	}
	
	fscanf(fpi, "%d", &action_num);  //holds "actions" number 

	FILE *fpo;
	fpo = fopen(output_txt, "a");

	// ACTION STARTS
	while(eof_num != action_num+10)
	{		
		char line_by_letter = '0';
		fscanf(fpi, "%s", line_by_word);
		//Distance
		if(line_by_word[0] == 'D')
		{
			fscanf(fpi, "%s", line_by_word); //First point
			temp1 = (int)strtod(line_by_word+1, NULL); 

			fscanf(fpi, "%s", line_by_word); //Second point//line
			//ex. distance(P1 P2)
			if(line_by_word[0] == 'P')
			{
				temp2 = (int)strtod(line_by_word+1, NULL);

				fprintf(fpo, "%s%d%s%d%s%.1f\n", "Distance(P", temp1, ",P", temp2, ") = ", Distance_pp(pt, temp1, temp2));
			}
			//ex. distance(P1 L23)
			else if(line_by_word[0] == 'L')
			{
				temp2 = (int)strtod(line_by_word+1, NULL);

				result_length = Distance_pl(pt, l, temp1, temp2);
				if(result_length == 0)
					fprintf(fpo, "%s%d%s%d%s\n", "Distance(P", temp1, ",L", temp2, ") = Not Implemented(because numerator or denumerator is equal to 0[perhaps both]. Try with another point locations)");
				else if(result_length != 0)
					fprintf(fpo, "%s%d%s%d%s%.1f\n", "Distance(P", temp1, ",L", temp2, ") = ", result_length);
			}
		}
		
		//Length
		else if(line_by_word[0] == 'L')
		{
			fscanf(fpi, "%s", line_by_word);
			if(line_by_word[0] == 'L')
			{
				temp = (int)strtod(line_by_word+1, NULL);

				fprintf(fpo, "%s%d%s%.1f\n", "Length(L", temp, ") = ", Length_Line(l, temp));
			}
			else if(line_by_word[0] == 'P')
			{
				temp = (int)strtod(line_by_word+2, NULL);

				fprintf(fpo, "%s%d%s%.1f\n", "Length(PG", temp, ") = ", Length_Polygon(pg, temp, polygons_num));
			}	
		}

		//Angle
		else if(line_by_word[0] == 'A' && line_by_word[1] == 'n')
		{
			fscanf(fpi, "%s", line_by_word);
			temp1 = (int)strtod(line_by_word+1, NULL);

			fscanf(fpi, "%s", line_by_word);
			temp2 = (int)strtod(line_by_word+1, NULL);
	
			fprintf(fpo, "%s%d%s%d%s%.1f\n", "Angle(L", temp1, ",L", temp2, ") = ", Angle_lines(l, temp1, temp2));

		}

		//Area
		else if(line_by_word[0] == 'A' && line_by_word[1] == 'r')
		{
			fscanf(fpi, "%s", line_by_word);
			temp = (int)strtod(line_by_word+2, NULL);

			fprintf(fpo, "%s%d%s%.1f\n", "Area(PG", temp, ") = ", Area_polygon(pg, temp, polygons_num));			
		}

		//skipping comments
		else if(line_by_word[0] == '/' && line_by_word[1] == '/')
		{
			while(line_by_letter != '\n')
				fscanf(fpi, "%c", &line_by_letter);
		}
		eof_num++;
	}

}

float Distance_pp(struct Point pt[MAX_POINT_NUM], int point1, int point2)
{
	float distance_x = 0;
	float distance_y = 0;
	float sum 	     = 0;

	distance_x = pow((pt[point1].x - pt[point2].x), 2);
	distance_y = pow((pt[point1].y - pt[point2].y), 2);

	sum = distance_x + distance_y;
	return sqrt(sum);
}

float Distance_pl(struct Point pt[MAX_POINT_NUM], struct Line l[MAX_LINE_NUM], int point1, int point2)
{
	float A, B, C;
	float a_x, a_y,
		  b_x, b_y;
	float d;

	//a(x,y)
	a_x = l[point2].pt_line[0].x;
	a_y = l[point2].pt_line[0].y;
	//b(x,y)
	b_x = l[point2].pt_line[1].x;
	b_y = l[point2].pt_line[1].y;

	//equation of line [(b_y)-(a_y)]*[x-(b_x)] = [(b_x)-(a_x)]*[y-(b_y)] || Ax + By + C = 0
	A = b_y - a_y;
	B = a_x - b_x;
	C = ((a_y)*(b_x)) - ((a_x)*(b_y));
	
	d = (float)(abs( ((A*pt[point1].x)+(B*pt[point1].y)+C) ) / sqrt((pt[point1].x*pt[point1].x)+(pt[point1].y*pt[point1].y)));

	if(sqrt((pt[point1].x*pt[point1].x)+(pt[point1].y*pt[point1].y)) == 0)
		return 0.0;

	return d;
}

float Length_Line(struct Line l[MAX_LINE_NUM], int point)
{
	float distance_x = 0;
	float distance_y = 0;
	float sum 		 = 0;

	distance_x = pow((l[point].pt_line[0].x - l[point].pt_line[1].x), 2);
	distance_y = pow((l[point].pt_line[0].y - l[point].pt_line[1].y), 2);

	sum = distance_x + distance_y;
	return sqrt(sum);
}

float Length_Polygon(struct Polygon pg[MAX_POLYGON_NUM], int point, int polygons_num[20])
{
	float sum = 0;
	if(Length_Line(pg[point].l_polygon, 0) == 0)
	{
		sum = Distance_pp(pg[point].pt_polygon, 0, polygons_num[point]-1);
		for(int i=0; i<polygons_num[point]-1; i++)		
			sum = sum + Distance_pp(pg[point].pt_polygon, i, i+1);		
		return sum;
	}
	else
	{
		for(int i=0; i<polygons_num[point]; i++)
			sum = sum + Length_Line(pg[point].l_polygon, i);
		return sum;
	}
}

float Area_polygon(struct Polygon pg[MAX_POLYGON_NUM], int point, int polygons_num[20])
{
	float area = 0;
	float sum  = 0;
	if(Length_Line(pg[point].l_polygon, 0) == 0)
	{
		sum = (pg[point].pt_polygon[polygons_num[point]-1].x*pg[point].pt_polygon[0].y)-(pg[point].pt_polygon[polygons_num[point]-1].y*pg[point].pt_polygon[0].x);
		for(int i=0; i<polygons_num[point]-1; i++)		
			sum = sum + (pg[point].pt_polygon[i].x*pg[point].pt_polygon[i+1].y)-(pg[point].pt_polygon[i].y*pg[point].pt_polygon[i+1].x);
		area = (float)abs(sum/2);
		return area;
	}
	else
	{
		sum = (pg[point].l_polygon[0].pt_line[0].x*pg[point].l_polygon[0].pt_line[1].y)-(pg[point].l_polygon[0].pt_line[0].y*pg[point].l_polygon[0].pt_line[1].x);
		sum = sum + (pg[point].l_polygon[0].pt_line[0].y*pg[point].l_polygon[polygons_num[point]-1].pt_line[0].x)-(pg[point].l_polygon[0].pt_line[0].x*pg[point].l_polygon[polygons_num[point]-1].pt_line[0].y);
		for(int i=0; i<polygons_num[point]-1; i++)
			sum = sum + (pg[point].l_polygon[i].pt_line[1].x*pg[point].l_polygon[i+1].pt_line[1].y)-(pg[point].l_polygon[i].pt_line[1].y*pg[point].l_polygon[i+1].pt_line[1].x);
		area = (float)abs(sum/2);
		return area;
	}
}

float Angle_lines(struct Line l[MAX_LINE_NUM], int line1, int line2)
{
	float a_x, a_y,
		  b_x, b_y;

	float slope1, slope2, slope3;

	//a(x,y)
	a_x = l[line1].pt_line[0].x;
	a_y = l[line1].pt_line[0].y;
	//b(x,y)
	b_x = l[line1].pt_line[1].x;
	b_y = l[line1].pt_line[1].y;

	if(b_x-a_x == 0.0)
	{
		b_y = 90.0;
		a_y = 0.0;
		b_x = 1.0;
		a_x = 0.0;
	}	
	slope1 = (float)(b_y-a_y)/(b_x-a_x); 

	//a(x,y)
	a_x = l[line2].pt_line[0].x;
	a_y = l[line2].pt_line[0].y;
	//b(x,y)
	b_x = l[line2].pt_line[1].x;
	b_y = l[line2].pt_line[1].y;

	if(b_x-a_x == 0.0)
	{
		b_y = 90.0;
		a_y = 0.0;
		b_x = 1.0;
		a_x = 0.0;
	}
	slope2 = (float)(b_y-a_y)/(b_x-a_x); 

	slope3 = (float)abs((slope1-slope2)/1+(slope1*slope2));

	return slope3;
}