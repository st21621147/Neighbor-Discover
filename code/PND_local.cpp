#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <list>

using namespace std;

const int N =1000;
const int modnum = 100;
const int startRange = 300; 
const int SCOPE = 100;
const int RADIORANGE = 10;
const double c_coll = 1.5;
const double c_idle = 1.5;

int neighborMap[N][N];
int latencyMap[N][N];


class node{
public:
	double pt;
	double pl;
	double c;
	double x, y;
	int id;
	int state;//0 idle, 1 transmit, 2 listen
	int startTime;
	bool stable;

	node(int ID)
	{
		id = ID;
		state = 0;
		c = 0.05+0.02*(rand()%6);
		pt = 1.0/N;
		pl = c-pt;
		startTime = rand()%startRange;
		x = rand()%(SCOPE+1);
	    y = rand()%(SCOPE+1);
		stable = false;
	}
	void setstate(int timeFrame)
	{
		if(timeFrame < startTime){
			state = 0;
			return;
		}
		int num = rand()%modnum;
		if(num < pt*modnum){
			state = 1;
		}
		else if(num < c*modnum){
			state = 2;
		}
		else{
			state = 0;
		}
	}
	
	void setPt(int choice)
	{
		if(choice == 0){
			pt = pt*c_idle;
			pl = c-pt;
		}
		else if(choice == 2){
			pt = pt/c_coll;
			pl = c-pt;
		}
		
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
	cout<<i<<","<<j<<","<<latencyMap[i][j]<<"\t";
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
	ss<<"PND_neighbor.txt";
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

void PND(int timeInterval)
{
	init();
	
	int onNeighbor[N];
	int max = 0;
	int un = 0;
	long long ave = 0;
	double temprob = 0;
	
	for(int i = 0; i < N; i++){
		onNeighbor[i] = 0;
	}
	
	for(int time = 0; time < timeInterval; time++){
		for(int i = 0; i < N; i++){
			if(nodelist[i].stable == false){
				nodelist[i].setstate(time);
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
						temprob = nodelist[i].pt;
						nodelist[i].pt = nodelist[j].pt;
						nodelist[j].pt = temprob;
						break;
					}
				}
			}
			else if(nodelist[i].state == 2 && onNeighbor[i] == 0){
				nodelist[i].setPt(0);
			}
			else if(nodelist[i].state == 2 && onNeighbor[i] >= 2){
				nodelist[i].setPt(2);
			}
		}
		
		for(int i = 0; i < N; i++){
			onNeighbor[i] = 0;
		}
	}
	stringstream sss;
	sss<<"PND_latency.txt";
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


int main(int argc, char** argv)
{
	srand(time(NULL));
	PND(100000);

	return 0;
}
