/**
 * @file main.cpp
 * @brief 
 */

#include <stdexcept>
#include <ctime>
#include <sys/time.h>
#include "../letturaScritturaFile.h"
#include "TSPSolver.h"

// error status and messagge buffer
int status;
char errmsg[255];

void esegui( const char* nome, int maxIter, double teta ){
	
	TSP tspInstance; 
    tspInstance.read(nome);
    TSPSolution aSolution(tspInstance);
    
    clock_t t1,t2;
    t1 = clock();
    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);
    
    TSPSolver tspSolver;
    tspSolver.initRnd(aSolution); /// creaione soluzione casuale
    
	TSPSolution bestSolution(tspInstance);
    tspSolver.solve(tspInstance,aSolution , maxIter , teta, bestSolution);

    t2 = clock();
    gettimeofday(&tv2, NULL);
		
    std::cout << "FROM solution: "; 
    aSolution.print();
    std::cout << "(value : " << tspSolver.evaluate(aSolution,tspInstance) << ")\n";
    std::cout << "TO   solution: "; 
    bestSolution.print();
    std::cout << "(value : " << tspSolver.evaluate(bestSolution,tspInstance) << ")\n";
    std::cout << "in " << (double)(tv2.tv_sec+tv2.tv_usec*1e-6 - (tv1.tv_sec+tv1.tv_usec*1e-6)) << " seconds (user time)\n";
    std::cout << "in " << (double)(t2-t1) / CLOCKS_PER_SEC << " seconds (CPU time)\n-----------------------\n\n";
		
}//esegui



int main (int argc, char const *argv[]){
  try {    
    
    if (argc < 4) throw std::runtime_error("usage: ./main filename.dat maxiter teta"); 
	int maxIter    = atoi(argv[2]); 
	double teta    = atof(argv[3]);
        
    esegui(argv[1],maxIter,teta);	
    
  }  catch(std::exception& e)  {
    std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
  }//try-catch
  return 0;
}
