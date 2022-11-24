#include "List.h"

void IDCT_Func(Dou64* CurBlk, Int32 BlkSize, Int32 BlkRow, Int32 BlkCol, Int32 Stride, Image* Tmp)
{
	Dou64 Data;
	Dou64 PI = pi;

	Dou64 Recon_R;

	for (Int32 i = 0; i < BlkSize; i++)
	{
		for (Int32 j = 0; j < BlkSize; j++)
		{
			Recon_R = 0;
			Data = 0;
			for (Int32 k = 0; k < BlkSize; k++)
			{
				for (Int32 l = 0; l < BlkSize; l++)
				{
					Data += CurBlk[k * BlkSize + l]
						* cos((2 * k + 1) * i * PI / (2 * BlkSize))
						* cos((2 * l + 1) * j * PI / (2 * BlkSize));

				}
			}
			if (j == 0 && i == 0)
				Recon_R = (Data / BlkSize);
			else if (j == 0 || i == 0)
				Recon_R = Data * (1.0 / sqrt(2.0)) / (BlkSize / 2);
			else
				Recon_R = Data / (BlkSize / 2);

			if (Recon_R < 0)
				Recon_R = (int)(Recon_R - 0.5);
			else
				Recon_R = (int)(Recon_R + 0.5);

			Tmp->Data[(BlkRow * BlkSize + i) * Stride + (BlkCol * BlkSize + j)] = Clip3(Recon_R);
		}
	}
}

void IDCTProcess(Image* Tmp, Image* DCT_Val, Int32 BlkSize)
{
	Dou64* CurBlk; //현재 블록

	Queue queue; // 입력영상을 블록단위로 1차원 정렬함

	Int32 Stride, wid, hei;
	wid = Stride = Tmp->info.wid;
	hei = Tmp->info.hei;

	InputImageSorting(DCT_Val->Data64, wid, hei, BlkSize, &queue);

	//IDCT
	CurBlk = (Dou64*)calloc(BlkSize * BlkSize, sizeof(Dou64));
	for (Int32 BlkRow = 0; BlkRow < hei / BlkSize; BlkRow++)
		for (Int32 BlkCol = 0; BlkCol < wid / BlkSize; BlkCol++)
		{
			memset(CurBlk, 0, sizeof(Dou64) * BlkSize * BlkSize);

			ExtractBlock(&queue, CurBlk, BlkSize);


			IDCT_Func(CurBlk, BlkSize, BlkRow, BlkCol, Stride, Tmp);
		}

	free(CurBlk);
}