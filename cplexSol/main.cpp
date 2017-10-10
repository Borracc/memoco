//main.cpp prima parte esame MEMOC

#include "../letturaScritturaFile.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include <sys/time.h>
#include "cpxmacro.h"

using namespace std;

// error status and messagge buffer
int status;
char errmsg[BUF_SIZE];

// data
//const int N = 6; // N: numero di nodi
int N; // N: numero di nodi
const int zero = 0; // zero: indice nodo iniziale

//costi archi
vector< vector< double > > c;

/*MAP FOR VARS: declare map*/
vector<vector<vector<int> > > map;
const int K = 2; // K: set di variabili differenti (nell es. x y --> K=2)

const int NAME_SIZE = 512;
char name[NAME_SIZE];
	
void setupLP(CEnv env, Prob lp, int & numVars) {
	
	/*MAP FOR VARS: allocate map
	k=0 X vars
	k=1 Y vars
	*/
	map.resize(N);
	for ( int i = 0 ; i < N ; ++i ) {
		map[i].resize(N);
		for ( int j = 0 ; j < N ; ++j ) {
			map[i][j].resize(K);
			for (int k = 0 ; k < K ; ++k ) {
				map[i][j][k] = -1;
			}//for
		}//for
	}//for
	
	int current_var_position = 0;
	
	// add x vars
	for (int i = 0; i < N; ++i)	{
		for (int j = 0; j < N; ++j) {
			if(i == j)
				continue;
			
			char xtype = 'I';
			double lb = 0.0; //lower bound
			double ub = CPX_INFBOUND; //upper bound
			double obj = 0.0;
			snprintf(name, NAME_SIZE, "x_%d_%d", i, j);
			char* xname = (char*)(&name[0]);
			
			CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &obj, &lb, &ub, &xtype, &xname );
			map[i][j][0] = current_var_position;
            ++current_var_position;
		}//for
	}//for
	
	// add y vars
	for (int i = 0; i < N; ++i)	{
		for (int j = 0; j < N; ++j)	{
			if(i == j)
				continue;
				
			char ytype = 'B';
			double lb = 0.0; //lower bound
			double ub = 1.0; //upper bound
			double obj = c[i][j];
			snprintf(name, NAME_SIZE, "y_%d_%d", i, j);
			char* yname = (char*)(&name[0]);
			
			CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &obj, &lb, &ub, &ytype, &yname );
			
			map[i][j][1] = current_var_position;
            ++current_var_position;
		}//for
	}//for
	
	//CONSTRAINTS
	numVars = CPXgetnumcols(env, lp);
	//CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, n var coinvolte, right side , &sense(= 'E', <=, >= 'G'), &matbeg (sempre 0), &idx[0] what are variables (indici delle variabili usate), &coef[0] coefficienti delle variabili, 0, 0 );
	
	// add constraints 1
	std::vector<int> idxZ(N);
	std::vector<double> coefZ(N);
	for (int j = 1; j < N; j++)	{
		idxZ[j-1]=map[zero][j][0];
		coefZ[j-1]=1.0;
	}//for
	
	char sense = 'E';
	int matbeg = 0;
	double rhs = N;	
	CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, N, &rhs, &sense, &matbeg, &idxZ[0], &coefZ[0], 0, 0 );

	// add constraints 2
	for(int k=1; k<N; k++){		
		std::vector<int> idx(2*N - 2);
		std::vector<double> coef(2*N -2);
		
		int h=0;
		for (int i = 0; i < N; ++i)	{
			if(i == k)
				continue;
			idx[h]=map[i][k][0];
			coef[h]=1.0;
			h++;
		}//for
		
		for (int j = 0; j < N; ++j)	{
			if(j == k)
				continue;
			idx[h]=map[k][j][0];
			coef[h]= -1.0;
			h++;
		}//for
		
		char sense = 'E';
		int matbeg = 0;
		double res = 1.0;
		CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, h, &res, &sense, &matbeg, &idx[0], &coef[0], 0, 0 );
	}//for
	
	// add constraints 3
	for(int i = 0; i < N; ++i) {
		std::vector<int> idx(N);
		std::vector<double> coef(N);
		
		int h=0;
		for (int j = 0; j < N; ++j)	{
			if(i == j)
				continue;
			idx[h]=map[i][j][1];
			coef[h]=1.0;
			h++;
		}//for
		
		char sense = 'E';
		int matbeg = 0;
		double res = 1.0;
		CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, h, &res, &sense, &matbeg, &idx[0], &coef[0], 0, 0 );
	}//for
	
	// add constraints 4
	for(int j = 0; j < N; ++j) {
		std::vector<int> idx(N-1);
		std::vector<double> coef(N-1);
		
		int h=0;
		for (int i = 0; i < N; ++i)	{
			if(i == j)
				continue;			
			idx[h]=map[i][j][1];
			coef[h]=1.0;
			h++;
		}//for
		
		char sense = 'E';
		int matbeg = 0;
		double res = 1.0;
		CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, h, &res, &sense, &matbeg, &idx[0], &coef[0], 0, 0 );
	}//for
	
	// add constraints 5
	std::vector<int> idx(2);
	std::vector<double> coef(2);
	
	for(int j = 0; j < N; ++j) {
		for (int i = 0; i < N; ++i)	{
			if(i == j)
				continue;
			idx[0]=map[i][j][0];
			idx[1]=map[i][j][1];
			coef[0]=1.0;
			coef[1]=-N;
			char sense = 'L';
			int matbeg = 0;
			double res = 0;
			
			CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, 2, &res, &sense, &matbeg, &idx[0], &coef[0], 0, 0 );
		}//for
	}//for
	
	// print (debug)
	CHECKED_CPX_CALL( CPXwriteprob, env, lp, "esameParte1.lp", 0 );
}//setupLP

