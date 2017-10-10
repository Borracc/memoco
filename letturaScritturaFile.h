//letturaScritturaFile.h

#include <fstream>
#include <iostream>
#include <vector>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <errno.h>
using namespace std;

char creaNomeFile(char* inizio, int n, char* fine, char* nomeFile){
	char numchar[5];
    strcpy(nomeFile,inizio);
    sprintf(numchar,"%d",n);
	strcat(nomeFile, numchar);
	strcat(nomeFile,".dat");
	return *nomeFile;
}//creaNomeFile

char creaNomeFileTXT(char* inizio, char* nomeFile){		
    strcpy(nomeFile,inizio);
	strcat(nomeFile,".txt");
	return *nomeFile;
}//creaNomeFileTXT

char creaRecord(char* file, int i, double valore, double tempo, char* nomeRecord){		
	char numchar[8];
    strcpy(nomeRecord,file);
    strcat(nomeRecord, " ");
    sprintf(numchar,"%d",i);
	strcat(nomeRecord, numchar);
	strcat(nomeRecord, " ");    
    sprintf(numchar,"%.3f",valore);
	strcat(nomeRecord, numchar);
	strcat(nomeRecord, " ");
	sprintf(numchar,"%.3f",tempo);
	strcat(nomeRecord, numchar);	 
	return *nomeRecord;
}//creaNomeFileTXT

char creaRecordMedie(char* file, double mediaValore, double mediaTempo, double minimo, char* nomeRecord){		
	char numchar[8];
    strcpy(nomeRecord,file);
	strcat(nomeRecord, " ");    
    sprintf(numchar,"%.3f",mediaValore);
	strcat(nomeRecord, numchar);
	strcat(nomeRecord, " ");
	sprintf(numchar,"%.3f",mediaTempo);
	strcat(nomeRecord, numchar);
	strcat(nomeRecord, " ");
	sprintf(numchar,"%.3f",minimo);
	strcat(nomeRecord, numchar);	 
	return *nomeRecord;
}//creaNomeFileTXT
/////////////////////////////////////////////

void letturaFile(char* nomeFile){
	//Lettura
	cout << "LETTURA ... " << nomeFile <<"<---"<< endl;
	ifstream InFile; /* Dichiarazione di tipo */
	InFile.open (nomeFile,ios::in); /* Apertura del file */
	if (!InFile) {
		cout << "Errore di apertura del file" << endl << strerror(errno); /* controllo */
	} else {
		char ch;
		while(!InFile.eof()){
			InFile.get(ch);
			cout << ch;
		}//while
		InFile.close(); /* chiusura file */
	}//if-else
}//letturaFile

void letturaFileMemorizza(char* nomeFile, char testo[][30]){
	//Lettura
	cout << "LETTURA ... " << nomeFile <<"<---"<< endl;
	ifstream InFile; /* Dichiarazione di tipo */
	InFile.open (nomeFile); /* Apertura del file */
	if (!InFile) {
		cout << "Errore di apertura del file" << endl << strerror(errno)<< endl; /* controllo */
	} else {
		char ch[30];
		int indice=0;
		while(!InFile.eof()){
			InFile.getline(ch,31);
			//cout << ch;
			for(int k=0; k<30; k++){
				testo[indice][k]=ch[k];
			}
			indice++;
		}//while
		InFile.close(); /* chiusura file */
	}//if-else
}//letturaFile

vector< vector< double > > letturaMatriceFile(char* nomeFile ){
	//Lettura
	cout << "LETTURA ... " << nomeFile <<"<---\n";
	vector< vector< double > > dati;
	int enne=0;
	ifstream InFile; /* Dichiarazione di tipo */
	InFile.open(nomeFile, ios::in); /* Apertura del file */
	if (!InFile) {
		cout << "Errore di apertura del file" << endl << strerror(errno)<< endl; /* controllo */
	} else {
		char chEnne[4];
		InFile.get(chEnne,5);
		enne = atoi(chEnne);
		dati.resize(enne);
		for ( int i = 0 ; i < enne ; ++i ) {
			dati[i].resize(enne);
		}//for
		int i=0;
		int j=0;
		int indice=0;
		while((!InFile.eof()) && (indice < enne*enne)){
			InFile >> dati[i][j];
			j++;
			indice++;
			if(indice%enne==0){
				i++;
				j=0;
			}//if
		}//while
		InFile.close(); /* chiusura file */
	}//if-else
	return dati;
}//letturaFile

///////////////////////////////////////////////////
void scritturaFile(char* nomeFile, char* testo){
	//Scrittura
	cout << "SCRITTURA ... " << nomeFile << endl;
	ofstream OutFile; /* Dichiarazione di tipo */
	OutFile.open (nomeFile,ios::out); /* Apertura del file */
	if (!OutFile) {
		cout << "Errore di apertura del file" << endl; /* controllo */
	} else {
		OutFile << testo << endl; /* scrittura dati */
		OutFile.close(); /* chiusura file */
	}//if-else
}//scritturaFile

void scritturaFileAppend(char* nomeFile, char* testo){
	//Scrittura
	cout << "SCRITTURA-APPEND ... " << nomeFile << endl;
	ofstream OutFile; /* Dichiarazione di tipo */
	OutFile.open (nomeFile,ios::out|ios::app);/* Apertura del file */
	if (!OutFile) {
		cout << "Errore di apertura del file" << endl; /* controllo */
	} else {
		OutFile << testo << endl; /* scrittura dati */
		OutFile.close(); /* chiusura file */
	}//if-else
}//scritturaFile

void scritturaMatriceFile(char* nomeFile, int enne, vector<vector<double> > matrix){
	//Scrittura
	cout << "SCRITTURA ... file "<< nomeFile << endl;
	ofstream OutFile; /* Dichiarazione di tipo */
	OutFile.open (nomeFile,ios::out); /* Apertura del file */
	if (!OutFile) {
		cout << "Errore di apertura del file" << endl; /* controllo */
	} else {
		OutFile << enne << endl; 
		for(int i=0;i<enne;i++) {
			for(int j=0;j<enne;j++) {
				OutFile << matrix[i][j] << " "; /* scrittura dati */
			}//for j
			OutFile << endl;
		}//for i
		OutFile.close(); /* chiusura file */
	}//if-else
}//scritturaFile

