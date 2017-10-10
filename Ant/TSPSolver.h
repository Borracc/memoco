/**
 * @file TSPSolver.h
 * @brief TSP solver (neighborhood search)
 *
 */

#ifndef TSPSOLVER_H
#define TSPSOLVER_H

#include <vector>

#include "TSPSolution.h"

/**
 * Class that solves a TSP problem by neighbourdood search and 2-opt moves
 */
class TSPSolver{
	
	public:
	
	  TSPSolver ( ) { }
	
	  double evaluate ( const TSPSolution& sol , const TSP& tsp ) const {
	    double total = 0.0;
	    for ( unsigned int i = 0 ; i < sol.sequence.size() - 1 ; ++i ) {
	      int from = sol.sequence[i]  ;
	      int to   = sol.sequence[i+1];
	      total += tsp.cost[from][to];
	    }//for
	    return total;
	  }//evaluate
  
	  bool solve ( TSP& tsp , const TSPSolution& initSol , int maxIter , double ro, TSPSolution& bestSol );
	  int bestNeighbor( TSP& tsp, int ind, bool* scelti); 
	  bool tour ( TSP& tsp, double ro, TSPSolution& sol );
};

#endif /* TSPSOLVER_H */
