#include "Filter.h"
#include "Output.h"

HistModiImage HistogramModification(UInt8* ori, Int32 wid, Int32 hei)
{
    HistModiImage modi;
    UInt8 String[3][10] = {"Equal","EndIn"};
    Int32 Cnt[PixRange]      = { 0 };

    ////////////////////////////////////////////////////////////////////
    //Equalization ����
    Int32 AccumCnt[PixRange] = { 0 }; //���� �󵵼�(accumulate)
    Int32 EqualLut[PixRange] = { 0 };
    Int32 maxPix = MaxVal;
    ////////////////////////////////////////////////////////////////////
    //EndInSearch ����
    Int32 LowNum, HighNum, LowSum = 0, HighSum = 0;
    Int32 LT, HT;
    Int32 LowPos = MinVal, HighPos = MaxVal;
    ////////////////////////////////////////////////////////////////////

    for (Int32 i = 0; i < wid * hei; i++)
        Cnt[ori[i]]++;

    modi.Equal = (UInt8*)calloc((wid * hei), sizeof(UInt8));
    modi.EIS   = (UInt8*)calloc((wid * hei), sizeof(UInt8));



 
    ////////////////////////////////////////////////////////////////////
    //Equalization ����

    AccumCnt[0] = Cnt[0];
    for (Int32 i = 1; i < PixRange; i++)            //���� �󵵼� ���
        AccumCnt[i] = AccumCnt[i-1] + Cnt[i];

    for (Int32 i = 0; i < PixRange; i++)            // ���� ���
        EqualLut[i] = (int)((double)maxPix / AccumCnt[PixRange - 1] * AccumCnt[i] + 0.5);        // (maxPix/��üȭ�Ҽ�) * AccumCnt+0.5

    for (Int32 i = 0; i < wid * hei; i++)           // Lookup table�� �̿��Ͽ� ���� ���� ȿ�� ����
        modi.Equal[i] = EqualLut[ori[i]];     // modi.Equal[i] = EqualLut[����ȭ�ڰ�]

    

    ////////////////////////////////////////////////////////////////////
    //EndInSearch ���� 

    LT = LowThres;  // 30���θ� ��Ӱ� ��� �ٲ��شٴ� �� 
    HT = HighThres;

    LowNum = wid * hei *(LT / 100.0);    // ��üȭ�Ҽ�*0.3
    HighNum = wid * hei * (HT /100.0);   // ��üȭ�Ҽ�*0.3

    while (1) //0�� � �߻�? 1�� � �߻�? �󵵼� üũ
    { 
        LowSum += Cnt[LowPos]; // LowSum�� ����� �󵵼� ���� 
        if (LowSum > LowNum) break;
        else LowPos++;
    //  if(LowSum>LowNum) ���� 30���θ� ���� �ʴ��� �Ѿ�� break, �ȳѾ�� LowPos ������Ű��
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