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
		printf("\n���� ��ġ\n");
	else
		printf("\n���� ����ġ\n");
}

void main()
{
	//�ð� ����
	clock_t start, end;
	float res;

	Image Ori, Tmp, Res;        // ���� ����ü, Ori:���� ���� ����, Tmp:�е� ���� ����, Res: ������� ����
	Image DCT_Val;     // ��ϴ����� ���� Magnitude�� Phase�� ���Ŀ� ���� 
	Int32 BlkSize = BlockSize;

	Initialization(&Ori, &Tmp, &Res, BlkSize);                                   // �������� �о���� �� �����Ҵ�

	start = clock();
	DCTProcess(&Tmp, &DCT_Val, BlkSize);
	memset(Tmp.Data, 0, sizeof(UInt8) * Tmp.info.ValidPixel);
	IDCTProcess(&Tmp, &DCT_Val, BlkSize);
	end = clock();

	Output(&Res, &Tmp, BlkSize);



	PSNR(&Ori, &Res);

	res = (float)(end - start) / CLOCKS_PER_SEC;

	printf("\nDCT �ҿ�� �ð� : %.3f \n", res);
	printf("start = %d \n", start);
	printf("end   = %d \n", end);

	printf("\n\n\n");





	Destroy(&Ori, &Tmp, &Res, &DCT_Val);                                          // ��� ���� �Ҵ� ����
}