//setupLP.h

#include <cstdio>
#include <iostream>
#include <vector>
#include "cpxmacro.h"

void setupLP(CEnv env, Prob lp, int & numVars) {
	
	std::cout << " setupLP ...  " << std::endl;
	
	std::cout << " map ...  " << std::endl;
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
			}
		}
	} 
	
	int current_var_position = 0;
	std::cout << " var X ...  " << std::endl;
	// add x vars
	const int x_init = CPXgetnumcols(env, lp); // first index for x vars
	for (int i = 0; i < N; ++i)	{
		for (int j = 0; j < N; ++j) {
			char xtype = 'I';
			double lb = 0.0; //lower bound
			double ub = CPX_INFBOUND; //upper bound
			double obj = 0.0;
			snprintf(name, NAME_SIZE, "x_%c_%c", nameN[i], nameN[j]);
			char* xname = (char*)(&name[0]);
			
			CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &obj, &lb, &ub, &xtype, &xname );
			
			map[i][j][0] = current_var_position;
            ++current_var_position;
		}
	}
	
	std::cout << " var Y ...  " << std::endl;
	// add y vars
	const int y_init = CPXgetnumcols(env, lp); // first index for y vars
	for (int i = 0; i < N; ++i)	{
		for (int j = 0; j < N; ++j)	{
			char ytype = 'B';
			double lb = 0.0; //lower bound
			double ub = 1.0; //upper bound
			double obj = c[i*N+j];
			snprintf(name, NAME_SIZE, "y_%c_%c", nameN[i], nameN[j]);
			char* yname = (char*)(&name[0]);
			
			CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &obj, &lb, &ub, &ytype, &yname );
			
			map[i][j][1] = current_var_position;
            ++current_var_position;
		}
	}
	
	std::cout << " CONSTRAINTS ...  " << std::endl;
//CONSTRAINTS
	numVars = CPXgetnumcols(env, lp);
//CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, n var coinvolte, right side , &sense(= 'E', <=, >= 'G'), &matbeg (sempre 0), &idx[0] what are variables (indici delle variabili usate), &coef[0] coefficienti delle variabili, 0, 0 );
	std::cout << " CONSTRAINTS 1 ...  " << std::endl;
	// add constraints 1
	std::vector<int> idxZ(N);
	std::vector<double> coefZ(N);
	for (int j = 0; j < N; ++j)	{
		idxZ[j]=map[zero][j][0];
		coefZ[j]=1.0;			
	}
	
	char sense = 'E';
	int matbeg = 0;
	double rhs = N;
	
	CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, N, &rhs, &sense, &matbeg, &idxZ[0], &coefZ[0], 0, 0 );
	std::cout << " CONSTRAINTS 2...  " << std::endl;
	// add constraints 2
	for(int k=0; k<N; k++){		
		std::vector<int> idx(2*N);
		std::vector<double> coef(2*N);
		if(k==zero){
			++k;
		}
		
		for (int i = 0; i < N; ++i)	{
			idx[i]=map[i][k][0];
			coef[i]=1.0;
		}
		
		for (int j = 0; j < N; ++j)	{
			idx[N+j]=map[k][j][0];
			coef[N+j]= -1.0;
		}
		
		char sense = 'E';
		int matbeg = 0;
		double res = 1.0;
		
		CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, N, &res, &sense, &matbeg, &idx[0], &coef[0], 0, 0 );
	}
	std::cout << " CONSTRAINTS 3...  " << std::endl;
	// add constraints 3
	for(int i = 0; i < N; ++i) {
		std::vector<int> idx(N);
		std::vector<double> coef(N);
		
		for (int j = 0; j < N; ++j)	{
			idx[j]=map[i][j][1];
			coef[j]=1.0;
		}
		
		char sense = 'E';
		int matbeg = 0;
		double res = 1.0;
		
		CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, N, &res, &sense, &matbeg, &idx[0], &coef[0], 0, 0 );
	}
	std::cout << " CONSTRAINTS 4...  " << std::endl;
	// add constraints 4
	for(int j = 0; j < N; ++j) {
		std::vector<int> idx(N);
		std::vector<double> coef(N);
		
		for (int i = 0; i < N; ++i)	{
			idx[i]=map[i][j][1];
			coef[i]=1.0;
		}
		
		char sense = 'E';
		int matbeg = 0;
		double res = 1.0;
		
		CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, N, &res, &sense, &matbeg, &idx[0], &coef[0], 0, 0 );
	}
	std::cout << " CONSTRAINTS 5...  " << std::endl;
	// add constraints 5
	int idxS;
	double coefS;
	for(int j = 0; j < N; ++j) {
		for (int i = 0; i < N; ++i)	{
			idxS=map[i][j][0];
			coefS=1.0;
			char sense = 'L';
			int matbeg = 0;
			double res = N*map[i][j][1];
			
			CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, 1, &res, &sense, &matbeg, &idxS, &coefS, 0, 0 );
		}
	}
	
	// print (debug)
	CHECKED_CPX_CALL( CPXwriteprob, env, lp, "esameParte1.lp", 0 );
}
