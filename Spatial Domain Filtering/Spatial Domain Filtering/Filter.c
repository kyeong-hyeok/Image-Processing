#include "main.h"
#include "Filter.h"

double MaskCoeff[6][49] =
{
    {-1,  0,  0,  0,  0,  0,  0,  0,  1}, //Embossing
    {-1, -1, -1, -1,  9, -1, -1, -1, -1}, //Sharpening
    {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}, //blurring
    {1, 0, -1, 2, 0, -2, 1, 0, -1, -1, -2, -1, 0, 0, 0, 1, 2, 1}, //Sobel        
    {0,0,-1,-1,-1,0,0,
    0,-2,-3,-3,-3,-2,0,
    -1,-3,5,5,5,-3,-1,
    -1,-3,5,16,5,-3,-1,
    -1,-3,5,5,5,-3,-1,
    0,-2,-3,-3,-3,-2,0,
    0,0,-1,-1,-1,0,0},   //DoG
    {1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0, 2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0}    
};

UInt8 FilterFunc(UInt8* TmpBlk, Int32 MaskSize, Int32 num)  // num�� �Է¹޴� ������, ��ȯ�� ���� -> ����� ��ȯ return 
{
    //////////////////////////////////////////////////////////////////
    //0~6�� ���͸� �̿��� ����� ���
    Int32 max = 0;
    double result = 0.0;
    double r = 0.0;
    double c = 0.0;
                //////switch�� ��� ���ϰ� �ٸ� ������� �ڵ� �ۼ��ص� �˴ϴ�. 6�� ���Ͱ� �Լ� ������
    switch (num)
    {
    case 1: //Embossing
        for (Int32 y = 0; y < MaskSize; y++)
            for (Int32 x = 0; x < MaskSize; x++)
                result+= TmpBlk[y * MaskSize + x] * MaskCoeff[0][y * MaskSize + x];
        result = Clip3(result);
        result += 128;
        break;
    case 2: //Sharpening
        for (Int32 y = 0; y < MaskSize; y++)
            for (Int32 x = 0; x < MaskSize; x++)
                result += TmpBlk[y * MaskSize + x] * MaskCoeff[1][y * MaskSize + x];
        break;
    case 3: //Blurring
        for (Int32 y = 0; y < MaskSize; y++)
            for (Int32 x = 0; x < MaskSize; x++)
                result += TmpBlk[y * MaskSize + x] * MaskCoeff[2][y * MaskSize + x];
        break;
    case 4: //Frei_Chen
        for (Int32 y = 0; y < MaskSize; y++) {
            for (Int32 x = 0; x < MaskSize; x++) {
                if (MaskCoeff[3][y * MaskSize + x] == 2 || MaskCoeff[3][y * MaskSize + x] == -2)
                    r += TmpBlk[y * MaskSize + x] * MaskCoeff[3][y * MaskSize + x] / sqrt(2);
                else
                    r += TmpBlk[y * MaskSize + x] * MaskCoeff[3][y * MaskSize + x];
            }
        }
        for (Int32 y = 0; y < MaskSize; y++) {
            for (Int32 x = 0; x < MaskSize; x++) {
                if (MaskCoeff[3][y * MaskSize + x] == 2 || MaskCoeff[3][y * MaskSize + x] == -2)
                    c += TmpBlk[y * MaskSize + x] * MaskCoeff[3][(y + MaskSize) * MaskSize + x] / sqrt(2);
                else
                    c += TmpBlk[y * MaskSize + x] * MaskCoeff[3][(y + MaskSize) * MaskSize + x];
            }
        }
        r = Clip3(r);
        c = Clip3(c);
        result = r + c;
        break;
    case 5: //Difference
        for (Int32 i = MaskSize * MaskSize - 1; i >= MaskSize * (MaskSize-1); i--)
            max = Max(max, abs(TmpBlk[i] - TmpBlk[MaskSize * MaskSize - 1 - i]));
        for(Int32 i = MaskSize; i< MaskSize*(MaskSize-1); i+=MaskSize)
            max = Max(max, abs(TmpBlk[i] - TmpBlk[MaskSize * MaskSize -1 - i]));
        result = Clip3(max);
        result += 60;
        break;
    case 6: //Median
        for (Int32 i = 0; i < MaskSize * MaskSize - 1; i++) {
            for (Int32 j = 0; j < MaskSize * MaskSize - 1 - i; j++) {
                if (TmpBlk[j] > TmpBlk[j + 1])
                    SWAP(TmpBlk[j], TmpBlk[j + 1]);
            }
        }
        result = TmpBlk[MaskSize * MaskSize - 2];
        break;
    case 7: //DoG
        for (Int32 y = 0; y < MaskSize; y++)
            for (Int32 x = 0; x < MaskSize; x++)
                result += TmpBlk[y * MaskSize + x] * MaskCoeff[4][y * MaskSize + x];
        break;
    case 8: //Gaussian
        for (Int32 y = 0; y < MaskSize; y++)
            for (Int32 x = 0; x < MaskSize; x++)
                result += TmpBlk[y * MaskSize + x] * MaskCoeff[5][y * MaskSize + x];
        break;
    }
    
   
    return (UInt8)Clip3((Int32)(result+0.5));
}

void ExtractBlock(UInt8* TmpBlk, UInt8* BlkTL, Int32 MaskSize, Int32 PStride)
{
    for (Int32 y = 0; y < MaskSize; y++)
        for (Int32 x = 0; x < MaskSize; x++)
            TmpBlk[y * MaskSize + x] = BlkTL[y * PStride + x];
}

