/*
Group Members:
1. Sthitapragyan Parida
2. Eric Lees
ECE 368 Project 1 B(Spring 2015)
Compile With: gcc main.c -o project1-B -lm 
Run using:
1. project1-B 0.5 0.7 1 10000
2. project1-B input.txt
*/
#include<limits.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include"event.h"
#include"lpq.h"
#include"hpq.h"
#include"utility_funcs.h"
#include"sys_state.h"
#include"servers.h"


void runSimulationMode1(float, float, float, long);
void runSimulationMode2(char *);
char decidePriority(long);
void generateRandomQueues(float, float, float, long, char *);
void readFile(char *);
float l2;
long h_num = 0, l_num = 0;
float l1, l0, mu, rho1, rho0;
long max_num;
int main(int argc, char *argv[])
{
	srand(time(NULL));
	event_front = NULL;
	event_rear = NULL;
	hpq_front = NULL;
	hpq_rear = NULL;
	lpq_front = NULL;
	lpq_rear = NULL;
	


	if(argc == 5)
	{	
		l1 = atof(argv[1]);
		l2 = atof(argv[2]);
		mu = atof(argv[3]);
		max_num = atoi(argv[4]);
		runSimulationMode1(l1, l2, mu, max_num);
	}
	else if(argc == 1)
	{
		int cn = 1;
		mu = 0.2;
		rho1 = 0.3;
		l1 = 4*mu*rho1;
		for(cn = 1; cn <= 7; cn+=2)
		{
			rho0 = (float)cn/10.0;
			l0 = 4*mu*rho0;
			max_num = 10000;
			runSimulationMode1(l0, l1, mu, max_num);

		}

		rho0 = 0.3;
		l0 = 4*mu*rho0;
		for(cn = 1; cn <= 7; cn+=2)
		{
			rho1 = (float)cn/10.0;
			l1 = 4*mu*rho1;
			max_num = 10000;
			runSimulationMode1(l0, l1, mu, max_num);

		}

	}

	else if(argc == 2)
	{
		runSimulationMode2(argv[1]);
	}
	else
	{
		printf("Wrong number of inputs\n");
	}
	
}

