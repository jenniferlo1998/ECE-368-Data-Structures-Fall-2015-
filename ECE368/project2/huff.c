#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 50

// HuffTree Structure
typedef struct leaf {
	char ch;
	int freq;
	struct leaf * left;  // left sub-tree
	struct leaf * right; // right sub-tree
} HuffNode;

// HuffNode Linked List Structure
typedef struct hufflistnode {
	struct hufflistnode *link;
	HuffNode * huffnode;
} HuffListNode;

//HuffTree Functions
HuffNode * HuffNode_create(char, int);
void HuffNode_destroy(HuffNode *);
HuffNode * HuffNode_merge(HuffNode *, HuffNode *);
int getLeafNodesCount(HuffNode *);
int findTreeHeight(HuffNode *);

//HuffTree Linked List Functions
HuffListNode * HuffListNode_create(HuffNode *);
HuffListNode * createInitialList(int *);
HuffListNode * insertAscendFreqList(HuffListNode *, HuffListNode *);
void deleteHuffCompleteList(HuffListNode *);
void deleteHuffListNode(HuffListNode * );
int listLength(HuffListNode *);
HuffListNode *  combineLeastTwoFreqHuffTrees(HuffListNode * );

//HuffTree and HuffTable Creation Functions
void createHuffTreeCode(int *, int, int, char *);
void calcHuffCodesHelper(HuffNode *, int **, int *, int);
void calcHuffCodes(HuffNode *, int **);
int findHuffCode(int *, char, int **, int, int);
int ** createHuffTable(HuffNode *);

//File Write Functions
void writeHeader(HuffNode *, FILE *);
void compressFile(char *, HuffNode *, int, int, int);
int count_freq(char *, int *);


//Count frequency of each ASCII character
int count_freq(char *fn, int *freq)
{
	int i = 0, total_count = 0;
 	char c;
	FILE * fp = fopen(fn, "r"); 
	for(i = 0; i <= 127; i++)	freq[i] = 0;
	
	int fc = 0;
	while(1 == 1) 
	{
		c = fgetc(fp);
		if(c == EOF)	break;
		if (freq[c] == 0) total_count++;
		freq[c]++;
	 
    	}
	fclose(fp); 
	return total_count;
}

//Create a new, properly intialized HuffNode
HuffNode * HuffNode_create(char ch, int freq)
{
	HuffNode * node = malloc(sizeof(HuffNode));
	node -> ch = ch;
	node -> freq = freq;
	node -> left = NULL;
	node -> right = NULL;
	return node;
}

// Merge Two Huff Trees into One
HuffNode * HuffNode_merge(HuffNode * node1, HuffNode * node2)
{
	HuffNode * node = malloc(sizeof(HuffNode));
	node -> ch = 0;
	node -> freq = node1->freq + node2->freq;
	node -> left = node1;
	node -> right = node2;
	return node;
}

//Destroy a tree, including all sub-children. Must handle NULL values safely.
void HuffNode_destroy(HuffNode * tree)
{
	if (tree == NULL)
	{
		return;
	}
	HuffNode_destroy(tree -> left);
	HuffNode_destroy(tree -> right);
	free(tree);
}

//Create HuffList Node
HuffListNode * HuffListNode_create(HuffNode * huffnode)
{
	HuffListNode *hufflistnode = malloc(sizeof(HuffListNode));
	hufflistnode -> huffnode = huffnode;
	hufflistnode -> link = NULL;
	return hufflistnode;
}

//Delete HuffList Node
void deleteHuffListNode(HuffListNode * node)
{
	if (node == NULL) return;
	HuffNode_destroy(node->huffnode);
	free(node);
}

//Delete Complete HuffList
void deleteHuffCompleteList(HuffListNode * head)
{	
	HuffListNode *temp = head;
	while(1 == 1)
	{
		if(head == NULL) break;
		temp = head;
		head = head->link;
		deleteHuffListNode(temp);	
	}
}

//Return HuffNode Linked List Length
int listLength(HuffListNode *head)
{
	int count = 0;
	HuffListNode *temp = head;
	while(temp != NULL)
	{
		
		count++;
		temp = temp->link;
	}
	return count;
}

//Create HuffNode for every character and create HuffNode Linked List
HuffListNode * createInitialList(int * freq)
{
	HuffNode * headHuffNode = HuffNode_create(0, 0);
	HuffListNode *head = HuffListNode_create(headHuffNode);
	int headsetflag = 0;
	int i = 0;

	for (i = 0; i < 128 ; i++)
	{
		if(freq[i] > 0)
		{
			char c = i;
			
			if(headsetflag)
			{
				HuffNode * newnode = HuffNode_create(c, freq[i]);
				HuffListNode * newHuffListNode = HuffListNode_create(newnode);
				head = insertAscendFreqList(head, newHuffListNode);
			}
			else
			{

				(head->huffnode)->ch = c;
				(head->huffnode)->freq = freq[i];
				headsetflag = 1;
			}
			
			
		}
	}
		
	//Add Extra EOF Character
	HuffNode * newnode = HuffNode_create(-1, 1);
	HuffListNode * newHuffListNode = HuffListNode_create(newnode);
	head = insertAscendFreqList(head, newHuffListNode);
	
	return head;

}

