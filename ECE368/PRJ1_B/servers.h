#include<stdio.h>
#include<math.h>
#include<stdlib.h>

typedef struct SERVERS
{
	long busy;
	long busy_total;
	long task_end_time;
		
} SERVERS;

SERVERS servers[64];

long i = 0;
void initServers()
{
	for(i = 0; i < 64; i++)
	{
		servers[i].busy = 0;
		servers[i].task_end_time = -1;
		servers[i].busy_total = 0;
	}
}
long getTotalFreeServers()
{
	long count = 0;
	long i = 0;
	for(i = 0; i < 64; i++)
	{	
		if(servers[i].busy == 0)
		{
			count++;
		}
	}
	return count;
}

void addServerTask(long task_end_time, long duration)
{
	long i = 0;
	long done = 0;
	//printf("Done%ld\n", task_end_time);
	while(i < 64 && done == 0)
	{
		if(servers[i].busy == 0)
		{
			servers[i].busy = 1;
			servers[i].task_end_time = task_end_time;
			servers[i].busy_total += duration;
			done = 1;	
		}
		i++;
	}

}

float getServersBusySum()
{
	float servers_busy_sum = 0;
	for (i = 0; i < 64 ; i++)
	{
		servers_busy_sum += servers[i].busy_total;
	}
	
}
void fixServers(long global_time)
{
	long i = 0;
	for (i = 0; i < 64 ; i++)
	{
		//			printf("Dude\n");
		if ( (servers[i].busy == 1) && (servers[i].task_end_time <= global_time) )
		{

			servers[i].busy = 0;
		}
	}
}


