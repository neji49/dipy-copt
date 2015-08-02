#include <stdio.h>
#include <stdlib.h>
#include "fit_tensor.h"

//Takes in a matrix(represeting several signals in row major format).
//Removes values below minimum value and takes logarithm of all values.
//Then pads, loads to gpu, and returns a padded matrix with a gpu pointer.
matrix* process_signal(matrix const* signal, double min_signal);

//Takes in an ols_fit matrix and signal matrix in row major.
//Converts signal to column major,multiplies, and exponentiates.
//Then loads to gpu(padding) and returns pointer in column major. 
matrix* generate_weights(matrix const* ols_fit_matrix, matrix const* signal);

//Takes in matrix, converts to column major, pads, and loads to gpu.
matrix* process_matrix(matrix const* design_matrix);

//Takes in host matrices to does a batched least squares fit
//Intended to test and investigate usage of ls function
double* cuda_test_batched_ls(double* ls_matrix, int rows1, int cols1, double* solutions, int rows2, int cols2, int batch_size);

//Takes in matrices with gpu pointers for data. 
//Does a weighted least squares regression.
//Returns six value tensor for each signal as single column major array on the gpu.
double* cuda_fitter(matrix const* design_matrix, matrix const* column_major_weights, matrix const* signals);

//Takes tensors as a single gpu array as first argument.
//Returns eigendecompositions as eigenvalues in column major, followed by eigenvectors.
double* cuda_decompose_tensors(double const* tensors_input, int number_of_tensors);

//Extracts eigendecomposition values, unpads, and moves into 2nd argument.
void extract_eigendecompositions(double const* eigendecompositions, tensor** output, int number_of_tensors);

//Clones double array and copies to gpu
double* cuda_double_copy_to_gpu(double const* local_array, int array_length);

//Clones double array and copies to host 
double* cuda_double_return_from_gpu(double const* cuda_array, int array_length);

//Allocates space for a double array on the device
void cuda_double_allocate(double** pointer, int pointer_length);

//Frees double device memory
void free_cuda_memory(double* pointer);

//Frees matrix with gpu pointer for data
void free_matrix_with_cuda_pointer(matrix* gpu_matrix);

//Kernel to take entire array and run cutoff log function
double* cutoff_log_cuda(double const* input, double min_signal, int array_length);

//Kernel to take entire array and exp it
double* exp_cuda(double const* input, int array_length);

//Wrapper for CUBLAS Dgemm call
matrix* cuda_matrix_dot(matrix const* matrix1, matrix const* matrix2);

//Function takes different weights and weights several copies of the same matrix and returns it. Expects arguments as gpu pointers.
double* matrix_weighter (double const* matrix, double const* weights, int rows, int columns, int length, bool trans);

//Transposes multiple matrices on the GPU
double* transpose_matrices(double const* matrices, int rows, int columns, int length);

//Returns the dot product of multiple paired matrices. Expects arguments as gpu pointers and in column major.
double* dot_matrices(double const* matrix_batch_one, int rows, double const* matrix_batch_two, int columns, int k, int length);

//Wrapper function to weight and fit the data
double* cuda_fitter(matrix const* design_matrix, matrix const* column_major_weights, matrix const* signal);

