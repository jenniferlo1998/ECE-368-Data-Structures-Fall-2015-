#include<stdio.h>
#include<math.h>
#ifndef SYSSTATE_H
#define SYSSTATE_H
typedef struct SysState
{
	long low_priority_total_num; 
	long high_priority_total_num;
	long low_priority_current_num;
	long high_priority_current_num;
	long event_arrive0_total;
	long event_depart0_total;
	long event_arrive1_total;
	long event_depart1_total;
	long sum_weighted0_queue;	
	long sum_weighted1_queue;
	long event_service0_total;
	long event_service1_total;	
	int server_busy; //1 = True 0 = False
	long server_busy_total;
	long server_idle_total;  	
	long global_time ;
} SysState;
#endif
