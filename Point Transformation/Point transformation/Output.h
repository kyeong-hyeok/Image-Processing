#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include"main.h"

typedef unsigned char  UInt8;
typedef unsigned short UInt16;
typedef unsigned int   UInt32;

typedef char           Int8;
typedef short          Int16;
typedef int            Int32;

void ImageOutput(UInt8* Data, Int32 wid, Int32 hei, Int8 String[]);
void OutputHistogram(InputImage* in, OutputImage* out, HistModiImage* modi);