void runSimulationMode2(char *filename)
{	
	SysState sys_state = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	float lbf = 0;	
	h_num = 0;
	l_num = 0;
	initServers();
	readFile(filename);
	long prev_global_time;
	sys_state.global_time = 0;
	long ch, cl;
	long htc = 0;
	long ltc = 0;

	while((getHPQCount() > 0 || getLPQCount() > 0))
	{
		//printf("%ld %ld %ld ", sys_state.global_time, getHPQCount(), getLPQCount());
		prev_global_time = sys_state.global_time;
		fixServers(sys_state.global_time);
		htc = getHPQTimeCount(sys_state.global_time);
		ltc = getLPQTimeCount(sys_state.global_time);
		//printf("%ld %ld %ld\n", htc, ltc, getTotalFreeServers());
		if(htc > 0)
		{
			long min_time = INT_MAX;
			int d_flag = 0;
			int hi = 0;
			for(ch = 0; ch < htc ; ch++)
			{
				
				HPQ hpq = getHPQIndexElement(hi);
				if(hpq.ns <= getTotalFreeServers())
				{
					hpq = deleteHPQIndexElement(hi);
					sys_state.event_arrive0_total += sys_state.global_time - hpq.arrival_time;

					hi--;
					long n = 0;
					d_flag = 1;
					lbf += (float)(max(hpq.srv_times, hpq.ns) - min(hpq.srv_times, hpq.ns))/avg(hpq.srv_times, hpq.ns);
					for(n = 0; n < hpq.ns; n++)
					{
						
						long tet = sys_state.global_time + hpq.srv_times[n];
						addServerTask(tet, hpq.srv_times[n]);
						min_time = (tet < min_time) ? tet : min_time;
					}
				}
				hi++;
			}
			if (d_flag == 1) sys_state.global_time = min_time;
			else
			{
				long min_time = INT_MAX;
				long c_flag = 0;
				for (i = 0; i < 64 ; i++)
				{
					if (servers[i].busy == 1)
					{
						long tet = servers[i].task_end_time;
						min_time = (tet < min_time) ? tet : min_time;
						c_flag = 1;
						
					}
				}
				if (c_flag == 1) sys_state.global_time = min_time;
			}
		}
		else if(ltc > 0)
		{
			long min_time = INT_MAX;
			int d_flag = 0;
			int li = 0;
			for(cl = 0; cl < ltc ; cl++)
			{
				LPQ lpq = getLPQIndexElement(li);
				if(lpq.ns <= getTotalFreeServers())
				{
					lpq = deleteLPQIndexElement(li);
					sys_state.event_arrive1_total += sys_state.global_time - lpq.arrival_time;
					li--;
					long n = 0;
					d_flag = 1;
					lbf += (float)(max(lpq.srv_times, lpq.ns) - min(lpq.srv_times, lpq.ns))/avg(lpq.srv_times, lpq.ns);
					for(n = 0; n < lpq.ns; n++)
					{
						long tet = sys_state.global_time + lpq.srv_times[n];
						addServerTask(tet, lpq.srv_times[n]);
						min_time = (tet < min_time) ? tet : min_time;
					}
				}
				li++;
			}
			if (d_flag == 1) sys_state.global_time = min_time;
			else
			{
				long min_time = INT_MAX;
				long c_flag = 0;
				for (i = 0; i < 64 ; i++)
				{
					if (servers[i].busy == 1)
					{
						long tet = servers[i].task_end_time;
						min_time = (tet < min_time) ? tet : min_time;
						c_flag = 1;
						
					}
				}
				if (c_flag == 1) sys_state.global_time = min_time;
			}

		}
		else
		{
			long min_time = INT_MAX;
			long c_flag = 0;
			for (i = 0; i < 64 ; i++)
			{
				if (servers[i].busy == 1)
				{
					long tet = servers[i].task_end_time;
					min_time = (tet < min_time) ? tet : min_time;
					c_flag = 1;
					
				}
			}
			if (c_flag == 1) sys_state.global_time = min_time;
			else if(c_flag == 0)
			{
				long htch = getHPQTimeCount(sys_state.global_time);
				long ltcl = getLPQTimeCount(sys_state.global_time);
				HPQ h;
				LPQ l;
				if(getHPQCount() > 0 && getLPQCount() > 0)
				{
					h = getHPQIndexElement(htch);
					l = getLPQIndexElement(ltcl);
	
					long ht = h.arrival_time;
					long lt = l.arrival_time;
					sys_state.global_time = (ht <= lt) ? ht : lt;

				}
				else if(getHPQCount() > 0 && getLPQCount() <= 0)
				{
					h = getHPQIndexElement(htch);
					long ht = h.arrival_time;
					sys_state.global_time = ht;
				}
				
				else if(getHPQCount() <= 0 && getLPQCount() > 0)
				{
					l = getLPQIndexElement(ltcl);
					long lt = l.arrival_time;
					sys_state.global_time = lt;
				}
			}
		}
		long queue_size = getLPQTimeCount(sys_state.global_time) + getHPQTimeCount(sys_state.global_time);
		sys_state.sum_weighted0_queue += queue_size * (sys_state.global_time - prev_global_time);

	}	
	float high_queue_time = (float)sys_state.event_arrive0_total/h_num;
	float low_queue_time = (float)sys_state.event_arrive1_total/l_num;
	float servers_busy_sum = getServersBusySum();
	float avg_cpu_util = servers_busy_sum/(64 * sys_state.global_time);
	float avg_queue_length = (float)sys_state.sum_weighted0_queue / sys_state.global_time;
	printf("%ld %f\n", h_num+l_num, lbf);
	lbf = lbf/(float)(h_num + l_num);
	

	FILE *fp;
	fp = fopen("proj1-b_output", "a");
	if (fp == NULL) 
	{
		printf("Couldn't open file for writing.\n");
		exit(0);
	}
   	fprintf(fp, "%f %f %ld\n", rho0, rho1, max_num);
	fprintf(fp, "%f %f %f\n", l0, l1, mu);
	fprintf(fp, "%f %f %f\n", high_queue_time, low_queue_time, avg_queue_length);
	fprintf(fp, "%f %f\n\n", avg_cpu_util, lbf);
	fclose(fp);
}


