#include "main.h"

void ImageOutput(UInt8* Data, Int32 DstPixNum, Int8 String[])
{
    UInt8 Name_extension[10] = ".raw";
    FILE* fp;

    strcat(String, Name_extension);

    fopen_s(&fp, String, "wb"); //���� ���� ����
    fwrite(Data, sizeof(UInt8), DstPixNum, fp);
    fclose(fp);
}

void Output(OutputImage* out, Int32 num, Node* head)
{
    UInt8 String[8][10] = { "Embo", "Sharp" , "Blur" ,"FrCh" ,"Diff", "Median", "DoG", "Gaussian"};
    UInt8 Name_Hist[50] = "Img_";
    UInt8 Cnt_Hist[10];
    Node* cur = head->next;

    printf("\n\n\n���� ���� ");

    for (Int32 i = 0; i < (head->Flag ? 1 : head->cntofFilter); i++)
    {
        strcat(Name_Hist, String[cur->num - 1]);
     
        if (head->Flag)
        {
            if (cur->num == 3)
                printf("Blurring ���͸� ");
            else
                printf("Median ���͸� ");
            printf("%dȸ �����Ͽ����ϴ�.\n\n\n\n", cur->cntofFilter);
            itoa(cur->cntofFilter,Cnt_Hist,10);
            strcat(Name_Hist, Cnt_Hist);
        }
            
        else
        {
            if (cur->num == 1)
                printf("Embossing ���͸� ");
            else if (cur->num == 2)
                printf("Sharpening ���͸� ");
            else if (cur->num == 3)
                printf("Blurring ���͸� ");
            else if (cur->num == 4)
                printf("Frei-Chen ���͸� ");
            else if (cur->num == 5)
                printf("Difference ���͸� ");
            else if (cur->num == 6)
                printf("Median ���͸� ");
            else if (cur->num == 7)
                printf("DoG ���͸� ");
            else if (cur->num == 8)
                printf("Gaussian ���͸� ");
            if (head->cntofFilter == 1 || head->cntofFilter - 1 == i)
                printf("�����Ͽ����ϴ�\n\n\n\n");
            else 
                printf("���� ��, ");
        }
        cur = cur->next;
    }
    ImageOutput(out->Res, out->info.ValidPixel, Name_Hist);
}