#pragma once
#include "main.h"
#include <math.h>

#define alpha      30  //출력 영상 = 원본 영상 + alpha 
#define beta       30  //출력 영상 = 원본 영상 - beta 
#define gamma      1.2 //출력 영상 = 원본 영상 * gamma 
#define delta      1.2 //출력 영상 = 원본 영상 / delta 

#define BitDepth  8
#define MinVal    0
#define MaxVal    (1 << BitDepth) - 1
#define PixRange  (1 << BitDepth)

#define LowThres  30
#define HighThres 30

void ArithmeticVer1(UInt8* ori, Int32 wid, Int32 hei, OutputImage* out);
HistModiImage HistogramModification(UInt8* ori, Int32 wid, Int32 hei);

UInt8 Clip3(Int32 minVal, Int32 maxVal, Int32 curVal);