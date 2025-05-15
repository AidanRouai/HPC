#!/bin/bash -l
#SBATCH -D ./
#SBATCH --export=ALL
#SBATCH -p nodes 
#SBATCH -t 1
#SBATCH --output=stencil_job_%j.out
#SBATCH --error=stencil_job_%j.err

# Load the required modules
module purge
module load compilers/intel/2019u5 
module load mpi/openmpi/4.0.1_gcc-5.5.0 

# Get job information
procs=${SLURM_NTASKS:-1}
cores=${SLURM_CPUS_PER_TASK:-1}

# Set the number of OpenMP threads
export OMP_NUM_THREADS=$cores

echo "Running with $procs MPI processes and $cores OpenMP threads per process"

# Compile the code 
make all

echo 
echo "=====RUNNING PROGRAMS====="

# Run the OpenMP-only versions first
echo "Running OpenMP version with $cores threads with GNU"
./stencil-nearly-gcc linked_datafiles/input_1_3840_2048.dat linked_datafiles/filter_5.dat linked_datafiles/output_omp_3840_2048x5.dat
echo

# Then run the MPI+OpenMP versions
echo "Running MPI version with $procs processes and $cores threads per process with GNU"
mpirun -np $procs ./stencil-complete-icc linked_datafiles/input_1_3840_2048.dat linked_datafiles/filter_5.dat linked_datafiles/output_mpi_3840_2048x5.dat
echo


# Clean up
make clean
