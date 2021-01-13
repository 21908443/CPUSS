#include <stdio.h> 
#include <unistd.h>
#include<stdio.h>
#include<sys/wait.h>
#include<sys/stat.h> 
#include <fcntl.h>
#include<iostream>
#include<string.h>

using namespace std;
int main()
{
	int f1 = mkfifo("new_to_ready", 0666);
	int f2 = mkfifo("ready_to_run", 0666);
	int f3 = mkfifo("run_to_ready", 0666);
	int f4 = mkfifo("run_to_block", 0666);
	int f5 = mkfifo("block_to_ready", 0666);
	int f6 = mkfifo("run_to_exit", 0666);
	int f7 = mkfifo("signal_catcher", 0666);


	pid_t p_id1=fork();
	if (p_id1 > 0)
	{
		execlp("./new", "new", NULL);							
	}
	else if (p_id1 == 0)
	{
		pid_t p_id2=fork();
		if (p_id2 > 0)
		{
			pid_t p_id3=fork();
			if (p_id3 > 0)
			{
				;
			}
			else if (p_id3 == 0)
			{
				execlp("./blocked", "blocked", NULL);				
			}
			else 
				cout<<"Fork3 error\n";
		}
		else if (p_id2 == 0)
		{
			pid_t p_id4=fork();
			if (p_id4 > 0)
			{
				;
			}
			else if (p_id4 == 0)
			{
				execlp("./exit", "exit", NULL);					
			}
			else 
				cout<<"Fork4 error\n";
			execlp("./running", "running", NULL);						
		}
		else 
			cout<<"Fork2 error\n";
		execlp("./ready", "ready", NULL);							
	}
	else 
		cout<<"Fork1 error\n";
	return 0;
}


