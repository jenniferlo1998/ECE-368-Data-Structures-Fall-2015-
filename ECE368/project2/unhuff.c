#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// HuffTree Structure
typedef struct leaf {
	char ch;
	int freq;
	struct leaf * left;  // left sub-tree
	struct leaf * right; // right sub-tree
} HuffNode;

HuffNode * HuffNode_create(char, int);
void HuffNode_destroy(HuffNode *);
void deCompressFile(char *);
HuffNode * readHeader(FILE * , int *);

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

// Get bit at position n in number
int getBit(int number, int pos)
{
	return((number & ( 1 << pos )) >> pos);
}

//Read HuffMan header and decompress file
void deCompressFile(char * fileNameHuff)
{
	int k;
	char c;
	
	char fileNameHuffUnhuff[50];
	strcpy(fileNameHuffUnhuff, fileNameHuff);
	strcat(fileNameHuffUnhuff, ".unhuff");

	FILE * fpr = fopen(fileNameHuff, "rb");
	FILE * fpw = fopen(fileNameHuffUnhuff, "wb");
	
	if(fpr ==  NULL)
	{
		printf("Error opening file %s\n", fileNameHuff);
		return;
	}
	
	int readFlag = 1;
	HuffNode * huffTree = readHeader(fpr, &readFlag);
	HuffNode * currNode = huffTree;
	while(readFlag == 1)
	{
		fread(&c, 1, 1, fpr);
		int n = c;
		if (n < 0) n = n + 256;
		for(k = 0;k < 8; k++)
		{
			if(getBit(n, (7 - k)) == 1)	currNode = currNode->right;
			else if (currNode->left != NULL) currNode = currNode->left;

			if(currNode->left == NULL && currNode->right == NULL && currNode != NULL)
			{
				char wc = (currNode->ch);
				if (wc == 255 || wc == -1)	
				{
					readFlag = 0;
					break;
				}
				fwrite(&wc, sizeof(char), 1, fpw);
				currNode = huffTree;
			}

		}
	}

	HuffNode_destroy(huffTree);
	fclose(fpr);
	fclose(fpw);
	
}

//Read HuffMan header from file
HuffNode * readHeader(FILE * fpr, int *readFlag)
{	
  
	char ch ;
	if (*readFlag == 1)	fread(&ch, 1, 1, fpr);
	if (ch == 0)
	{
		*readFlag = 0;
		return;
	}
	if (ch == '1')
	{	
		char ch2;
		fread(&ch2, 1, 1, fpr);
		return HuffNode_create(ch2, 0);
	}
	else
	{
		HuffNode *  left = readHeader(fpr, readFlag);
		HuffNode *  right = readHeader(fpr, readFlag);
		HuffNode * newnode = HuffNode_create(0, 0);
		newnode->left = left;
		newnode->right = right;
		return newnode;
	}
}


// Main function to call Decompress Function
int main(int argc, char**argv)
{
	if (argc != 2)	
	{
		printf("Invalid number of arguments.\nUsage: ./unhuff <FileName.huff>\n");
		return -1;
	}
	deCompressFile(argv[1]);
	return 0;
}
