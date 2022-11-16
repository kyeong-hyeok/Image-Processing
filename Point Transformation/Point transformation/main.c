#include "main.h"
#include "Filter.h"
#include "Output.h"

void main()
{
    InputImage in;         // �Է� ���� ����ü
    OutputImage out;       // Arithmetic ��� ���� ����ü
    HistModiImage modi;    // HistModi   ��� ���� ����ü
  
    Initialization(&in, &out);                                          // �������� �о���� �� �����Ҵ�
    ArithmeticVer1(in.Ori, in.info.wid, in.info.hei, &out);             // Arithmetic operation (+, -, x, /)
    modi = HistogramModification(in.Ori, in.info.wid, in.info.hei);     // Equalization, End-in-search
    OutputHistogram(&in, &out, &modi);   // �ܰ躰�� �����ϰ� ������ �ּ�ó��                               // ��� Histogram ���
    Destroy(&in, &out, &modi);                                          // ���� �Ҵ� ����
}