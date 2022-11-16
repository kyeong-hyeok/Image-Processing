#include "Filter.h"
#include "Output.h"

HistModiImage HistogramModification(UInt8* ori, Int32 wid, Int32 hei)
{
    HistModiImage modi;
    UInt8 String[3][10] = {"Equal","EndIn"};
    Int32 Cnt[PixRange]      = { 0 };

    ////////////////////////////////////////////////////////////////////
    //Equalization 변수
    Int32 AccumCnt[PixRange] = { 0 }; //누적 빈도수(accumulate)
    Int32 EqualLut[PixRange] = { 0 };
    Int32 maxPix = MaxVal;
    ////////////////////////////////////////////////////////////////////
    //EndInSearch 변수
    Int32 LowNum, HighNum, LowSum = 0, HighSum = 0;
    Int32 LT, HT;
    Int32 LowPos = MinVal, HighPos = MaxVal;
    ////////////////////////////////////////////////////////////////////

    for (Int32 i = 0; i < wid * hei; i++)
        Cnt[ori[i]]++;

    modi.Equal = (UInt8*)calloc((wid * hei), sizeof(UInt8));
    modi.EIS   = (UInt8*)calloc((wid * hei), sizeof(UInt8));



 
    ////////////////////////////////////////////////////////////////////
    //Equalization 과정

    AccumCnt[0] = Cnt[0];
    for (Int32 i = 1; i < PixRange; i++)            //누적 빈도수 계산
        AccumCnt[i] = AccumCnt[i-1] + Cnt[i];

    for (Int32 i = 0; i < PixRange; i++)            // 수식 계산
        EqualLut[i] = (int)((double)maxPix / AccumCnt[PixRange - 1] * AccumCnt[i] + 0.5);        // (maxPix/전체화소수) * AccumCnt+0.5

    for (Int32 i = 0; i < wid * hei; i++)           // Lookup table을 이용하여 원본 영상에 효과 적용
        modi.Equal[i] = EqualLut[ori[i]];     // modi.Equal[i] = EqualLut[원본화솟값]

    

    ////////////////////////////////////////////////////////////////////
    //EndInSearch 과정 

    LT = LowThres;  // 30프로를 어둡게 밝게 바꿔준다는 것 
    HT = HighThres;

    LowNum = wid * hei *(LT / 100.0);    // 전체화소수*0.3
    HighNum = wid * hei * (HT /100.0);   // 전체화소수*0.3

    while (1) //0이 몇개 발생? 1이 몇개 발생? 빈도수 체크
    { 
        LowSum += Cnt[LowPos]; // LowSum을 만들고 빈도수 축적 
        if (LowSum > LowNum) break;
        else LowPos++;
    //  if(LowSum>LowNum) 하위 30프로를 넘지 않는지 넘어가면 break, 안넘어가면 LowPos 증가시키기
    }

    while (1)
    {
        HighSum += Cnt[HighPos];
        if (HighSum > HighNum) break;
        else HighPos--;
    }

    for (int i = 0; i < wid * hei; i++)
    {
	   if (ori[i] <= LowPos) modi.EIS[i] = MinVal;
	   else if (ori[i] >= HighPos) modi.EIS[i] = MaxVal;
	   else
	   {
		  double BufPix;
          BufPix = (double)(ori[i] - LowPos) / (HighPos - LowPos) * 255 + 0.5;

		  if (BufPix < MinVal)      modi.EIS[i] = MinVal;
		  else if (BufPix > MaxVal) modi.EIS[i] = MaxVal;
		  else                      modi.EIS[i] = BufPix;
	   }
    }

    ImageOutput(modi.Equal, wid, hei, String[0]);
    ImageOutput(modi.EIS, wid, hei, String[1]);


    return modi;
}