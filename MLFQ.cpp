// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

// data_structs_3.cpp : Defines the entry point for the console application.
//COPY FOR EXPERIMENTATION
#include "stdafx.h"
#include <iostream>
#include <iomanip>
using namespace std;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Data initialization
struct process_data
{
	int wait;
	int pxs;
	int arrive;
	int jind;
	int resp;
	int TT;
	int queue;
	bool IO;
	int a, b, c, d, e, f, g, h, i, j, k, l, m, n, O, p, q;
	int datas[17] = { a,b,c,d,e,f,g,h,i,j,k,l,m,n, O,p,q };

};

struct slinklist
{
	process_data data;
	struct slinklist * next;
};

typedef struct slinklist node;
node * start = NULL;

node *getnode(int px[], int pxs)//Creates newnode with predetermined input
{
	node *newnode;
	newnode = (node*)malloc(sizeof(node));
	newnode->data.wait = 0;
	newnode->data.arrive = 0;
	newnode->data.jind = 0;
	newnode->data.resp = 0;
	newnode->data.TT = 0;
	newnode->data.pxs = pxs;
	newnode->data.queue = 1;
	newnode->data.IO = false;
	for (int j = 0; j < 17; j++)
		newnode->data.datas[j] = 0;
	
	for (int j = 0; j < pxs; j++)
		newnode->data.datas[j] = px[j];
		
	newnode->next = NULL;
	return newnode;
}

