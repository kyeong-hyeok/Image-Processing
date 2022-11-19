#include "main.h"

//1번: Embossing, 2번: Sharpening, 3번: Blurring, 4번: Sobel
//5번: Homogeneity, 6번: Median

void main()
{
    Node* head = NewNode();
    Node* cur = NULL;

    Int32 num = 0;
    
    while (num != -1)
    {
        printf("Lena 영상을 이용하시겠습니까? Scratch-Lena영상을 이용하시겠습니까?[Lena: 1 / Scratch-Lena: 2 / 종료: -1]\n");
        scanf_s("%d", &num);
        head->Flag = num == 2 ? true : false;
        if (num == -1)
            goto TerminalCode;
        else if(!(num == 1 || num ==2)) //잘못된 숫자 입력된 경우
            continue;
        system("cls");
        if (head->Flag)
        {
            printf("3번: Blurring, 6번: Median\n");
            printf("어떤 필터를 사용하시겠습니까?[종료: -1]\n");
            scanf_s("%d", &num);

            system("cls");
            printf("3번: Blurring, 6번: Median\n");
            printf("어떤 필터링 사용하시겠습니까?[종료: -1]\n");
        }

        else
        {
            printf("1번: Embossing, 2번: Sharpening, 3번: Blurring, 4번: Frei-Chen, 5번: Difference, 6번: Median, 7번 : DoG, 8번: Gaussian\n");
            printf("어떤 필터를 사용하시겠습니까?[종료: -1]\n");
            scanf_s("%d", &num);

            system("cls");
            printf("1번: Embossing, 2번: Sharpening, 3번: Blurring, 4번: Frei-Chen, 5번: Difference, 6번: Median, 7번 : DoG, 8번: Gaussian\n");
            printf("어떤 필터링 사용하시겠습니까?[종료: -1]\n");
        }
        
        if ((1 <= num && num <= 8) && !head->Flag)
            printf("%d번 필터를 선택했습니다.\n", num);
        else if((num == 3 || num == 6) && head->Flag)
            printf("%d번 필터를 선택했습니다.\n", num);
        else if(num != -1) //잘못된 숫자 입력된 경우
            continue;
        else
        {
        TerminalCode:
            printf("종료합니다.\n");
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