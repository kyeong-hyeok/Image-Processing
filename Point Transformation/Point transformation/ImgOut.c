#include "Output.h"

void ImageOutput(UInt8* Data, Int32 wid, Int32 hei, Int8 String[])
{
    char Name_Hist[50] = "Img_";
    char Name_extension[10] = ".raw";

    FILE* fp;

    strcat(Name_Hist, String);
    strcat(Name_Hist, Name_extension);

    fopen_s(&fp, Name_Hist, "wb"); //원본 영상 열기
    fwrite(Data, sizeof(UInt8), wid * hei, fp);
    fclose(fp);
}