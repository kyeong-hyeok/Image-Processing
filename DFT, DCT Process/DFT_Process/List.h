#pragma once
#include "main.h"

typedef struct Node
{
	Dou64 data;
	struct Node* next;
}Node;

typedef struct Queue
{
	Node* front;
	Node* rear;
	Int32 count; // 큐 안의 노드 개수  
}Queue;

void InputImageSorting(Dou64* Data, Int32 Wid, Int32 Hei, Int32 BlkSize, Queue* queue);
void ExtractBlock(Queue* queue, Dou64* CurBlk, Int32 BlkSize);