#include "List.h"

void ImageOutput(UInt8* Data, Int32 DstPixNum, Int8 String[])
{
	UInt8 Name_extension[10] = ".raw";
	FILE* fp;

	strcat(String, Name_extension);

	fopen_s(&fp, String, "wb"); //원본 영상 열기
	fwrite(Data, sizeof(UInt8), DstPixNum, fp);
	fclose(fp);
}

void Output(Image* Res, Image* Tmp, Int32 BlkSize)
{
	UInt8 String[10];
	UInt8 Name_Hist[50] = "DFT_";

	itoa(BlkSize, String, 10);

	for (Int32 y = 0; y < Res->info.hei; y++)
		for (Int32 x = 0; x < Res->info.wid; x++)
			Res->Data[y * Res->info.wid + x] = Tmp->Data[y * Tmp->info.wid + x];
	strcat(Name_Hist, String);

	ImageOutput(Res->Data, Res->info.ValidPixel, Name_Hist);
}