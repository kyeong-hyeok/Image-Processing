#include "Output.h"
#include "Filter.h"


void HISTOGRAM(UInt8* Data, Int32 wid, Int32 hei, Int32 max, Int8 String[])
{
	FILE* fp;

	Int32    Cnt[PixRange] = { 0 };
	double NormalCnt[PixRange] = { 0 };
	UInt8 Output[PixRange][PixRange] = { 0 };

	UInt8 Name_Hist[50] = "Hist_";
	UInt8 Name_extension[10] = ".raw";

	Int32 MaxCnt = 0;

	for (Int32 i = 0; i < wid * hei; i++) //빈도수 카운트
		Cnt[Data[i]]++;

	for (Int32 i = 0; i < PixRange; i++) {//가장 많은 화소값 찾기
		MaxCnt = MaxCnt < Cnt[i] ? Cnt[i] : MaxCnt;
	}
	for (Int32 i = 0; i < PixRange; i++) //정규화: 최대 높이 255
		NormalCnt[i] = (double)Cnt[i] / MaxCnt * 255 + 0.5; //(0-1 사이의 값으로 바꾼 후 255를 곱하기)


	//히스토그램 만들기, 검은색 바탕에 흰색 막대그래프
	for (Int32 i=0; i<=255; i++ ) //x축  0-255 반복문
	{
		for (Int32 j=255; j>255-NormalCnt[i]; j--) //y축 왼쪽위 0,0 이므로 왼쪽 아래가 0,255 위로 갈수록 0,254 0,253
		{
			Output[j][i] = 255;
		}
	}

	strcat(Name_Hist, String);
	strcat(Name_Hist, Name_extension);

	fopen_s(&fp, Name_Hist, "wb");
	fwrite(Output, sizeof(UInt8), PixRange * PixRange, fp);
	fclose(fp);
}

void OutputHistogram(InputImage* in, OutputImage* out, HistModiImage* modi)
{
	UInt8 String[9][10] = { "Ori", "Plus" , "Minus" ,"Mul" ,"Div" ,"Equal", "EndIn", "PLDivi", "GamCo"};
	HISTOGRAM(in->Ori,    in->info.wid, in->info.hei, MaxVal, String[0]);
	HISTOGRAM(out->Plus,  in->info.wid, in->info.hei, MaxVal, String[1]);
	HISTOGRAM(out->Minus, in->info.wid, in->info.hei, MaxVal, String[2]);
	HISTOGRAM(out->Multi, in->info.wid, in->info.hei, MaxVal, String[3]);
	HISTOGRAM(out->Divi,  in->info.wid, in->info.hei, MaxVal, String[4]);
	HISTOGRAM(modi->Equal,in->info.wid, in->info.hei, MaxVal, String[5]);
	HISTOGRAM(modi->EIS,  in->info.wid, in->info.hei, MaxVal, String[6]);
	HISTOGRAM(out->PLDivi, in->info.wid, in->info.hei, MaxVal, String[7]);
	HISTOGRAM(out->GamCo, in->info.wid, in->info.hei, MaxVal, String[8]);
}