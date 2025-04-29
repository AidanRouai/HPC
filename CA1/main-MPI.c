#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "file-reader.h"
#include "stencil.h"
#include <omp.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Validate args
    if (argc != 4) {
        if (rank == 0) {
            fprintf(stderr, "Usage: %s <input_file> <filter_file> <output_file>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    char *input_file = argv[1];
    char *filter_file = argv[2];
    char *output_file = argv[3];
    
    int b, m, n, k;
    float *input_array = NULL;
    float *filter_array = NULL;
    float *output_array = NULL;

    // Root process reads input and filter files
    if (rank == 0) {
        // Root process reads input and filter files
        int num_dims = read_num_dims(input_file);
        if (num_dims != 3) {
            fprintf(stderr, "Error: Input file must have 3 dimensions.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        int *input_dims = read_dims(input_file, num_dims);
        if (input_dims == NULL) {
            fprintf(stderr, "Error reading dimensions from input file.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        b = input_dims[0];
        m = input_dims[1];
        n = input_dims[2];
        free(input_dims);

        input_array = read_array(input_file, (int[]){b, m, n}, num_dims);
        if (input_array == NULL) {
            fprintf(stderr, "Error reading input array from file.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        int filter_num_dims = read_num_dims(filter_file);
        if (filter_num_dims != 1) {
            fprintf(stderr, "Error: Filter file must have 1 dimension.\n");
            free(input_array);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        int *filter_dims = read_dims(filter_file, filter_num_dims);
        if (filter_dims == NULL) {
            fprintf(stderr, "Error reading dimensions from filter file.\n");
            free(input_array);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        k = filter_dims[0];
        free(filter_dims);

        int filter_dims_2d[2] = {k, k};
        filter_array = read_array(filter_file, filter_dims_2d , 2);

        if (filter_array == NULL) {
            fprintf(stderr, "Error reading filter array from file.\n");
            free(input_array);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        output_array = malloc(b * m * n * sizeof(float));
        if (output_array == NULL) {
            fprintf(stderr, "Error allocating memory for output array.\n");
            free(input_array);
            free(filter_array);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    // Broadcast dimensions and filter to all processes
    MPI_Bcast(&b, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        filter_array = malloc(k * k * sizeof(float));
        if (filter_array == NULL) {
            fprintf(stderr, "Error allocating memory for filter array on process %d.\n", rank);
            MPI_Finalize();
            return 1;
        }
    }

    MPI_Bcast(filter_array, k * k, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Scatter input data to all processes
    int local_b = b / size + (rank < b % size ? 1 : 0);
    int offset = rank * (b / size) + (rank < b % size ? rank : b % size);
    float *local_input = malloc(local_b * m * n * sizeof(float));
    float *local_output = malloc(local_b * m * n * sizeof(float));

    if (local_input == NULL || local_output == NULL) {
        fprintf(stderr, "Error allocating memory for local arrays on process %d.\n", rank);
        free(filter_array);
        MPI_Finalize();
        return 1;
    }

    // Scatter the input array
    int *sendcounts = NULL;
    int *displs = NULL;
    if (rank == 0) {
        sendcounts = malloc(size * sizeof(int));
        displs = malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) {
            sendcounts[i] = (b / size + (i < b % size ? 1 : 0)) * m * n;
            displs[i] = i * (b / size) * m * n + (i < b % size ? i * m * n : b % size * m * n);
        }
    }

    MPI_Scatterv(input_array, sendcounts, displs, MPI_FLOAT, local_input, local_b * m * n, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Start timing only the stencil execution
    double start_time = 0, end_time = 0;
    if (rank == 0) {
        start_time = omp_get_wtime();
    }
    
    stencil(local_input, local_output, filter_array, m, n, k, local_b);
    
    if (rank == 0) {
        end_time = omp_get_wtime();
        printf("STENCIL TIME: %f\n", end_time - start_time);
    }
    
    // Gather results back to root process
    MPI_Gatherv(local_output, local_b * m * n, MPI_FLOAT, output_array, sendcounts, displs, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Rank 0 writes output to file
    if (rank == 0) {
        write_to_output_file(output_file, output_array, (int[]){b, m, n}, 3);
        free(input_array);
        free(output_array);
        free(sendcounts);
        free(displs);
    }

    free(local_input);
    free(local_output);
    free(filter_array);

    MPI_Finalize();
    return 0;
}

