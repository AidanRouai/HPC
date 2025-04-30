#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

// Declare the functions implemented in other .c files
int read_num_dims(char *filename);
int *read_dims(char *filename, int num_dims);
float *read_array(char *filename, int *dims, int num_dims);
void write_to_output_file(char *filename, float *output, int *dims, int num_dims);
void stencil(float *input_vec, float *output_vec, float *filter_vec, int m, int n, int k, int b);


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

    int b = input_dims[0];
    int m = input_dims[1];
    int n = input_dims[2];

    float *input_array = read_array(input_file, input_dims, num_dims);
    if (input_array == NULL) {
        printf("Error reading input array from file\n");
        free(input_dims);
        return 1;
    }

    //Read filter file

    int filter_num_dims = read_num_dims(filter_file);

    int *filter_dims = read_dims(filter_file, filter_num_dims);
    if (filter_dims == NULL) {
        printf("Error reading dimensions from filter file\n");
        free(input_array);
        return -1;
    }

    int k = filter_dims[0];

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
    printf("STENCIL TIME: %f", end_time - start_time);

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