/**
 * @file TSPSolver.cpp
 * @brief TSP solver (Ant System)
 *
 */

#include "TSPSolver.h"
#include <iostream>

bool TSPSolver::solve ( TSP& tsp , const TSPSolution& initSol , int maxIter, double ro, TSPSolution& bestSol ){
  try  {
    bool stop = false;
    int  iter = 0;

    TSPSolution currSol(initSol);
    double bestValue, currValue;
    bestValue = currValue = evaluate(bestSol,tsp);
    
    while ( ! stop ) {
      ++iter;
      if ( tsp.n < 20 ) currSol.print();
      
      tour(tsp, ro, currSol);
      
      currValue = evaluate(currSol,tsp);
      std::cout << " (" << iter << ") value " << evaluate(currSol,tsp)  << " ---  best:" << bestValue << " ---";
      
      if ( currValue < bestValue ) {
        bestValue = currValue;
        bestSol = currSol;
        double lung=1/bestValue;
        //attribuisco del feromone "bonus" al nuovo cammino ottimo
        for ( unsigned int i = 0 ; i < currSol.sequence.size() -1 ; ++i ) {
	    	int from = currSol.sequence[i]  ;
	    	int to   = currSol.sequence[i+1];
	    	tsp.feromone[from][to]=tsp.feromone[from][to]+(1/lung);
	    	tsp.feromone[to][from]=tsp.feromone[to][from]+(1/lung);
	    }//for
        
        std::cout << "************";
      }//if
      
      if ( iter > maxIter ) { /// stopping criteria
        stop = true;
      }//if
      std::cout << std::endl;  
    }//while
    
  }  catch(std::exception& e)  {
    std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
    return false;
  }//try-catch
  return true;
}//solve

int TSPSolver::bestNeighbor(TSP& tsp, int ind, bool* scelti){
	
  	double bestVal = tsp.min;
  	int bestInd = 0;
  	for(int i = 0 ; i < tsp.n ; i++ ){
  		if(!scelti[i]){
			double tmp = (1/tsp.cost[ind][i])+tsp.feromone[ind][i];
			if(tmp > bestVal){
				bestVal=tmp;
				bestInd=i;
			}//if
		}//if
	}//for
  	return bestInd;
  }//bestNeighbor

bool TSPSolver::tour ( TSP& tsp, double ro, TSPSolution& sol ) {
	int partenza = rand() % (sol.sequence.size()-1)  ;
	
	bool scelti[tsp.n];
 	sol.sequence[sol.sequence.size()-1]=partenza;
 	sol.sequence[0]=partenza;
 	for(int i=0; i<tsp.n; i++){
 		scelti[i]=false;
	}//for
	scelti[partenza]=true;
	
	int indice= partenza;	
    for ( unsigned int i = 1 ; i < sol.sequence.size() -1 ; ++i ) {
      int idx = bestNeighbor(tsp, indice, scelti);
      sol.sequence[i]=idx;
      scelti[idx]=true;
      indice=idx;
    }//for
    
    
    double lung = evaluate(sol,tsp) ;
	// cospargi il percorso di un valore pari all'inverso della lunghezza
    for ( unsigned int i = 0 ; i < sol.sequence.size() -1 ; ++i ) {
    	int from = sol.sequence[i]  ;
    	int to   = sol.sequence[i+1];
    	tsp.feromone[from][to]=tsp.feromone[from][to]+(1/lung);
    	tsp.feromone[to][from]=tsp.feromone[to][from]+(1/lung);
    }//for
    
    tsp.disperdiFeromone(ro);
    
    return true;
}//tour


