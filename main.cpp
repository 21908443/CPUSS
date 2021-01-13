#include <stdio.h> 
#include <unistd.h>
#include<stdio.h>
#include<sys/wait.h>
#include<sys/stat.h> 
#include <fcntl.h> // library for fcntl function 
#include<iostream>
#include<string.h>

using namespace std;
int main()
{
//-------------------------------------Creating all the pipes--------------------------------------------------------------------------
	int fd1 = mkfifo("new_to_ready", 0666);
	int fd2 = mkfifo("ready_to_run", 0666);
	int fd3 = mkfifo("run_to_ready", 0666);
	int fd4 = mkfifo("run_to_block", 0666);
	int fd5 = mkfifo("block_to_ready", 0666);
	int fd6 = mkfifo("run_to_exit", 0666);
	int fd7 = mkfifo("signal_catcher", 0666);


	pid_t id1=fork();
	if (id1 > 0)
	{
		execlp("./new", "new", NULL);							//this will be our NEW process
	}
	else if (id1 == 0)
	{
		pid_t id2=fork();
		if (id2 > 0)
		{
			pid_t id3=fork();
			if (id3 > 0)
			{
				;
			}
			else if (id3 == 0)
			{
				execlp("./blocked", "blocked", NULL);				//this child process of READY will act as BLOCKED process
			}
			else 
				cout<<"Fork3 error\n";
		}
		else if (id2 == 0)
		{
			pid_t id4=fork();
			if (id4 > 0)
			{
				;
			}
			else if (id4 == 0)
			{
				execlp("./exit", "exit", NULL);					//this will be our EXIT process
			}
			else 
				cout<<"Fork4 error\n";
			execlp("./running", "running", NULL);						//This will be our RUNNING process
		}
		else 
			cout<<"Fork2 error\n";
		execlp("./ready", "ready", NULL);							//this will be our ready process which is child process of NEW
	}
	else 
		cout<<"Fork1 error\n";
	return 0;
}


