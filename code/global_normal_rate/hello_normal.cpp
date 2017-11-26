#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <list>
#include <random>

using namespace std;

const int N =1000;
const int startRange = 300; 
const int SCOPE = 100;
const int RADIORANGE = 5;
const double dutycycle = 0.1;
const double pt = 0.5;

int neighborMap[N][N];
int latencyMap[N][N];
vector<int> primeset;
default_random_engine generator;
normal_distribution<double> distribution(50.0,15.0);

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
	int n;
	int c;
	double x, y;
	double theta;//duty cycle
	int id;
	int startTime;
	int state;

	node(int ID)
	{
		id = ID;
		theta = dutycycle;
		startTime = rand()%startRange;
		x = distribution(generator);
	    y = distribution(generator);
	    hello_generate_cn();
	}
	
	void hello_generate_cn(){
		double num = 48.5 / ( 97*theta - 1 );
		for(int i = 0; i < primeset.size(); i++){
			if(primeset[i] == num){
				this->n = primeset[i];
				break;
			}
			else if(primeset[i] > num && i >= 1){
				if( primeset[i]-num < num-primeset[i-1]){
					this->n = primeset[i];
				}
				else{
					this->n = primeset[i-1];
				}
				break;
			}
			else if(primeset[i] > num && i == 0){
				this->n = primeset[i];
				break;
			}
		}
		c = 97;
		
	}
	
	void setState(int time){
		if(time - startTime > 0){
	  		if((time-startTime) % (c * n) < c/2.0 || ( time-startTime ) % c == 0){
	  			if(rand()%10 < pt*10){
	  				state = 1;
				}
				else{
				  	state = 2;
				}
			}
	  		else{
	  			state = 0;
			}
		}
		else if (time - startTime == 0){
			if(rand()%10 < pt*10){
  				state = 1;
			}
			else{
			  	state = 2;
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
	ss<<"hello_neighbor.txt";
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

void Hello(int timeInterval)
{
	init();
	
	int onNeighbor[N];
	int channelSum = 0;
	int discovered = 0;
	
	stringstream sss;
	sss<<"Hello_normal_rate.txt";
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
	
}


int main(int argc, char** argv)
{
	srand(time(NULL));
	findAllPrimes(1000);
	Hello(1000000);
	return 0;
}
