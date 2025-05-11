# Parallel Matrix Convolution Engine

This project implements a stencil computation for 3D data arrays, with both OpenMP and MPI parallelization options.

## Project Structure

- `main-nearly.c` — Main program for OpenMP (shared-memory) version.
- `main-mpi.c` — Main program for MPI (distributed-memory) version.
- `stencil.c` — Core stencil computation kernel.
- `file-reader.c` — File I/O utilities for reading/writing multidimensional arrays.
- `compare.c` — Utility to compare output files for correctness.
- `Makefile` — Build instructions for GCC and ICC, with and without MPI.
- `slurmJob.sh` — Example SLURM batch script for running the program on a cluster.
- `CA1_setup.sh` — Script to set up symbolic links to large data files.

## Building

To build all versions (GCC/ICC, OpenMP/MPI), run:

```sh
sbatch slurmJob.sh
```

This produces the following executables:
- `stencil-nearly-gcc` — OpenMP version (GCC)
- `stencil-complete-gcc` — MPI+OpenMP version (GCC)
- `stencil-nearly-icc` — OpenMP version (ICC)
- `stencil-complete-icc` — MPI+OpenMP version (ICC)

## Usage

### OpenMP Version

```sh
./stencil-nearly-gcc <input_file> <filter_file> <output_file>
```

### MPI Version

```sh
mpirun -np <num_procs> ./stencil-complete-gcc <input_file> <filter_file> <output_file>
```

### Comparing Output

To compare your output with a reference:

```sh
./compare <your_output_file> <reference_output_file>
```

## SLURM Job Submission

Use `slurmJob.sh` as a template for submitting jobs to a SLURM-managed cluster. Edit the script as needed for your environment.

## Data Files

Use `CA1_setup.sh` to create symbolic links to the required large data files in your workspace.

## Notes

- Input and output files must be in the format expected by [`file-reader.c`](file-reader.c).
- The stencil kernel is implemented in [`stencil.c`](stencil.c).

---
