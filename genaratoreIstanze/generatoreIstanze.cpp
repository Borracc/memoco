//generatoreIstanze.cpp
//generatore di configurazioni casuali e casuali a Settori
//variando numero di buchi(n 10->100),

#include "../letturaScritturaFile.h"
#include <stdio.h> 
#include <stdlib.h>
#include <math.h> 
#include <ctime>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
using std::cin; //indica che cin e' definito nello spazio dei nomi "std".
using std::cout; //indica che cout e' definito nello spazio dei nomi "std".

//genera un numero intero random fra min e max
int interoRandom(int min,int max){
	return rand() % (max-min+1) + min;
}//interoRandom

//genera un numero reale random fra min e max
double realeRandom(double min,double max){
	double r = rand() / (double)RAND_MAX;//genera un numero reale tra [0.0, 1.0] 1.0 incluso
	r=r*(max-min)+min;
	return r;
}//realeRandom

//genera un una matrice emme(enne x enne) di interi random fra min e max
void generaCostiInteriRandom (int enne, double * emme, int min, int max){
    //costi archi
	for(int i=0;i<enne;i++){
		for( int j=0; j<enne; j++){
			emme[i*enne+j]=interoRandom(min,max);
		}//for j
	}//for i
}//generacostiInteriRandom

//genera un una matrice emme(enne x enne) di reali random fra min e max
void generaCostiRealiRandom (int enne, double * emme, double min, double max){
    //costi archi
	for(int i=0;i<enne;i++){
		for( int j=0; j<enne; j++){
			emme[i*enne+j]=realeRandom(min,max);
		}//for j
	}//for i
}//generaCostiRealiRandom

//verifica presenza di copie
bool isDoppio(int enne, int * x, int * y, int indice){
	bool esito=false;
	for(int i=0; i<enne; i++){
		if(i==indice){
			i++;
		}//if
		if((x[i]==x[indice])&&(y[i]==y[indice])){
			esito=true;
		}//if
	}//for i
	return esito;
}//isDoppio

//genera enne punti di coordinateintere random con x fra 0 e a e y fra 0 e b e li memorizza in 2 array[enne]
void generaVettoriInteriRandom (int enne, int * x, int * y, int a, int b){
    //costi archi
	for(int i=0;i<enne;i++){
		x[i]=interoRandom(0,a);
		y[i]=interoRandom(0,b);
	}//for i
	
	//verifica assenza punti coincidenti
	int ind=0;
	while(ind<enne){
		if(isDoppio(enne, x, y, ind)){
			x[ind]=interoRandom(0,a);
			y[ind]=interoRandom(0,b);
		}else{
			ind++;
		}//if-else
	}//while
}//generaVettoriInteriRandom

double distanzaEuclidea(int ax, int ay, int bx, int by){
	return sqrt(pow(ax-bx,2)+pow(ay-by,2));
}//distanzaEuclidea

int distanzaManhattan(int ax, int ay, int bx, int by){
	return (fabs((ax-bx))+fabs((ay-by)));
}//distanzaEuclidea

void stampaIstanza(int n, int *x, int *y, vector<vector<double> > dE, vector<vector<double> > dM){
	//vettori coordinate
	cout << "\nX: ";
	for(int i=0;i<n;i++){
		cout << " "<< x[i] <<" ";
	}//for i
	
	cout << "\nY: ";
	for(int i=0;i<n;i++){
		cout << " "<< y[i] <<" ";
	}//for i
	
	//distanze
	cout.precision(3);
	cout << "\nDistanze Euclidee: \n";
	for(int i=0;i<n;i++){
		for( int j=0; j<n; j++){
			if(i!=j){
				cout << " "<< dE[i][j] <<" ";
			}else{
				cout << " --- ";
			}//if-else
		}//for j
		cout << "\n";
	}//for i
	
	cout << "\nDistanze Manhattan: \n";
	for(int i=0;i<n;i++){
		for( int j=0; j<n; j++){
			if(i!=j){
				cout << " "<< dM[i][j] <<" ";
			}else{
				cout << " --- ";
			}//if-else
		}//for j
		cout << "\n";
	}//for i
}//stampaIstanza

