#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "../header/file.h"
#include "../header/matrix.h"

int  data_file_length  = 0;
int  matrix_size       = 0;

// Returns the matrix size by reading for row
// For ex: if given matrix is 5x5, then it will return 5
int get_matrix_size(char* path_to_data_file) {
	// Getting size of the data file
	data_file_length = getFileLength(path_to_data_file);
	// Then reading all content into a buffer
	char* fileBuffer = (char*)malloc(data_file_length * sizeof(char));
	readFromFile(path_to_data_file, fileBuffer, data_file_length);
	// Tokenizing the buffer using \n character
	char * lineBuffer = strtok(fileBuffer, "\n");
	// Finding number of commas inside the lineBuffer
	int lineLength = strlen(lineBuffer);
	int count = 1;
	for(int i=0; i<lineLength; i++)
		if(lineBuffer[i] == ',')
			count++;
	// Free the buffer
	free(fileBuffer);
	// Checking if it is valid
	if(count < 2) {
		fprintf(stderr, "Matrix size is not valid\n");
		exit(EXIT_FAILURE);
	}
	// Returning the result
	return count;
}

// Getting matrix into a 2d int array with the size of matrixSize
int** get_matrix_int(char* path_to_data_file) {
	// First getting size of the matrix
	matrix_size = get_matrix_size(path_to_data_file);
	// Allocating memory for matrix
	int** matrix = allocate2D(matrix_size);  
	// Reading all content into a buffer
	char* fileBuffer = (char*)malloc((data_file_length * sizeof(char) + 1));
	readFromFile(path_to_data_file, fileBuffer, data_file_length); fileBuffer[data_file_length] = '\0';
	// Since we know the size of the matrix, we will use a fixed size for loop instead of while loop
	for(int row=0; row < matrix_size; row++) {
		for(int column=0; column < matrix_size; column++) {
			if(row == 0 && column == 0) 
				matrix[row][column] = atoi(strtok(fileBuffer, ","));
			else if(column == matrix_size - 1)
				matrix[row][column] = atoi(strtok(NULL, "\n"));
			else
				matrix[row][column] = atoi(strtok(NULL, ","));
		}
	}
	// Deallocating the variables
	free(fileBuffer);
	// Returning the matrix
	return matrix;
}

// Getting matrix into a char buffer and returning it
char* get_matrix_str(char* path_to_data_file) {
	// First getting size of the matrix
	matrix_size = get_matrix_size(path_to_data_file);
	// Allocating space for file buffer
	char* fileBuffer = (char*)malloc((data_file_length * sizeof(char) + 1));
	// Reading all content into a buffer
	readFromFile(path_to_data_file, fileBuffer, data_file_length); fileBuffer[data_file_length] = '\0';
	// Returning the matrix
	return fileBuffer;
}



int is_matrix_invertible(int** matrix) {
	// Finding determinant
	int determinant = _get_determinant(matrix, matrix_size);
	// Deallocating matrix since we don't need it anymore
	deallocate2D(matrix, matrix_size);
	// Returning result
	return determinant == 0 ? 0 : 1 ; 
}

void printMatrix(int** matrix, int size) {
	int i, j;
	for(i=0; i<size; i++) {
		for(j=0; j<size; j++)
			printf("%d\t", matrix[i][j]);
		printf("\n");
	}
}

int _get_determinant(int** matrix, int size) {
	int determinant = 0;
	
	if(size == 1) {
		return matrix[0][0];
	}
	else if(size == 2) {
		int multPositive = matrix[0][0] * matrix[1][1];
		int multNegative = matrix[0][1] * matrix[1][0];
		determinant = multPositive - multNegative;
	} else {
		for(int i=0; i < size; i++) {
			determinant += matrix[0][i] * _get_cofactor(matrix, size, 0, i);
		}
	}

	return determinant;
}

int _get_cofactor(int** matrix, int size, int row, int column) {
	int cofactor = 0;
	int coMatrixSize = size - 1;
	// Allocating the space for cofactor matrix
	int** coMatrix = allocate2D(coMatrixSize);
	// Getting the coMatrix
	for(int i=0, coMatrixRow=0; i<size; i++) {
		if(i == row) continue;
		for(int j=0, coMatrixColumn=0; j<size; j++) {
			if(j == column) continue;
			coMatrix[coMatrixRow][coMatrixColumn] = matrix[i][j];
			coMatrixColumn++;
		}
		coMatrixRow++;
	}   
	// Calculating the cofactor
	int part1 = pow((-1), row + column);
	int part2 = _get_determinant(coMatrix, coMatrixSize);
	cofactor = part1 * part2;
	// Freeing allocated variables
	deallocate2D(coMatrix, coMatrixSize);
	// Returning the cofactor value
	return cofactor;
}

int** allocate2D(int size) {
	int** temp;

	temp = (int**)malloc(size * sizeof(int*));
	for(int i=0; i<size; i++)
		temp[i] = (int*)malloc(size * sizeof(int));

	return temp;
}

void deallocate2D(int** array, int size) {
	for(int i=0; i<size; i++)
		free(array[i]);
	free(array);
}