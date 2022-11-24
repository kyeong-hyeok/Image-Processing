#include "List.h"
void PSNR(Image* Ori, Image* Res)
{
	Int32 i, j;
	Int32 wid = WIDTH; Int32 hei = HEIGHT;
	Dou64 mse = 0, psnr = 0, max = 0;
	UInt8 Img1 = 0, Img2 = 0;

	for (i = 0; i < hei; i++)
	{
		for (j = 0; j < wid; j++)
		{
			Img1 = Ori->Data[i * wid + j];
			Img2 = Res->Data[i * wid + j];

			mse += ((Img1 - Img2) * (Img1 - Img2));
			if (Img1 > max)
				max = Img1;
		}
	}

	mse = mse / (wid * hei);
	printf("MSE : %lf\n", mse);
	psnr = 20 * log10(max / sqrt(mse));
	printf("PSNR : %lf\n", psnr);

	if (mse == 0)
		printf("\n영상 일치\n");
	else
		printf("\n영상 불일치\n");
}

void main()
{
	//시간 측정
	clock_t start, end;
	float res;

	Image Ori, Tmp, Res;        // 영상 구조체, Ori:원본 영상 저장, Tmp:패딩 영상 저장, Res: 결과영상 저장
	Image DCT_Val;     // 블록단위로 구한 Magnitude와 Phase를 픽쳐에 저장 
	Int32 BlkSize = BlockSize;

	Initialization(&Ori, &Tmp, &Res, BlkSize);                                   // 원본영상 읽어오기 및 동적할당

	start = clock();
	DCTProcess(&Tmp, &DCT_Val, BlkSize);
	memset(Tmp.Data, 0, sizeof(UInt8) * Tmp.info.ValidPixel);
	IDCTProcess(&Tmp, &DCT_Val, BlkSize);
	end = clock();

	Output(&Res, &Tmp, BlkSize);



	PSNR(&Ori, &Res);

	res = (float)(end - start) / CLOCKS_PER_SEC;

	printf("\nDCT 소요된 시간 : %.3f \n", res);
	printf("start = %d \n", start);
	printf("end   = %d \n", end);

	printf("\n\n\n");





	Destroy(&Ori, &Tmp, &Res, &DCT_Val);                                          // 모든 동적 할당 해제
}