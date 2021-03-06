#include "cuda_util.h"

void fit_complete_signal(matrix* ols_fit, matrix* design_matrix, matrix* signal, double min_signal, 
        double min_diffusivity, tensor** tensor_output) {
    int number_of_signals = signal->rows;
    int signal_elements = signal->columns;
    matrix* processed_signal_gpu = process_signal(signal, min_signal);

    //Switching dimensions to simulator transpose and conversion to column major 
    processed_signal_gpu->rows = signal_elements;
    processed_signal_gpu->columns = number_of_signals;

    matrix* column_major_weights_gpu = generate_weights(ols_fit, processed_signal_gpu);
    matrix* column_major_design_matrix_gpu = process_matrix(design_matrix);

    //Switching dimensions back
    processed_signal_gpu->rows = number_of_signals;
    processed_signal_gpu->columns = signal_elements;

    double* tensors_gpu = cuda_fitter(column_major_design_matrix_gpu, column_major_weights_gpu, 
            processed_signal_gpu);
    double* padded_eigendecompositions_gpu = cuda_decompose_tensors(tensors_gpu, column_major_design_matrix_gpu->columns , number_of_signals);
    cudaDeviceSynchronize();
    extract_eigendecompositions(padded_eigendecompositions_gpu, tensor_output, number_of_signals, min_diffusivity);
}
