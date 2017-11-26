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

vector<int> RDS;
int T;
double n; 
double pt;
int neighborMap[N][N];
int latencyMap[N][N];

class node{
public:
	int startTime;
  	double x, y;
  	int ID;
  	int state;//0 sleep, 1 transmit, 2 listen
	
	node(int id)
	{
		ID = id;
	    startTime = rand()%T;
	    x = rand()%(SCOPE+1);
	    y = rand()%(SCOPE+1);
		state = 0;
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

void RDSConstruct(int Zbase)
{
	int lambda=ceil(sqrt(Zbase));
	int micro=ceil(ceil(sqrt(Zbase))/2);	
	for(int i = 1; i <= lambda; i++ ){
		RDS.push_back(i);
	}
	for(int i = 1; i <= micro; i++ ){
		RDS.push_back(1+i*lambda);
	}
	sort(RDS.begin(),RDS.end());
    RDS.erase(unique(RDS.begin(), RDS.end()), RDS.end());
}

void RDSAlano(int timeInterval)
{
	init();
	RDSConstruct(T);
	int onNeighbor[N];
//	int un = 0;
//	long long ave = 0;
//	int max = 0;
	int channelSum = 0;
	int discovered = 0;

	stringstream sss;
	sss<<"RDS_rate.txt";
	fstream ff(sss.str().c_str(), std::ios::out);
	
	for(int i = 0; i < N; i++){
		onNeighbor[i] = 0;
		for(int j = 0; j < N; j++){
			if(neighborMap[i][j] == 1){
				channelSum++;
			}
		}
	}
	
	for(int time = 0; time < timeInterval; time++){
		for(int i = 0; i < N; i++){
			if(nodelist[i].startTime <= time){
				vector<int>::iterator it=find(RDS.begin(),RDS.end(), (time-nodelist[i].startTime)%T+1);
				if (it != RDS.end())//find
				{
					Alano(i);
				}
				else//not find
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
						if(latencyMap[i][j] == -1){
							discovered++;
						}
						setLatency(i, j, time);
						break;
					}
				}
			}
		}
		ff<<discovered*1.0/channelSum<<endl;
		for(int i = 0; i < N; i++){
			onNeighbor[i] = 0;
		}
	}
	
	// stringstream sss;
	// sss<<"RDS_latency.txt";
	// fstream ff(sss.str().c_str(), std::ios::out);
	
	// for(int i = 0; i < N; i++){
	// 	for(int j = 0; j < N; j++){
	// 		ff<<latencyMap[i][j]<<",";
	// 	}
	// 	ff<<endl;
	// }
	
//	for(int i = 0; i < N; i++){
//		for(int j = 0; j < N; j++){
//			if(latencyMap[i][j] == -1){
//				un++;
//				max = timeInterval;
//				break;
//			}
//			else if(latencyMap[i][j] > max){
//				max = latencyMap[i][j];
//			}
//		}
//		ave += max;
//		max = 0;
//	}
//
//	
//	cout<<un<<endl;
//	cout<<ave<<endl;
//	cout<<"discovery rate:"<<(N-un)*1.0/N<<endl;
//	cout<<"average latency:"<<ave*1.0/N<<endl; 
}


int main()
{
	srand(time(NULL));
	RDSAlano(100000);
	return 0;
}
