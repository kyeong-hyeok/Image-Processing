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

UInt8 FilterFunc(UInt8* TmpBlk, Int32 MaskSize, Int32 num)  // num은 입력받는 정수값, 반환값 존재 -> 결과값 반환 return 
{
    //////////////////////////////////////////////////////////////////
    //0~6번 필터를 이용한 결과값 출력
    Int32 max = 0;
    double result = 0.0;
    double r = 0.0;
    double c = 0.0;
                //////switch문 사용 안하고 다른 방법으로 코드 작성해도 됩니다. 6개 필터가 함수 안으로
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
            BlkTL = Tmp + y * PStride + x;//Tmp는 패딩 버퍼(영상), 블록의 왼쪽 위 지점을 포인터로 가리킴(패딩 영상에서 블록 추출을 위해 사용함) ==> BlkTL = Tmp + ???????;으로 작성되어야 함 
            //공간 잡고 특정 부분 가리킨다 페딩된 영상에서 블록 단위로 떼올것 3x3 5x5 블록의 왼쪽 위 지점이 처음주소가 되게 만들기 blktl이 언제나 왼쪽 위지점이 되게 BlkTL = Tmp + 0(첫번째 주소라면)
            // 두번째주소 BlkTL = Tmp + 1 그 다음 +4 , 0번째 공간 좌표 0 0 1번째 1 0 그다음 0 1 y*stride(3x3이면 514, 5x5이면 516?) + x , tmp + y*pstride + x일듯
            ////////////////////////////////////////////////////////////////////////////////
            ExtractBlock(TmpBlk, BlkTL, MaskSize, PStride);
            Res[y * Stride + x] = FilterFunc(TmpBlk, MaskSize, num); //필터 설계 및 적용, 코드 작성시 주석해제
        }
           
    free(TmpBlk);
}

void FilteringProcess(Int32 num, Node* head)
{
    InputImage in;         // 입력 영상 구조체
    OutputImage out;       // 출력 영상 구조체
    Int32 MaskSize = (num < 7 || num==8) ? 3 : 7;
    Int32 AddSize = (MaskSize / 2) * 2;
    Int32 TmpWid, TmpHei, TmpValidPixel;
    Int32 TotalCnt = 1;
    Int32 Cnt = 0;
    Int32 OnOffFlag = 0;
    Node* cur = head->next; //현재 노드

    Initialization(&in, &out, head->Flag, MaskSize); //영상 정보 입력 및 패딩
    
    if (head->Flag)
    {
		printf("반복 횟수?\n");
		scanf_s("%d", &TotalCnt);

        cur->cntofFilter = TotalCnt; //스무딩 횟수 저장
    }

    while (OnOffFlag != -1)
    {
        MaskSize = (num < 7 || num == 8) ? 3 : 7;
        AddSize = (MaskSize / 2) * 2;
        TmpWid = in.info.wid + AddSize;                 //패딩된 영상 가로, 세로 길이
        TmpHei = in.info.hei + AddSize;
        TmpValidPixel = TmpWid * TmpHei;

        if (head->cntofFilter == 1)
            in.Tmp = (UInt8*)calloc(TmpValidPixel, sizeof(UInt8)); //패딩된 영상
        else
            in.Tmp = realloc(in.Tmp, sizeof(UInt8) * TmpValidPixel); //패딩된 영상

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
            printf("\n\n추가로 다른 필터도 적용할까요?[Yes:1 / No: -1]\n");
            scanf_s("%d", &OnOffFlag);
				
            if (OnOffFlag != -1)
            {
                printf("1번: Embossing, 2번: Sharpening, 3번: Blurring, 4번: Frei-Chen, 5번: Difference, 6번: Median, 7번 : DoG, 8번: Gaussian\n");
                printf("어떤 필터를 사용하시겠습니까?\n");
                scanf_s("%d", &num);
                if (1 <= num && num <= 8)
                    printf("%d번 필터를 선택했습니다.\n", num);
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


