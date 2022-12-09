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

#define WIDTH      832              // ������ ���� ũ��
#define HEIGHT     480              // ������ ���� ũ��
#define FRAME	   20

#define BlockSize 8

#define BitDepth  8
#define MinVal    0
#define MaxVal    (1 << BitDepth) - 1

#define Clip3(x) (x < MinVal) ? MinVal : (x > MaxVal) ? MaxVal : x

#define pi        3.141592653589793238462643383279                                               


#define TransQuantFlag 1  //0�̸� ��ȯ,����ȭ ���� ���� //1�̸� ��ȯ,����ȭ ������
#if TransQuantFlag
#define EntropyFlag 1     //0�̸� ��Ʈ�����ڵ� ���� ���� //1�̸� ��Ʈ�����ڵ� ������
#endif

typedef unsigned char  Pel;
typedef unsigned char  UInt8;
typedef unsigned short UInt16;
typedef unsigned int   UInt32;

typedef char           Int8;
typedef short          Int16;
typedef int            Int32;
typedef double		   Dou64;
typedef bool		   Bool;

enum PredictionMode
{
	PLANAR,
	DC,
	VERTICAL,
	HORIZONTAL,
	DIAGONAL,
	VDIAGONAL,
	HDIAGONAL,
	modeNum
};

typedef struct _recBuffer
{
	UInt8 refPix[2][BlockSize * 2 + 1];
}refBuf;

typedef struct _Image_Buffer //��Ʈ���� �ڵ����� ���� ����
{
	Int32 Huff_codes[16];
	Int32 Huff_freq[16];
	Int32 Huff_ch[16];
	Int32 Huff_length[16];
	Int32 Huff_Size;

	Int32* Quant_blk;
	Int32 Best_Mode;
}ImgBuf;

void EncodingProcess(UInt8* OriLuma, UInt8* RecLuma);
refBuf xFillReferenceSamples(UInt8* recPos, Int32 ctbX, Int32 ctbY, Int32 stride, Int32 BlkSize);
void PredictionFunc(UInt8* Pred, refBuf ref, Int32 BlkSize, Int32 BlkStride, UInt8 mode);
void TransQuantFunc(Int32* ResiBlk, Int32 BlkSize, Int32* QuantBlk);
void InvTransQuantFunc(Int32 *QuantBlk, Int32 BlkSize, Int32 *InvResiBlk);
#if EntropyFlag
void EncoderEntropyCoding(Int32 BLK, ImgBuf* img);
#endif