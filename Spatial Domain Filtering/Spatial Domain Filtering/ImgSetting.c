#include "main.h"

void Initialization(InputImage* in, OutputImage* out, bool Flag, Int32 MaskSize)
{
	FILE* fp;

	in->info.wid = WIDTH;                                   //���� ���� ����, ���� ����
	in->info.hei = HEIGHT;
	in->info.ValidPixel = in->info.wid * in->info.hei;

    out->info.wid = WIDTH;                                   //��� ���� ����, ���� ����
	out->info.hei = HEIGHT;
	out->info.ValidPixel = out->info.wid * out->info.hei;

	in->Ori  = (UInt8*)calloc( in->info.ValidPixel, sizeof(UInt8)); //���� ����
	out->Res = (UInt8*)calloc(out->info.ValidPixel, sizeof(UInt8)); //��� ���� (���͸��� ����)

	if (Flag)
		fopen_s(&fp, FileName2, "rb"); // ���� �Է� ���� �б� ���� ����
	else
		fopen_s(&fp, FileName1, "rb"); // ���� �Է� ���� �б� ���� ����

	fread(in->Ori, sizeof(UInt8), in->info.ValidPixel, fp);

	fclose(fp);
}

void Destroy(InputImage* in, OutputImage* out, Node* head)
{
	DeleteNode(head);//��� ��� ����

	free(in->Ori);
	free(in->Tmp);
	free(out->Res);
}