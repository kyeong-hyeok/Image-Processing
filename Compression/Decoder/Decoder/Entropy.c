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

//���� �Լ�
HeapType* create()
{
	return (HeapType*)malloc(sizeof(HeapType));
}

//�ʱ�ȭ �Լ�
void init(HeapType* h)
{
	h->heap_size = 0;
}

//���� ����� ������ heap_size�� ���� h�� item�� �����Ѵ�.
//���� �Լ�
void insert_min_heap(HeapType* h, element item)
{
	int i;
	i = ++(h->heap_size);

	//Ʈ���� �Ž��� �ö󰡸鼭 �θ� ���� ���ϴ� ����
	while ((i != 1) && (item.key < h->heap[i / 2].key)) {
		h->heap[i] = h->heap[i / 2];
		i /= 2;
	}
	h->heap[i] = item; // ���ο� ��� ����
}

//���� �Լ�
element delete_min_heap(HeapType* h)
{
	int parent, child;
	element item, temp;

	item = h->heap[1];
	temp = h->heap[(h->heap_size)--];
	parent = 1;
	child = 2;
	while (child <= h->heap_size) {
		//���� ����� �ڽĳ���� �� ���� �ڽĳ�带 ã�´�.
		if ((child < h->heap_size) && (h->heap[child].key) > h->heap[child + 1].key)
			child++;
		if (temp.key < h->heap[child].key) break;

		//�� �ܰ� �Ʒ��� �̵�
		h->heap[parent] = h->heap[child];
		parent = child;
		child *= 2;
	}
	h->heap[parent] = temp;
	return item;
}

//���� Ʈ�� ���� �Լ�
TreeNode* make_tree(TreeNode* left, TreeNode* right) {
	TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
	node->left = left;
	node->right = right;
	return node;
}

//���� Ʈ�� ���� �Լ�
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

void print_array(int codes[], int n, Int32 Idx, ImgBuf* Img)
{
	for (int i = 0; i < n; i++)
	{
		Img->Huff_codes[Idx] <<= 1;
		if (codes[i] == 1)
		{
			Img->Huff_codes[Idx] |= 1;
		}
	}
	Img->Huff_length[Idx] = n;
}

void print_codes(TreeNode* root, int codes[], int top, ImgBuf* Img)
{
	//1�� �����ϰ� ��ȯȣ���Ѵ�.
	if (root->left)
	{
		codes[top] = 1;
		print_codes(root->left, codes, top + 1, Img);
	}
	//0�� �����ϰ� ��ȯȣ���Ѵ�.
	if (root->left)
	{
		codes[top] = 0;
		print_codes(root->right, codes, top + 1, Img);
	}
	//�ܸ�����̸� �ڵ带 ����Ѵ�.
	if (is_leaf(root))
	{
		Int32 Idx = 0;
		for (Int32 i = 0; i < 16; i++)
		{
			if (root->ch == Img->Huff_ch[i])
			{
				Idx = i;
				break;
			}
		}
		print_array(codes, top, Idx, Img);
	}
}

//������ �ڵ� ���� �Լ�
void huffman_tree(int freq[], int ch_list[], int n, ImgBuf* Img)
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
		//�ּҰ��� ������ �� ���� ��带 ����
		e1 = delete_min_heap(Set);
		e2 = delete_min_heap(Set);

		//�ΰ��� ��带 ��ģ��. 
		x = make_tree(e1.ptree, e2.ptree);
		e.key = x->CNT = e1.key + e2.key;
		e.ptree = x;
		insert_min_heap(Set, e);
	}
	e = delete_min_heap(Set);
	print_codes(e.ptree, codes, top, Img);
	destroy_tree(e.ptree);
	free(Set);
}

void Huffman(ImgBuf* img)
{
	Int32 size = img->Huff_Size;

	huffman_tree(img->Huff_freq, img->Huff_ch, size, img);
}

