#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<errno.h>
#include<ctime>
#include<iomanip>
#include<sys/types.h>
#include<pthread.h>
#include "Queue.h"
#include<queue>

int seconds=0;
int num;
queue<Proc> Object;
//----------------------------------------------------------FCFS implementation-----------------------------------------------------------------------------------


void FCFS_implement(Proc& temp)										//function for implementing FCFS algorithm
{
	int flag;											//this will tell the ready process when to send the next Proc
	cout<<"We recieved "<<temp.Proc_num<<" in RUNNING at seconds="<<seconds<<" with rem time="<<temp.rem_time<<"\n";
	while (temp.rem_time >0)//(int i=0; i<5; i++)					//one second will pass in each iteration
	{
		sleep(1);								//running the clock
		seconds++;
		temp.exit_time++;
		temp.rem_time--;					//with each second remaining execution time of current process will decrease
		if (temp.rem_time <= 0)					//if remaining execution time has ended then Proc will go to exit process 
		{
			cout<<temp.Proc_num<<" will now go to EXIT state at seconds="<<seconds<<"\n";
			temp.exit_time=seconds;
			int pipe5_opener = open("run_to_exit", O_WRONLY);
			write(pipe5_opener, &temp, sizeof(temp));
			close(pipe5_opener);
			flag=1;									//we will also have to tell ready process that now send the next Proc
			int signaler = open("signal_catcher", O_WRONLY);			//as proc has gone to blocked so sending dispatch signal
			write(signaler, &flag, sizeof( flag ));
			close(signaler);
			flag=0;
			return;
		}
		if (seconds % 5 == 0)							//check for I/O after every 5 seconds
		{
			if ( (rand()%2) == 0)						//random number 0 will mean that TAKE I/O
			{
				cout<<temp.Proc_num<<" will now go to blocked state at seconds="<<seconds<<"\n";
				temp.iotype= (rand() % 3)+1;				//determing which type of I/O
				int pipe4_opener = open("run_to_block", O_WRONLY);
				write(pipe4_opener, &temp, sizeof(temp));
				close(pipe4_opener);
				flag=1;
				int signaler = open("signal_catcher", O_WRONLY);		//as proc has gone to blocked so sending dispatch signal
				write(signaler, &flag, sizeof( flag ));
				close(signaler);
				flag=0;
				return;
			}
		}
	}
}

//-------------------------------------------------------------RR implementation--------------------------------------------------------------------------------

