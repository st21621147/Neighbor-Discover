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

const int RADIORANGE = 5;
const int SCOPE = 100;
const int N = 1000;//the number of nodes 
const double EXPECTATION = 50.0;
const double STDDEV = 15.0;

//double n; 
//double pt;
double die;
int neighborMap[N][N];
int latencyMap[N][N];
vector<int> primeset;
default_random_engine generator;
normal_distribution<double> distribution(EXPECTATION, STDDEV);

void findAllPrimes(int n)
{
    primeset.push_back(2);
    for(int k = 3; k<=n; k++)
    {
        bool isPrime = true;
        for(int i = 2; i*i <= k; i++)
        {
            if(k % i == 0)
            {
                isPrime = false;
                break;
            }
        }
        if(isPrime)
        {
            primeset.push_back(k);
            
        }
    }
}

class node{
public:
	int startTime;
  	double x, y;
  	int ID;
  	int state;//0 sleep, 1 transmit, 2 listen
	int T;
	double dutycycle;
	double fi;
	double ni;
	double pt;
	bool dead;
	
	node(int id)
	{
		ID = id;
	    
	    x = distribution(generator);
	    y = distribution(generator);
		state = 0;
//		dutycycle = 0.1*(rand()%5+1);
		dutycycle = 0.05+0.02*(rand()%6);
		computerT();
		startTime = rand()%T;
		fi = 1.0/(2*3.14159*STDDEV*STDDEV)*exp(-0.5/STDDEV/STDDEV*((x-EXPECTATION)*(x-EXPECTATION)+(y-EXPECTATION)*(y-EXPECTATION)));
		ni = N*3.14159*RADIORANGE*RADIORANGE*fi;
		pt = 1.0/ni;
		double r = rand()/(RAND_MAX+1.0);
		if(r < die){
			dead = true;
		}
		else{
			dead = false;
		}
	}
	
	void computerT()
	{
		int temp = ceil(2/dutycycle);
		vector<int>::iterator it = find(primeset.begin(), primeset.end(), temp);
		while(it == primeset.end()){
			temp++;
			it = find(primeset.begin(),primeset.end(), temp);
		}
		T = temp;
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

void setLatency(int i, int j, int time)
{
	if(latencyMap[i][j] != -1) return;
	if(nodelist[i].startTime < nodelist[j].startTime){
		latencyMap[i][j] = time - nodelist[j].startTime;
	}
	else{
		latencyMap[i][j] = time - nodelist[i].startTime;
	}
//	cout<<i<<","<<j<<","<<latencyMap[i][j]<<"\t";
}

void init()
{
	for(int i = 0; i < N; i++) 
	{
		node* newnode = new node(i);
		nodelist.push_back(*newnode);
		delete newnode;
	}
	
	stringstream ss;
	ss<<"local_neighbor.txt";
	fstream f(ss.str().c_str(), std::ios::out);
	
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			if(i == j || ifNeighbor(i, j) == false || nodelist[i].dead == true || nodelist[j].dead == true){
				neighborMap[i][j] = 0;
				latencyMap[i][j] = 0;
			}
			else{
				neighborMap[i][j] = 1;
				latencyMap[i][j] = -1;
			}
			f<<neighborMap[i][j]<<","; 
		}
		f<<endl;
	}
}

void Alano(int i) 
{
	double r = rand()/(RAND_MAX+1.0);
	if(r < nodelist[i].pt){
		nodelist[i].state = 1;
	}
	else{
		nodelist[i].state = 2;
	}
}


void TPAlano(int timeInterval)
{
	init();
	
	int onNeighbor[N];
	int un = 0;
	long long ave = 0;
	int max = 0;
	
	
	
	for(int i = 0; i < N; i++){
		onNeighbor[i] = 0;
	}
	
	for(int time = 0; time < timeInterval; time++){
		for(int i = 0; i < N; i++){
			if(nodelist[i].startTime <= time){
				int t1 = (time-nodelist[i].startTime)%nodelist[i].T;
				int t2 = (time-nodelist[i].startTime)%(nodelist[i].T-1)+1;
				if (t1 == 0 || t1 == t2)
				{
					Alano(i);
				}
				else
				{
					nodelist[i].state = 0;
				}
			}
		}
		
		for(int i = 0; i < N; i++){
			if(nodelist[i].state == 2){
				for(int j = 0; j < N; j++){
					if(neighborMap[i][j] == 1 && nodelist[j].state == 1){
						onNeighbor[i]++;
					}
				}
			}
		}
		
		for(int i = 0; i < N; i++){
			if(nodelist[i].state == 2 && onNeighbor[i] == 1){
				for(int j = 0; j < N; j++){
					if(neighborMap[i][j] == 1 && nodelist[j].state == 1){
						setLatency(i, j, time);
						break;
					}
				}
			}
		}
		
		for(int i = 0; i < N; i++){
			onNeighbor[i] = 0;
		}
	}
	
	stringstream sss;
	sss<<"local_normal_latency.txt";
	fstream ff(sss.str().c_str(), std::ios::out|ios::app);
	
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			if(latencyMap[i][j] == -1){
				un++;
				max = timeInterval;
				break;
			}
			else if(latencyMap[i][j] > max){
				max = latencyMap[i][j];
			}
		}
		ave += max;
		max = 0;
	}
	
	ff<<die<<","<<(N-un)*1.0/N<<","<<ave*1.0/N<<endl; 
}

void Robust()
{
	for(die = 0; die < 0.301; die += 0.05){
		TPAlano(100000);
	}
}

int main()
{
	srand(time(NULL));
	findAllPrimes(1000); 
	Robust();

	return 0;
}