//Add new HuffListNode in order of frequency(lowest first)
HuffListNode * insertAscendFreqList(HuffListNode * head, HuffListNode* new)
{
	if (head == NULL) 
	{
		return new;
	}	
	else if(head->link == NULL) 
	{
		if(new->huffnode->freq > head->huffnode->freq)
		{
			head->link = new;
			return head;
		}	
		else
		{
			new->link = head;
			return new;
		}
	
	}
	else if (new->huffnode->freq < head->huffnode->freq)
	{
			new->link = head;
			return new;
	}
	else
	{
		HuffListNode *temp;	
		temp = head;
		while(temp->link != NULL)
		{
			HuffListNode *temp2 = temp->link;
			if (new-> huffnode->freq < temp2->huffnode->freq) break;
			temp = temp -> link;
		}
			
		if (temp -> link == NULL)
		{
			temp->link = new;
		}
		else
		{
			new->link = temp->link;
			temp->link = new;
		}
		return head;
	}
	
}

//Combine two least frequency huffnodes into one
HuffListNode *  combineLeastTwoFreqHuffTrees(HuffListNode * head)
{
	if(listLength(head) < 2) return head;
	HuffListNode * node1 = head;
	HuffListNode * node2 = head->link;
	head = node2->link;
	
	HuffNode * newnode = HuffNode_merge(node1->huffnode, node2->huffnode);
	HuffListNode * newHuffListNode = HuffListNode_create(newnode);
	head = insertAscendFreqList(head, newHuffListNode);
	return head;
}

//Return number of leaf nodes in hufftree
int getLeafNodesCount(HuffNode *huffTree)
{
	if(huffTree == NULL)	return 0;
	if(huffTree->left == NULL && huffTree->right==NULL)	return 1;            
	return getLeafNodesCount(huffTree->left) + getLeafNodesCount(huffTree->right);      
}

//Return height of hufftree
int findTreeHeight(HuffNode * huffTree)
{
	if (huffTree == NULL)	return 0;
	int l = findTreeHeight(huffTree->left);
 	int r = findTreeHeight(huffTree->right);
 	if(l>r)	return(l+1);
	return(r+1);
}

//Calculate HuffCode for each character from hufftree
void calcHuffCodes(HuffNode * huffTree, int ** huffCodes)
{
  int i = 0;
  calcHuffCodesHelper(huffTree, huffCodes, &i, 1);
}

//Helper function to recursively find huffcodes
void calcHuffCodesHelper(HuffNode * huffTree, int **huffCodes, int * i, int j)
{
	if (huffTree == NULL)	return;
	HuffNode * left = huffTree->left;
	HuffNode * right = huffTree->right;
	
	if((left == NULL) && (right == NULL))
	{
		huffCodes[*i][0] = huffTree->ch;
		(*i)++;
		return;
	}
	
	if(left != NULL)
	{
		int numRow = getLeafNodesCount(left);
		int k;
 		for(k = *i; k < (*i) + numRow; k++)
		{
			huffCodes[k][j] = 0;
		}
		calcHuffCodesHelper(left, huffCodes, i, j+1);
    	}
	
	if(right != NULL)
	{
		int numRow = getLeafNodesCount(right);
		int k;
 		for(k = *i; k < (*i) + numRow; k++)
		{
			huffCodes[k][j] = 1;
		}
		calcHuffCodesHelper(right, huffCodes, i, j+1);
    	}
}

// Create hufftable from huff tree
int ** createHuffTable(HuffNode * huffTree)
{
	int tree_height = findTreeHeight(huffTree);
	int leaf_nodes = getLeafNodesCount(huffTree);
  	int ** huffCodes = malloc(sizeof(int*) * leaf_nodes);
	int i, j;
	for(i = 0; i < leaf_nodes; i++)
	{
		huffCodes[i] = malloc(sizeof(int) * (tree_height + 1) );
		for(j = 0; j < tree_height+1; j++)
		{
			huffCodes[i][j] = -1;
		}
	}
	calcHuffCodes(huffTree, huffCodes);
	return huffCodes;
}

//Create HuffTree and compress file
void createHuffTreeCode(int *freq, int total_count, int totalFileChars, char * fileName)
{
	HuffListNode * listHead = createInitialList(freq);
	HuffListNode * temp = listHead;
	
	while(temp != NULL)
	{
		temp = temp->link;
	}
	while(listLength(listHead) > 1)
	{
		listHead = combineLeastTwoFreqHuffTrees(listHead);
	}

	int tree_height = findTreeHeight(listHead->huffnode);
	int leaf_nodes = getLeafNodesCount(listHead->huffnode);
	compressFile(fileName, listHead->huffnode, tree_height, leaf_nodes, totalFileChars);
	deleteHuffCompleteList(listHead);
}

