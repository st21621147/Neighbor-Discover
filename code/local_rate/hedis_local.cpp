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

const int N =500;
const int startRange = 300; 
const int SCOPE = 100;
const int RADIORANGE = 10;
const double pt = 0.5;

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
	int anchor;
	double x, y;
	double theta;//duty cycle
	int id;
	int startTime;
	int state;

	node(int ID)
	{
		id = ID;
		theta = 0.05+0.02*(rand()%6);
		startTime = rand()%startRange;
		x = rand()%(SCOPE+1);
	    y = rand()%(SCOPE+1);
	    state = 0;
	    hedis_generate_anchor();
	}
	
	void hedis_generate_anchor(){
		double num = 2/theta;
		for(int i = 0; i < primeset.size(); i++){
			if(primeset[i] == num){
				this->anchor = primeset[i];
				break;
			}
			else if(primeset[i] > num && i >= 1){
				this->anchor = primeset[i]-num < num -  primeset[i-1] ? primeset[i] : primeset[i-1];
				break;
			}
			else if(primeset[i] > num && i == 0){
				this->anchor = primeset[i];
				break;
			}
		}
	}
	
	void setState(int time){
		if( time - startTime > 0){
	  		if ( (time - startTime) % anchor == 0 || (time - startTime - 1) % ( anchor + 1 ) == 0 ){
	      		if(rand()%10 < pt*10){
	  				state = 1;
				}
				else{
				  	state = 2;
				}
	  		}
		}
		else{
			state = 0;
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
	ss<<"hedis_neighbor.txt";
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

void Hedis(int timeInterval)
{
	init();
	
	int onNeighbor[N];
	int channelSum = 0;
	int discovered = 0;
	
	stringstream sss;
	sss<<"Hedis_rate.txt";
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
			nodelist[i].setState(time);
		}
		for(int i = 0; i < N; i++){
			if(nodelist[i].state == 1){
				for(int j = 0; j < N; j++){
					if(neighborMap[i][j] == 1 && nodelist[j].state == 2){
						onNeighbor[i]++;
					}
				}
			}
		}
		
		for(int i = 0; i < N; i++){
			if(nodelist[i].state == 1 && onNeighbor[i] == 1){
				for(int j = 0; j < N; j++){
					if(neighborMap[i][j] == 1 && nodelist[j].state == 2){
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
}


int main(int argc, char** argv)
{
	srand(time(NULL));
	findAllPrimes(1000);
	Hedis(100000);

	return 0;
}
