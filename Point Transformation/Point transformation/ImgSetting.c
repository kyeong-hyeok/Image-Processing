#include "main.h"

void Initialization(InputImage*in, OutputImage* out)
{
    FILE* fp;

    in->info.wid = WIDTH;
    in->info.hei = HEIGHT;

    in->Ori = (UInt8*)malloc(sizeof(UInt8) * (in->info.wid * in->info.hei));
    memset(in->Ori, 0, sizeof(UInt8) * (in->info.wid * in->info.hei));
    fopen_s(&fp, "lena_512x512.raw", "rb"); // 원본 입력 영상 읽기 모드로 열기
    fread(in->Ori, sizeof(UInt8), (in->info.wid * in->info.hei), fp);
    fclose(fp);


    out->info.wid = WIDTH;
    out->info.hei = HEIGHT;

    out->Plus  = (UInt8*)calloc(out->info.wid * out->info.hei, sizeof(UInt8));
    out->Minus = (UInt8*)calloc(out->info.wid * out->info.hei, sizeof(UInt8));
    out->Multi = (UInt8*)calloc(out->info.wid * out->info.hei, sizeof(UInt8));
    out->Divi  = (UInt8*)calloc(out->info.wid * out->info.hei, sizeof(UInt8));
    out->PLDivi = (UInt8*)calloc(out->info.wid * out->info.hei, sizeof(UInt8));
    out->GamCo = (UInt8*)calloc(out->info.wid * out->info.hei, sizeof(UInt8));

}

void Destroy(InputImage* in, OutputImage* out, HistModiImage *modi)
{
    free(in->Ori);

    free(out->Plus);
    free(out->Minus);
    free(out->Multi);
    free(out->Divi);
    free(out->PLDivi);
    free(out->GamCo);
    free(modi->Equal);
    free(modi->EIS);
}