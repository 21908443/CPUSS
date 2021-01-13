#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> // library for fcntl function 
#include <iostream>
#include <string>
#include <pthread.h>
#include "Queue.h"
#include <queue>
using namespace std;
pthread_t tid1;								
int seconds =0;											//variable for implementing clock
int avg_wait=0, avg_TAT=0, no_of_procs=0, throughput=0;							//for calculating throughput and cumulative waiting and turn around time
void* clock(void* arg)										//thread for implemeting clock
{
	while (1)
	{
		sleep(1);
		seconds++;
		if (seconds % 30 == 0)								//after every 30 seconds write current stats in the file
		{
			int last_file = open("processes_stat.txt", O_RDWR | O_CREAT | O_APPEND, 0777 );
			dup2(last_file, 1);
			cout<<"Throughput = "<<throughput<<"-------------";
			cout<<" Avg waiting time = "<<(avg_wait/no_of_procs)<<"----------------avg TAT = "<<(avg_TAT/no_of_procs)<<" at seconds="<<seconds<<endl;
			close(last_file);
		}
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_create(&tid1, NULL, clock, NULL);
	int last_file = open("processes_stat.txt", O_TRUNC | O_CREAT, 0777 );					//creating file to maintain the stats
	close(last_file);
	Proc P2;												//temporary Proc for holding the data from pie
    	int reading = -1, current_screen;
    	int pipe5_opener = open("run_to_exit",O_RDONLY | O_NONBLOCK);						
    	for(int i=0; 1; i++)
    	{
    		reading = read(pipe5_opener, &P2, sizeof(P2));
		while ( reading == -1)
		{
			reading = read(pipe5_opener, &P2, sizeof(P2));
			if (reading == -1)
			{
				if (errno == EAGAIN) 
				{ 
					sleep(1);
				}
				else 
				{ 
					perror("read");  
				}
			} 
		}
		if ( reading != 0 )
		{
			no_of_procs++;									//one more Proc has completed execution
			int turn_around = P2.exit_time - P2.arrival_time;				//updating cumulative values
			avg_TAT += turn_around;
			avg_wait += turn_around - P2.burst_time;
			throughput ++;
			last_file = open("processes_stat.txt", O_RDWR | O_APPEND, 0777 );		//here the output will be saved of the exit processes
			current_screen = dup(1);							//for the sake of output redirection
			dup2(last_file, 1);								//now cout will write in the txt file
//----------------------writing contenst on file--------------------------------------------------------------------------------------------------
			cout<<"We received following Proc in the exit state "<<endl;
			cout<<"Recieved Proc = "<<P2.Proc_num<<endl;
			cout<<"Algorithm Name = "<<P2.algo<<endl;
			cout<<"Arrival Time = "<<P2.arrival_time<<endl;
			cout<<"Burst Time = "<<P2.burst_time<<endl;
			cout<<"Waiting Time = "<<(turn_around-P2.burst_time)<<endl;
			cout<<"Turnaround Time = "<<turn_around<<endl;
			dup2(current_screen, 1);							//now cout will write on terminal
			close(last_file);

		}
	}
    	close(pipe5_opener);
	return 0;
}
