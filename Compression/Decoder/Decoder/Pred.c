#include "main.h"

typedef struct _refsample
{
	UInt8(*at)(refBuf, Int32, Int32);
}refSam;

UInt8 refSample(refBuf ref, Int32 side, Int32 pos)
{
	return ref.refPix[side][pos];
}

refBuf xFillReferenceSamples(UInt8* recPos, Int32 ctbX, Int32 ctbY, Int32 stride, Int32 BlkSize)
{
	Int32 StopX = stride / BlkSize;
	refBuf ref;
	recPos -= stride;
	recPos -= 1;

	////////////////////////////////////////////////////////////////////////////////////////////////
	//recPos �̿� ==> ref.refPix[][] = recPos[];                          //ref.refPix[0][]�� ��� ����ȭ��, ref.refPix[1][]�� ���� ����ȭ��
	if (ctbX == 0 && ctbY == 0) {	// ���� �� �� ���
		for (Int32 i = 0; i < BlkSize + 1; i++)
			ref.refPix[1][i] = 128;		// ����, �»��(��)
		for (Int32 i = 0; i < BlkSize * 2 + 1; i++)
			ref.refPix[0][i] = 128;		// �»��(��), ���, ����
	}
	else if (ctbX == 0) {	// ���� ��� ���(��� �� ����)
		for (Int32 i = 0; i < BlkSize + 1; i++)
			ref.refPix[1][i] = 128;		// ����, �»��(��)
		for (Int32 i = 1; i < BlkSize * 2 + 1; i++)
			ref.refPix[0][i] = recPos[i];	//���, ����
		ref.refPix[0][0] = 128;		// �»��(��) 
	}
	else if (ctbY == 0) {	// ù��° ��(���� �� ����)
		for (Int32 i = 0; i < BlkSize * 2 + 1; i++)
			ref.refPix[0][i] = 128; // �»��(��), ���, ����
		for (Int32 i = 1, k = stride; i < BlkSize + 1; i++, k += stride)
			ref.refPix[1][i] = recPos[k];	// ����
		ref.refPix[1][0] = 128;	// �»��(��)
	}
	else if (ctbX == StopX - 1) {	//���� �� ���(��� �� ����)
		for (Int32 i = BlkSize + 1; i < BlkSize * 2 + 1; i++)
			ref.refPix[0][i] = 128; // ����
		for (Int32 i = 0, k = 0; i < BlkSize + 1; i++, k += stride)
			ref.refPix[1][i] = recPos[k];	// ����, �»��(��)
		for (Int32 i = 0; i < BlkSize + 1; i++)
			ref.refPix[0][i] = recPos[i];	// �»��(��), ���
	}
	else {
		for (Int32 i = 0, k = 0; i < BlkSize + 1; i++, k += stride)
			ref.refPix[1][i] = recPos[k];	// ����, �»��(��)
		for (Int32 i = 0; i < BlkSize * 2 + 1; i++)
			ref.refPix[0][i] = recPos[i];	// �»��(��), ���, ����
	}
	////////////////////////////////////////////////////////////////////////////////////////////////

	for (Int32 i = BlkSize; i < BlkSize * 2; i++)
	{
		ref.refPix[1][i + 1] = 128;	// ���ϴ�
	}

	return ref;
}

