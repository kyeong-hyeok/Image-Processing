#include "main.h"
#include "Filter.h"
#include "Output.h"

void main()
{
    InputImage in;         // 입력 영상 구조체
    OutputImage out;       // Arithmetic 출력 영상 구조체
    HistModiImage modi;    // HistModi   출력 영상 구조체
  
    Initialization(&in, &out);                                          // 원본영상 읽어오기 및 동적할당
    ArithmeticVer1(in.Ori, in.info.wid, in.info.hei, &out);             // Arithmetic operation (+, -, x, /)
    modi = HistogramModification(in.Ori, in.info.wid, in.info.hei);     // Equalization, End-in-search
    OutputHistogram(&in, &out, &modi);   // 단계별로 실행하고 싶으면 주석처리                               // 모든 Histogram 출력
    Destroy(&in, &out, &modi);                                          // 동적 할당 해제
}