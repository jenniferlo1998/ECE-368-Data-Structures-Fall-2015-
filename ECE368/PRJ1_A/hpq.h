/*
Group Members:
1. Sthitapragyan Parida
2. Eric Lees
ECE 368 Project 1 (Spring 2015)
*/
#include<stdio.h>
#include <stdlib.h>
#include<math.h>
#ifndef HPQ_H
#define HPQ_H
typedef struct HPQ
{
	int arrival_time;
	int duration; 
		
} HPQ;

typedef struct HPQNode
{
	HPQ hpq;
	struct HPQNode *link;
} HPQNode;

HPQNode *hpq_front,*hpq_rear;   /* Global Declarations */

void hpqInsert(HPQ new_hpq)
{
	HPQNode *temp, *iter, *iter_front; 
	temp = (HPQNode *)malloc(sizeof(HPQNode));

	//iter = (HPQNode *)malloc(sizeof(HPQNode));
	//iter_front = (HPQNode *)malloc(sizeof(HPQNode));

	if( temp == NULL)
	{
		printf(" Out of Memory !! Overflow !!!");
	}
	else
	{
		temp->hpq.arrival_time = new_hpq.arrival_time;
		temp->hpq.duration = new_hpq.duration;
		//temp->hpq.duration = 0;

		temp->link = NULL;
		if(hpq_front == NULL)
		{
			hpq_front = hpq_rear = temp;
		}
		
		else if(hpq_front == hpq_rear)
		{
			if(temp->hpq.arrival_time > hpq_front->hpq.arrival_time)
			{
				hpq_rear->link=temp;
				hpq_rear = temp;
			}
			else
			{
				temp->link = hpq_front;
				hpq_front = temp;
			}
		}
		
		else
		{
			
			iter = hpq_front;
			iter_front = iter;
			if(temp->hpq.arrival_time <= hpq_front->hpq.arrival_time)
			{
				temp->link = hpq_front;
				hpq_front = temp;
			}
			else if(temp->hpq.arrival_time >= hpq_rear->hpq.arrival_time)
			{
				hpq_rear->link = temp;
				hpq_rear = temp;
			}
			else
			{
			while(iter != hpq_rear)
			{
	
					if(iter->hpq.arrival_time >= temp->hpq.arrival_time)
					{
						iter_front->link = temp;
						temp->link = iter;
						break;
					} 
					iter_front = iter;
					iter = iter->link;
				}
			}
			if (iter == hpq_rear)
			{
				iter_front->link =temp;
				temp->link = hpq_rear;
			}		
				
			//hpq_rear->link=temp;
			//hpq_rear = temp;
			
		}
	}
}
 
HPQ hpqDelete()
{
	HPQ hpq;
	HPQNode *temp;
	if(hpq_front ==  NULL) 
	{
		printf(" Underflow!!!");
		return;
	}
	else
	{
		temp=hpq_front;
		hpq.arrival_time = hpq_front->hpq.arrival_time;
		hpq.duration = hpq_front->hpq.duration;
		if(hpq_front == hpq_rear) hpq_rear=NULL;
		hpq_front=hpq_front->link;
		temp->link=NULL;
		free(temp);
		return(hpq);
	}
}
 
int getHPQCount()
{
	HPQNode *temp;
	if( hpq_front == NULL) return 0;
	else
	{
		temp=hpq_front;
		int count = 0;
		while(temp)
		{
			count = count + 1;
			temp=temp->link;
		}
	return count;
	}
}
int getHPQueueCount(long global_time)
{
	HPQNode *temp;
	if( hpq_front == NULL) return 0;
	else
	{
		temp=hpq_front;
		int count = 0;
		while(temp && (temp->hpq.arrival_time <= global_time))
		{
			count = count + 1;
			temp=temp->link;
		}
	return count;
	}
}

#endif
