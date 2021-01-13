#ifndef QUEUE_H
#define QUEUE_H
#include<iostream>
using namespace std;

struct Proc
{
	char algo[10];
	char Proc_num[10];
	int quantum;
	int arrival_time;
	int burst_time;
	int rem_time;
	int exit_time;
	int iotype;
	Proc()
	{
		quantum=arrival_time=burst_time=rem_time=0;
		exit_time=0;
		iotype=0;
	}
	void show()
	{
		cout<<"Algorithm = "<<algo<<endl;
		if (algo[0]=='R')
		{
			cout<<"Quantum = "<<quantum<<endl;
		}
		else
		{
			cout<<"Quantum = Null\n";
		}
		cout<<Proc_num<<endl;
		cout<<"Arrival time = "<<arrival_time<<endl;
		cout<<"Burst time = "<<burst_time<<endl;
		cout<<"Reamining execution time="<<rem_time<<endl;
	}
	bool operator > (const Proc &right)
	{
		if (arrival_time > right.arrival_time)
			return true;
		else
			return false;
	}
	bool operator < (const Proc &right)
	{
		if (arrival_time < right.arrival_time)
			return true;
		else
			return false;
	}
	Proc operator = (const Proc &temp)
	{
		for(int i=0; i<10; i++)
		{
			algo[i]=temp.algo[i];
			Proc_num[i]=temp.Proc_num[i];
		}
		quantum=temp.quantum;
		arrival_time=temp.arrival_time;
		burst_time=temp.burst_time;	
		rem_time=temp.rem_time;
		exit_time=temp.exit_time;
		iotype=temp.iotype;
		return *this;
	}

};

#endif

