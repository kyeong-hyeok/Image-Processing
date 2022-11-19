#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <windows.h>
#include <stdbool.h>

#define WIDTH      512              // ������ ���� ũ��
#define HEIGHT     512              // ������ ���� ũ��

#define FileName1 "lena_512x512.raw"
#define FileName2 "Lena-scratch-noise.raw"

#define PrintOut 0 //ȭ�Ұ� ���

typedef unsigned char  UInt8;
typedef unsigned short UInt16;
typedef unsigned int   UInt32;

typedef char           Int8;
typedef short          Int16;
typedef int            Int32;

typedef struct _Information
{
	Int32 wid; //���� ���� ����
	Int32 hei; //���� ���� ����
	Int32 ValidPixel; //��üȭ�Ҽ�
}Information;

typedef struct _Input_Image
{
	UInt8* Ori; //���� ���� ���� ����
	UInt8* Tmp; //�е��� ���� ���� ����
	Information info;
}InputImage;

typedef struct _Output_Image
{
	UInt8* Res; //��� ���� ���� ����(���͸� �����)
	Information info;
}OutputImage;

//////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _Node {
    Int32 num;
    Int32 cntofFilter;
	bool Flag; //True: ������(blurring Ȥ�� Median) ���͸� N�� �����ϴ� ���, False: ���͸� 1���� �����ϴ� ��� 
    struct _Node* next;
}Node;

void Initialization(InputImage* in, OutputImage* out, bool Flag, Int32 MaskSize);
void Destroy(InputImage* in, OutputImage* out, Node* head);

void FilteringProcess(Int32 num, Node* head);

void ImageOutput(UInt8* Data, Int32 DstPixNum, Int8 String[]);
void Output(OutputImage* out, Int32 num, Node* head);

//////////////////////////////////////////////////////////////////////////////////////////////////////
Node* NewNode();//���ο� ��� �����ϴ� �Լ�
void DeleteNode(Node* head); //���ϴ� ��带 ����