void create_s_list(int n, int P0[8], int p1[13], int p2[17], int p3[17], int p4[15], int p5[17], int p6[17], int p7[15], int p8[17], int p1s, int p2s, int p3s, int p4s, int p5s, int p6s, int p7s, int p8s)//Creates n number of nodes for a Single Linked List
{
	int i;
	node *newnode;
	node *temp;

	int *P1[] = { p1, p2, p3, p4, p5, p6, p7, p8 };//array of arrays
	int P2[] = { p1s,p2s,p3s,p4s,p5s,p6s,p7s,p8s };//size of arrays
	for (i = 0; i < n; i++)
	{
		newnode = getnode(P1[i], P2[i]);
		if (start == NULL)
		{
			start = newnode;
		}
		else
		{
			temp = start;
			while (temp->next != NULL)
				temp = temp->next;
			temp->next = newnode;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Performs Round Robin Scheduling with TQ = 6; Displays Context Switch Info
void queue1_Sch(int &time)
{
	node *temp;
	int minarrive;//arrival time of soonest queue 1 process
	int minIndex = 0;//index of soonest queue 1 process
	bool Onearrived;//indicates if a queue 1 process has arrived
	while (true)
	{
		temp = start;
		Onearrived = false;
		minarrive = INT_MAX;

		for (int i = 0; i < 8; i++)//gets index of Queue 1 process with soonest arrival time
		{
			if (temp->data.arrive < minarrive && temp->data.arrive != -1 && temp->data.queue ==1 && temp->data.arrive<=time)
			{
				minarrive = temp->data.arrive;
				minIndex = i;
				Onearrived = true;
			}
			temp = temp->next;
		}

		if (Onearrived == false)//if no queue 1 process has arrived
			break;

		int ctr = 1;//make temp point to process in execution
		temp = start;
		while (ctr <= minIndex)
		{
			temp = temp->next;
			ctr++;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Context Switch Information Displayed
		cout << endl<< "Current Execution Time: " << time << endl << "Process on CPU: P" << ctr << "-->" << temp->data.datas[temp->data.jind]<<endl;
		cout << "Ready Queue:" << endl;
        ctr = 1;
		temp = start;
		while (ctr <= 8)
		{
			if (ctr != minIndex + 1 && temp->data.arrive != -1 && temp->data.IO == false)//if process is not in either IO or CPU execution
			{
				cout << " P" << ctr << " Burst time:" << temp->data.datas[temp->data.jind] << endl;
			}
				temp = temp->next;
			
			ctr++;
		}
		cout << "Processes in I/O:" << endl;
		ctr = 1;
		temp = start;
		while (ctr <= 8)
		{
			if (temp->data.IO && ctr != minIndex + 1 && temp->data.arrive != -1)//If process is in IO and not in execution...
			{
				cout << " P" << ctr << " I/O time Remaining:" << temp->data.datas[temp->data.jind - 1] << endl;
			}
			temp = temp->next;

			ctr++;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	    ctr = 1;
		temp = start;
		while (ctr <= minIndex)//Select Process in execution again->index of node with lowest arrival time
		{
			temp = temp->next;
			ctr++;
		}

		if (temp->data.jind == 0)//Response Time, jind gets either incremented from one pass in the loop, or process gets demoted to lower queue
		{
			temp->data.resp = time;
		}

		temp->data.wait += (time - temp->data.arrive);//total waiting time is waiting time + (time(it just got on the cpu) minus the arrival time of process

		if (temp->data.datas[temp->data.jind] > 6)//if burst is greater than 6
		{
			time += 6;// incrementing time the length of TQ
			temp->data.arrive = time;
			temp->data.datas[temp->data.jind] -= 6;
			temp->data.queue = 2;
			temp->data.IO = false;//process in not in IO in this case
		}

		else if (temp->data.datas[temp->data.jind] <= 6)
		{
			time += temp->data.datas[temp->data.jind];
			temp->data.arrive = time + temp->data.datas[temp->data.jind + 1];
			temp->data.jind = temp->data.jind + 2;
			temp->data.IO = true;//Process in IO after finishing burst
		}
		ctr = 1;//deincrements IO times of processes currently in IO
		temp = start;
		while (temp != NULL)
		{
			if (temp->data.IO && ctr != minIndex + 1)
				temp->data.datas[temp->data.jind-1] -= 6;
			if (temp->data.datas[temp->data.jind-1] <= 0)
				temp->data.IO = false;
			temp = temp->next;
			ctr++;
		}
		
		ctr = 1;
		temp = start;
		while (ctr <= minIndex)
		{
			temp = temp->next;
			ctr++;
		}

		if (temp->data.jind > (temp->data.pxs - 1))//if jind surpasses data, process is done
		{
			temp->data.IO = false;
			temp->data.arrive = -1;
			temp->data.TT = time;
			cout << endl << "                                  Process " << ctr << " Completed" << endl;
		}

	}

}

//Performs Round Robin Scheduling TQ = 12 with preemption; Displays Context Switch Info
void queue2_Sch(int &time)
{
	node *temp;
	int minarrive;
	int minQ1arr = INT_MAX;  //arrival time of soonest Q1 process
	int minIndex = 0;
	bool Twoarrived;//indicates if queue 2 process is in the ready queue
	int timecalc;//preserves time of execution

	temp = start;
	for (int i = 0; i < 8; i++)//Find arrival time of soonest Q1 process
	{
		if (temp->data.arrive != -1 && temp->data.queue == 1 && temp->data.arrive < minQ1arr)
			minQ1arr = temp->data.arrive;
		temp = temp->next;
	}


	while (true)
	{
		temp = start;
		Twoarrived = false;
		minarrive = INT_MAX;
		timecalc = time;

		temp = start;
		for (int i = 0; i < 8; i++)//gets index of Queue 2 process with soonest arrival time
		{
			if (temp->data.arrive < minarrive && temp->data.arrive != -1 && temp->data.queue == 2 && temp->data.arrive<=time)
			{
				Twoarrived = true;
				minarrive = temp->data.arrive;
				minIndex = i;//index of process with soonest arrival time
			}
			temp = temp->next;
		}

		if (Twoarrived == false)
			return;

		int ctr = 1;
		temp = start;
		while (ctr <= minIndex)
		{
			temp = temp->next;
			ctr++;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Context Switch Information Displayed
		cout << endl<< "Current Execution Time: " << time << endl << "Process on CPU: P" << ctr << "-->" << temp->data.datas[temp->data.jind] << endl;
		cout << "Ready Queue:" << endl;
		ctr = 1;
		temp = start;
		while (ctr <= 8)
		{
			if (ctr != minIndex + 1 && temp->data.arrive != -1 && temp->data.IO == false)
			{
				cout << " P" << ctr << " Burst time:" << temp->data.datas[temp->data.jind] << endl;
			}
			temp = temp->next;

			ctr++;
		}
		cout << "Processes in I/O:" << endl;
		ctr = 1;
		temp = start;
		while (ctr <= 8)
		{
			if (temp->data.IO && ctr != minIndex + 1 && temp->data.arrive != -1)
			{
				cout << " P" << ctr << " I/O time Remaining:" << temp->data.datas[temp->data.jind-1] << endl;
			}
			temp = temp->next;

			ctr++;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ctr = 1;
		temp = start;
		while (ctr <= minIndex)
		{
			temp = temp->next;
			ctr++;
		}


		temp->data.wait += (time - temp->data.arrive);//total waiting time is waiting time + (time(it just got on the cpu) minus the arrival time of process

		if (temp->data.datas[temp->data.jind] > 12)//if burst is greater than 12
		{
			for (int i = 0; i < 12; i++)
			{
				time++;
				if (time >= minQ1arr)//if a Q1 process arrives
					break;
			}
			temp->data.datas[temp->data.jind] -= (time - timecalc);
			temp->data.arrive = time;
			if((time-timecalc)>=12)
				temp->data.queue = 3;
			temp->data.IO = false;
		}

		else if (temp->data.datas[temp->data.jind] <= 12)// If burst is less than the time quantum
		{
			for (int i = 0; i < temp->data.datas[temp->data.jind]; i++)
			{
				time++;
				if (time >= minQ1arr)//if a Q1 process arrives
					break;
			}
			if ((time - timecalc) >= temp->data.datas[temp->data.jind])//if the burst was finished
			{
				temp->data.arrive = time + temp->data.datas[temp->data.jind + 1];
				temp->data.jind = temp->data.jind + 2;
				temp->data.IO = true;
			}
			else //if the burst hasn't finished
			{
				temp->data.datas[temp->data.jind] -= (time - timecalc);
				temp->data.arrive = time;
				temp->data.IO = false;
			}
		}

		ctr = 1;//deincrement IO times of process in IO 
		temp = start;
		while (temp != NULL)
		{
			if (temp->data.IO && ctr != minIndex + 1)
				temp->data.datas[temp->data.jind-1] -= (time - timecalc);
			if (temp->data.datas[temp->data.jind-1]<=0)
				temp->data.IO = false;
			temp = temp->next;
			ctr++;
		}

		ctr = 1;
		temp = start;
		while (ctr <= minIndex)
		{
			temp = temp->next;
			ctr++;
		}

		if (temp->data.jind > (temp->data.pxs - 1))//if jind surpasses data, process is done
		{
			temp->data.IO = false;
			temp->data.arrive = -1;
			temp->data.TT = time;
		    cout << endl << "                                  Process " << ctr << " Completed" << endl;
		}

		if (minQ1arr <= time)//If queue 1 process has arrived return to wait_tt function
			return;

	}
}

//Performs First Come First Serve Scheduling with preemption; Displays Context Switch Info
void FCFS_Sch(int &time)
{
	node *temp;
	int timecalc;//preserves time of execution
	int minarrive;//soonest arrival time
	int minIndex;//index of process with soonest arrival time
	int minQ12arr= INT_MAX;//soonest arrival time of higher priority process
	bool AnyArrived;//indicates if any process is in the ready queue

	temp = start;
	for (int i = 0; i < 8; i++)//Find arrival time of soonest Q1/2 process
	{
		if (temp->data.arrive != -1 && temp->data.arrive < minQ12arr && temp->data.queue !=3)
			minQ12arr = temp->data.arrive;
		temp = temp->next;
	}

	while (true)
	{
		timecalc = time;
		AnyArrived = false;
		minarrive = INT_MAX;

		temp = start;
		for (int i = 0; i < 8; i++)
		{
			if (temp->data.arrive < minarrive && temp->data.arrive != -1 && temp->data.arrive<=time)
			{
				AnyArrived = true;
				minarrive = temp->data.arrive;
				minIndex = i;//index of process with soonest arrival time
			}
			temp = temp->next;
		}

		if (AnyArrived == false)//if no process in ready queue, return to wait_tt function
			return;

		int ctr = 1;
		temp = start;
		while (ctr <= minIndex)
		{
			temp = temp->next;
			ctr++;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Context Switch Information Displayed
		cout << endl<<"Current Execution Time: " << time << endl << "Process on CPU: P" << ctr << "-->" << temp->data.datas[temp->data.jind] << endl;
		cout << "Ready Queue:" << endl;
		ctr = 1;
		temp = start;
		while (ctr <= 8)
		{
			if (ctr != minIndex + 1 && temp->data.arrive != -1 && temp->data.IO ==false)
			{
				cout << " P" << ctr << " Burst time:" << temp->data.datas[temp->data.jind] << endl;
			}
			temp = temp->next;

			ctr++;
		}
		cout << "Processes in I/O:" << endl;
		ctr = 1;
		temp = start;
		while (ctr <= 8)
		{
			if (temp->data.IO && ctr != minIndex + 1 && temp->data.arrive != -1)
			{
				cout << " P" << ctr << " I/O time Remaining:" << temp->data.datas[temp->data.jind - 1] << endl;
			}
			temp = temp->next;

			ctr++;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ctr = 1;
		temp = start;
		while (ctr <= minIndex)
		{
			temp = temp->next;
			ctr++;
		}

		temp->data.wait = temp->data.wait + (time - temp->data.arrive);//total waiting time is waiting time + (time(it just got on the cpu) minus the arrival time of process

		for (int i = 0; i < temp->data.datas[temp->data.jind]; i++)
		{
			time++;
			if (time >= minQ12arr)//if a Q1 or Q2 process arrives
				break;
		}
		if ((time - timecalc) >= temp->data.datas[temp->data.jind])//if CPU burst is not interrupted
		{
			temp->data.arrive = time + temp->data.datas[temp->data.jind + 1];//new arrival time is newtime + IO
			temp->data.jind = temp->data.jind + 2;
			temp->data.IO = true;
		}
		else//CPU burst is interrupted, deincrement process burst time
		{
			temp->data.datas[temp->data.jind] -= (time - timecalc);
			temp->data.arrive = time;
			temp->data.IO = false;
		}

		ctr = 1;//deincrement IO times of processes in IO
		temp = start;
		while (temp != NULL)
		{
			if (temp->data.IO && ctr != minIndex + 1)
				temp->data.datas[temp->data.jind-1] -= (time - timecalc);
			if (temp->data.datas[temp->data.jind-1] <= 0)
				temp->data.IO = false;
			temp = temp->next;
			ctr++;
		}

		ctr = 1;
		temp = start;
		while (ctr <= minIndex)
		{
			temp = temp->next;
			ctr++;
		}


		if (temp->data.jind > (temp->data.pxs - 1))//Indicate if process is finished
		{
			temp->data.IO = false;
			temp->data.arrive = -1;
			temp->data.TT = time;
			cout << endl << "                                  Process " << ctr << " Completed" << endl;
		}

		if (time >= minQ12arr)//if higher priority process arrives return to wait_tt function
			return;
	}
}

//Calculates the overall waiting time and turnaround time for each process, also shows when CPU is idle and for how long
void wait_tt(int &time)
{
	node *temp;
	//int time = 0;
	int minIndex = 0;//min index is the index of the current process
	int minarrive;//soonest arrival time
	int idle = 0;//indicate whether CPU is idle
	bool one;//indicates if queue 1 process is in ready queue
	bool two;//indicates if queue 2 process is in ready queue
	bool ready;//indicates if any process is in ready queue
	bool anyP;//indicates if any process is not finished
	while (true)
	{
		one = false;
		two = false;
		ready = false;
		anyP = false;
		temp = start;
		minarrive = INT_MAX;

		temp = start;
		for (int i = 0; i < 8; i++)//See if ready process in Queue 1
		{
			if (temp->data.queue == 1 && temp->data.arrive != -1 && temp->data.arrive <= time)
			{
				one = true;
				break;
			}
			temp = temp->next;
		}

		temp = start;
		for (int i = 0; i < 8; i++)//See if ready process in Queue 2
		{
			if (temp->data.queue == 2 && temp->data.arrive != -1 && temp->data.arrive <= time)
			{
				two = true;
				break;
			}
			temp = temp->next;
		}

		temp = start;
		for (int i = 0; i < 8; i++)//See if any process in ready Queue
		{
			if (temp->data.arrive != -1 && temp->data.arrive <= time)
			{
				ready = true;
				break;
			}
			temp = temp->next;
		}

		temp = start;
		for (int i = 0; i < 8; i++)//gets arrival time of soonest process, see if any process is not finished
		{
			if (temp->data.arrive < minarrive && temp->data.arrive != -1)
			{
				minarrive = temp->data.arrive;
				minIndex = i;//index of process with soonest arrival time
				anyP = 1;
			}
			temp = temp->next;
		}

		int ctr = 1;
		temp = start;
		while (ctr <= minIndex)
		{
			temp = temp->next;
			ctr++;
		}

		if (anyP == false)//If all processes are finished
			return;

		if (one)
			queue1_Sch(time);

		else if (two)
			queue2_Sch(time);

		else if (ready)
			FCFS_Sch(time);

		else if(ready==false)
		{//increment time to soonest arrival time, indicate idleness, and deincrement IO times for the length of time spent idle
			temp->data.IO = true;
			idle = minarrive - time;
			time = minarrive;
			cout << endl <<"IDLE for " << idle << "   " << endl <<endl;
			
			temp = start;
			while (temp != NULL)
			{
				if (temp->data.IO)
					temp->data.datas[temp->data.jind - 1] -= idle;
				if (temp->data.datas[temp->data.jind - 1] <= 0)
					temp->data.IO = false;
				temp = temp->next;
			}
		}
	}

}

//Computes sum of Burst times for a process
int SUM(int A[])
{
	int sum = 0;
	for (int i = 0; i < 17; i++)
	{
		if(i%2 == 0)
		sum += A[i];
	}
	return sum;
}

//Computes average response time, waiting time, and turnaround time for each process 
void AVG(double &r, double &w, double &tt)
{
	node*temp = start;
	while (temp != NULL)
	{
		r += temp->data.resp;
		w += temp->data.wait;
		tt += temp->data.TT;
		temp = temp->next;
	}
	r /= 8;
	w /= 8;
	tt /= 8;
}

//Computes sum of burst times of the set of all processes
void total_sum(double& dTS)
{
	node *temp;
	dTS = 0;
	temp = start;
	while (temp != NULL)
	{
		dTS += SUM(temp->data.datas);
		temp = temp->next;
	}
}

//Computes CPU utilization
void CPU_UT(int time, double dTS, double &util)
{
	util = ((dTS / time)*100);
}

//Prints the overall response time, waiting time, turnaround time for each process as well as their averages. Also prints CPU utilization and the time when all process are finished. 
void print(double r, double w, double tt,double util, int time)
{
	cout <<endl<< "Resp " << "wait  " << "TT" << endl;
	node*temp = start;
	while (temp != NULL)
	{
		if (temp->data.resp > 9 && temp->data.wait > 99)
		{
			cout << temp->data.resp << "   " << temp->data.wait << "   " << temp->data.TT << endl;
			temp = temp->next;
		}
		else if (temp->data.resp > 9 && temp->data.wait <= 99)
		{
			cout << temp->data.resp << "   " << temp->data.wait << "    " << temp->data.TT << endl;
			temp = temp->next;
		}
		else
		{
			cout << temp->data.resp << "    " << temp->data.wait << "   " << temp->data.TT << endl;
			temp = temp->next;
		}
	}

	cout << r << "   " << w << "   " << tt << "<---AVG" << endl;
	cout << "%CPU: " << util << endl << "TimeOfCompletion: " << time << endl <<endl;
}

int main()
{
	int time = 0;
	double dTS = 0;//Total Sum of all burst times as type double
	double util = 0;
	double avgr = 0, avgw = 0, avgtt = 0;
	int P0[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	int p1[13] = { 6, 17, 8, 19, 12, 31, 11, 18, 9, 22, 8, 26, 10 };
	int p2[17] = { 19, 38, 11, 24, 15, 21, 12, 27, 12, 34, 11, 34, 9, 29, 9, 31, 7 };
	int p3[17] = { 3, 37, 14, 41, 8, 30, 4, 19, 7, 33, 5, 18, 4, 26, 5, 31, 16 };
	int p4[15] = { 15, 35, 14, 41, 16, 45, 18, 51, 14, 61, 13, 54, 16, 61, 15 };
	int p5[17] = { 9, 24, 7, 21, 15, 31, 6, 26, 7, 31, 3, 18, 6, 21, 6, 33, 3 };
	int p6[17] = { 4, 38, 3, 41, 5, 29, 4, 26, 7, 32, 4, 22, 3, 26, 5, 22, 8 };
	int p7[15] = { 14, 36, 17, 31, 16, 32, 15, 41, 14, 42, 17, 39, 16, 33, 15 };
	int p8[17] = { 5, 14, 4, 33, 6, 31, 4, 31, 6, 27, 5, 21, 4, 19, 6, 11, 6 };

	create_s_list(8, P0, p1, p2, p3, p4, p5, p6, p7, p8, 13, 17, 17, 15, 17, 17, 15, 17);//a singly linked list where each node is a process from p1 to p8, each with all burst and IO times
	total_sum(dTS);
	wait_tt(time);
	CPU_UT(time, dTS, util);
	AVG(avgr, avgw, avgtt);
	print(avgr, avgw, avgtt, util, time);
	return 0;
}

