#include "List.h"

void IDFT_Func(Dou64* CurBlkMag, Dou64* CurBlkPha, Int32 BlkSize, Int32 BlkRow, Int32 BlkCol, Int32 Stride, Image* Tmp)
{
	Complex* Data;
	Dou64 PI = pi;

	Dou64 Recon_R;

	Data = (Complex*)calloc(BlkSize * BlkSize, sizeof(Complex));

#if CalcType
	Int32 count = 0;
	Dou64* CosTable, * SinTable;
	FILE* fp, * op;

	CosTable = (Dou64*)calloc(WIDTH * HEIGHT * 2, sizeof(Dou64));
	SinTable = (Dou64*)calloc(WIDTH * HEIGHT * 2, sizeof(Dou64));


	fopen_s(&fp, "cosTable.txt", "rb");
	fopen_s(&op, "sinTable.txt", "rb");

	fread(CosTable, sizeof(double), WIDTH * HEIGHT * 2, fp);
	fread(SinTable, sizeof(double), WIDTH * HEIGHT * 2, op);

	fclose(fp);
	fclose(op);
#endif

	for (Int32 i = 0; i < BlkSize; i++)
	{
		for (Int32 j = 0; j < BlkSize; j++)
		{
			Data[i * BlkSize + j].Re = CurBlkMag[i * BlkSize + j] * cos(CurBlkPha[i * BlkSize + j]);
			Data[i * BlkSize + j].Im = CurBlkMag[i * BlkSize + j] * sin(CurBlkPha[i * BlkSize + j]);
		}
	}

	for (Int32 i = 0; i < BlkSize; i++)
	{
		for (Int32 j = 0; j < BlkSize; j++)
		{
			Recon_R = 0;
			for (Int32 k = 0; k < BlkSize; k++)
			{
				for (Int32 l = 0; l < BlkSize; l++)
				{
					
#if CalcType 
					Recon_R += Data[k * BlkSize + l].Re * CosTable[j * l + i * k] -
						Data[k * BlkSize + l].Im * SinTable[j * l + i * k];
#else
					Recon_R += Data[k * BlkSize + l].Re * cos(2.0 * PI / (double)BlkSize * (j * l + i * k)) -
						Data[k * BlkSize + l].Im * sin(2.0 * PI / (double)BlkSize * (j * l + i * k));
#endif
				}
			}

			if (Recon_R < 0)
				Recon_R = (int)(Recon_R - 0.5);

			else
				Recon_R = (int)(Recon_R + 0.5);

			Tmp->Data[(BlkRow * BlkSize + i) * Stride + (BlkCol * BlkSize + j)] = Clip3(Recon_R);
		}
	}

	free(Data);

#if CalcType
	free(CosTable);
	free(SinTable);
#endif
}

void IDFTProcess(Image* Tmp, Image* Magnitude, Image* Phase, Int32 BlkSize)
{
	Dou64* CurBlkMag, *CurBlkPha; //현재 블록

	Queue queueMag, queuePha; // 입력영상을 블록단위로 1차원 정렬함

	Int32 Stride, wid, hei;
	wid = Stride = Tmp->info.wid;
	hei = Tmp->info.hei;

	InputImageSorting(Magnitude->Data64, wid, hei, BlkSize, &queueMag);
	InputImageSorting(Phase->Data64,     wid, hei, BlkSize, &queuePha);

	//IDFT
	CurBlkMag = (Dou64*)calloc(BlkSize * BlkSize, sizeof(Dou64));
	CurBlkPha = (Dou64*)calloc(BlkSize * BlkSize, sizeof(Dou64));
	for (Int32 BlkRow = 0; BlkRow < hei / BlkSize; BlkRow++)
		for (Int32 BlkCol = 0; BlkCol < wid / BlkSize; BlkCol++)
		{
			memset(CurBlkMag, 0, sizeof(Dou64) * BlkSize * BlkSize);
			memset(CurBlkPha, 0, sizeof(Dou64) * BlkSize * BlkSize);

			ExtractBlock(&queueMag, CurBlkMag, BlkSize);
			ExtractBlock(&queuePha, CurBlkPha, BlkSize);

			IDFT_Func(CurBlkMag, CurBlkPha, BlkSize, BlkRow, BlkCol, Stride, Tmp);
		}

	free(CurBlkMag);
	free(CurBlkPha);
}