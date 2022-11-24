#include "List.h"
#if LPF_Flag
#define D0				32.0 // Cut off frequency 
#define N0				4.0  // Filter dimension
void LPF(Complex* Data, Int32 wid, Int32 hei)
{
	// 버터워스 LPF 구현 

	Int32 x, y;
	Int32 tempx, tempy;
	Int32 halfcols, halfrows;
	Dou64 butterworth, coordinate;
	halfcols = hei / 2;
	halfrows = wid / 2;

	for (y = 0; y < hei; y++)
	{
		if (y >= halfcols)
			tempy = y - halfcols;
		else
			tempy = y + halfcols;

		for (x = 0; x < wid; x++)
		{
			if (x >= halfrows)
				tempx = x - halfrows;
			else
				tempx = x + halfrows;

			coordinate = sqrt(pow((double)(tempx - halfcols), 2.0) + pow((double)(tempy - halfrows), 2.0));
			butterworth = 1.0 / (1.0 + pow(coordinate / D0, 2 * N0));


			Data[y * wid + x].Re *= butterworth;
			Data[y * wid + x].Im *= butterworth;
		}
	}
}
#endif

void DFT_Func(Dou64* CurBlk, Int32 BlkSize, Int32 BlkRow, Int32 BlkCol, Int32 Stride, Image* Magnitude, Image* Phase)
{
	Complex *Data;
	Dou64 PI = pi;

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

	Data = (Complex*)calloc(BlkSize * BlkSize, sizeof(Complex));

	for (Int32 i = 0; i < BlkSize; i++)
	{
		for (Int32 j = 0; j < BlkSize; j++)
		{
			for (Int32 k = 0; k < BlkSize; k++)
			{
				for (Int32 l = 0; l < BlkSize; l++)
				{
#if CalcType 
					Data[i * BlkSize + j].Re += CurBlk[k * BlkSize + l] * CosTable[j * l + i * k];
					Data[i * BlkSize + j].Im -= CurBlk[k * BlkSize + l] * SinTable[j * l + i * k];
#else
					Data[i * BlkSize + j].Re += CurBlk[k * BlkSize + l] * cos(2.0 * PI * (j * l + i * k) / (double)BlkSize);
					Data[i * BlkSize + j].Im -= CurBlk[k * BlkSize + l] * sin(2.0 * PI * (j * l + i * k) / (double)BlkSize);
#endif
					
				}
			}
			Data[i * BlkSize + j].Re /= (BlkSize * BlkSize);
			Data[i * BlkSize + j].Im /= (BlkSize * BlkSize);
		}
	}

#if LPF_Flag
	LPF(Data, WIDTH, HEIGHT);
#endif


	for (Int32 i = 0; i < BlkSize * BlkSize; i++)
	{
		Data[i].Mag = sqrt(pow(Data[i].Re, 2) + pow(Data[i].Im, 2));
		Data[i].Pha = atan2(Data[i].Im , Data[i].Re);
	}

	for (Int32 y = 0; y < BlkSize; y++)      //블록을 원래 있던 픽쳐 위치에 저장
	{
		for (Int32 x = 0; x < BlkSize; x++)
		{
			Magnitude->Data64[(BlkRow * BlkSize + y) * Stride + (BlkCol * BlkSize + x)] = Data[y * BlkSize + x].Mag;
			Phase->Data64[(BlkRow * BlkSize + y) * Stride + (BlkCol * BlkSize + x)] = Data[y * BlkSize + x].Pha;
		}
	}

#if CalcType

	Int32 wid = WIDTH;
	Int32 hei = HEIGHT;

	Dou64 C, Temp, Spec;
	Dou64* SpecTmp;
	UInt8* Shuffling;

	SpecTmp = (Dou64*)calloc(BlkSize * BlkSize, sizeof(Dou64));
	Shuffling = (UInt8*)calloc(BlkSize * BlkSize, sizeof(UInt8));

	if (BlkSize == wid && BlkSize == hei)
	{
		C = hypot(Data[(BlkRow * BlkSize) + (BlkCol * (wid + (wid % BlkSize)) * BlkSize)].Re, Data[(BlkRow * BlkSize) + (BlkCol * (wid + (wid % BlkSize)) * BlkSize)].Im);
		for (Int32 i = 0; i < BlkSize; i++)
		{
			Spec = 0;
			for (Int32 j = 0; j < BlkSize; j++)
			{
				Temp = hypot(Data[(BlkRow * BlkSize) + (BlkCol * (wid + (wid % BlkSize)) * BlkSize) + ((wid + (wid % BlkSize)) * i) + j].Re, Data[(BlkRow * BlkSize) + (BlkCol * (wid + (wid % BlkSize)) * BlkSize) + ((wid + (wid % BlkSize)) * i) + j].Im);
				Spec = (C * log(1.0 + abs(Temp))) < 0.0 ? 0.0 : (C * log(1.0 + abs(Temp))) > 255.0 ? 255.0 : (C * log(1.0 + abs(Temp)));
				SpecTmp[i * BlkSize + j] = Spec;
			}
		}

		// 셔플링
		for (Int32 i = 0; i < BlkSize; i += (BlkSize / 2))
		{
			for (Int32 j = 0; j < BlkSize; j += (BlkSize / 2))
			{
				for (Int32 k = 0; k < (BlkSize / 2); k++)
				{
					for (Int32 l = 0; l < (BlkSize / 2); l++)
					{
						Shuffling[wid * (k + i) + (l + j)] = (UInt8)SpecTmp[wid * (255 - k + i) + (255 - l + j)];
					}
				}
			}
		}
		FILE* cp;
		fopen_s(&cp, "DFT_Spectrum.raw", "wb");

		fwrite(Shuffling, sizeof(UInt8), BlkSize * BlkSize, cp);

		free(SpecTmp);
		free(Shuffling);

		fclose(cp);
	}


#endif


	free(Data);

#if CalcType
	free(CosTable);
	free(SinTable);
#endif
}

