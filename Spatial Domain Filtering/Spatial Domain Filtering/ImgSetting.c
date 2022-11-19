#include "main.h"

void Initialization(InputImage* in, OutputImage* out, bool Flag, Int32 MaskSize)
{
	FILE* fp;

	in->info.wid = WIDTH;                                   //원본 영상 가로, 세로 길이
	in->info.hei = HEIGHT;
	in->info.ValidPixel = in->info.wid * in->info.hei;

    out->info.wid = WIDTH;                                   //결과 영상 가로, 세로 길이
	out->info.hei = HEIGHT;
	out->info.ValidPixel = out->info.wid * out->info.hei;

	in->Ori  = (UInt8*)calloc( in->info.ValidPixel, sizeof(UInt8)); //원본 영상
	out->Res = (UInt8*)calloc(out->info.ValidPixel, sizeof(UInt8)); //결과 영상 (필터링된 영상)

	if (Flag)
		fopen_s(&fp, FileName2, "rb"); // 원본 입력 영상 읽기 모드로 열기
	else
		fopen_s(&fp, FileName1, "rb"); // 원본 입력 영상 읽기 모드로 열기

	fread(in->Ori, sizeof(UInt8), in->info.ValidPixel, fp);

	fclose(fp);
}

void Destroy(InputImage* in, OutputImage* out, Node* head)
{
	DeleteNode(head);//모든 노드 삭제

	free(in->Ori);
	free(in->Tmp);
	free(out->Res);
}