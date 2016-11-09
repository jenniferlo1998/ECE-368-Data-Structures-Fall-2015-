#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "sorting.h"

int Save_File(char *Filename, long *Array, int Size)
{
	FILE *myFile;
	myFile = fopen(Filename, "w");
	if (myFile != NULL)
	{
		int i;
		fprintf(myFile, "%d\n", Size);
		for(i = 0; i < Size; i++)
		{
			fprintf(myFile, "%ld\n", Array[i]);
		}
	}
	fclose(myFile);
	return Size;	
}


void Save_Seq1(char *Filename, int N)
{
	int count = 0;
	int i2 = 0;
	int i3 = 0;
	
	long *array = (long * ) malloc(N * sizeof(long));
	array[count++] = 1;
	
	while (array[count-1] < N)
	{
		if(array[i2]*2 < array[i3]*3)
		{
			array[count] = array[i2]*2;
			i2++;
		}
		else if(array[i2]*2 > array[i3]*3)
		{
			array[count] = array[i3]*3;
			i3++;
		}
		else
		{
			array[count] = array[i2]*2;
			i2++;
			i3++;
		} 
		count++;
	}
	Save_File(Filename, array, count-1);	
}

void Save_Seq2(char *Filename, int N)
{
	long count = 0;
	long current = N;
	long *array = malloc(N * sizeof(long));
	array[count++] = current;
	while (current != 1)
	{
		current = (long)(current/1.3);
		if ((current == 9) || (current == 10))
		{
			current = 11;
		}
		else if (current < 1)
		{
			current = 1;
		}
		array[count++] = current;
	}
	Save_File(Filename, array, count);	

}

long * Load_File(char *Filename, int *Size)
{
	long *array;
	int size;
	int i;
	FILE *myFile;
	myFile = fopen(Filename, "r");
	if(myFile != NULL)
	{
		fscanf(myFile, "%d", &size);
		*Size = size;
		array = (long *) malloc(size * sizeof(long));
		for (i = 0; i < size; i++)
		{
			fscanf(myFile, "%ld",&array[i]);
		}
	}
	fclose(myFile);
	return array;
}

void Shell_Insertion_Sort(long *Array, int Size, double *N_Comp, double *N_Move)
{
	int seq_size = 0;
	int i2 = 0;
	int i3 = 0;
	
	long *seq1 = (long * ) malloc(Size * sizeof(long));
	seq1[seq_size++] = 1;
	
	while (seq1[seq_size-1] < Size)
	{
		if(seq1[i2]*2 < seq1[i3]*3)
		{
			seq1[seq_size] = seq1[i2]*2;
			i2++;
		}
		else if(seq1[i2]*2 > seq1[i3]*3)
		{
			seq1[seq_size] = seq1[i3]*3;
			i3++;
		}
		else
		{
			seq1[seq_size] = seq1[i2]*2;
			i2++;
			i3++;
		} 
		seq_size++;
	}


	long t;
	long i, j, k, temp_r;

	*N_Comp = 0;
	*N_Move = 0;
	for(t = seq_size-1; t >= 0; t--)
	{
		long k = seq1[t];
		for (j = k; j <= Size-1; j++)
		{
			temp_r = Array[j];
			*N_Move = *N_Move + 1;
			i = j;
			while ( (i >= k) && (Array[i-k] > temp_r) )
			{
				*N_Comp = *N_Comp + 1;
				Array[i] = Array[i-k];
				*N_Move = *N_Move + 1;
				i = i - k;
			}
			Array[i] = temp_r;
			*N_Move = *N_Move + 1;

		} 
	}	
}

void Improved_Bubble_Sort(long *Array, int Size, double *N_Comp, double *N_Move)
{
	long width = Size;
	float ratio = 1.3;
	int swap = 1;
	*N_Comp = 0;
	*N_Move = 0;
        while( (width != 1) && (swap == 1) )
	{

        	width = (int)(width / ratio);
        	if (width < 1)
		{
			width = 1;
		}        
        	int i = 0;
        	swap = 0;
		while ((i + width) < Size)
		{
			*N_Comp = *N_Comp + 1;
			if (Array[i] > Array[i+width])
			{
				*N_Move += 3;
				long temp = Array[i];
				Array[i] = Array[i+width];
				Array[i+width] = temp;
        	        	swap = 1; 
			}
			i++;
		}
        }

}

