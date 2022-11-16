#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define WIDTH      512              // 영상의 가로 크기
#define HEIGHT     512              // 영상의 세로 크기

typedef unsigned char  UInt8;
typedef unsigned short UInt16;
typedef unsigned int   UInt32;

typedef char           Int8;
typedef short          Int16;
typedef int            Int32;

typedef struct _Information
{
    Int32 wid;
    Int32 hei;
}Information;

typedef struct _Input_Image
{
    UInt8 *Ori;
    Information info;
}InputImage;

typedef struct _Output_Image
{
    UInt8* Plus;
    UInt8* Minus;
    UInt8* Multi; //Multiplication
    UInt8* Divi;  //Division
    UInt8* PLDivi;
    UInt8* GamCo;
    Information info;
}OutputImage;

typedef struct _Histogram_Modification_Image
{
    UInt8* Equal;
    UInt8* EIS;
}HistModiImage;

void Initialization(InputImage* in, OutputImage* out);
void Destroy(InputImage* in, OutputImage* out, HistModiImage *modi);