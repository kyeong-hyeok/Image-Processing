#include "main.h"

Node* NewNode() //�� ��� ������ ���� ����
{
    Node* New = NULL;
    New = (Node*)malloc(sizeof(Node));

    New->num = 0;
    New->cntofFilter = 0;
    New->Flag = false;

    return (New);
}

void DeleteNode(Node* head)
{
    Node* cur;

    do 
    {
        cur = head->next;
        for (Int32 i = 0; i < head->cntofFilter - 1; i++)//������ ����� ��ġ�� �˷��ݴϴ�
        {
            cur = cur->next;
        }
        free(cur);

        head->cntofFilter--; //������ ��尡 ���̹Ƿ� �� ����� ������ �ٿ��ݴϴ�.
    } while (head->cntofFilter);
    
}