void ImageFiltering(UInt8* Tmp, UInt8* Res, Int32 num, Int32 wid, Int32 hei, Int32 ValidPixel)
{
    Int32 MaskSize = (num < 7 || num == 8) ? 3 : 7;
    Int32 AddSize = (MaskSize / 2) * 2;
    Int32 Stride = wid;
    Int32 PStride = wid + AddSize;
    UInt8* BlkTL;
    UInt8* TmpBlk = (UInt8*)calloc(MaskSize * MaskSize, sizeof(UInt8));

    memset(Res, 0, sizeof(UInt8) * (ValidPixel));
    for (Int32 y = 0; y < hei; y++)
        for (Int32 x = 0; x < wid; x++)
        {
            memset(TmpBlk, 0, sizeof(UInt8) * (MaskSize * MaskSize));
            ////////////////////////////////////////////////////////////////////////////////
            BlkTL = Tmp + y * PStride + x;//Tmp�� �е� ����(����), ����� ���� �� ������ �����ͷ� ����Ŵ(�е� ���󿡼� ��� ������ ���� �����) ==> BlkTL = Tmp + ???????;���� �ۼ��Ǿ�� �� 
            //���� ��� Ư�� �κ� ����Ų�� ����� ���󿡼� ��� ������ ���ð� 3x3 5x5 ����� ���� �� ������ ó���ּҰ� �ǰ� ����� blktl�� ������ ���� �������� �ǰ� BlkTL = Tmp + 0(ù��° �ּҶ��)
            // �ι�°�ּ� BlkTL = Tmp + 1 �� ���� +4 , 0��° ���� ��ǥ 0 0 1��° 1 0 �״��� 0 1 y*stride(3x3�̸� 514, 5x5�̸� 516?) + x , tmp + y*pstride + x�ϵ�
            ////////////////////////////////////////////////////////////////////////////////
            ExtractBlock(TmpBlk, BlkTL, MaskSize, PStride);
            Res[y * Stride + x] = FilterFunc(TmpBlk, MaskSize, num); //���� ���� �� ����, �ڵ� �ۼ��� �ּ�����
        }
           
    free(TmpBlk);
}

void FilteringProcess(Int32 num, Node* head)
{
    InputImage in;         // �Է� ���� ����ü
    OutputImage out;       // ��� ���� ����ü
    Int32 MaskSize = (num < 7 || num==8) ? 3 : 7;
    Int32 AddSize = (MaskSize / 2) * 2;
    Int32 TmpWid, TmpHei, TmpValidPixel;
    Int32 TotalCnt = 1;
    Int32 Cnt = 0;
    Int32 OnOffFlag = 0;
    Node* cur = head->next; //���� ���

    Initialization(&in, &out, head->Flag, MaskSize); //���� ���� �Է� �� �е�
    
    if (head->Flag)
    {
		printf("�ݺ� Ƚ��?\n");
		scanf_s("%d", &TotalCnt);

        cur->cntofFilter = TotalCnt; //������ Ƚ�� ����
    }

    while (OnOffFlag != -1)
    {
        MaskSize = (num < 7 || num == 8) ? 3 : 7;
        AddSize = (MaskSize / 2) * 2;
        TmpWid = in.info.wid + AddSize;                 //�е��� ���� ����, ���� ����
        TmpHei = in.info.hei + AddSize;
        TmpValidPixel = TmpWid * TmpHei;

        if (head->cntofFilter == 1)
            in.Tmp = (UInt8*)calloc(TmpValidPixel, sizeof(UInt8)); //�е��� ����
        else
            in.Tmp = realloc(in.Tmp, sizeof(UInt8) * TmpValidPixel); //�е��� ����

		do
		{
			ImagePadding(Cnt == 0 ? in.Ori : out.Res, in.Tmp, in.info.wid, in.info.hei, MaskSize);
			ImageFiltering(in.Tmp, out.Res, num, in.info.wid, in.info.hei, in.info.ValidPixel);
			Cnt++;
		} while (Cnt < TotalCnt);

        if (head->Flag)
            break;
        else
        {
            printf("\n\n�߰��� �ٸ� ���͵� �����ұ��?[Yes:1 / No: -1]\n");
            scanf_s("%d", &OnOffFlag);
				
            if (OnOffFlag != -1)
            {
                printf("1��: Embossing, 2��: Sharpening, 3��: Blurring, 4��: Frei-Chen, 5��: Difference, 6��: Median, 7�� : DoG, 8��: Gaussian\n");
                printf("� ���͸� ����Ͻðڽ��ϱ�?\n");
                scanf_s("%d", &num);
                if (1 <= num && num <= 8)
                    printf("%d�� ���͸� �����߽��ϴ�.\n", num);
                else
                    goto OutputCode;
                head->cntofFilter++;
                cur->next = NewNode();
                cur = cur->next;
                cur->num = num;
            }
        }
    }
    
OutputCode:
    Output(&out, num, head);

#if PrintOut
    FILE* fp;
    AddSize = (MaskSize / 2) * 2;
    fopen_s(&fp, "text.txt", "w+");

    for (Int32 i = 0; i < in.info.hei; i++)
    {
        for (Int32 j = 0; j < in.info.wid; j++)
            fprintf(fp, "%d\t", in.Ori[i * in.info.wid + j]);
        fprintf(fp, "\n");
    }
    fprintf(fp, "\n\n\n");

    for (Int32 i = 0; i < out.info.hei; i++)
    {
        for (Int32 j = 0; j < out.info.wid; j++)
            fprintf(fp, "%d\t", out.Res[i * out.info.wid + j]);
        fprintf(fp, "\n");
    }

    fclose(fp);
#endif

    Destroy(&in, &out, head);
}