void PredictionFunc(UInt8* Pred, refBuf ref, Int32 BlkSize, Int32 BlkStride, UInt8 mode)
{
	UInt8* refMain;
	UInt8* refSide;

	UInt8 refAbove[2 * BlockSize + 1];
	UInt8 refLeft[2 * BlockSize + 1];

	Bool bIsModeVer = (mode == HORIZONTAL || mode == HDIAGONAL) ? 0 : 1;
	Int32 intraPredAngle = mode == DIAGONAL ? 32 : mode == VDIAGONAL || mode == HDIAGONAL ? 32 : 0;
	UInt8 predBuf[BlockSize * BlockSize];

	refSam pSrc;
	pSrc.at = refSample;

	if (intraPredAngle >= 0)
	{
		for (Int32 x = 0; x <= BlkSize * 2; x++)
			refAbove[x] = pSrc.at(ref, 0, x);

		for (Int32 y = 0; y <= BlkSize * 2; y++)
			refLeft[y] = pSrc.at(ref, 1, y);

		refMain = bIsModeVer ? refAbove : refLeft;
		refSide = bIsModeVer ? refLeft : refAbove;
	}

	else
	{
		for (Int32 x = 0; x <= BlkSize; x++)
			refAbove[x + BlkSize] = pSrc.at(ref, 0, x);

		for (Int32 y = 0; y <= BlkSize; y++)
			refLeft[y + BlkSize] = pSrc.at(ref, 1, y);

		refMain = bIsModeVer ? refAbove + BlkSize : refLeft + BlkSize;
		refSide = bIsModeVer ? refLeft + BlkSize : refAbove + BlkSize;

		// Extend the Main reference to the left.
		Int32 sizeSide = BlkSize;
		Int32 sidestart = BlkSize;
		for (Int32 k = -sizeSide; k <= -1; k++)
		{
			refMain[k] = refSide[sidestart];
			sidestart -= 1;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	Int32 sum = 0;
	Pel* pDsty = predBuf;
	if (mode == PLANAR)
	{
		for (Int32 y = 0; y < BlkSize; y++) {
			for (Int32 x = 0; x < BlkSize; x++)
				pDsty[y * BlkStride + x] =
				((BlkSize - 1 - y) * refAbove[x] + (y + 1) * refLeft[BlkSize]
					+ (BlkSize - 1 - x) * refLeft[y] + (x + 1) * refAbove[BlkSize]
					+ BlkSize) >> 4;
		}
	}

	else if (mode == VERTICAL || mode == HORIZONTAL)
	{
		if (mode == VERTICAL) {
			for (Int32 y = 0; y < BlkSize; y++)
				for (Int32 x = 0; x < BlkSize; x++)
					pDsty[y * BlkStride + x] = refMain[x + 1];
		}
		else {
			for (Int32 y = 0; y < BlkSize; y++)
				for (Int32 x = 0; x < BlkSize; x++)
					pDsty[y * BlkStride + x] = refMain[y + 1];
		}
	}
	else if (mode == DIAGONAL || mode == VDIAGONAL || mode == HDIAGONAL)
	{
		if (mode == DIAGONAL) {
			for (Int32 y = 0; y < BlkSize; y++)
				for (Int32 x = 0; x < BlkSize; x++) {
					if (x >= y)
						pDsty[y * BlkStride + x] = refAbove[x - y];
					else
						pDsty[y * BlkStride + x] = refLeft[y - x];
				}
		}
		else if (mode == VDIAGONAL) {
			for (Int32 y = 0; y < BlkSize; y++)
				for (Int32 x = 0; x < BlkSize; x++)
					pDsty[y * BlkStride + x] = refAbove[x + y + 2];
		}
		else if (mode == HDIAGONAL) {
			for (Int32 y = 0; y < BlkSize; y++)
				for (Int32 x = 0; x < BlkSize; x++)
					pDsty[y * BlkStride + x] = refLeft[x + y + 2];
		}


	}
	else
	{
		for (Int32 y = 1; y < BlkSize + 1; y++)
			sum += refLeft[y];
		for (Int32 x = 1; x < BlkSize + 1; x++)
			sum += refAbove[x];
		sum = sum >> 4;
		for (Int32 y = 0; y < BlkSize; y++)
			for (Int32 x = 0; x < BlkSize; x++)
				pDsty[y * BlkStride + x] = sum;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////

	for (Int32 y = 0; y < BlkSize; y++)
		for (Int32 x = 0; x < BlkSize; x++)
			Pred[y * BlkStride + x] = predBuf[y * BlkStride + x]; //������� ��������
}