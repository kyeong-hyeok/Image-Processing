#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <windows.h>
#include <stdbool.h>

#define WIDTH      512              // 영상의 가로 크기
#define HEIGHT     512              // 영상의 세로 크기

#define FileName1 "lena_512x512.raw"
#define FileName2 "Lena-scratch-noise.raw"

#define PrintOut 0 //화소값 출력

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
	UInt8* Ori; //원본 영상 저장 버퍼
	UInt8* Tmp; //패딩된 영상 저장 버퍼
	Information info;
}InputImage;

typedef struct _Output_Image
{
	UInt8* Res; //결과 영상 저장 버퍼(필터링 적용됨)
	Information info;
}OutputImage;

//////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _Node {
    Int32 num;
    Int32 cntofFilter;
	bool Flag; //True: 스무딩(blurring 혹은 Median) 필터를 N번 적용하는 경우, False: 필터를 1번만 적용하는 경우 
    struct _Node* next;
}Node;

void Initialization(InputImage* in, OutputImage* out, bool Flag, Int32 MaskSize);
void Destroy(InputImage* in, OutputImage* out, Node* head);

void FilteringProcess(Int32 num, Node* head);

void ImageOutput(UInt8* Data, Int32 DstPixNum, Int8 String[]);
void Output(OutputImage* out, Int32 num, Node* head);

//////////////////////////////////////////////////////////////////////////////////////////////////////
Node* NewNode();//새로운 노드 생성하는 함수
void DeleteNode(Node* head); //원하는 노드를 지움