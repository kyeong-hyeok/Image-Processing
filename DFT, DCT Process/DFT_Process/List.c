#include "List.h"

void initQueue(Queue* queue)
{
    queue->front = queue->rear = NULL;
    queue->count = 0;    // ť ���� ��� ������ 0���� ����
}

Int32 isEmpty(Queue* queue)
{
    return queue->count == 0;    // ť���� ��� ������ 0�̸� �� ����
}

void enqueue(Queue* queue, Dou64 data)
{
    Node* newNode = (Node*)malloc(sizeof(Node)); // newNode ����
    newNode->data = data;
    newNode->next = NULL;

    if (isEmpty(queue))    // ť�� ������� ��
    {
        queue->front = newNode;
    }
    else    // ������� ���� ��
    {
        queue->rear->next = newNode;    //�� ���� ������ newNode�� ����
    }
    queue->rear = newNode;    //�� �ڸ� newNode�� ����   
    queue->count++;    //ť���� ��� ������ 1 ����
}

Dou64 dequeue(Queue* queue)
{
    Dou64 data;
    Node* ptr;
    if (isEmpty(queue))    //ť�� ����� ��
    {
        printf("Error : Queue is empty!\n");
        return 0;
    }
    ptr = queue->front;    //�� ���� ��� ptr ���� 
    data = ptr->data;    // return �� ������  
    queue->front = ptr->next;    //�� ���� ptr�� ���� ���� ����
    free(ptr);    // ptr ���� 
    queue->count--;    //ť�� ��� ������ 1 ����

    return data;
}

void InputImageSorting(Dou64* Data, Int32 Wid, Int32 Hei, Int32 BlkSize, Queue* queue)
{
    Int32 Stride = Wid;

    initQueue(queue);

    for (Int32 BlkRow = 0; BlkRow < Hei / BlkSize; BlkRow++)
        for (Int32 BlkCol = 0; BlkCol < Wid / BlkSize; BlkCol++)
            for (Int32 y = 0; y < BlkSize; y++)
                for (Int32 x = 0; x < BlkSize; x++)
                    enqueue(queue, Data[(BlkRow * BlkSize + y) * Stride + (BlkCol * BlkSize + x)]);
        
}

void ExtractBlock(Queue* queue, Dou64* CurBlk, Int32 BlkSize)
{
    for (Int32 i = 0; i < BlkSize * BlkSize; i++)
        CurBlk[i] = dequeue(queue);
}