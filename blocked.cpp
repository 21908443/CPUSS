#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<errno.h>
#include<sys/types.h>
#include "Queue.h"
#include<queue>
using namespace std;
queue <Proc> Input;											
queue <Proc> Output;											
queue <Proc> Printer;											
	
pthread_t tid1, tid2, tid3;										


void* InputQueue(void* arg)
{
	Proc P2;
	while (1)
	{
		if ( !Input.empty() )
		{
			P2=Input.front();
			Input.pop();
			int sleeper=rand()%11+15;							//getting the duration of Input by generating a ramdom number
			cout<<P2.Proc_num<<" now will perform Input I/O of "<<sleeper<<" seconds\n";
			sleep(sleeper);
			cout<<P2.Proc_num<<" has now successfully performed Input I/O for "<<sleeper<<" seconds\n";
			cout<<endl<<endl;
			cout<<"Now "<<P2.Proc_num<<" going to ready\n";					//Now Proc will go back to ready state
			int pipe2_opener = open("block_to_ready", O_WRONLY);
			write(pipe2_opener, &P2, sizeof(P2));
			close(pipe2_opener);	
		}
	}
	pthread_exit(NULL);
}

void* OutputQueue(void* arg)
{
	Proc P2;
	while (1)
	{
		if ( !Output.empty() )
		{
			P2 = Output.front();
			Output.pop();
			int sleeper=rand()%11+15;							//getting the duration of Output by generating a ramdom number
			cout<<P2.Proc_num<<" now will perform Output I/O of "<<sleeper<<" seconds\n";
			sleep(sleeper);
			cout<<P2.Proc_num<<" has now successfully performed Output I/O for "<<sleeper<<" seconds\n";
			cout<<endl<<endl;
			cout<<"Now "<<P2.Proc_num<<" going to ready\n";
			int pipe2_opener = open("block_to_ready", O_WRONLY);				//Now Proc will go back to ready state
			write(pipe2_opener, &P2, sizeof(P2));
			close(pipe2_opener);	
		}
	}
	pthread_exit(NULL);
}

void* PrinterQueue(void* arg)
{
	Proc P2;
	while (1)
	{
		if ( !Printer.empty() )
		{
			P2=Printer.front();
			Printer.pop();
			int sleeper=rand()%11+15;						//getting the duration of Printer by generating a ramdom number
			cout<<P2.Proc_num<<" now will perform Printer I/O of "<<sleeper<<" seconds\n";
			sleep(sleeper);
			cout<<P2.Proc_num<<" has now successfully performed Printer I/O for "<<sleeper<<" seconds\n";
			cout<<endl<<endl;
			cout<<"Now "<<P2.Proc_num<<" going to ready\n";
			int pipe2_opener = open("block_to_ready", O_WRONLY);				//Now Proc will go back to ready state
			write(pipe2_opener, &P2, sizeof(P2));
			close(pipe2_opener);	
		}
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_create(&tid1, NULL, InputQueue, NULL);
	pthread_create(&tid2, NULL, OutputQueue, NULL);
	pthread_create(&tid3, NULL, PrinterQueue, NULL);
 
    	Proc P2;											//temporary Proc for holding data from the pipe
    	int reading, waiter, pipe2_opener;
    	srand(time(NULL));										//for random number generation
    	reading = -1;
    	int pipe4_opener = open("run_to_block",O_RDONLY | O_NONBLOCK);
    	while (1)											//infinte loop for checking the pipes continuously
    	{
    		reading = read(pipe4_opener, &P2, sizeof(P2));
		while ( reading == -1)
		{
			reading = read(pipe4_opener, &P2, sizeof(P2));
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
			;
		}
		else
		{
			if (P2.iotype == 1)								//looking on the type of input
				Input.push(P2);
			else if (P2.iotype == 2)
				Output.push(P2);
			else if (P2.iotype == 3)
				Printer.push(P2);
		}
    	}		
    	close(pipe4_opener);
}

