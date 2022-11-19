#include "main.h"

void ImageOutput(UInt8* Data, Int32 DstPixNum, Int8 String[])
{
    char Name_Hist[50] = "Img_";
    char Name_extension[10] = ".raw";

    FILE* fp;

    strcat(Name_Hist, String);
    strcat(Name_Hist, Name_extension);

    fopen_s(&fp, Name_Hist, "wb"); //���� ���� ����
    fwrite(Data, sizeof(UInt8), DstPixNum, fp);
    fclose(fp);
}

void Output(OutputImage* out)
{
    UInt8 ScaString[4][10] = { "ScaNear", "ScaBi" , "ScaBS", "ScaCu" }; // ����2-5��
    UInt8 RotString[4][10] = { "RotNear", "RotBi" , "RotBS", "RotCu"}; // ����1-5��
    ImageOutput(out->sca.Near, out->sca.info.ValidPixel, ScaString[0]); //���� 2-2��
    ImageOutput(out->sca.Bi, out->sca.info.ValidPixel, ScaString[1]);   //���� 2-3��
    ImageOutput(out->sca.BS, out->sca.info.ValidPixel, ScaString[2]);   //���� 2-4��
    ImageOutput(out->sca.Cu, out->sca.info.ValidPixel, ScaString[3]);// ����2-5��


    ImageOutput(out->rot.Near, out->rot.info.ValidPixel, RotString[0]); //���� 1-2��
    ImageOutput(out->rot.Bi, out->rot.info.ValidPixel, RotString[1]);   //���� 1-3��
    ImageOutput(out->rot.BS, out->rot.info.ValidPixel, RotString[2]);   //���� 1-4��
    ImageOutput(out->rot.Cu, out->rot.info.ValidPixel, RotString[3]);// ����1-5��
}