all: 
	make gccnearly
	make gcccomplete
	make iccnearly
	make icccomplete

gccnearly:
    gcc -std=c99 -fopenmp main-nearly.c stencil.c file-reader.c -o stencil-nearly-gcc

gcccomplete:
    mpicc -std=c99 -fopenmp main-mpi.c stencil.c file-reader.c -o stencil-complete-gcc

iccnearly:
    icc -std=c99 -qopenmp main-nearly.c stencil.c file-reader.c -o stencil-nearly-icc

icccomplete:
    mpiicc -std=c99 -qopenmp main-mpi.c stencil.c file-reader.c -o stencil-complete-icc
