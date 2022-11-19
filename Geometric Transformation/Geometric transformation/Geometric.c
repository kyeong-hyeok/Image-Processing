#include "main.h"
#include "Geometric.h"

extern Int32 SrcStride;

void ScalingProcess(UInt8* ori, Int32 SrcWid, Int32 SrcHei, Int32 DstPixNum, double ScaValX, double ScaValY, OutputImage* out)
{
    Int32 DstStride = out->sca.info.wid;
    double SrcX, SrcY;

    out->sca.Near = (UInt8*)calloc(DstPixNum, sizeof(UInt8));
    out->sca.Bi   = (UInt8*)calloc(DstPixNum, sizeof(UInt8));
    out->sca.BS   = (UInt8*)calloc(DstPixNum, sizeof(UInt8));
    out->sca.Cu   = (UInt8*)calloc(DstPixNum, sizeof(UInt8));

    for (Int32 y = 0; y < out->sca.info.hei; y++)      //목적 영상 세로 화소
        for (Int32 x = 0; x < out->sca.info.wid; x++)  //목적 영상 가로 화소
        {
            SrcX = x / ScaValX;                 //순서2-1번 x/스케일 벡터값 역방향매핑이라 나누기임
            SrcY = y / ScaValY;                //순서2-1번
            out->sca.Near[y * DstStride + x] = NearesetNeighbor(ori, SrcX, SrcY, SrcWid, SrcHei, SrcStride); //순서 2-2번    rotation에 있는거 배껴도됨 rotation 완성했으면 그냥 불러오는거 rotation완성하면 scaling건들거 별로 없음 몇초면 구현할 수 있다
            out->sca.Bi  [y * DstStride + x] = Bilinear        (ori, SrcX, SrcY, SrcWid, SrcHei, SrcStride); //순서 2-3번
            out->sca.BS  [y * DstStride + x] = B_Spline        (ori, SrcX, SrcY, SrcWid, SrcHei, SrcStride); //순서 2-4번
            out->sca.Cu  [y * DstStride + x] = Cubic           (ori, SrcX, SrcY, SrcWid, SrcHei, SrcStride); //순서 2-5번
        }
}

void RotationProcess(UInt8* ori, Int32 DstWid, Int32 DstHei, Int32 DstPixNum, double Angle, OutputImage* out)
{
    Int32 DstStride = DstWid;
    Int32 MinP, MaxP;   /*EmptySpaceFlag*/

    bool EmptySpaceFlag;

    double SrcX, SrcY;
    double Seta;
    double pi = PI;
    int Cx = 256;
    int Cy = 256;
    FILE* fp;
    fopen_s(&fp, "ro360.raw", "wb");

    out->rot.Near = (UInt8*)calloc(DstPixNum, sizeof(UInt8));
    out->rot.Bi = (UInt8*)calloc(DstPixNum, sizeof(UInt8));
    out->rot.BS = (UInt8*)calloc(DstPixNum, sizeof(UInt8));
    out->rot.Cu = (UInt8*)calloc(DstPixNum, sizeof(UInt8));

    for (int i = 0; i <= 360; i+=4) {
        Seta = pi / 180.0 * i; //Angle: 입력받는 회전 각도
        for (Int32 y = 0; y < DstHei; y++)      //목적 영상 세로 화소
            for (Int32 x = 0; x < DstWid; x++)  //목적 영상 가로 화소
            {
                ///////////////////////////////////////////////////////////////////////////////////////
                SrcX = cos(Seta) * (x-Cx) + sin(Seta) * (y-Cy)+Cx;                                             // cos(Seta), sin(Seta) //순서 1-1번 역행렬 공식 여기에 쓰기 행렬의 곱으로 들어가야 함
                SrcY = -sin(Seta) * (x - Cx) + cos(Seta) * (y - Cy)+Cy;                                            //순서 1-1번

                MinP = (int)(SrcX + 0.5) <= (int)(SrcY + 0.5) ? (int)(SrcX + 0.5) : (int)(SrcY + 0.5);
                MaxP = (int)(SrcX + 0.5) <= (int)(SrcY + 0.5) ? (int)(SrcY + 0.5) : (int)(SrcX + 0.5);
                EmptySpaceFlag = (MinP < 0 || MaxP > 512) ? true : false;

                //out->rot.Near[y * DstStride + x] = EmptySpaceFlag ? 0 : NearesetNeighbor(ori, SrcX, SrcY, DstWid, DstHei, SrcStride); //순서 1-2번   Y*stride+x 그냥 y,x, stride로 쓰면 안됨 위에 보니까 y,x써도 될듯 이중 반복문에 변수명 확인하기
                out->rot.Bi[y * DstStride + x] = EmptySpaceFlag ? 0 : Bilinear(ori, SrcX, SrcY, DstWid, DstHei, SrcStride); //순서 1-3번   한번에 하지말고 하나씩
                //out->rot.BS[y * DstStride + x] = EmptySpaceFlag ? 0 : B_Spline(ori, SrcX, SrcY, DstWid, DstHei, SrcStride); //순서 1-4번
                //out->rot.Cu[y * DstStride + x] = EmptySpaceFlag ? 0 : Cubic(ori, SrcX, SrcY, DstWid, DstHei, SrcStride); //순서 1-5번(B스플라인 참고)
            }
        fwrite(out->rot.Bi, sizeof(UInt8), DstPixNum, fp);
    }
    fclose(fp);
}

void GeometricTransformation(InputImage* in, OutputImage* out)
{
    double ScaValX = ScaleFactorX;
    double ScaValY = ScaleFactorY;
    double Angle = AngleTheta;

    out->sca.info.wid = (double)in->info.wid * ScaValX + 0.5;               //스케일링 목적영상 가로 길이
    out->sca.info.hei = (double)in->info.hei * ScaValY + 0.5;               //스케일링 목적영상 세로 길이
    out->sca.info.ValidPixel = out->sca.info.wid * out->sca.info.hei;       //스케일링 목적영상 전체화소수

    out->rot.info.wid = in->info.wid;                                       //회전 목적영상 가로 길이
    out->rot.info.hei = in->info.hei;                                       //회전 목적영상 세로 길이
    out->rot.info.ValidPixel = out->rot.info.wid * out->rot.info.hei;       //회전 목적영상 전체화소수

    RotationProcess(in->Ori, out->rot.info.wid, out->rot.info.hei, out->rot.info.ValidPixel, Angle, out); //순서 1번 회전

    if (ScaValX < 1 || ScaValY < 1)                                                                       //순서 2번
    {
        free(in->Ori);
        Initialization(in, true);
    }

    ScalingProcess (in->Ori, in->info.wid, in->info.hei, out->sca.info.ValidPixel, ScaValX, ScaValY, out); //순서 2번
    
}