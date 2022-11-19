#include "main.h"

Int32 SrcStride = WIDTH;

void main()
{
    InputImage in;         // 입력 영상 구조체
    OutputImage out;       // 출력 영상 구조체

    Initialization(&in, false);                                   // 원본영상 읽어오기 및 동적할당
    GeometricTransformation(&in, &out);
    Output(&out);                                                // 결과 영상 출력
    Destroy(&in, &out);                                          // 모든 동적 할당 해제
}