void DecoderEntropyCoding(Int32 BLK, ImgBuf* Img)
{
	Int32 ALLZeroFlag = 0; //1�ΰ�� Img->Quant_blk(����ȭ�� ��ȯ ��� �� ��)�� ��� 0�� �� ����
						   //0�ΰ�� Img->Quant_blk(����ȭ�� ��ȯ ��� �� ��) �� 0�� �ƴ� ���� �� ����
	UInt8 Bitstream = 0;
	UInt8 BitCnt = 0;
	UInt8 tmp = 0;

	UInt8 CurInfo = 0;

	Int32 CoeffNum = 0;
	Int32 LenthNum = 0;
	Int32 cnt = 0;
	fread(&Bitstream, sizeof(UInt8), 1, dp);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////������� ���� ����
	////
	if ((Bitstream & 128) == 0) {
		BitCnt++;
		Bitstream <<= 1;
		if ((Bitstream & 128) == 0) {
			BitCnt++;
			Bitstream <<= 1;
			Img->Best_Mode = 0;
		}
		else {
			BitCnt++;
			Bitstream <<= 1;
			if ((Bitstream & 128) == 0)
				Img->Best_Mode = 1;
			else
				Img->Best_Mode = 2;
			BitCnt++;
			Bitstream <<= 1;
		}
	}
	else {
		BitCnt++;
		Bitstream <<= 1;
		if ((Bitstream & 128) == 0) {
			BitCnt++;
			Bitstream <<= 1;
			if ((Bitstream & 128) == 0)
				Img->Best_Mode = 3;
			else
				Img->Best_Mode = 4;
			BitCnt++;
			Bitstream <<= 1;
		}
		else {
			BitCnt++;
			Bitstream <<= 1;
			if ((Bitstream & 128) == 0)
				Img->Best_Mode = 5;
			else
				Img->Best_Mode = 6;
			BitCnt++;
			Bitstream <<= 1;
		}
	}
	////������� ����
	//Img->Best_Mode = 
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////������� ���� ��
	CurInfo = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////����ȭ ��� ���� ����
	////ALLZeroFlag ����
	CurInfo = Bitstream & 128;
	if (CurInfo) {
		ALLZeroFlag = 1; //��� 0�� ���
		for (Int32 i = 0; i < BLK * BLK; i++)
			Img->Quant_blk[i] = 0;
	}
	
	if (!ALLZeroFlag) //0�� �ƴ� ȭ�Ұ� �ִ� ���
	{
		Int32 SuffixCnt=0, PrefixCnt=0;
		for (Int32 i = 0; i < 16; i++) //�ʱ�ȭ ���� //�ǵ��� ����
		{
			Img->Huff_ch[i] = 10000;
			Img->Huff_freq[i] = 0;
			Img->Huff_codes[i] = 0;
			Img->Huff_length[i] = 0;
		}
		Img->Huff_Size = 0; //�ʱ�ȭ ���� //�ǵ��� ����
		 
		
		CurInfo = 0;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Img->Huff_Size ��������
		for (Int32 i = 0; i < 8; i++) {
			tmp = Bitstream & 128;
			Bitstream <<= 1;
			BitCnt++;
			if (BitCnt == 8) {
				fread(&Bitstream, sizeof(UInt8), 1, dp);
				BitCnt = 0;
			}
			if (tmp == 128) {
				PrefixCnt = i;
				SuffixCnt = PrefixCnt;
				break;
			}
		}
		CurInfo = 1;
		for (Int32 i = 0; i < SuffixCnt; i++) {
			CurInfo <<= 1;
			tmp = Bitstream & 128;
			tmp >>= 7;
			CurInfo = CurInfo | tmp;
			Bitstream <<= 1;
			BitCnt++;
			if (BitCnt == 8) {
				fread(&Bitstream, sizeof(UInt8), 1, dp);
				BitCnt = 0;
			}
		}
		Img->Huff_Size = CurInfo - 1;
		
		//Img->Huff_Size = CurInfo;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Img->Huff_ch, Img->Huff_freq ��������
		CurInfo = 0;
		for (Int32 i = 0; i < Img->Huff_Size; i++)
		{
			PrefixCnt = 0;
			SuffixCnt = 0;
			for (Int32 j = 0; j < 8; j++) {
				tmp = Bitstream & 128;
				Bitstream <<= 1;
				BitCnt++;
				if (BitCnt == 8) {
					fread(&Bitstream, sizeof(UInt8), 1, dp);
					BitCnt = 0;
				}
				if (tmp == 128) {
					PrefixCnt = j;
					SuffixCnt = PrefixCnt;
					break;
				}
			}
			CurInfo = 1;
			for (Int32 j = 0; j < SuffixCnt; j++) {
				CurInfo <<= 1;
				tmp = Bitstream & 128;
				tmp >>= 7;
				CurInfo = CurInfo | tmp;
				Bitstream <<= 1;
				BitCnt++;
				if (BitCnt == 8) {
					fread(&Bitstream, sizeof(UInt8), 1, dp);
					BitCnt = 0;
				}
			}
			Img->Huff_ch[i] = CurInfo-1;
			

			CurInfo = 0;
			PrefixCnt = 0;
			SuffixCnt = 0;
			for (Int32 j = 0; j < 8; j++) {
				tmp = Bitstream & 128;
				tmp >>= 7;
				Bitstream <<= 1;
				BitCnt++;
				if (BitCnt == 8) {
					fread(&Bitstream, sizeof(UInt8), 1, dp);
					BitCnt = 0;
				}
				if (tmp == 1) {
					PrefixCnt = j;
					SuffixCnt = PrefixCnt;
					break;
				}
			}
			CurInfo = 1;
			for (Int32 j = 0; j < SuffixCnt; j++) {
				CurInfo <<= 1;
				tmp = Bitstream & 128;
				tmp >>= 7;
				CurInfo = CurInfo | tmp;
				Bitstream <<= 1;
				BitCnt++;
				if (BitCnt == 8) {
					fread(&Bitstream, sizeof(UInt8), 1, dp);
					BitCnt = 0;
				}
			}
			Img->Huff_freq[i] = CurInfo - 1;
			
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Huffman(Img);
		//����ȭ ȭ�Ұ� ����
		for (Int32 i = 0; i < BLK * BLK; i++)
		{
			CurInfo = 0;
			Int32 find = 0;
			for (Int32 j = 0; j < 8; j++) {
				CurInfo <<= 1;
				tmp = Bitstream & 128;
				tmp >>= 7;
				CurInfo = CurInfo | tmp;
				BitCnt++;
				Bitstream <<= 1;
				if (BitCnt == 8) {
					fread(&Bitstream, sizeof(UInt8), 1, dp);
					BitCnt = 0;
				}
				for (Int32 k = 0; k < Img->Huff_Size; k++) {
					if ((Img->Huff_codes[k] == CurInfo) && (Img->Huff_length[k] == (j + 1))) {
						Img->Quant_blk[i] = Img->Huff_ch[k];
						find = 1;
						break;
					}
				}
				if (find) break;
			}
			
		}
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////+,- ��ȣ
		CurInfo = 0;
		for (Int32 i = 0; i < BLK * BLK; i++)
		{
			CurInfo = Bitstream & 128;
			if (CurInfo == 128)
				Img->Quant_blk[i] *= -1;
			Bitstream <<= 1;
			BitCnt++;
			if (BitCnt == 8) {
				fread(&Bitstream, sizeof(UInt8), 1, dp);
				BitCnt = 0;
			}
		}


	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////����ȭ ��� ���� ��
}
#endif