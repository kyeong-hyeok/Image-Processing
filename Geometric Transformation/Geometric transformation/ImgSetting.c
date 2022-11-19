#include "main.h"

void Initialization(InputImage* in, bool Flag)
{
    FILE* fp;

    in->info.wid = WIDTH;
    in->info.hei = HEIGHT;
    in->info.ValidPixel = in->info.wid * in->info.hei;  // 전체 화소수

    in->Ori = (UInt8*)calloc(in->info.ValidPixel, sizeof(UInt8));
    if (Flag)
        fopen_s(&fp, "lena_512x512_using_blur.raw", "rb"); // 원본 입력 영상 읽기 모드로 열기
    else
        fopen_s(&fp, "lena_512x512.raw", "rb"); // 원본 입력 영상 읽기 모드로 열기
    
    fread(in->Ori, sizeof(UInt8), in->info.ValidPixel, fp);

    fclose(fp);
}

void Destroy(InputImage* in, OutputImage* out)
{
    free(in->Ori);

    free(out->sca.Near); //순서 2-2번
    free(out->sca.Bi);   //순서 2-3번
    free(out->sca.BS);   //순서 2-4번
    free(out->sca.Cu);   //순서 2-5번
    free(out->rot.Near); //순서 1-2번
    free(out->rot.Bi);   //순서 1-3번
    free(out->rot.BS);   //순서 1-4번
    free(out->rot.Cu);   //순서 1-5번
}