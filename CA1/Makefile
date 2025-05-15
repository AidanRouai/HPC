#Targets
omp_target = main-nearly.c stencil.c
mpi_target = main-mpi.c stencil.c
file_reader_target = file-reader.c

#Executables
omp_out = stencil-nearly
mpi_out = stencil-complete

#Flags
op = -O3

all: 
	make gccnearly
	make gcccomplete
	make iccnearly
	make icccomplete

gccnearly: $(omp_target) $(file_reader_target)
	gcc -fopenmp -std=c99 $(op) $(omp_target) $(file_reader_target) -o $(omp_out)-gcc -lm

gcccomplete: $(mpi_target) $(file_reader_target)
	mpicc -fopenmp -std=c99 $(op) $(mpi_target) $(file_reader_target) -o $(mpi_out)-gcc -lm

iccnearly: $(omp_target) $(file_reader_target)
	icc -qopenmp -std=c99 $(op) $(omp_target) $(file_reader_target) -o $(omp_out)-icc

icccomplete: $(mpi_target) $(file_reader_target)
	mpiicc -qopenmp -std=c99 $(op) $(mpi_target) $(file_reader_target) -o $(mpi_out)-icc

clean:
	rm -f $(omp_out)-gcc $(mpi_out)-gcc $(omp_out)-icc $(mpi_out)-icc $(mpi_out)-db

.PHONY: all gccnearly gcccomplete iccnearly icccomplete clean
