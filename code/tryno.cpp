#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <list>
#include <algorithm>

using namespace std;

const int RADIORANGE = 10;
const int SCOPE = 100;
const int N = 500;//the number of nodes 
const double dutycycle = 0.1;
const double STDDEV = 10.0;
const double EXPECTATION = 50.0;

vector<int> RDS;
int T;
double n; 
double pt;
int neighborMap[N][N];
int latencyMap[N][N];
default_random_engine generator;
normal_distribution<double> distribution(50.0,10.0);

class node{
public:
	int startTime;
  	double x, y;
  	int ID;
  	int state;//0 sleep, 1 transmit, 2 listen
  	double fi;
  	double ni;
	
	node(int id)
	{
		ID = id;
	    startTime = rand()%T;
	    x = distribution(generator);
	    y = distribution(generator);
		state = 0;
		fi = 1/(2*3.14159*STDDEV*STDDEV)*exp(-0.5/STDDEV/STDDEV*((x-EXPECTATION)*(x-EXPECTATION)+(y-EXPECTATION)*(y-EXPECTATION)));
		ni = N*3.14159*RADIORANGE*RADIORANGE*fi;
	}
};


vector<node> nodelist;

bool ifNeighbor(int i, int j)
{
	if(sqrt((nodelist[i].x-nodelist[j].x)*(nodelist[i].x-nodelist[j].x)+(nodelist[i].y-nodelist[j].y)*(nodelist[i].y-nodelist[j].y)) < RADIORANGE){
		return true;
	}
	else{
		return false;
	} 
}

void init()
{
	T = ceil(9/(4*dutycycle*dutycycle));
	n = N*(3.14159*RADIORANGE*RADIORANGE)/(SCOPE*SCOPE);
	pt = 1/n;
	
	for(int i = 0; i < N; i++) 
	{
		node* newnode = new node(i);
		nodelist.push_back(*newnode);
		delete newnode;
	}
	
	stringstream ss;
	ss<<"neighbor.txt";
	fstream f(ss.str().c_str(), std::ios::out);
	
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			if(i == j || ifNeighbor(i, j) == false){
				neighborMap[i][j] = 0;
				latencyMap[i][j] = 0;
			}
			else{
				neighborMap[i][j] = 1;
				latencyMap[i][j] = -1;
//				cout<<"1";
			}
			f<<neighborMap[i][j]<<","; 
		}
		f<<endl;
	}
}



int main()
{
	init();
	int nb[N]={0};
	int nnnn[N]={0};
	
	for(int i = 0; i < N; i++){
		
		for(int j = 0; j < N; j++){
			if(neighborMap[i][j] == 1){
				nb[i]++;
			}
		}
	}
	for(int i = 0; i < N; i++){
		cout<<i<<"="<<nb[i]<<"="<<nodelist[i].ni<<"\t";
		nnnn[nb[i]]++;
	}
	
//	for(int i = 0; i < N; i++){
//		cout<<i<<"="<<nnnn[i]<<"\t";
//	}
	
	return 0;
}
