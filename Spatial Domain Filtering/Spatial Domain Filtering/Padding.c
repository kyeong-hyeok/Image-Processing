#include "Filter.h"

void ImagePadding(UInt8* Data, UInt8* Tmp, Int32 wid, Int32 hei, Int32 MaskSize) // ����ũ �Ⱦ��°� ��� 3���� ����ũ ������ 3
{
	Int32 AddSize = (MaskSize / 2) * 2;
	Int32 jump = MaskSize / 2;
	Int32 Stride = wid;
	Int32 PStride = wid + AddSize;

	UInt8 InitValue = 0; 
	//////////////////////////////////////////////////////////////////////////////
	InitValue = 0; //Half padding �ڵ� �ۼ�
	for (Int32 y = 0; y < AddSize / 2; y++) {             //�е� ���� ��ü �ʱ�ȭ �ʱ�ȭ�� �ϸ� �� 3x3��� �����¿� 1�پ� ���� �������� 2�پ� �߰� 514x514�� �� �Ϲ�ȭ ���ϱ� �ʱ�ȭ
		for (Int32 x = 0; x < wid+AddSize; x++) {
			Tmp[y * PStride + x] = InitValue;
		}
	}
	for (Int32 y = AddSize / 2; y < hei + AddSize/2; y++) {             //�е� ���� ��ü �ʱ�ȭ �ʱ�ȭ�� �ϸ� �� 3x3��� �����¿� 1�پ� ���� �������� 2�پ� �߰� 514x514�� �� �Ϲ�ȭ ���ϱ� �ʱ�ȭ
		for(Int32 x=0; x<AddSize/2; x++) 
			Tmp[y * PStride + x] = InitValue;
		for(Int32 x=wid + AddSize/2; x<wid +AddSize; x++)
			Tmp[y * PStride + x] = InitValue;
	}
	for (Int32 y = hei + AddSize - 1; y > hei + AddSize/2 - 1; y--) {             //�е� ���� ��ü �ʱ�ȭ �ʱ�ȭ�� �ϸ� �� 3x3��� �����¿� 1�پ� ���� �������� 2�پ� �߰� 514x514�� �� �Ϲ�ȭ ���ϱ� �ʱ�ȭ
		for (Int32 x = 0; x < wid+AddSize; x++) {
			Tmp[y * PStride + x] = InitValue;
		}
	}
	/*** ���� ���۸� �е� ���ۿ� ���� ***/
	for (Int32 y = 0; y < hei; y++)
	{
		for (Int32 x = 0; x < wid; x++)
		{
			Tmp[(y + jump) * PStride + (x + jump)] = Data[y * Stride + x];
		}
	}
}