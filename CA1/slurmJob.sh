#!/bin/bash

#SBATCH -D ./
#SBATCH --export=ALL
#SBATCH -p nodes 1
#SBATCH -t 1

# Load the required modules
module load gcc
module load openmpi

# Set the number of threads
export OMP_NUM_THREADS=1

# Run the stencil program (non-MPI version)
./stencil-nearly-gcc input_1_3840_2048.dat filter_5.dat output_8.dat 