void RR_implement(Proc& temp)
{
	int flag;
	cout<<"We recieved "<<temp.Proc_num<<" in RUNNING at seconds="<<seconds<<" with rem time="<<temp.rem_time<<"\n";
	for(int i=0; i<temp.quantum; i++)					//one second will pass in each iteration
	{
		sleep(1);
		seconds++;						//running clock
		temp.exit_time++;
		temp.rem_time--;					//with each second remaining execution time of current process will decrease
		if (temp.rem_time <= 0)					//if remaining execution time is 0 then Proc should go to exit process
		{
			cout<<temp.Proc_num<<" will now go to EXIT state at seconds="<<seconds<<"\n";
			temp.exit_time=seconds;
			int pipe5_opener = open("run_to_exit", O_WRONLY);
			write(pipe5_opener, &temp, sizeof(temp));
			close(pipe5_opener);
			flag=1;
			int signaler = open("signal_catcher", O_WRONLY);			//as proc has gone to blocked so sending dispatch signal
			write(signaler, &flag, sizeof( flag ));
			close(signaler);
			flag=0;
			return;
		}
		if (seconds % 5 == 0)							//after every 5 seconds check for the I/O
		{
			if ( (rand()%2) == 0)						//0 means take I/O
			{
				cout<<temp.Proc_num<<" will now go to blocked state at seconds="<<seconds<<"\n";
				temp.iotype= (rand() % 3)+1;				//determining the type of the I/O
				int pipe4_opener = open("run_to_block", O_WRONLY);
				write(pipe4_opener, &temp, sizeof(temp));
				close(pipe4_opener);
				flag=1;
				int signaler = open("signal_catcher", O_WRONLY);		//as proc has gone to blocked so sending dispatch signal
				write(signaler, &flag, sizeof( flag ));
				close(signaler);
				flag=0;
				return;
			}
		}
		if (i == temp.quantum-1)				//if quantum has finished then send this Proc back to the ready and dispatch next Proc
		{
			cout<<temp.Proc_num<<" will now go to READY state because quantum has finished in running\n";
			int pipe6_opener = open("run_to_ready", O_WRONLY);
			write(pipe6_opener, &temp, sizeof(temp));
			close(pipe6_opener);
			flag=1;
			int signaler = open("signal_catcher", O_WRONLY);			//as proc has gone to blocked so sending dispatch signal
			write(signaler, &flag, sizeof( flag ));
			close(signaler);
			flag=0;
			return;
		}
	}
}
//-----------------------------------------------------------SJF implementation-----------------------------------------------------------------------------------------
void SJF_implement(Proc& temp)
{
	int flag;
	cout<<"We recieved "<<temp.Proc_num<<" in RUNNING at seconds="<<seconds<<" with rem time="<<temp.rem_time<<"\n";
	for(int i=0; 1 ; i++)					//one second will pass in each iteration
	{
		sleep(1);
		seconds++;
		temp.exit_time++;
		temp.rem_time--;					//with each second remaining execution time of current process will decrease
		if (temp.rem_time <= 0)					//if remaining execution time has ended dispatch next Proc
		{
			cout<<temp.Proc_num<<" will now go to EXIT state at seconds="<<seconds<<"\n";
			temp.exit_time=seconds;
			int pipe5_opener = open("run_to_exit", O_WRONLY);
			write(pipe5_opener, &temp, sizeof(temp));
			close(pipe5_opener);
			flag=1;
			int signaler = open("signal_catcher", O_WRONLY);			//as proc has gone to blocked so sending dispatch signal
			write(signaler, &flag, sizeof( flag ));
			close(signaler);
			flag=0;
			return;
		}
		if (seconds % 5 == 0)							//check for I/O after every 5 seconds
		{
			if ( (rand()%2) == 0)
			{
				cout<<temp.Proc_num<<" will now go to blocked state at seconds="<<seconds<<"\n";
				temp.iotype= (rand() % 3)+1;				//determing the type of I/O
				int pipe4_opener = open("run_to_block", O_WRONLY);
				write(pipe4_opener, &temp, sizeof(temp));
				close(pipe4_opener);
				flag=1;
				int signaler = open("signal_catcher", O_WRONLY);		//as proc has gone to blocked so sending dispatch signal
				write(signaler, &flag, sizeof( flag ));
				close(signaler);
				flag=0;
				return;
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

/*void SRTF(Proc& temp)
{
	int flag;
	cout<<"We recieved "<<temp.Proc_num<<" in RUNNING at seconds="<<seconds<<" with rem time="<<temp.rem_time<<"\n";
	while (1)
	{
		sleep(1);
		seconds++;
		P2.rem_time--;
		P2.exit_time++;
	}
}*/
using namespace std;
int main()
{
	srand(time(NULL));		
	bool recieved = false;
	Proc P2;
	int reading = -1;
	int fifo_read = open("ready_to_run",O_RDONLY | O_NONBLOCK);
	while( 1 )
	{
		sleep(1);
		seconds++;
	    	reading = read(fifo_read, &P2, sizeof(P2));
		while ( reading == -1)
		{
			reading = read(fifo_read, &P2, sizeof(P2));
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
		if ( reading == 0 )
		{
			sleep(1);
		}
		else
		{
			seconds = P2.exit_time;
			if (P2.algo[0] == 'F')							//only FCFC starts with F
				FCFS_implement(P2);	
			else if (P2.algo[0]=='R')						//Only RR starts with RR
				RR_implement(P2);
			else if (P2.algo[0]=='S' /*&& P2.algo[1]=='J'*/)
				SJF_implement(P2);
/*			else if(P2.algo[0]=='S' && P2.algo[1]=='R')
				SRTF_implement(P2);*/
						
	    	}
	}	    	
	close(fifo_read);
}		
