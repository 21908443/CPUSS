#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> // library for fcntl function 
#include<iostream>
#include<string.h>
#include<pthread.h>
//#include "Queue.cpp"
#include "Queue.h"
#include <queue>
using namespace std; 
queue <Proc> Obj;								
bool required=true;									//this variable will tell when to send the next Proc
int seconds=0;
pthread_t tid1, tid2, tid3, tid4, tid5, tid6;

void* clock(void* arg)									//thread for implementing clock
{
	while(1)
	{
		sleep(1);
		seconds++;
	}
	pthread_exit(NULL);
}

void* catchSignal(void* arg)								//this thread will continuously check for the signal of dispatching proc
{
	int signaler = open("signal_catcher", O_RDONLY);
	int temp=0, reading=-1;
	while (1)
    	{
    		reading = read(signaler, &temp, sizeof(temp));
		while ( reading == -1)
		{
			reading = read(signaler, &temp, sizeof(temp));
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
		if (reading != 0)
		{
		          if (temp==1)									//in my logic everything except 1 means that no dispatch yet
				required=true;
		}
    	}	
	close(signaler);							
	pthread_exit(NULL);
}

void* newtoready(void* arg)										//this thread will recieve Procs from the new process
{
	int nread, fd2;     										
  	Proc P1;
	fd2=open("new_to_ready", O_RDONLY | O_NONBLOCK);
    	while (1) 
    	{
        	nread = read(fd2, &P1, sizeof(P1)); 
        	if(nread == -1)
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
          	else if (nread != 0) 
		{  
				seconds = P1.arrival_time;						//for synchronization of time
            			cout<<"We received "<<P1.Proc_num<<" from NEW state with rem time = "<<P1.rem_time<<" at seconds="<<seconds<<endl; 
				Obj.push(P1);
		}		
        } 
	close(fd2);
	pthread_exit(NULL);
}

void* readytorunning(void* arg)									//this thread will send Procs from ready to running process
{
	Proc P2;
	int pipe2_opener;
	while ( 1 )
	{
		if (!Obj.empty())
		{
			if (required==true)						//as long as queue is non empty and dispatch signal is on, keep dispatching procs
			{
				P2 = Obj.front();
				Obj.pop();
				while(seconds < P2.arrival_time)
					;
				P2.exit_time=seconds;
				cout<<P2.Proc_num<<" going to running pipe at seconds = "<<seconds<<" with rem_time="<<P2.rem_time<<"\n";
				pipe2_opener = open("ready_to_run", O_WRONLY);
				write(pipe2_opener, &P2, sizeof(P2));
				close(pipe2_opener);
				required = false;					//can not send more Procs untill we receive the dispatching signal
			}
		}
	}
	pthread_exit(NULL);
}

void* runningtoready(void* arg)								//this thread will recieve the Procs from running which have been timed out
{
	int nread, fd4;     											// write link
  	Proc P1;
	fd4=open("run_to_ready", O_RDONLY | O_NONBLOCK);
    	while (1) 
    	{
        	nread = read(fd4, &P1, sizeof(P1)); 
        	if(nread == -1)
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
          	else if (nread != 0) 
		{
				cout<<"We received from "<<P1.Proc_num<<" from RUUNING state with rem time = "<<P1.rem_time<<" at seconds="<<seconds<<endl; 
				Obj.push(P1);
		}		
        } 
	close(fd4);
	pthread_exit(NULL);
}

void* blockedtoready(void* arg)										//this thread will recieve Procs from the blocked process
{
	int nread, fd5;     											// write link
  	Proc P1;
	fd5=open("block_to_ready", O_RDONLY | O_NONBLOCK);
    	while (1) 
    	{
        	nread = read(fd5, &P1, sizeof(P1)); 
        	if(nread == -1)
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
          	else if (nread != 0) 
		{  
            			cout<<"We received "<<P1.Proc_num<<" with rem time = "<<P1.rem_time<<" at seconds="<<seconds<<endl; 
				Obj.push(P1);
		}		
        } 
	close(fd5);
	pthread_exit(NULL);
}
int main()
{
		pthread_create(&tid1, NULL, clock, NULL);
		pthread_create(&tid2, NULL, catchSignal, NULL);
		pthread_create(&tid3, NULL, newtoready, NULL);
		pthread_create(&tid4, NULL, readytorunning, NULL);
		pthread_create(&tid5, NULL, runningtoready, NULL);
		pthread_create(&tid6, NULL, blockedtoready, NULL);
		pthread_exit(NULL);
}


