#pragma once
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<iostream>
#include<io.h>
using namespace std;
class bitstream
{
public:
	char* bits;
	int length; //unit:bits
	bitstream();
	bitstream(const char* bitstring);
	void Free();
	bitstream slice(int left, int right, int unit_len = 8);
	int write(const char* filename);
	void append(bitstream b);
	long read(const char* filename, int offset = 0, int bytes = -1);
	bitstream operator+(const bitstream& b);
	char getbit(int i)const;
	friend ostream &operator<<(ostream &output, const bitstream& b);
	bool operator<(const bitstream& b);
	void setbit(int i,int n);
	void operator=(const bitstream& b);
	void operator=(const char* b);
	bool operator==(const bitstream& b);
	bool operator==(const char* b);
};

