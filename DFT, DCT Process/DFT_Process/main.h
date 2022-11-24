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



//#define FileName1 "city_1280x720.raw"
//
//#define WIDTH      1280              // ������ ���� ũ��
//#define HEIGHT     720              // ������ ���� ũ��

#define BlockSize  4

#define PrintOut 0 //ȭ�Ұ� ���

#if WIDTH == BlockSize && HEIGHT == BlockSize
#define CalcType 1
#define LPF_Flag 1
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

typedef struct _Complex
{
	Dou64 Re; //���� ���� �Ǽ��� �� ����
	Dou64 Im; //���� ���� ����� �� ����
	Dou64 Mag; //���� ���� Magnitude �� ����
	Dou64 Pha; //���� ���� Phase �� ����
}Complex;

void Initialization(Image* Ori, Image* Tmp, Image* Res, Int32 BlkSize);
void Destroy(Image* Ori, Image* Tmp, Image* Res, Image* Magnitude, Image* Phase);

void Output(Image* Res, Image* Tmp, Int32 BlkSize);

void DFTProcess(Image* Tmp, Image* Magnitude, Image* Phase, Int32 BlkSize);
void IDFTProcess(Image* Res, Image* Magnitude, Image* Phase, Int32 BlkSize);