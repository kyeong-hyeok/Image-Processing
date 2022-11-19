#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdbool.h>

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
    Int32 wid; //영상 가로 길이
    Int32 hei; //영상 세로 길이

    Int32 ValidPixel; //전체화소수
}Information;

typedef struct _Input_Image
{
    UInt8* Ori;
    Information info;
}InputImage;

typedef struct _Scale
{
	UInt8* Near;
	UInt8* Bi;
	UInt8* BS;
	UInt8* Cu;
	Information info;
}Scale;

typedef struct _Rotation
{
	UInt8* Near;
	UInt8* Bi;
	UInt8* BS;
	UInt8* Cu;
	Information info;
}Rotation;

typedef struct _Output_Image
{
	Scale sca;
	Rotation rot;
}OutputImage;


void Initialization(InputImage* in, bool Flag);
void Destroy(InputImage* in, OutputImage* out);

void GeometricTransformation(InputImage* in, OutputImage* out);


void ImageOutput(UInt8* Data, Int32 DstPixNum, Int8 String[]);
void Output(OutputImage* out);