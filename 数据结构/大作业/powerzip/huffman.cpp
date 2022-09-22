#include"huffman.h"
#include<stdlib.h>
#include<string.h>
void HuffmanCoding(HuffmanTree& HT, HuffmanCode& HC, HTLeaf* w, int n)
{
	if (n <= 1) return;
	int m = 2 * n - 1, i;
	HuffmanTree p;
	HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode));
	for (p = HT + 1, i = 1; i <= n; ++i, ++p, ++w) *p = { w->weight,0,0,0 };
	for (; i <= m; ++i, ++p) *p = { 0,0,0,0 };
	for (i = n + 1; i <= m; ++i)
	{
		int s1, s2;
		Select(HT, i - 1, s1, s2);
		HT[s1].parent = i;
		HT[s2].parent = i;
		HT[i].lchild = s1;
		HT[i].rchild = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
		HT[s1].weight = 2147483647;
		HT[s2].weight = 2147483647;
	}
	HC = (HuffmanCode)malloc((n + 1) * sizeof(bitstream));
	char *cd = (char*)malloc(n * sizeof(char));
	cd[n - 1] = '\0';
	for (i = 1; i <= n; ++i)
	{
		int start = n - 1;
		for (int c = i, f = HT[i].parent; f != 0; c = f, f = HT[f].parent)
		{
			if (HT[f].lchild == c) cd[--start] = '0';
			else cd[--start] = '1';
		}
		HC[i] = &cd[start];
	}
}
void CalcuFrequency(const char* filename, HTLeaf*& w, int& n)
{
	bitstream content;
	LeafList FreqList;
	content.read(filename);
	for (int i = 0; i < content.length / 8; i++)
	{
		bitstream fragment = content.slice(i, i + 1);
		HTLeaf Node = { 1,fragment };
		FreqList.OrderInsert(Node);
	}
	n = FreqList.length;
	w = (HTLeaf*)malloc((n + 1)*sizeof(HTLeaf));
	FreqList.traverse(w);
	content.Free();
}
void Select(HuffmanTree HT, int n, int& s1, int& s2)
{
	int min1 = 2147483647, min2 = 2147483647;
	s1 = s2 = 0;
	for (int i = 1; i <= n; i++)
	{
		if (min1 > HT[i].weight)
		{
			min1 = HT[i].weight;
			s1 = i;
		}
	}
	for (int i = 1; i <= n; i++)
	{
		if (i == s1) continue;
		if (min2 >= HT[i].weight)
		{
			min2 = HT[i].weight;
			s2 = i;
		}
	}
}

LeafList::LeafList()
{
	head = new ListNode;
	head->data = { 0,bitstream() };
	head->next = NULL;
	length = 0;
}

void LeafList::OrderInsert(HTLeaf Node)
{
	ListNode* NewNode = new ListNode;
	NewNode->data.weight = Node.weight;
	NewNode->data.data = Node.data;
	for (ListNode* p = head; p; p = p->next)
	{
		if (p != head && p->data.data == NewNode->data.data)
		{
			p->data.weight++;
			break;
		}
		else if (p->next == NULL)
		{
			p->next = NewNode;
			NewNode->next = NULL;
			length++;
			break;
		}
		else if (p == head && NewNode->data.data < p->next->data.data)
		{
			NewNode->next = p->next;
			p->next = NewNode;
			length++;
			break;
		}
		else if (p != head && p->data.data < NewNode->data.data && NewNode->data.data < p->next->data.data)
		{
			NewNode->next = p->next;
			p->next = NewNode;
			length++;
			break;
		}
	}
}

void LeafList::traverse(HTLeaf* w)
{
	int i = 0;
	for (ListNode* p=head->next;p;p=p->next)
	{
		w[i].weight = p->data.weight;
		w[i].data = p->data.data;
		i++;
	}
}

HTLeaf& LeafList::operator[](int i)
{
	ListNode* p = head->next;
	for (int j = 0; j < i; j++)
	{
		if (!p) break;
		p = p->next;
	}
	return p->data;
}

bitstream CodeUnit(HuffmanCode HC, HTLeaf* w, bitstream origin, int n)
{
	int low = 0, high = n - 1, mid = (low + high) / 2;
	while (low < high)
	{
		if (w[mid].data < origin)
		{
			low = mid + 1;
			mid = (low + high) / 2;
		}
		else if (origin < w[mid].data)
		{
			high = mid - 1;
			mid = (low + high) / 2;
		}
		else break;
	}
	return HC[mid + 1];
}

void Zip(const char* filename)
{
	HTLeaf* w;
	int n;
	HuffmanCode HC;
	HuffmanTree HT;
	bitstream content, result;
	cout << "Calculating frequency..." << endl;
	CalcuFrequency(filename, w, n);
	cout << "Generating tree..." << endl;
	HuffmanCoding(HT, HC, w, n);
	content.read(filename);
	cout << "Encoding..." << endl;
	for (int i = 0; i < content.length / 8; i++)
	{
		result.append(CodeUnit(HC, w, content.slice(i, i + 1), n));
	}
	content.Free();
	FILE* outfile = fopen((string(filename) + ".pzip").c_str(), "wb");
	cout << "Writing the tree..." << endl;
	fwrite(&n, sizeof(n), 1, outfile);
	fwrite(HT, sizeof(HTNode), 2 * n, outfile);
	fwrite(&result.length, sizeof(result.length), 1, outfile);
	fclose(outfile);
	cout << "Writing tree leaves..." << endl;
	for (int i = 0; i < n; i++) w[i].data.write((string(filename) + ".pzip").c_str());
	cout << "Writing file contents..." << endl;
	result.write((string(filename) + ".pzip").c_str());
	result.Free();
	cout << "Completed!" << endl;
}

void UnZip(const char* filename)
{
	HTLeaf* w;
	HuffmanTree HT;
	bitstream crypt, leaves, result;
	string tmp;
	int n, cur, len;
	FILE* infile = fopen(filename, "rb");
	fread(&n, sizeof(n), 1, infile);
	long deb2 = ftell(infile);
	cout << "Reading huffman tree..." << endl;
	HT = (HuffmanTree)malloc(2 * n * sizeof(HTNode));
	w = (HTLeaf*)malloc(n * sizeof(HTLeaf));
	fread(HT, sizeof(HTNode), 2 * n, infile);
	long deb = ftell(infile);
	fread(&len, sizeof(len), 1, infile);
	long offset = ftell(infile);
	cout << "Reading tree leaves..." << endl;
	offset = leaves.read(filename, offset, n);
	for (int i = 0; i < n; i++) w[i].data = leaves.slice(i, i + 1);
	leaves.Free();
	cout << "Reading the encoded..." << endl;
	crypt.read(filename, offset);
	crypt.length = len;
	fclose(infile);
	cur = 2 * n - 1;
	cout << "Decoding..." << endl;
	for (int i = 0; i < crypt.length; i++)
	{
		if (crypt.getbit(i) == 0) cur = HT[cur].lchild;
		else cur = HT[cur].rchild;
		if (cur >= 1 && cur <= n)
		{
			result.append(w[cur - 1].data);
			cur = 2 * n - 1;
		}
	}
	result.write(string(filename).substr(0, strlen(filename) - 5).c_str());
	cout << "Completed!" << endl;
}