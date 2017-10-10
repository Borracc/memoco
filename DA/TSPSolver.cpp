/**
 * @file TSPSolver.cpp
 * @brief TSP solver
 */

#include "TSPSolver.h"
#include <iostream>

void findBestMoveN(const TSP& tsp, const TSPSolution& currSol, int enne, TSPMove& move){
	double best=tsp.infinite;
	for(int k=0; k<enne; k++){
		// cerco scambio random
	  	int a = 1 + (rand() % (tsp.n-1));
	  	int b = 1 + (rand() % (tsp.n-1));
	  	while(a == b){
	  		b = 1 + (rand() % (tsp.n-1));
	  	}//while
	  	int h = currSol.sequence[a-1];
    	int i = currSol.sequence[a];
    	int j = currSol.sequence[b];
    	int l = currSol.sequence[b+1];
	  	double variation = - tsp.cost[h][i] - tsp.cost[j][l] + tsp.cost[h][j] + tsp.cost[i][l] ;
	  	if(variation < best){
	  		best=variation;
	  		move.from=a;
			move.to=b;
		  }//if
	}//for
	
	
	
}//findBestMoveN

bool TSPSolver::solve ( const TSP& tsp , const TSPSolution& initSol , int maxIter, double teta , TSPSolution& bestSol ){
  try  {
    bool stop = false;
    int  iter = 0;
    TSPSolution currSol(initSol);
    TSPSolution rifSol(initSol);
    double bestValue, rifValue, currValue;
    bestValue = currValue = rifValue = evaluate(currSol,tsp);
    TSPMove move;
    while ( ! stop ) {
      ++iter;
      if ( tsp.n < 20 ) currSol.print();
      std::cout << " (" << iter << ") value " << rifValue << "\t(" << evaluate(currSol,tsp) << ")";
	  
	  findBestMoveN(tsp, currSol, tsp.n - iter, move);
	  
      std::cout << "\tmove: " << move.from << " , " << move.to;
 	  
      currSol = swap(currSol,move);
      currValue = evaluate(currSol,tsp);
      if ( currValue < rifValue * teta ) {
        rifValue = currValue;
      }//if
      
      if ( currValue < bestValue ) {
        bestValue = currValue;
        bestSol = currSol;
        std::cout << "\t***";
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

TSPSolution& TSPSolver::swap ( TSPSolution& tspSol , const TSPMove& move ) {
  TSPSolution tmpSol(tspSol);
  for ( int i = move.from ; i <= move.to ; ++i ) {
    tspSol.sequence[i] = tmpSol.sequence[move.to-(i-move.from)];
  }//for
  return tspSol;
}//swap