//Find huffcode for a character from huff table
int findHuffCode(int *huffCode, char c, int **huffCodes, int tree_height, int leaf_nodes)
{
	int i, j;
	for (i = 0; i < tree_height; i++) huffCode[i] = -1;
	for (i = 0; i < leaf_nodes; i++)
	{
		if(huffCodes[i][0] == c)
		{
			j = 1;
			while(huffCodes[i][j] != -1)
			{
				huffCode[j-1] = huffCodes[i][j];
				j++;
			}
			break;
		}
	}
	return j-1;
}

//Write hufftree to file as header
void writeHeader(HuffNode * huffTree, FILE * fpw)
{
  
	if (huffTree->left == NULL && huffTree->right == NULL)
	{
		char chVal = '1';
		fwrite(&chVal, sizeof(char), 1, fpw);
		chVal = huffTree->ch;
		fwrite(&chVal, sizeof(char), 1, fpw);
	}
	else
	{
		char chVal = '0';
		fwrite(&chVal, sizeof(char), 1, fpw);
		writeHeader(huffTree->left, fpw);
		writeHeader(huffTree->right, fpw);
	}
}

//Compress file using hufftree
void compressFile(char * fileName, HuffNode * huffTree, int tree_height, int leaf_nodes, int totalFileChars)
{
	int **huffCodes = createHuffTable(huffTree);
	char fileNameHuff[50];
	strcpy(fileNameHuff, fileName);
	strcat(fileNameHuff, ".huff");
	FILE * fpr = fopen(fileName, "r");
	FILE * fpw = fopen(fileNameHuff, "wb"); 
	int frc = 0;
	int fwc = 0;
	char c;
	int * huffCode = malloc(tree_height * sizeof(int));
	int i, bitlen, k;
	
	writeHeader(huffTree, fpw);
	char chVal = 0;

	
	int buffer[BUFFER_SIZE];
	int bufLen = 0;
	for(i = 0; i < BUFFER_SIZE;i++) buffer[i] = -1;
	
	//Write Bits
	int breakFlag = 0;
	while(breakFlag != 1) 
	{
		c = fgetc(fpr);
		if(c == EOF)	
		{
			breakFlag = 1;
		}
		frc++;
		bitlen = findHuffCode(huffCode, c, huffCodes, tree_height, leaf_nodes);
		
		//Push to buffer
		for(k = 0;k < bitlen; k++) 
		{
			buffer[bufLen++] = huffCode[k];
		}

		//If byte available
		while(bufLen >= 8)
		{
			//Calculate Byte Value
			int byteVal = 0;
			for(k = 0; k < 8; k++)
			{
				byteVal += (1 << (7-k)) * buffer[k];
			}
			char chVal = byteVal;
			
			//Write Byte Value
			fwrite(&chVal, sizeof(char), 1, fpw); 	

			//Move Buffer Elements
			for(k = 8; k <= bufLen ; k++) buffer[k-8] = buffer[k];
			
			//Adjust Buffer Length
			bufLen = bufLen - 8;


		}		
	 	fwc += bitlen;
    	}
	
	// If padding is needed
	int padNum = 0; 
	if(bufLen > 0)
	{
		//pad buffer with zeros
		for(k = bufLen ; k < 8; k++)
		{
			padNum++;
			bufLen++;
			buffer[k] = 0;
			
		}
	
		//Calculate Byte Value
		int byteVal = 0;
		for(k = 0; k < 8; k++) 
		{
			byteVal += (1 << (7-k)) * buffer[k];
		}
		char chVal = byteVal;
		
		//Write Byte Value
		fwrite(&chVal, sizeof(char), 1, fpw);	

		//Adjust Buffer Length
		bufLen = bufLen - 8;
	}
	//Cleanup
	for(i = 0; i < leaf_nodes; i++)
	{
		free(huffCodes[i]);
	}
	free(huffCodes);
	free(huffCode);
	fclose(fpr);
	fclose(fpw); 
}

//Main function to handle function calls
int main(int argc, char**argv)
{
	if (argc != 2)	
	{
		printf("Invalid number of arguments.\nUsage: ./huff <FileName>\n");
		return -1;
	}

	int freq[128], num_chars = 0;
	num_chars = count_freq(argv[1], freq);
	int i = 0;
	int totalFileChars = 0;
	for(i = 0; i < 128; i++)
	{
		if(freq[i] > 0)		
		{
			char c = i;
			totalFileChars += freq[i];
		}
	}
	createHuffTreeCode(freq, num_chars, totalFileChars, argv[1]);	
	return 0;
}
