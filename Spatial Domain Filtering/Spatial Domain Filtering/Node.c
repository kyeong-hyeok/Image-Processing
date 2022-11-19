#include "main.h"

Node* NewNode() //새 노드 생성시 값을 저장
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
        for (Int32 i = 0; i < head->cntofFilter - 1; i++)//삭제할 노드의 위치를 알려줍니다
        {
            cur = cur->next;
        }
        free(cur);

        head->cntofFilter--; //삭제된 노드가 있이므로 총 노드의 갯수를 줄여줍니다.
    } while (head->cntofFilter);
    
}