#include "Filter.h"
#include "Output.h"

void ArithmeticVer1(UInt8 *ori, Int32 wid, Int32 hei, OutputImage* out)
{
    UInt8 String[7][10] = { "Ori", "Plus" , "MinZus" ,"Mul" ,"Div", "PLDivi", "GamCo" };

    UInt8 PlusLut[PixRange], MinusLut[PixRange], MultiLut[PixRange], DiviLut[PixRange] , PLDiviLut[PixRange], GamCoLut[PixRange];

    for (Int32 Idx = 0; Idx < PixRange; Idx++)                                                 //Lookup table �����
    {
        PlusLut [Idx] = Clip3(0, 255, (int)(Idx + alpha+0.5));
        MinusLut[Idx] = Clip3(0, 255, (int)(Idx - beta+0.5));
        MultiLut[Idx] = Clip3(0, 255, (int)(Idx * gamma+0.5));
        DiviLut[Idx] = Clip3(0, 255, (int)(Idx / delta+0.5));
        PLDiviLut[Idx] = Clip3(0, 255, (int)((Idx + 30) / 1.2 + 0.5));
        GamCoLut[Idx] = Clip3(0, 255, (int)(pow(Idx/255.0,1/1.5)*255.0 + 0.5));
    }

    for (Int32 cur = 0; cur < wid * hei; cur++) //��üȭ�ҿ� ���� ���ڴٴ� ��                                               //Lookup table �̿�
    {
        out->Plus[cur] = PlusLut[ori[cur]]; // -���� ȭ�ڰ�, ����ü�� �̷���� �ִµ� �����ͷ� �Ѱܼ� ȭ��ǥ �� �ᵵ��]
        out->Minus[cur] = MinusLut[ori[cur]];
        out->Multi[cur] = MultiLut[ori[cur]];
        out->Divi[cur] = DiviLut[ori[cur]];
        out->PLDivi[cur] = PLDiviLut[ori[cur]];
        out->GamCo[cur] = GamCoLut[ori[cur]];
       
    }

    ImageOutput(out->Plus,  wid, hei, String[1]);
    ImageOutput(out->Minus, wid, hei, String[2]);
    ImageOutput(out->Multi, wid, hei, String[3]);
    ImageOutput(out->Divi, wid, hei, String[4]);
    ImageOutput(out->PLDivi, wid, hei, String[5]);
    ImageOutput(out->GamCo, wid, hei, String[6]);
    //�Լ��� ������ �Ű������� �ٲٱ�
    //ImageOutput(out->Minus,  wid, hei, String[2]);
    //

}

UInt8 Clip3(Int32 minVal, Int32 maxVal, Int32 curVal)
{
    return curVal < minVal ? minVal : curVal > maxVal ? maxVal : curVal;
}