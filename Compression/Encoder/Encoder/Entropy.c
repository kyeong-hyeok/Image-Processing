#include "main.h"

#define MAX_ELEMENT 200

#if EntropyFlag
extern FILE* dp;

typedef struct TreeNode {
	int CNT;
	int ch;
	struct TreeNode* left;
	struct TreeNode* right;
} TreeNode;

typedef struct {
	TreeNode* ptree;
	int ch;
	int key;
} element;

typedef struct {
	element heap[MAX_ELEMENT];
	int heap_size;
} HeapType;

//생성 함수
HeapType* create()
{
	return (HeapType*)malloc(sizeof(HeapType));
}

//초기화 함수
void init(HeapType* h)
{
	h->heap_size = 0;
}

//현재 요소의 개수가 heap_size인 히프 h에 item을 삽입한다.
//삽입 함수
void insert_min_heap(HeapType* h, element item)
{
	int i;
	i = ++(h->heap_size);

	//트리를 거슬러 올라가면서 부모 노드와 비교하는 과정
	while ((i != 1) && (item.key < h->heap[i / 2].key)) {
		h->heap[i] = h->heap[i / 2];
		i /= 2;
	}
	h->heap[i] = item; // 새로운 노드 삽입
}

//삭제 함수
element delete_min_heap(HeapType* h)
{
	int parent, child;
	element item, temp;

	item = h->heap[1];
	temp = h->heap[(h->heap_size)--];
	parent = 1;
	child = 2;
	while (child <= h->heap_size) {
		//현재 노드의 자식노드중 더 작은 자식노드를 찾는다.
		if ((child < h->heap_size) && (h->heap[child].key) > h->heap[child + 1].key)
			child++;
		if (temp.key < h->heap[child].key) break;

		//한 단계 아래로 이동
		h->heap[parent] = h->heap[child];
		parent = child;
		child *= 2;
	}
	h->heap[parent] = temp;
	return item;
}

//이진 트리 생성 함수
TreeNode* make_tree(TreeNode* left, TreeNode* right) {
	TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
	node->left = left;
	node->right = right;
	return node;
}

//이진 트리 제거 함수
void destroy_tree(TreeNode* root)
{
	if (root == NULL) return;
	destroy_tree(root->left);
	destroy_tree(root->right);
	free(root);
}

int is_leaf(TreeNode* root)
{
	return !(root->left) && !(root->right);
}

void print_array(int codes[], int n, Int32 Idx, ImgBuf* img)
{
	for (int i = 0; i < n; i++)
	{
		img->Huff_codes[Idx] <<= 1;
		if (codes[i] == 1)
		{
			img->Huff_codes[Idx] |= 1;
		}
	}
	img->Huff_length[Idx] = n;
}

void print_codes(TreeNode* root, int codes[], int top, ImgBuf* img)
{
	//1을 저장하고 순환호출한다.
	if (root->left)
	{
		codes[top] = 1;
		print_codes(root->left, codes, top + 1, img);
	}
	//0을 저장하고 순환호출한다.
	if (root->left)
	{
		codes[top] = 0;
		print_codes(root->right, codes, top + 1, img);
	}
	//단말노드이면 코드를 출력한다.
	if (is_leaf(root))
	{
		Int32 Idx = 0;
		for (Int32 i = 0; i < 16; i++)
		{
			if (root->ch == img->Huff_ch[i])
			{
				Idx = i;
				break;
			}
		}
		print_array(codes, top, Idx, img);
	}
}

//허프만 코드 생성 함수
void huffman_tree(int freq[], int ch_list[], int n, ImgBuf* img)
{
	TreeNode* node, * x;
	HeapType* Set;
	element e, e1, e2;
	int codes[100];
	int top = 0;

	Set = create();
	init(Set);
	for (int i = 0; i < n; i++) {
		node = make_tree(NULL, NULL);
		e.ch = node->ch = ch_list[i];
		e.key = node->CNT = freq[i];
		e.ptree = node;
		insert_min_heap(Set, e);
	}

	for (int i = 1; i < n; i++) {
		//최소값을 가지는 두 개의 노드를 삭제
		e1 = delete_min_heap(Set);
		e2 = delete_min_heap(Set);

		//두개의 노드를 합친다. 
		x = make_tree(e1.ptree, e2.ptree);
		e.key = x->CNT = e1.key + e2.key;
		e.ptree = x;
		insert_min_heap(Set, e);
	}
	e = delete_min_heap(Set);
	print_codes(e.ptree, codes, top, img);
	destroy_tree(e.ptree);
	free(Set);
}

