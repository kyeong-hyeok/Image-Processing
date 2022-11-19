#include "Geometric.h"

UInt8 NearesetNeighbor(UInt8* ori, double SrcX, double SrcY, Int32 Wid, Int32 Hei, Int32 Stride) //순서 1-2번
{
    Int32 PosX, PosY;
    PosX = EmptySpace(0, Wid - 1, (Int32)(SrcX + 0.5));
    PosY = EmptySpace(0, Hei - 1, (Int32)(SrcY + 0.5));

    return ori[PosY * Stride + PosX];
}

UInt8 Bilinear(UInt8* ori, double SrcX, double SrcY, Int32 Wid, Int32 Hei, Int32 Stride) //순서 1-3번
{
    Int32 PosX, PosY, PosXPlus1, PosYPlus1;
    double HorWei, VerWei; //Horizontal Weight, Vertical Weight
    double Tmp = 0;
    Int32 TL, TR, BL, BR; //각 화소 위치

    PosX = EmptySpace(0, Wid - 1, (Int32)SrcX);      // 실습2 p13 A위치가 POSX, POSY 쓸 때 EmptySpace 써줘야함 A,B,C,D 반올림된 위치 아니고 정수위치임 PosX = EmptySpace(0, Wid - 1, (Int32)(SrcX + 0.5)); 이용하되 반올림이 아니고 해당 정수로 가게 해야함
    PosY = EmptySpace(0, Hei - 1, (Int32)SrcY);
    PosXPlus1 = EmptySpace(0, Wid - 1, (Int32)(SrcX + 1));
    PosYPlus1 = EmptySpace(0, Hei - 1, (Int32)(SrcY + 1));

    HorWei = SrcX - (Int32)SrcX;
    VerWei = SrcY - (Int32)SrcY;

    TL = PosY * Stride + PosX;                 //화소 위치 찾기 Y * Stride + X 꼴
    TR = PosY * Stride + PosXPlus1;
    BL = PosYPlus1 * Stride + PosX;
    BR = PosYPlus1 * Stride + PosXPlus1;

    Tmp = ((1 - HorWei) * ori[TL] + HorWei * ori[TR]) * (1 - VerWei) + ((1 - HorWei) * ori[BL] + HorWei * ori[BR]) * VerWei;                  //Bi - linear 수식  가중치는 계산되어있음. HorWei -> p13의 x위치, VerWei-> y   A화소값 가중치 구하는 법 Horwei * ori[TL탑레프트] + 1-HOW * ori[TR]? 가로방향 두개랑 세로방향
        

    return (UInt8)(Clip3(Tmp + 0.5));
}

double BSpline_function(double x) //순서 1-4번 
{

    double result;

    if (fabs(x) >= 0 && fabs(x) < 1)    // x는 거리
    {
        result = 1 / 2.0 * pow(fabs(x), 3) - pow(fabs(x), 2) + 2 / 3.0;
    }
    else if (fabs(x) >= 1 && fabs(x) < 2) 
    {
        result = -1 / 6.0 * pow(fabs(x), 3) + pow(fabs(x), 2) - 2 * fabs(x) + 4 / 3.0;
    }
    else
        result = 0;

    return result;
}

