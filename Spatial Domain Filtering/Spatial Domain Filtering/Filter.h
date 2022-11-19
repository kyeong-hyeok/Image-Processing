#pragma once
#include <math.h>
#include <memory.h>

#define PaddingType 1   // 0: Zero Padding, 1: Half Padding, 2: Copy Padding


#define BitDepth  8
#define MinVal    0
#define MaxVal    (1 << BitDepth) - 1
#define PixRange  (1 << BitDepth)


#define EmptySpace(a, b, x) (x < a) ? a : (x > b)  ? b : x
#define Clip3(x) (x < MinVal) ? MinVal : (x > MaxVal) ? MaxVal : x

#define Max(x, y) x > y ? x : y
#define SWAP(x, y) {int tmp; tmp = x; x = y; y = tmp;}

#define sq2 = sqrt(2);

typedef unsigned char  UInt8;
typedef unsigned short UInt16;
typedef unsigned int   UInt32;

typedef char           Int8;
typedef short          Int16;
typedef int            Int32;

void ImagePadding(UInt8* Ori, UInt8* Tmp, Int32 wid, Int32 hei, Int32 MaskSize);