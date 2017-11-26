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

double n; 
double pt;
int neighborMap[N][N];
int latencyMap[N][N];
vector<int> primeset;

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
	node(int id)
	{
		ID = id;
	    x = rand()%(SCOPE+1);
	    y = rand()%(SCOPE+1);
		state = 0;
		dutycycle = 0.05+0.02*(rand()%6);
		computerT();
		startTime = rand()%T;
	}
	
	void computerT()
	{
		int temp = ceil(2.0/dutycycle);
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
	n = N*(3.14159*RADIORANGE*RADIORANGE)/(SCOPE*SCOPE);
	pt = 1.0/n;
	
	for(int i = 0; i < N; i++) 
	{
		node* newnode = new node(i);
		cout<<i<<endl;
		nodelist.push_back(*newnode);
		delete newnode;
	}
	
	stringstream ss;
	ss<<"local_neighbor.txt";
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
			}
			f<<neighborMap[i][j]<<","; 
		}
		f<<endl;
	}
}

void Alano(int i) 
{
	int r = rand()%100;
	if(r < pt*100){
		nodelist[i].state = 1;
	}
	else{
		nodelist[i].state = 2;
	}
}


void TPAlano(int timeInterval)
{
	cout<<"before"<<endl;
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
	sss<<"local_latency.txt";
	fstream ff(sss.str().c_str(), std::ios::out);
	
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			ff<<latencyMap[i][j]<<",";
		}
		ff<<endl;
	}
	
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
	
	cout<<un<<endl;
	cout<<ave<<endl;
	cout<<"discovery rate:"<<(N-un)*1.0/N<<endl;
	cout<<"average latency:"<<ave*1.0/N<<endl; 
}


int main()
{
	srand(time(NULL));
	findAllPrimes(1000); 
	TPAlano(50000);

	return 0;
}