int main (int argc, char *argv[]){
	
	if (argc < 2) throw std::runtime_error("usage: ./main filename.dat"); 
	
	//costi archi
	c = letturaMatriceFile(argv[1]);
	
	N = c.size();
	
	std::cout << "ESEGUZIONE ...  " << std::endl;
	DECL_ENV( env );
    DECL_PROB( env, lp );
	int numVars;
	setupLP(env, lp, numVars);
	
	CPXsetdblparam(env, CPX_PARAM_TILIM, 240.0);
	
	// optimize
	try{
		clock_t t1,t2;
	    t1 = clock();
	    struct timeval  tv1, tv2;
	    gettimeofday(&tv1, NULL);
	    
		CHECKED_CPX_CALL( CPXmipopt, env, lp );
		// print
		double objval=0.0;
		CHECKED_CPX_CALL( CPXgetobjval, env, lp, &objval );
		std::cout << "Objval: " << objval << std::endl;
		
		t2 = clock();
		gettimeofday(&tv2, NULL);
		
		int n = CPXgetnumcols(env, lp);
		cout << "check num var: " << n << " == " << numVars << endl;
		if (n != numVars) { throw std::runtime_error(std::string(__FILE__) + ":" + STRINGIZE(__LINE__) + ": " + "different number of variables"); }
		std::vector<double> varVals;
		varVals.resize(n);
		CHECKED_CPX_CALL( CPXgetx, env, lp, &varVals[0], 0, n - 1 );
		
		double user_time = (double)(tv2.tv_sec+tv2.tv_usec*1e-6 - (tv1.tv_sec+tv1.tv_usec*1e-6));
		double cpu_time = (double)(t2-t1) / CLOCKS_PER_SEC;
		
		for ( int i = 0 ; i < n ; ++i ) {
			std::cout << "var in position " << i << " : " << varVals[i] << std::endl;
  		}//for
		
		CHECKED_CPX_CALL( CPXsolwrite, env, lp, "esTSP.sol" );
		
		std::cout << "(value : " << objval << ")\n";	
	    std::cout << "in " << user_time << " seconds (user time)\n";
	    std::cout << "in " << cpu_time << " seconds (CPU time)\n-----------------------\n\n";
		
		// free
		CPXfreeprob(env, &lp);
		CPXcloseCPLEX(&env);
	}catch(std::exception& e){
		std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
	}//try-catch

	return 0;
}//main
