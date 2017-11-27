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
const int N = 500;//the number of nodes 
const int STARTRANGE = 250;

double dutycycle;
double pt;
int neighborMap[N][N];
int latencyMap[N][N];

class node{
public:
//	double pt;
//	double pl;
//	double c;
	int startTime;
  	double x, y;
  	int ID;
  	int state;//0 sleep, 1 transmit, 2 listen
	
	node(int id)
	{
		ID = id;
	    startTime = rand()%STARTRANGE;
	    x = rand()%(SCOPE+1);
	    y = rand()%(SCOPE+1);
		state = 0;
//		c = dutycycle;
//		pt = 1.0/N;
//		pl = c-pt;
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
	ss<<"Aloha_neighbor.txt";
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



void Aloha(int timeInterval)
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
				int temp = rand()%100;
				if (temp < dutycycle * 100)//find
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
	sss<<"Aloha_latency.txt";
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
//		cout<<max<<" ";
		max = 0;
	}

//	cout<<"discovery rate:"<<(N-un)*1.0/N<<endl;
	ff<<dutycycle<<","<<ave*1.0/N<<endl; 
}

void Duty()
{
	for(dutycycle = 0.1; dutycycle < 0.51; dutycycle += 0.05){
		pt = 1.0/N/dutycycle;
		Aloha(100000);
	}
	
}


int main()
{
	srand(time(NULL));
	Duty();

	return 0;
}
