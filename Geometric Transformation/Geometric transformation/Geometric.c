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

    for (Int32 y = 0; y < out->sca.info.hei; y++)      //���� ���� ���� ȭ��
        for (Int32 x = 0; x < out->sca.info.wid; x++)  //���� ���� ���� ȭ��
        {
            SrcX = x / ScaValX;                 //����2-1�� x/������ ���Ͱ� ����������̶� ��������
            SrcY = y / ScaValY;                //����2-1��
            out->sca.Near[y * DstStride + x] = NearesetNeighbor(ori, SrcX, SrcY, SrcWid, SrcHei, SrcStride); //���� 2-2��    rotation�� �ִ°� �貸���� rotation �ϼ������� �׳� �ҷ����°� rotation�ϼ��ϸ� scaling�ǵ�� ���� ���� ���ʸ� ������ �� �ִ�
            out->sca.Bi  [y * DstStride + x] = Bilinear        (ori, SrcX, SrcY, SrcWid, SrcHei, SrcStride); //���� 2-3��
            out->sca.BS  [y * DstStride + x] = B_Spline        (ori, SrcX, SrcY, SrcWid, SrcHei, SrcStride); //���� 2-4��
            out->sca.Cu  [y * DstStride + x] = Cubic           (ori, SrcX, SrcY, SrcWid, SrcHei, SrcStride); //���� 2-5��
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
        Seta = pi / 180.0 * i; //Angle: �Է¹޴� ȸ�� ����
        for (Int32 y = 0; y < DstHei; y++)      //���� ���� ���� ȭ��
            for (Int32 x = 0; x < DstWid; x++)  //���� ���� ���� ȭ��
            {
                ///////////////////////////////////////////////////////////////////////////////////////
                SrcX = cos(Seta) * (x-Cx) + sin(Seta) * (y-Cy)+Cx;                                             // cos(Seta), sin(Seta) //���� 1-1�� ����� ���� ���⿡ ���� ����� ������ ���� ��
                SrcY = -sin(Seta) * (x - Cx) + cos(Seta) * (y - Cy)+Cy;                                            //���� 1-1��

                MinP = (int)(SrcX + 0.5) <= (int)(SrcY + 0.5) ? (int)(SrcX + 0.5) : (int)(SrcY + 0.5);
                MaxP = (int)(SrcX + 0.5) <= (int)(SrcY + 0.5) ? (int)(SrcY + 0.5) : (int)(SrcX + 0.5);
                EmptySpaceFlag = (MinP < 0 || MaxP > 512) ? true : false;

                //out->rot.Near[y * DstStride + x] = EmptySpaceFlag ? 0 : NearesetNeighbor(ori, SrcX, SrcY, DstWid, DstHei, SrcStride); //���� 1-2��   Y*stride+x �׳� y,x, stride�� ���� �ȵ� ���� ���ϱ� y,x�ᵵ �ɵ� ���� �ݺ����� ������ Ȯ���ϱ�
                out->rot.Bi[y * DstStride + x] = EmptySpaceFlag ? 0 : Bilinear(ori, SrcX, SrcY, DstWid, DstHei, SrcStride); //���� 1-3��   �ѹ��� �������� �ϳ���
                //out->rot.BS[y * DstStride + x] = EmptySpaceFlag ? 0 : B_Spline(ori, SrcX, SrcY, DstWid, DstHei, SrcStride); //���� 1-4��
                //out->rot.Cu[y * DstStride + x] = EmptySpaceFlag ? 0 : Cubic(ori, SrcX, SrcY, DstWid, DstHei, SrcStride); //���� 1-5��(B���ö��� ����)
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

    out->sca.info.wid = (double)in->info.wid * ScaValX + 0.5;               //�����ϸ� �������� ���� ����
    out->sca.info.hei = (double)in->info.hei * ScaValY + 0.5;               //�����ϸ� �������� ���� ����
    out->sca.info.ValidPixel = out->sca.info.wid * out->sca.info.hei;       //�����ϸ� �������� ��üȭ�Ҽ�

    out->rot.info.wid = in->info.wid;                                       //ȸ�� �������� ���� ����
    out->rot.info.hei = in->info.hei;                                       //ȸ�� �������� ���� ����
    out->rot.info.ValidPixel = out->rot.info.wid * out->rot.info.hei;       //ȸ�� �������� ��üȭ�Ҽ�

    RotationProcess(in->Ori, out->rot.info.wid, out->rot.info.hei, out->rot.info.ValidPixel, Angle, out); //���� 1�� ȸ��

    if (ScaValX < 1 || ScaValY < 1)                                                                       //���� 2��
    {
        free(in->Ori);
        Initialization(in, true);
    }

    ScalingProcess (in->Ori, in->info.wid, in->info.hei, out->sca.info.ValidPixel, ScaValX, ScaValY, out); //���� 2��
    
}