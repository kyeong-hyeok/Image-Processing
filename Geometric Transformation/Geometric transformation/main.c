#include "main.h"

Int32 SrcStride = WIDTH;

void main()
{
    InputImage in;         // �Է� ���� ����ü
    OutputImage out;       // ��� ���� ����ü

    Initialization(&in, false);                                   // �������� �о���� �� �����Ҵ�
    GeometricTransformation(&in, &out);
    Output(&out);                                                // ��� ���� ���
    Destroy(&in, &out);                                          // ��� ���� �Ҵ� ����
}