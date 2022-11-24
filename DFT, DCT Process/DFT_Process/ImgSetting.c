#include "List.h"

void Initialization(Image* Ori, Image* Tmp, Image* Res, Int32 BlkSize)
{
	FILE* fp;

	Ori->info.wid = WIDTH;
	Ori->info.hei = HEIGHT;
	Ori->info.ValidPixel = Ori->info.wid * Ori->info.hei;

	Res->info.wid = WIDTH;
	Res->info.hei = HEIGHT;
	Res->info.ValidPixel = Res->info.wid * Res->info.hei;

	if (Ori->info.wid % BlkSize != 0)
		Tmp->info.wid = Ori->info.wid + (BlkSize - (Ori->info.wid % BlkSize));
	else
		Tmp->info.wid = Ori->info.wid;
	
	if (Ori->info.hei % BlkSize != 0)
		Tmp->info.hei = Ori->info.hei + (BlkSize - (Ori->info.hei % BlkSize));
	else
		Tmp->info.hei = Ori->info.hei;
	
	Tmp->info.ValidPixel = Tmp->info.wid * Tmp->info.hei;


	Ori->Data = (UInt8*)calloc(Ori->info.ValidPixel, sizeof(UInt8));
	Tmp->Data = (UInt8*)calloc(Tmp->info.ValidPixel, sizeof(UInt8));
	Res->Data = (UInt8*)calloc(Res->info.ValidPixel, sizeof(UInt8));

	fopen_s(&fp, FileName1, "rb"); // 원본 입력 영상 읽기 모드로 열기

	fread(Ori->Data, sizeof(UInt8), Ori->info.ValidPixel, fp);

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////원본 영상의 가로,세로 길이가 블록 크기로 나누어 떨어지지 않는 경우, 패딩 필요
	//Tmp->Data
	for (Int32 y = 0; y < Ori->info.hei; y++)
		for (Int32 x = 0; x < Ori->info.wid; x++)
			Tmp->Data[y * Tmp->info.wid + x] = Ori->Data[y * Ori->info.wid + x];

	if (Ori->info.wid != Tmp->info.wid)
		for (Int32 y = 0; y < Ori->info.hei; y++)
			for (Int32 x = Ori->info.wid; x < Tmp->info.wid; x++)
				Tmp->Data[y * Tmp->info.wid + x] = Ori->Data[y * Ori->info.wid + (Ori->info.wid - 1)];

	if (Ori->info.wid != Tmp->info.wid)
		for (Int32 y = Ori->info.hei; y < Tmp->info.hei; y++)
			for (Int32 x = 0; x < Ori->info.wid; x++)
				Tmp->Data[y * Tmp->info.wid + x] = Ori->Data[(Ori->info.hei - 1) * Ori->info.wid + x];

	if (Ori->info.wid != Tmp->info.wid && Ori->info.hei != Tmp->info.hei)
		for (Int32 y = Ori->info.hei; y < Tmp->info.hei; y++)
			for (Int32 x = Ori->info.wid; x < Tmp->info.wid; x++)
				Tmp->Data[y * Tmp->info.wid + x] = Ori->Data[(Ori->info.hei - 1) * Ori->info.wid + (Ori->info.wid - 1)];

	///////////////////////////////////////////////////////////////////////////////////////////////////

	fclose(fp);
}

void Destroy(Image* Ori, Image* Tmp, Image* Res, Image *Magnitude, Image *Phase)
{
	free(Ori->Data);
	free(Tmp->Data);
	free(Tmp->Data64);
	free(Res->Data);
	free(Magnitude->Data64);
	free(Phase->Data64);
}