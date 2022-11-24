#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <windows.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define FileName1 "lena_512x512.raw"

#define WIDTH      512              // ������ ���� ũ��
#define HEIGHT     512              // ������ ���� ũ��


#define BlockSize  32

#define PrintOut 0 //ȭ�Ұ� ���

#if WIDTH == BlockSize && HEIGHT == BlockSize
#define CalcType 1
#endif
#define pi        3.141592653589793238462643383279

#define BitDepth  8
#define MinVal    0
#define MaxVal    (1 << BitDepth) - 1

#define Clip3(x) (x < MinVal) ? MinVal : (x > MaxVal) ? MaxVal : x

typedef unsigned char  UInt8;
typedef unsigned short UInt16;
typedef unsigned int   UInt32;

typedef char           Int8;
typedef short          Int16;
typedef int            Int32;
typedef double		   Dou64;

typedef struct _Information
{
	Int32 wid; //���� ���� ����
	Int32 hei; //���� ���� ����
	Int32 ValidPixel; //��üȭ�Ҽ�
}Information;

typedef struct _Image
{
	UInt8* Data;
	Dou64* Data64;
	Information info;
}Image;

void Initialization(Image* Ori, Image* Tmp, Image* Res, Int32 BlkSize);
void Destroy(Image* Ori, Image* Tmp, Image* Res, Image* DCT_Val);

void Output(Image* Res, Image* Tmp, Int32 BlkSize);

void DCTProcess(Image* Tmp, Image* DCT_Val, Int32 BlkSize);
void IDCTProcess(Image* Res, Image* DCT_Val, Int32 BlkSize);