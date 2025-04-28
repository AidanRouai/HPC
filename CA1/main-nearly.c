#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file-reader.c"
#include "stencil.c"
#include <omp.h>

int main(int argc, char **argv) {
    //Validate args

    if (argc != 4) {
        printf("Usage: %s <input_file> <filter_file> <output_file>", argv[0]);
        return 1;
    }
    //Read input file
    char *input_file = argv[1];
    char *filter_file = argv[2];
    char *output_file = argv[3];

    int num_dims = read_num_dims(input_file);
    if (num_dims != 3) {
        printf("Error reading number of dimensions from input file\n");
        return 1;
    }
    int *input_dims = read_dims(input_file, num_dims);
    if (input_dims == NULL) {
        printf("Error reading dimensions from input file\n");
        return 1;
    }
    int b, m, n = input_dims[0], input_dims[1], input_dims[2];
    free(input_dims);

    float *input_array = read_array(input_file, input_dims, num_dims);
    if (input_array == NULL) {
        printf("Error reading input array from file\n");
        free(input_dims);
        return 1;
    }

    //Read filter file

    int filter_num_dims = read_num_dims(filter_file);
    if (filter_num_dims != 1) {
        printf("Error reading number of dimensions from filter file\n");
        free(input_array);
        return -1;
    }

    int *filter_dims = read_dims(filter_file, filter_num_dims);
    if (filter_dims == NULL) {
        printf("Error reading dimensions from filter file\n");
        free(input_array);
        return -1;
    }

    int k = filter_dims[0];
    free(filter_dims);

    float *filter_array = read_array(filter_file, filter_dims, filter_num_dims);
    if (filter_array == NULL) {
        printf("Error reading filter array from file\n");
        free(input_array);
        return -1;
    }

    //Allocate output array
    float *output_array = malloc(b * m * n * sizeof(float));
    if (output_array == NULL) {
        printf("Error allocating memory for output array\n");
        free(input_array);
        free(filter_array);
        return -1;
    }

    //Start timer for performance measurement   
    double start_time = omp_get_wtime();

    //Call stencil
    stencil(input_array, output_array, filter_array, 
            m, n, k, b);

    //Stop timer 
    double end_time = omp_get_wtime();

    //Print STENCIL TIME
    printf("Stencil time: %f seconds\n", end_time - start_time);

    //Write output 
    write_to_output_file(output_file, output_array, input_dims, num_dims);

    //Free memory
    free(input_array);
    free(filter_array);
    free(output_array);
    free(input_dims);
    free(filter_dims);

    return 0;
}