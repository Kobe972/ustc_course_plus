#include"bitstream.h"
typedef struct
{
	unsigned int weight;
	unsigned int parent, lchild, rchild;
}HTNode, *HuffmanTree;
typedef struct
{
	unsigned int weight;
	bitstream data;
}HTLeaf;
typedef bitstream* HuffmanCode;

void HuffmanCoding(HuffmanTree& HT, HuffmanCode& HC, HTLeaf* w, int n);
void CalcuFrequency(const char *filename, HTLeaf*& w, int& n);
void PrintTree(HuffmanTree HT);
void Select(HuffmanTree HT, int n, int& s1, int& s2);
void Zip(const char* filename);
void UnZip(const char* filename);
bitstream CodeUnit(HuffmanCode HC, HTLeaf* w, bitstream origin, int n);

struct ListNode
{
	HTLeaf data;
	ListNode* next;
};
class LeafList
{
public:
	LeafList();
	ListNode* head;
	int length;
	void OrderInsert(HTLeaf Node);
	void traverse(HTLeaf* w);
	HTLeaf& operator[](int i);
};