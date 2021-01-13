#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <iostream>
#include <string>
#include <pthread.h>
#include "Queue.h"
#include <queue>
using namespace std;
pthread_t p1;								
int sec =0;											
int average_wait=0, average_tat=0, number_process=0, thr_put=0;					
void* clock(void* arg)										
{
	while (1)
	{
		sleep(1);
		sec++;
		if (sec % 30 == 0)								
		{
			int last_file = open("processes_stat.txt", O_RDWR | O_CREAT | O_APPEND, 0777 );
			dup2(last_file, 1);
			cout<<"thr_put = "<<thr_put<<"-------------";
			cout<<" Avg waiting time = "<<(average_wait/number_process)<<"----------------average time around = "<<(average_tat/number_process)<<" at sec="<<sec<<endl;
			close(last_file);
		}
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_create(&p1, NULL, clock, NULL);
	int last_file = open("processes_stat.txt", O_TRUNC | O_CREAT, 0777 );	
	close(last_file);
	Proc P2;												
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
			number_process++;									
			int turn_around = P2.exit_time - P2.arrival_time;				
			average_tat += turn_around;
			average_wait += turn_around - P2.burst_time;
			thr_put ++;
			last_file = open("processes_stat.txt", O_RDWR | O_APPEND, 0777 );		
			current_screen = dup(1);							
			dup2(last_file, 1);								

			cout<<"We received following Proc in the exit state "<<endl;
			cout<<"Recieved Proc = "<<P2.Proc_num<<endl;
			cout<<"Algorithm Name = "<<P2.algo<<endl;
			cout<<"Arrival Time = "<<P2.arrival_time<<endl;
			cout<<"Burst Time = "<<P2.burst_time<<endl;
			cout<<"Waiting Time = "<<(turn_around-P2.burst_time)<<endl;
			cout<<"Turnaround Time = "<<turn_around<<endl;
			dup2(current_screen, 1);
			close(last_file);

		}
	}
    	close(pipe5_opener);
	return 0;
}
