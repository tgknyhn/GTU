#ifndef MATRIX_H
#define MATRIX_H

int   get_matrix_size     (char* path_to_data_file);
int** get_matrix_int      (char* path_to_data_file);
char* get_matrix_str      (char* path_to_data_file);
int   is_matrix_invertible(int** matrix);
int  _get_determinant     (int** matrix, int size);
int  _get_cofactor        (int** matrix, int size, int row, int column);

int** allocate2D          (int size);
void  deallocate2D        (int** array, int size);


#endif