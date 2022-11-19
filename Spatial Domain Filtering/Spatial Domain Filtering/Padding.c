#include "Filter.h"

void ImagePadding(UInt8* Data, UInt8* Tmp, Int32 wid, Int32 hei, Int32 MaskSize) // 마스크 안쓰는거 블록 3개면 마스크 사이즈 3
{
	Int32 AddSize = (MaskSize / 2) * 2;
	Int32 jump = MaskSize / 2;
	Int32 Stride = wid;
	Int32 PStride = wid + AddSize;

	UInt8 InitValue = 0; 
	//////////////////////////////////////////////////////////////////////////////
	InitValue = 0; //Half padding 코드 작성
	for (Int32 y = 0; y < AddSize / 2; y++) {             //패딩 버퍼 전체 초기화 초기화만 하면 됨 3x3블록 상하좌우 1줄씩 수평 수직으로 2줄씩 추가 514x514가 됨 일반화 잘하기 초기화
		for (Int32 x = 0; x < wid+AddSize; x++) {
			Tmp[y * PStride + x] = InitValue;
		}
	}
	for (Int32 y = AddSize / 2; y < hei + AddSize/2; y++) {             //패딩 버퍼 전체 초기화 초기화만 하면 됨 3x3블록 상하좌우 1줄씩 수평 수직으로 2줄씩 추가 514x514가 됨 일반화 잘하기 초기화
		for(Int32 x=0; x<AddSize/2; x++) 
			Tmp[y * PStride + x] = InitValue;
		for(Int32 x=wid + AddSize/2; x<wid +AddSize; x++)
			Tmp[y * PStride + x] = InitValue;
	}
	for (Int32 y = hei + AddSize - 1; y > hei + AddSize/2 - 1; y--) {             //패딩 버퍼 전체 초기화 초기화만 하면 됨 3x3블록 상하좌우 1줄씩 수평 수직으로 2줄씩 추가 514x514가 됨 일반화 잘하기 초기화
		for (Int32 x = 0; x < wid+AddSize; x++) {
			Tmp[y * PStride + x] = InitValue;
		}
	}
	/*** 원본 버퍼를 패딩 버퍼에 저장 ***/
	for (Int32 y = 0; y < hei; y++)
	{
		for (Int32 x = 0; x < wid; x++)
		{
			Tmp[(y + jump) * PStride + (x + jump)] = Data[y * Stride + x];
		}
	}
}