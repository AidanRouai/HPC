gccnearly:
	gcc -fopenmp main-nearly.c stencil.c file-reader.c -o stencil-nearly-gcc

gcccomplete:
	mpicc -fopenmp main-mpi.c stencil.c file-reader.c -o stencil-complete-gcc

iccnearly:
	icc -qopenmp main-nearly.c stencil.c file-reader.c -o stencil-nearly-icc

icccomplete:
	mpiicc -qopenmp main-mpi.c stencil.c file-reader.c -o stencil-complete-icc
