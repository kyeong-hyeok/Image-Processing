#include "List.h"

void DCT_Func(Dou64* CurBlk, Int32 BlkSize, Int32 BlkRow, Int32 BlkCol, Int32 Stride, Image* DCT_Val)
{
	Dou64* Data;
	Dou64 PI = pi;

	Data = (Dou64*)calloc(BlkSize * BlkSize, sizeof(Dou64));

	Dou64 temp, Recon_R;

	for (Int32 i = 0; i < BlkSize; i++)
	{
		for (Int32 j = 0; j < BlkSize; j++)
		{
			temp = Recon_R = 0;
			for (Int32 k = 0; k < BlkSize; k++)
			{
				for (Int32 l = 0; l < BlkSize; l++)
				{
					temp = CurBlk[k * BlkSize + l]
						* cos((2 * i + 1) * k * PI / (2 * BlkSize))
						* cos((2 * j + 1) * l * PI / (2 * BlkSize));

					if (l == 0 && k == 0)
						temp *= 0.5;
					else if (l == 0 || k == 0)
						temp *= (1.0 / sqrt(2.0));

					Recon_R += temp;
				}
			}
			Data[i * BlkSize + j] = Recon_R * 2.0 / BlkSize;
		}
	}

	for (Int32 y = 0; y < BlkSize; y++)      //블록을 원래 있던 픽쳐 위치에 저장
		for (Int32 x = 0; x < BlkSize; x++)
			DCT_Val->Data64[(BlkRow * BlkSize + y) * Stride + (BlkCol * BlkSize + x)] = Data[y * BlkSize + x];

	free(Data);
}

void DCTProcess(Image* Tmp, Image* DCT_Val, Int32 BlkSize)
{
	Dou64* CurBlk; //현재 블록

	Queue queue; // 입력영상을 블록단위로 1차원 정렬함

	Int32 Stride, wid, hei;
	wid = Stride = Tmp->info.wid;
	hei = Tmp->info.hei;

	//DCT 결과를 픽쳐 단위로 저장
	DCT_Val->Data64 = (Dou64*)calloc(Tmp->info.ValidPixel, sizeof(Dou64));
	Tmp->Data64 = (Dou64*)calloc(Tmp->info.ValidPixel, sizeof(Dou64));
	for (Int32 i = 0; i < Tmp->info.ValidPixel; i++)
		Tmp->Data64[i] = Tmp->Data[i];

	InputImageSorting(Tmp->Data64, wid, hei, BlkSize, &queue);

	//DCT
	CurBlk = (Dou64*)calloc(BlkSize * BlkSize, sizeof(Dou64));
	for (Int32 BlkRow = 0; BlkRow < hei / BlkSize; BlkRow++)    //블록의 왼쪽 위 좌표를 찾기 위한 반복문
		for (Int32 BlkCol = 0; BlkCol < wid / BlkSize; BlkCol++)//블록의 왼쪽 위 좌표를 찾기 위한 반복문
		{
			memset(CurBlk, 0, sizeof(Dou64) * (BlkSize * BlkSize));
			ExtractBlock(&queue, CurBlk, BlkSize);

			DCT_Func(CurBlk, BlkSize, BlkRow, BlkCol, Stride, DCT_Val); //curBlk: 현재 블록(blkSize x blkSize)
		}
	free(CurBlk);




	
		UInt8* spectrum;

		FILE* cp;
		fopen_s(&cp, "DCT_Spectrum.raw", "wb");

		spectrum = (UInt8*)calloc(wid*hei, sizeof(UInt8));
		for (Int32 i = 0; i < wid; i++)
		{
			for (Int32 j = 0; j < hei; j++)
			{
				spectrum[i * wid + j] = Clip3(DCT_Val->Data64[i * wid + j]);
			}
		}

		fwrite(spectrum, sizeof(UInt8), wid * hei, cp);

		free(spectrum);
		fclose(cp);
	

}