UInt8 B_Spline(UInt8* ori, double SrcX, double SrcY, Int32 Wid, Int32 Hei, Int32 Stride) //순서 1-4번
{
    Int32 PosXMinus1, PosX, PosXPlus1, PosXPlus2;
    Int32 PosYMinus1, PosY, PosYPlus1, PosYPlus2;

    double HorWei, VerWei; //Horizontal Weight, Vertical Weight
    double TmpHor[4] = { 0,0,0,0 };
    double Tmp = 0;

    Int32 X_Pix[] = { 0, 0, 0, 0 };
    Int32 Y_Pix[] = { 0, 0, 0, 0 };
    double Distance_Hor[] = { 0.0, 0.0, 0.0, 0.0 };
    double Distance_Ver[] = { 0.0, 0.0, 0.0, 0.0 };

    PosXMinus1 = EmptySpace(0, Wid - 1, (Int32)(SrcX-1));// EmptySpace(0, Hei - 1, (Int32)(SrcY + 0.5)) 뒤에만 바꿔주면 됨
    PosX       = EmptySpace(0, Wid - 1, (Int32)(SrcX));
    PosXPlus1  = EmptySpace(0, Wid - 1, (Int32)(SrcX+1));
    PosXPlus2  = EmptySpace(0, Wid - 1, (Int32)(SrcX+2));

    PosYMinus1 = EmptySpace(0, Hei - 1, (Int32)(SrcY - 1));
    PosY       = EmptySpace(0, Hei - 1, (Int32)(SrcY));
    PosYPlus1  = EmptySpace(0, Hei - 1, (Int32)(SrcY + 1));
    PosYPlus2  = EmptySpace(0, Hei - 1, (Int32)(SrcY + 2));
    
    HorWei = SrcX - (Int32)SrcX;
    VerWei = SrcY - (Int32)SrcY;

    X_Pix[0] = PosXMinus1; X_Pix[1] = PosX; X_Pix[2] = PosXPlus1; X_Pix[3] = PosXPlus2;
    Y_Pix[0] = PosYMinus1; Y_Pix[1] = PosY; Y_Pix[2] = PosYPlus1; Y_Pix[3] = PosYPlus2;
    Distance_Hor[0] = HorWei + 1; Distance_Hor[1] = HorWei; Distance_Hor[2] = 1 - HorWei; Distance_Hor[3] = (1 - HorWei) + 1;
    Distance_Ver[0] = VerWei + 1; Distance_Ver[1] = VerWei; Distance_Ver[2] = 1 - VerWei; Distance_Ver[3] = (1 - VerWei) + 1;

    for (Int32 y = 0; y < 4; y++)   // BIlinear에서 못하겠으면 이거보기
    {
        for (Int32 x = 0; x < 4; x++)
        {
            TmpHor[y] += BSpline_function(Distance_Hor[x]) * ori[Y_Pix[y] * Stride + X_Pix[x]];
        }
    }

    for (Int32 i = 0; i < 4; i++)
    {
        Tmp += BSpline_function(Distance_Ver[i]) * TmpHor[i];
    }

    return (UInt8)(Clip3(Tmp + 0.5));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//순서 1-5번
double Cubic_function(double x)
{
    double a = 0.5;
    double result;

    if (fabs(x) >= 0 && fabs(x) < 1)    // x는 거리
    {
        result = (a+2) * pow(fabs(x), 3) - (a+3) * pow(fabs(x), 2) + 1;
    }
    else if (fabs(x) >= 1 && fabs(x) < 2)
    {
        result = a * pow(fabs(x), 3) - 5 * a * pow(fabs(x), 2) + 8 * a * fabs(x) - 4 * a;
    }
    else
        result = 0;

    return result;
}

UInt8 Cubic(UInt8* ori, double SrcX, double SrcY, Int32 Wid, Int32 Hei, Int32 Stride) //순서 1-4번
{
    Int32 PosXMinus1, PosX, PosXPlus1, PosXPlus2;
    Int32 PosYMinus1, PosY, PosYPlus1, PosYPlus2;

    double HorWei, VerWei; //Horizontal Weight, Vertical Weight
    double TmpHor[4] = { 0,0,0,0 };
    double Tmp = 0;

    Int32 X_Pix[] = { 0, 0, 0, 0 };
    Int32 Y_Pix[] = { 0, 0, 0, 0 };
    double Distance_Hor[] = { 0.0, 0.0, 0.0, 0.0 };
    double Distance_Ver[] = { 0.0, 0.0, 0.0, 0.0 };

    PosXMinus1 = EmptySpace(0, Wid - 1, (Int32)(SrcX - 1));// EmptySpace(0, Hei - 1, (Int32)(SrcY + 0.5)) 뒤에만 바꿔주면 됨
    PosX       = EmptySpace(0, Wid - 1, (Int32)(SrcX));
    PosXPlus1  = EmptySpace(0, Wid - 1, (Int32)(SrcX + 1));
    PosXPlus2  = EmptySpace(0, Wid - 1, (Int32)(SrcX + 2));

    PosYMinus1 = EmptySpace(0, Hei - 1, (Int32)(SrcY - 1));
    PosY       = EmptySpace(0, Hei - 1, (Int32)(SrcY));
    PosYPlus1  = EmptySpace(0, Hei - 1, (Int32)(SrcY + 1));
    PosYPlus2  = EmptySpace(0, Hei - 1, (Int32)(SrcY + 2));

    HorWei = SrcX - (Int32)SrcX;
    VerWei = SrcY - (Int32)SrcY;

    X_Pix[0] = PosXMinus1; X_Pix[1] = PosX; X_Pix[2] = PosXPlus1; X_Pix[3] = PosXPlus2;
    Y_Pix[0] = PosYMinus1; Y_Pix[1] = PosY; Y_Pix[2] = PosYPlus1; Y_Pix[3] = PosYPlus2;
    Distance_Hor[0] = HorWei + 1; Distance_Hor[1] = HorWei; Distance_Hor[2] = 1 - HorWei; Distance_Hor[3] = (1 - HorWei) + 1;
    Distance_Ver[0] = VerWei + 1; Distance_Ver[1] = VerWei; Distance_Ver[2] = 1 - VerWei; Distance_Ver[3] = (1 - VerWei) + 1;

    for (Int32 y = 0; y < 4; y++)   // BIlinear에서 못하겠으면 이거보기
    {
        for (Int32 x = 0; x < 4; x++)
        {
            TmpHor[y] += Cubic_function(Distance_Hor[x]) * ori[Y_Pix[y] * Stride + X_Pix[x]];
        }
    }

    for (Int32 i = 0; i < 4; i++)
    {
        Tmp += Cubic_function(Distance_Ver[i]) * TmpHor[i];
    }

    return (UInt8)(Clip3(Tmp + 0.5));
}