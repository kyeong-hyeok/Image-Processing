#include "main.h"

//1��: Embossing, 2��: Sharpening, 3��: Blurring, 4��: Sobel
//5��: Homogeneity, 6��: Median

void main()
{
    Node* head = NewNode();
    Node* cur = NULL;

    Int32 num = 0;
    
    while (num != -1)
    {
        printf("Lena ������ �̿��Ͻðڽ��ϱ�? Scratch-Lena������ �̿��Ͻðڽ��ϱ�?[Lena: 1 / Scratch-Lena: 2 / ����: -1]\n");
        scanf_s("%d", &num);
        head->Flag = num == 2 ? true : false;
        if (num == -1)
            goto TerminalCode;
        else if(!(num == 1 || num ==2)) //�߸��� ���� �Էµ� ���
            continue;
        system("cls");
        if (head->Flag)
        {
            printf("3��: Blurring, 6��: Median\n");
            printf("� ���͸� ����Ͻðڽ��ϱ�?[����: -1]\n");
            scanf_s("%d", &num);

            system("cls");
            printf("3��: Blurring, 6��: Median\n");
            printf("� ���͸� ����Ͻðڽ��ϱ�?[����: -1]\n");
        }

        else
        {
            printf("1��: Embossing, 2��: Sharpening, 3��: Blurring, 4��: Frei-Chen, 5��: Difference, 6��: Median, 7�� : DoG, 8��: Gaussian\n");
            printf("� ���͸� ����Ͻðڽ��ϱ�?[����: -1]\n");
            scanf_s("%d", &num);

            system("cls");
            printf("1��: Embossing, 2��: Sharpening, 3��: Blurring, 4��: Frei-Chen, 5��: Difference, 6��: Median, 7�� : DoG, 8��: Gaussian\n");
            printf("� ���͸� ����Ͻðڽ��ϱ�?[����: -1]\n");
        }
        
        if ((1 <= num && num <= 8) && !head->Flag)
            printf("%d�� ���͸� �����߽��ϴ�.\n", num);
        else if((num == 3 || num == 6) && head->Flag)
            printf("%d�� ���͸� �����߽��ϴ�.\n", num);
        else if(num != -1) //�߸��� ���� �Էµ� ���
            continue;
        else
        {
        TerminalCode:
            printf("�����մϴ�.\n");
        }
            
        if (num != -1)
        {
            head->next = NewNode();
            head->cntofFilter++;
            cur = head->next;
            cur->num = num;

            FilteringProcess(num, head);
        }
            
    }
    free(head);
}