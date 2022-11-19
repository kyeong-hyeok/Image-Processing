#include "main.h"

void Initialization(InputImage* in, bool Flag)
{
    FILE* fp;

    in->info.wid = WIDTH;
    in->info.hei = HEIGHT;
    in->info.ValidPixel = in->info.wid * in->info.hei;  // ��ü ȭ�Ҽ�

    in->Ori = (UInt8*)calloc(in->info.ValidPixel, sizeof(UInt8));
    if (Flag)
        fopen_s(&fp, "lena_512x512_using_blur.raw", "rb"); // ���� �Է� ���� �б� ���� ����
    else
        fopen_s(&fp, "lena_512x512.raw", "rb"); // ���� �Է� ���� �б� ���� ����
    
    fread(in->Ori, sizeof(UInt8), in->info.ValidPixel, fp);

    fclose(fp);
}

void Destroy(InputImage* in, OutputImage* out)
{
    free(in->Ori);

    free(out->sca.Near); //���� 2-2��
    free(out->sca.Bi);   //���� 2-3��
    free(out->sca.BS);   //���� 2-4��
    free(out->sca.Cu);   //���� 2-5��
    free(out->rot.Near); //���� 1-2��
    free(out->rot.Bi);   //���� 1-3��
    free(out->rot.BS);   //���� 1-4��
    free(out->rot.Cu);   //���� 1-5��
}