void DFTProcess(Image* Tmp, Image* Magnitude, Image* Phase, Int32 BlkSize)
{
	Dou64* CurBlk; //현재 블록

	Queue queue; // 입력영상을 블록단위로 1차원 정렬함

	Int32 Stride, wid, hei;
	wid = Stride = Tmp->info.wid;
	hei = Tmp->info.hei;

	//Magnitude와 Phase를 픽쳐 단위로 저장
	Magnitude->Data64 = (Dou64*)calloc(Tmp->info.ValidPixel, sizeof(Dou64));
	Phase->Data64     = (Dou64*)calloc(Tmp->info.ValidPixel, sizeof(Dou64));
	Tmp->Data64       = (Dou64*)calloc(Tmp->info.ValidPixel, sizeof(Dou64));

	for (Int32 i = 0; i < Tmp->info.ValidPixel; i++)
		Tmp->Data64[i] = Tmp->Data[i];

	InputImageSorting(Tmp->Data64, wid, hei, BlkSize, &queue);

	//DFT
	CurBlk = (Dou64*)calloc(BlkSize * BlkSize, sizeof(Dou64));
	for (Int32 BlkRow = 0; BlkRow < hei / BlkSize; BlkRow++)    //블록의 왼쪽 위 좌표를 찾기 위한 반복문
		for (Int32 BlkCol = 0; BlkCol < wid / BlkSize; BlkCol++)//블록의 왼쪽 위 좌표를 찾기 위한 반복문
		{
			memset(CurBlk, 0, sizeof(Dou64) * (BlkSize * BlkSize));
			ExtractBlock(&queue, CurBlk, BlkSize);

			DFT_Func(CurBlk, BlkSize, BlkRow, BlkCol, Stride, Magnitude, Phase); //curBlk: 현재 블록(blkSize x blkSize)
		}
	free(CurBlk);
}

