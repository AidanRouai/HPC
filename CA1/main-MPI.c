#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "file-reader.h"
#include "stencil.h"
#include <omp.h>

// MPI_Init
// MPI_Comm_rank and MPI_Comm_size

// Argument checking (only rank 0 checks)
// Broadcast filenames if needed (optional)

// Read input and filter (only rank 0 reads full input)
// Broadcast or scatter input across processes
// Broadcast filter to all processes

// Allocate output arrays locally
// Start timing (only rank 0 times)

// Call stencil on local matrices

// Stop timing (only rank 0 times)

// Gather all output matrices back to rank 0
// Rank 0 writes output to file

// Cleanup memory
// MPI_Finalize