void Huffman(ImgBuf* img)
{
	Int32 size = 0;

	for (Int32 i = 0; i < 16; i++)
	{
		img->Huff_ch[i] = 10000;
		img->Huff_freq[i] = 0;
		img->Huff_codes[i] = 0;
		img->Huff_length[i] = 0;
	}
	img->Huff_Size = 0;

	for (Int32 i = 0; i < 16; i++)
	{
		Int32 flag = 0;
		img->Quant_blk[i];
		for (Int32 j = 0; j < 16; j++)
		{
			if (img->Quant_blk[i] == img->Huff_ch[j] && flag == 0)
			{
				flag = 1;
				img->Huff_freq[j]++;
			}
		}
		if (flag == 0)
		{
			img->Huff_ch[size] = img->Quant_blk[i];
			img->Huff_freq[size++]++;
		}
	}

	huffman_tree(img->Huff_freq, img->Huff_ch, size, img);
	img->Huff_Size = size;
}


void EncoderEntropyCoding(Int32 BLK, ImgBuf* Img)
{
	Int32 ALLZeroFlag = 1; //1인경우 Img->Quant_blk(양자화된 변환 블록 내 값)이 모두 0이 들어가 있음
	                       //0인경우 Img->Quant_blk(양자화된 변환 블록 내 값) 중 0이 아닌 값이 들어가 있음
	UInt8 Bitstream = 0;
	UInt8 BitCnt = 0;
	UInt8 tmp = 0;

	UInt8 CurInfo = 0; //현재 비트스트림으로 만들고자 하는 정보
	///////////////////////////////////////////////////////////////////////////////////
	//Img->Best_Mode 저장

	if (Img->Best_Mode == 0) {
		for (Int32 i = 0; i < 2; i++) {
			Bitstream <<= 1;
			Bitstream = Bitstream | 0;
			BitCnt++;
			if (BitCnt == 8) {
				fwrite(&Bitstream, sizeof(UInt8), 1, dp);
				Bitstream = 0;
				BitCnt = 0;
			}
		}
	}
	else {
		CurInfo = Img->Best_Mode + 1;
		CurInfo <<= 5;
		for (Int32 i = 0; i < 3; i++) {
			Bitstream <<= 1;
			tmp = CurInfo & 128;
			tmp >>= 7;
			Bitstream = Bitstream | tmp;
			BitCnt++;
			if (BitCnt == 8) {
				fwrite(&Bitstream, sizeof(UInt8), 1, dp);
				Bitstream = 0;
				BitCnt = 0;
			}
			CurInfo <<= 1;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////
	
	for (Int32 i = 0; i < BLK * BLK; i++)
		if (Img->Quant_blk[i])
		{
			ALLZeroFlag = 0;
			break;
		}

	if (ALLZeroFlag) //양자화 블록 내 화소가 모두 0인 경우
	{
		Bitstream <<= 1;
		Bitstream = Bitstream | 1;
		BitCnt++;
		Int32 cnt = BitCnt;
		for (Int32 i = 0; i < 8 - cnt; i++) {
			Bitstream <<= 1;
			Bitstream = Bitstream | 0;
			BitCnt++;
		}
		if (BitCnt == 8) {
			fwrite(&Bitstream, sizeof(UInt8), 1, dp);
			Bitstream = 0;
			BitCnt = 0;
		}
	}

	else //양자화 블록 내 0이 화소가 있는 경우
	{
		Int32 QuantBuf[BlockSize * BlockSize / 4] = { 0 };
		Int32 SuffixCnt = 0, PrefixCnt = 0;
		Int32 cnt = 0;	// 개수 저장
		for (Int32 i = 0; i < BLK * BLK; i++)
		{
			QuantBuf[i] = Img->Quant_blk[i];
			Img->Quant_blk[i] = abs(Img->Quant_blk[i]);
		}

		Huffman(Img);
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////Img->Huff_Size
		CurInfo = Img->Huff_Size + 1;
		for (Int32 i = 0; i < 8; i++) {
			tmp = CurInfo & 128;
			if (tmp == 128) {
				SuffixCnt = 7 - i;
				PrefixCnt = SuffixCnt;
				break;
			}
			CurInfo <<= 1;
		}

		for (Int32 i = 0; i < PrefixCnt; i++) {
			Bitstream <<= 1;
			Bitstream = Bitstream | 0;
			BitCnt++;
			if (BitCnt == 8) {
				fwrite(&Bitstream, sizeof(UInt8), 1, dp);
				Bitstream = 0;
				BitCnt = 0;
			}
		}
		CurInfo = Img->Huff_Size + 1;
		CurInfo <<= (8 - SuffixCnt - 1);
		for (Int32 i = 0; i < SuffixCnt+1; i++) {
			Bitstream <<= 1;
			tmp = CurInfo & 128;
			tmp >>= 7;
			Bitstream = Bitstream | tmp;
			BitCnt++;
			if (BitCnt == 8) {
				fwrite(&Bitstream, sizeof(UInt8), 1, dp);
				Bitstream = 0;
				BitCnt = 0;
			}
			CurInfo <<= 1;
		}
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////Img->Huff_ch, Img->Huff_freq
		for (Int32 i = 0; i < Img->Huff_Size; i++)
		{
			CurInfo = Img->Huff_ch[i] + 1;
			SuffixCnt = 0, PrefixCnt = 0;
			for (Int32 j = 0; j < 8; j++) {
				tmp = CurInfo & 128;
				if (tmp == 128) {
					SuffixCnt = 7 - j;
					PrefixCnt = SuffixCnt;
					break;
				}
				CurInfo <<= 1;
			}
			for (Int32 j = 0; j < PrefixCnt; j++) {
				Bitstream <<= 1;
				Bitstream = Bitstream | 0;
				BitCnt++;
				if (BitCnt == 8) {
					fwrite(&Bitstream, sizeof(UInt8), 1, dp);
					Bitstream = 0;
					BitCnt = 0;
				}
			}
			CurInfo = Img->Huff_ch[i] + 1;
			CurInfo <<= (8 - SuffixCnt - 1);
			for (Int32 j = 0; j < SuffixCnt+1; j++) {
				Bitstream <<= 1;
				tmp = CurInfo & 128;
				tmp >>= 7;
				Bitstream = Bitstream | tmp;
				BitCnt++;
				if (BitCnt == 8) {
					fwrite(&Bitstream, sizeof(UInt8), 1, dp);
					Bitstream = 0;
					BitCnt = 0;
				}
				CurInfo <<= 1;
			}
			

			CurInfo = Img->Huff_freq[i] + 1;
			PrefixCnt = 0;
			SuffixCnt = 0;
			for (Int32 j = 0; j < 8; j++) {
				tmp = CurInfo & 128;
				if (tmp == 128) {
					SuffixCnt = 7 - j;
					PrefixCnt = SuffixCnt;
					break;
				}
				CurInfo <<= 1;
			}
			for (Int32 j = 0; j < PrefixCnt; j++) {
				Bitstream <<= 1;
				Bitstream = Bitstream | 0;
				BitCnt++;
				if (BitCnt == 8) {
					fwrite(&Bitstream, sizeof(UInt8), 1, dp);
					Bitstream = 0;
					BitCnt = 0;
				}
			}
			CurInfo = Img->Huff_freq[i] + 1;
			CurInfo <<= (8 - SuffixCnt - 1);
			for (Int32 j = 0; j < SuffixCnt+1; j++) {
				Bitstream <<= 1;
				tmp = CurInfo & 128;
				tmp >>= 7;
				Bitstream = Bitstream | tmp;
				BitCnt++;
				if (BitCnt == 8) {
					fwrite(&Bitstream, sizeof(UInt8), 1, dp);
					Bitstream = 0;
					BitCnt = 0;
				}
				CurInfo <<= 1;
			}
			
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////img->Quant_blk[i]
		Int32 k = 0;
		for (Int32 i = 0; i < BLK * BLK; i++)
		{
			for (Int32 j = 0; j < Img->Huff_Size; j++) {
				if (Img->Huff_ch[j] == Img->Quant_blk[i]) {
					k = j;
					break;
				}
			}
			CurInfo = Img->Huff_codes[k];
			CurInfo <<= (8 - Img->Huff_length[k]);
			for (Int32 j = 0; j < Img->Huff_length[k]; j++) {
				Bitstream <<= 1;
				tmp = CurInfo & 128;
				tmp >>= 7;
				Bitstream = Bitstream | tmp;
				BitCnt++;
				if (BitCnt == 8) {
					fwrite(&Bitstream, sizeof(UInt8), 1, dp);
					Bitstream = 0;
					BitCnt = 0;
				}
				CurInfo <<= 1;
			}
			
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////+,- 부호
		for (Int32 i = 0; i < BLK * BLK; i++)
		{
			CurInfo = QuantBuf[i] >= 0 ? 0 : 1;
			Bitstream <<= 1;
			Bitstream = Bitstream | CurInfo;
			BitCnt++;
			if (BitCnt == 8) {
				fwrite(&Bitstream, sizeof(UInt8), 1, dp);
				Bitstream = 0;
				BitCnt = 0;
			}
		}


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		cnt = 8 - BitCnt;
		for (Int32 i = 0; i < cnt; i++) {
			Bitstream <<= 1;
			Bitstream = Bitstream | 0;
			BitCnt++;
			if (BitCnt == 8) {
				fwrite(&Bitstream, sizeof(UInt8), 1, dp);
				Bitstream = 0;
				BitCnt = 0;
			}
		}

	}
}
#endif