void runSimulationMode1(float l0, float l1, float mu, long max_num)
{
	generateRandomQueues(l0, l1, mu, max_num, "dummy.txt");
	runSimulationMode2("dummy.txt");
	remove("dummy.txt");	
}
void readFile(char * filename)
{
	FILE *fin = fopen(filename, "r");
	char line[1024];
	long i = 0;
	while (fgets(line, 1024, fin))
	{
		if (strlen(line) < 3) break;
		long arrival, priority, ns, srv_times[32];
		arrival = 0;
		priority = 0;
		ns = 0;
		long ind = 0;
		char * end;
		long l;
  		end = line;
		long base = 10;
		while((ind-3) < ns || (ind <=2))
		{
			l=strtol(end, &end, base);
			if(ind == 0)
			{
				arrival = l;	
			}
			else if(ind == 1)
			{
				priority = l;	
			}
			else if(ind == 2)
			{
				ns = l;	
			}
			else
			{
			srv_times[ind - 3] = l;
			}
			ind += 1;
		}
		if(ind >= 4 )
		{
			if(priority == 0)
			{
				HPQ hpq;
				hpq.arrival_time = arrival;
				hpq.ns = ns;
				hpq.srv = 0;
				long i = 0;
				for (i = 0 ; i < 32; i++)
				{
					if (i < hpq.ns)
					{
						
						hpq.srv_times[i] = srv_times[i];
					}
					else
					{
						hpq.srv_times[i] = -1;
					}
				}
				hpqInsert(hpq);
				h_num++;
			}
			else if(priority == 1)
			{
				LPQ lpq;
				lpq.arrival_time = arrival;
				lpq.ns = ns;
				lpq.srv = 0;
				long i = 0;
				for (i = 0 ; i < 32; i++)
				{
					if (i < lpq.ns)
					{
						
						lpq.srv_times[i] = srv_times[i];
					}
					else
					{
						lpq.srv_times[i] = -1;
					}
				}
				lpqInsert(lpq);
				l_num++;
			}

		}
	}
	fclose(fin);
	
	/*
	LPQ l = lpqDelete();
	lpqInsert(l);
	//printf("%ld\n", getLPQCount());
	//printf("%ld\n", getHPQCount());
	long hcount = getHPQCount();

	for(i = 0; i < hcount; i++)
	{
		HPQ h = getHPQIndexElement(i);

		////printf ("%ld 1 %ld\n", h.arrival_time, h.ns);
		long i = 0;
		//for(i = 0; i < h.ns; i++) { //printf("%ld ", h.srv_times[i]); }
		////printf("\n");			
			
	}
	//printf("_____________________\n\n");
	long lcount = getLPQCount();
	for(i = 0; i < lcount; i++)
	{
		LPQ l = getLPQIndexElement(i);

		////printf ("%ld 0 %ld\n", l.arrival_time, l.ns);
		long i = 0;
		//for(i = 0; i < l.ns; i++) { //printf("%ld ", l.srv_times[i]); }
		////printf("\n");			
			
	}
	deleteHPQIndexElement(2);
	deleteHPQIndexElement(3);
	//printf("-----------------------\n");
	while (getHPQCount() != 0)
	{
		HPQ h = hpqDelete();
		////printf ("%ld 1 %ld\n", h.arrival_time, h.ns);
		//for(i = 0; i < h.ns; i++) { //printf("%ld ", h.srv_times[i]); }
		////printf("\n");			
			
	}
	deleteLPQIndexElement(2);
	deleteLPQIndexElement(3);
	//printf("-----------------------\n");
	while (getLPQCount() != 0)
	{
		LPQ l = lpqDelete();
		////printf ("%ld 1 %ld\n", l.arrival_time, l.ns);
		//for(i = 0; i < h.ns; i++) { //printf("%ld ", h.srv_times[i]); }
		////printf("\n");			
			
	}
	
	*/
}

void generateRandomQueues(float lambda0, float lambda1, float mu,long num_sim, char *fn)
{
	FILE *fp;
	fp = fopen(fn, "w");
	long i = 0;
	if (fp == NULL) 
	{
		printf("Couldn't open file for writing.\n");
		exit(0);
	}
	long count = 1;
	long h_at = 0;
	long nsh = randomInt(1, 32);
	fprintf(fp, "%ld %ld %ld ", h_at, (long)0, nsh);
	for(i = 0; i < nsh; i++)
	{
		long h_st = genRandInterval(mu);
		fprintf(fp, "%ld ", h_st);
	
	}
	fprintf(fp, "\n");

	long l_at = 0;
	long nsl = randomInt(1, 32);
	fprintf(fp, "%ld %ld %ld ", l_at, (long)1, nsl);
	for(i = 0; i < nsl; i++)
	{
		long h_st = genRandInterval(mu);
		fprintf(fp, "%ld ", h_st);
	
	}
	fprintf(fp, "\n");

   	while(count < num_sim)
	{
		h_at += genRandInterval(lambda0);
		l_at += genRandInterval(lambda1);
		long nsh = randomInt(1, 32);
		fprintf(fp, "%ld %ld %ld ", h_at, (long)0, nsh);
		for(i = 0; i < nsh; i++)
		{
			long h_st = genRandInterval(mu);
			fprintf(fp, "%ld ", h_st);
		
		}
		fprintf(fp, "\n");

		long nsl = randomInt(1, 32);
		fprintf(fp, "%ld %ld %ld ", l_at, (long)1, nsl);
		for(i = 0; i < nsh; i++)
		{
			long h_st = genRandInterval(mu);
			fprintf(fp, "%ld ", h_st);
		}
		fprintf(fp, "\n");
		count++;
	}
	fclose(fp);
}