void generaSettoriRandom(int enne, int * x, int * y, int a, int b){
	int settori[25];//=1 se settore scelto: =0 se non scelto
	for (int j=0; j<25; j++){
		settori[j]=0;
	}//for j
	
	int nSett= 7;
	
	int g=0;
	while(g<nSett){
		int caso=interoRandom(0,24);
		if(settori[caso]==0){
			settori[caso]=1;
			g++;
		}//if
	}//while
	
	int i=0;
	int j=0;
	while((i<enne)&&(j<25)){
		if(settori[j]==1){
			int k=j/5;
			int l=j%5;
		
			int mina=k*(a/5);
			int maxa=((k+1)*(a/5))-1;
			int minb=l*(b/5);
			int maxb=((l+1)*(b/5))-1;
			//costi archi
			for(int h=0;h<enne/7;h++){
				x[i]=interoRandom(mina,maxa);
				y[i]=interoRandom(minb,maxb);
				while(isDoppio(enne, x, y, i)){//verifica punto ripetuto
					x[i]=interoRandom(mina,maxa);
					y[i]=interoRandom(minb,maxb);
				}//while
				i++;
			}//for h

		}//if
		j++;
	}//while
	
	if(i<enne){
		do{
			x[i]=interoRandom(0,a);
			y[i]=interoRandom(0,b);
			if(!isDoppio(enne, x, y, i)){
				i++;
			}//if
		}while(i<enne);
	}//if
}//genareSettori

void generaIstanzaSettori(int n, int a, int b){
	
	int x[n];
	int y[n];
	
	generaSettoriRandom(n,x,y,a,b);	
	
	vector< vector< double > > dE;
    vector< vector< double > > dM;
	
	dE.resize(n);
	dM.resize(n);
	for ( int i = 0 ; i < n ; ++i ) {
		dE[i].resize(n);
		dM[i].resize(n);
	}//for
	
	for(int i=0;i<n;i++){
		for( int j=0; j<n; j++){
			dE[i][j]=distanzaEuclidea(x[i],y[i],x[j],y[j]);
			dM[i][j]=distanzaManhattan(x[i],y[i],x[j],y[j]);
		}//for j
	}//for i
	
	char nomeFile[30];
	creaNomeFile((char*)"../istanze/SE",n,(char*)".dat",nomeFile);
	scritturaMatriceFile(nomeFile,n,dE);
	scritturaFileAppend((char*)"../istanze/nomiFiles.txt",nomeFile);
	creaNomeFile((char*)"../istanze/SM",n,(char*)".dat",nomeFile);
	scritturaMatriceFile(nomeFile,n,dM);
	scritturaFileAppend((char*)"../istanze/nomiFiles.txt",nomeFile);	
	
}//generaIstanzaSettori


void generaIstanzaCasuale(int n, int a, int b){
	int x[n];
	int y[n];
    
	generaVettoriInteriRandom(n,x,y,a,b); 	
	
	vector< vector< double > > dE;
    vector< vector< double > > dM;
	
	dE.resize(n);
	dM.resize(n);
	for ( int i = 0 ; i < n ; ++i ) {
		dE[i].resize(n);
		dM[i].resize(n);
	}//for
	
	for(int i=0;i<n;i++){
		for( int j=0; j<n; j++){
			dE[i][j]=distanzaEuclidea(x[i],y[i],x[j],y[j]);
			dM[i][j]=distanzaManhattan(x[i],y[i],x[j],y[j]);
		}//for j
	}//for i

	char nomeFile[30];
	creaNomeFile((char*)"../istanze/CE",n,(char*)".dat",nomeFile);
	scritturaMatriceFile(nomeFile,n,dE);
	scritturaFileAppend((char*)"../istanze/nomiFiles.txt",nomeFile);
	creaNomeFile((char*)"../istanze/CM",n,(char*)".dat",nomeFile);
	scritturaMatriceFile(nomeFile,n,dM);
	scritturaFileAppend((char*)"../istanze/nomiFiles.txt",nomeFile);
		
}//generaIstanzaCasuale

int main(){
	srand((unsigned)time(NULL));
	
	for(int n=10; n<=100; n=n+10){
		generaIstanzaCasuale(n,20,20);
	}//for n
	
	for(int n=10; n<=100; n=n+10){
		generaIstanzaSettori(n,20,20);
	}//for n
	
    cout << "Questo programma e' stato scritto da Davide Meneghetti.\n";
    //fflush(stdin);
    cout << "Premere invio per continuare.";
    //getchar();
    return 0;
}// main
