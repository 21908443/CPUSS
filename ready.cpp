#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include<iostream>
#include<string.h>
#include<pthread.h>
//#include "Queue.cpp"
#include "Queue.h"
#include <queue>
using namespace std; 
queue <Proc> Obj;								
bool req=true;									
int sec=0;
pthread_t p1, p2, p3, p4, p5, p6;

void* clock(void* arg)									
{
	while(1)
	{
		sleep(1);
		sec++;
	}
	pthread_exit(NULL);
}

void* SignalCatch(void* arg)								
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
		          if (temp==1)									
				req=true;
		}
    	}	
	close(signaler);							
	pthread_exit(NULL);
}

void* readyfornew(void* arg)										
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
				sec = P1.arrival_time;						
            			cout<<"We received "<<P1.Proc_num<<" from NEW state with rem time = "<<P1.rem_time<<" at sec="<<sec<<endl; 
				Obj.push(P1);
		}		
        } 
	close(fd2);
	pthread_exit(NULL);
}

void* readyforrun(void* arg)									
{
	Proc P2;
	int pipe2_opener;
	while ( 1 )
	{
		if (!Obj.empty())
		{
			if (req==true)						
			{
				P2 = Obj.front();
				Obj.pop();
				while(sec < P2.arrival_time)
					;
				P2.exit_time=sec;
				cout<<P2.Proc_num<<" going to running pipe at sec = "<<sec<<" with rem_time="<<P2.rem_time<<"\n";
				pipe2_opener = open("ready_to_run", O_WRONLY);
				write(pipe2_opener, &P2, sizeof(P2));
				close(pipe2_opener);
				req = false;					
			}
		}
	}
	pthread_exit(NULL);
}

void* runforready(void* arg)								
{
	int nread, fd4;     											
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
				cout<<"We received from "<<P1.Proc_num<<" from RUUNING state with rem time = "<<P1.rem_time<<" at sec="<<sec<<endl; 
				Obj.push(P1);
		}		
        } 
	close(fd4);
	pthread_exit(NULL);
}

void* blockforready(void* arg)										
{
	int nread, fd5;     											
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
            			cout<<"We received "<<P1.Proc_num<<" with rem time = "<<P1.rem_time<<" at sec="<<sec<<endl; 
				Obj.push(P1);
		}		
        } 
	close(fd5);
	pthread_exit(NULL);
}
int main()
{
		pthread_create(&p1, NULL, clock, NULL);
		pthread_create(&p2, NULL, SignalCatch, NULL);
		pthread_create(&p3, NULL, readyfornew, NULL);
		pthread_create(&p4, NULL, readyforrun, NULL);
		pthread_create(&p5, NULL, runforready, NULL);
		pthread_create(&p6, NULL, blockforready, NULL);
		pthread_exit(NULL);
}


