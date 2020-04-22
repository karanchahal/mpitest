openmp:
	g++ -fopenmp -o solve solveopenmp.cpp
mpi:
	mpic++ -fopenmp -o solve solvempi.cpp
hybrid:
	mpic++ -fopenmp -o solve solvehybrid.cpp

