openmp:
	g++ -fopenmp -o solve solveopenmp.cpp
mpi:
	mpic++ --std=c++11 -fopenmp -o solve solvempi.cpp
hybrid:
	mpic++ --std=c++11 -fopenmp -o solve solvehybrid.cpp

