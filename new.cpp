#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include<iostream>
#include<string.h>
#include "Queue.h"
#include <queue>
using namespace std;

void show(queue<Proc> temp)												
{
	if (temp.empty())	
	{
		cout<<"The queue is empty\n";
		return;
	}
	while(!temp.empty())
	{
		temp.front().show();
		cout<<endl;
		temp.pop();
	}
	cout<<endl<<endl;
}

void smaller_burst(Proc* temp, int strt, int end)
{
	for(int i=strt; i<=end; i++)
	{
		for(int j=strt; j<=end; j++)
		{
			if ( i == j)
				j++;
			if (j > end)
				break;
			if (temp[i].burst_time < temp[j].burst_time)
			{
				Proc swap;
				swap=temp[i];
				temp[i]=temp[j];
				temp[j]=swap;	
			}
		}
	}
}
void sort_array(Proc* temp, int strt, int end)	
{
	for(int i=strt; i<=end; i++)
	{
		for(int j=strt; j<=end; j++)
		{
			if ( i == j)
				j++;
			if (j > end)
				break;
			if (temp[i] < temp[j])
			{
				Proc swap;
				swap=temp[i];
				temp[i]=temp[j];
				temp[j]=swap;	
			}
		}
	}
} 
void child_write() 									
{ 
	queue <Proc> Obj;								
	int seconds=0;									
	Proc temp_array [20];								
	int max_index=-1;							
	int file1, fd1, dup_descripter;
	char name[10];									
	Proc P1;									
	int index;									
	dup_descripter = dup(0);							
	file1 = open("processes.txt", O_RDONLY);							
	if (file1 < 0)
		cout<<"the file unable to open\n";
	dup2(file1, 0);									
	cin>>temp_array[0].algo;
	if (temp_array[0].algo[0]=='R')								
		cin>>temp_array[0].quantum;

	for(int i=0; 1; i++)
	{
		if ( !(cin>>temp_array[i].Proc_num) )						
		{
			break;
		}
		cin>>temp_array[i].arrival_time;	
		cin>>temp_array[i].burst_time;
		temp_array[i].rem_time=temp_array[i].burst_time;
		max_index++;
		for(int j=0; j<10; j++)
			temp_array[i].algo[j]=temp_array[0].algo[j];				
		temp_array[i].quantum = temp_array[0].quantum;
	}
	dup2(dup_descripter, 0);
	close(file1);
	sort_array(temp_array, 0, max_index);							
	if (temp_array[0].algo[0] == 'S' && temp_array[0].algo[1]=='J')				
	{
		for(int i=0, temp_index=0; i<max_index; )
		{
			while (temp_array[i].arrival_time == temp_array[++temp_index].arrival_time)
				;
			if ( (max_index - i) > 0)
			{
				smaller_burst(temp_array, i, temp_index-1);
				i = temp_index;
			}
		}
	}
	
	for(int i=0; i<=max_index; i++)								
	{
		Obj.push(temp_array[i]);
	}
	do
	{
		if (!Obj.empty())
		{
			P1 = Obj.front();
			Obj.pop();
			while (seconds < P1.arrival_time)
			{
				sleep(1);
				seconds++;
			}
			cout<<P1.Proc_num<<" going to the ready queue from admit state at seconds="<<seconds<<" with burst time="<<P1.burst_time<<"\n";
			fd1 = open("new_to_ready", O_WRONLY);
			write (fd1, &P1, sizeof(P1));
			close(fd1);
		}
	} while (1);
} 

int main() 
{ 
        child_write();  
    	return 0; 
} 

