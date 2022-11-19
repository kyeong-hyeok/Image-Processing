#include "main.h"

void ImageOutput(UInt8* Data, Int32 DstPixNum, Int8 String[])
{
    UInt8 Name_extension[10] = ".raw";
    FILE* fp;

    strcat(String, Name_extension);

    fopen_s(&fp, String, "wb"); //원본 영상 열기
    fwrite(Data, sizeof(UInt8), DstPixNum, fp);
    fclose(fp);
}

void Output(OutputImage* out, Int32 num, Node* head)
{
    UInt8 String[8][10] = { "Embo", "Sharp" , "Blur" ,"FrCh" ,"Diff", "Median", "DoG", "Gaussian"};
    UInt8 Name_Hist[50] = "Img_";
    UInt8 Cnt_Hist[10];
    Node* cur = head->next;

    printf("\n\n\n원본 영상에 ");

    for (Int32 i = 0; i < (head->Flag ? 1 : head->cntofFilter); i++)
    {
        strcat(Name_Hist, String[cur->num - 1]);
     
        if (head->Flag)
        {
            if (cur->num == 3)
                printf("Blurring 필터를 ");
            else
                printf("Median 필터를 ");
            printf("%d회 적용하였습니다.\n\n\n\n", cur->cntofFilter);
            itoa(cur->cntofFilter,Cnt_Hist,10);
            strcat(Name_Hist, Cnt_Hist);
        }
            
        else
        {
            if (cur->num == 1)
                printf("Embossing 필터를 ");
            else if (cur->num == 2)
                printf("Sharpening 필터를 ");
            else if (cur->num == 3)
                printf("Blurring 필터를 ");
            else if (cur->num == 4)
                printf("Frei-Chen 필터를 ");
            else if (cur->num == 5)
                printf("Difference 필터를 ");
            else if (cur->num == 6)
                printf("Median 필터를 ");
            else if (cur->num == 7)
                printf("DoG 필터를 ");
            else if (cur->num == 8)
                printf("Gaussian 필터를 ");
            if (head->cntofFilter == 1 || head->cntofFilter - 1 == i)
                printf("적용하였습니다\n\n\n\n");
            else 
                printf("적용 후, ");
        }
        cur = cur->next;
    }
    ImageOutput(out->Res, out->info.ValidPixel, Name_Hist);
}