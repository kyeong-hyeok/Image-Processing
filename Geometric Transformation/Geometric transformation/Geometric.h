#pragma once
#include <math.h>

#define ScaleFactorX  0.47   //0.47 확대 축소값
#define ScaleFactorY  0.47

#define AngleTheta    23	// 회전 각도

#define BitDepth  8
#define MinVal    0
#define MaxVal    (1 << BitDepth) - 1

#define PI			  3.141592653589793238462643383279	

#define EmptySpace(a, b, x) (x < a) ? a : (x > b)  ? b : x		// 역방향 매핑을 했는데 hole 위치가 나올 때 가장 가까운 화소로 매핑되게 해놓음
#define Clip3(x) (x < MinVal) ? MinVal : (x > MaxVal) ? MaxVal : x		// clipping 함수를 매크로로 바꿔놓음

typedef unsigned char  UInt8;
typedef unsigned short UInt16;
typedef unsigned int   UInt32;

typedef char           Int8;
typedef short          Int16;
typedef int            Int32;


UInt8 NearesetNeighbor(UInt8* ori, double SrcX, double SrcY, Int32 Wid, Int32 Hei, Int32 Stride);
UInt8 Bilinear        (UInt8* ori, double SrcX, double SrcY, Int32 Wid, Int32 Hei, Int32 Stride);
UInt8 B_Spline        (UInt8* ori, double SrcX, double SrcY, Int32 Wid, Int32 Hei, Int32 Stride);
UInt8 Cubic           (UInt8* ori, double SrcX, double SrcY, Int32 Wid, Int32 Hei, Int32 Stride);