/*

void main(int argc, char **argv)
{
	int i, count;
	long *array;
	double comp, move;
	clock_t t;
	double time_taken;

	printf("Shell Insertion Sort 1000\n");
	array = Load_File("1000.txt", &count);
	//for (i = 0; i < count; i++)	printf("%ld\n", array[i]);
	t = clock();
	Shell_Insertion_Sort(array, count, &comp, &move);
	t = clock() - t;
	time_taken = (double) ((double)t)/CLOCKS_PER_SEC;
	printf("Comparisions:%.0f Moves:%.0f TimeTaken:%f\n\n", comp, move, time_taken);	
	//for (i = 0; i < count; i++)	printf("%ld\n", array[i]);

	printf("Shell Insertion Sort 10000\n");
	array = Load_File("10000.txt", &count);
	//for (i = 0; i < count; i++)	printf("%ld\n", array[i]);
	t = clock();
	Shell_Insertion_Sort(array, count, &comp, &move);
	t = clock() - t;
	time_taken = (double) ((double)t)/CLOCKS_PER_SEC;
	printf("Comparisions:%.0f Moves:%.0f TimeTaken:%f\n\n", comp, move, time_taken);	
	//for (i = 0; i < count; i++)	printf("%ld\n", array[i]);

	printf("Shell Insertion Sort 100000\n");
	array = Load_File("100000.txt", &count);
	//for (i = 0; i < count; i++)	printf("%ld\n", array[i]);
	t = clock();
	Shell_Insertion_Sort(array, count, &comp, &move);
	t = clock() - t;
	time_taken = (double) ((double)t)/CLOCKS_PER_SEC;
	printf("Comparisions:%.0f Moves:%.0f TimeTaken:%f\n\n", comp, move, time_taken);	
	//for (i = 0; i < count; i++)	printf("%ld\n", array[i]);

	printf("Shell Insertion Sort 1000000\n");
	array = Load_File("1000000.txt", &count);
	//for (i = 0; i < count; i++)	printf("%ld\n", array[i]);
	t = clock();
	Shell_Insertion_Sort(array, count, &comp, &move);
	t = clock() - t;
	time_taken = (double) ((double)t)/CLOCKS_PER_SEC;
	printf("Comparisions:%.0f Moves:%.0f TimeTaken:%f\n\n", comp, move, time_taken);	
	//for (i = 0; i < count; i++)	printf("%ld\n", array[i]);

	printf("Bubble Sort 1000\n");
	array = Load_File("1000.txt", &count);
	//for (i = 0; i < count; i++)	printf("%ld\n", array[i]);
	t = clock();
	Improved_Bubble_Sort(array, count, &comp, &move);
	t = clock() - t;
	time_taken = (double) ((double)t)/CLOCKS_PER_SEC;
	printf("Comparisions:%.0f Moves:%.0f TimeTaken:%f\n\n", comp, move, time_taken);	
	//for (i = 0; i < count; i++)	printf("%ld\n", array[i]);

	printf("Bubble Sort 10000\n");
	array = Load_File("10000.txt", &count);
	//for (i = 0; i < count; i++)	printf("%ld\n", array[i]);
	t = clock();
	Improved_Bubble_Sort(array, count, &comp, &move);
	t = clock() - t;
	time_taken = (double) ((double)t)/CLOCKS_PER_SEC;
	printf("Comparisions:%.0f Moves:%.0f TimeTaken:%f\n\n", comp, move, time_taken);	
	//for (i = 0; i < count; i++)	printf("%ld\n", array[i]);

	printf("Bubble Sort 100000\n");
	array = Load_File("100000.txt", &count);
	//for (i = 0; i < count; i++)	printf("%ld\n", array[i]);
	t = clock();
	Improved_Bubble_Sort(array, count, &comp, &move);
	t = clock() - t;
	time_taken = (double) ((double)t)/CLOCKS_PER_SEC;
	printf("Comparisions:%.0f Moves:%.0f TimeTaken:%f\n\n", comp, move, time_taken);	
	//for (i = 0; i < count; i++)	printf("%ld\n", array[i]);

	printf("Bubble Sort 1000000\n");
	array = Load_File("1000000.txt", &count);
	//for (i = 0; i < count; i++)	printf("%ld\n", array[i]);
	t = clock();
	Improved_Bubble_Sort(array, count, &comp, &move);
	t = clock() - t;
	time_taken = (double) ((double)t)/CLOCKS_PER_SEC;
	printf("Comparisions:%.0f Moves:%.0f TimeTaken:%f\n\n", comp, move, time_taken);	
	//for (i = 0; i < count; i++)	printf("%ld\n", array[i]);

	printf("Shell Insertion Test\n");
	long arrayTest[10] = {2, 7, 10, 3, 6, 1, 5, 9, 4, 8};
	count = 10;
	for (i = 0; i < count; i++)	printf("%ld\n", arrayTest[i]);
	t = clock();
	Shell_Insertion_Sort(arrayTest, count, &comp, &move);
	t = clock() - t;
	time_taken = (double) ((double)t)/CLOCKS_PER_SEC;
	printf("Comparisions:%.0f Moves:%.0f TimeTaken:%f\n\n", comp, move, time_taken);	
	for (i = 0; i < count; i++)	printf("%ld\n", arrayTest[i]);
	
	printf("Improved Bubble Test\n");
	long arrayTest2[10] = {2, 7, 10, 3, 6, 1, 5, 9, 4, 8};
	for (i = 0; i < count; i++)	printf("%ld\n", arrayTest2[i]);
	t = clock();
	Improved_Bubble_Sort(arrayTest2, count, &comp, &move);
	t = clock() - t;
	time_taken = (double) ((double)t)/CLOCKS_PER_SEC;
	printf("Comparisions:%.0f Moves:%.0f TimeTaken:%f\n\n", comp, move, time_taken);	
	for (i = 0; i < count; i++)	printf("%ld\n", arrayTest2[i]);

		
}
*/
