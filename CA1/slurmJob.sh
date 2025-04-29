#!/bin/bash

#SBATCH --job-name=stencil_test # Job name
#SBATCH --output=stencil_test.out # Output file name
#SBATCH --error=stencil_test.err # Error file name
#SBATCH --nodes=1 # Number of nodes
#SBATCH --ntasks=1 # Number of tasks (processes)
#SBATCH --cpus-per-task=1 # Number of CPU cores per task
#SBATCH --time=00:10:00 # Time limit (hh:mm:ss)
#SBATCH --partition=compute # Partition name

# Load the required modules
module load gcc
module load openmpi

# Set the number of threads
export OMP_NUM_THREADS=8

# Run the stencil program
mpirun -np 1 ./stencil_program #input file, filter file, output file