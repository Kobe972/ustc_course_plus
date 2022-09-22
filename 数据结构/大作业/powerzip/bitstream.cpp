#include "bitstream.h"

bitstream::bitstream()
{
	bits = NULL;
	length = 0;
}

bitstream::bitstream(const char* bitstring)
{
	this->length = strlen(bitstring);
	int bytes = ceil((float)this->length / 8);
	bits = (char*)malloc(bytes);
	for (int i = 0; i < bytes; i++)
	{
		bits[i] = 0;
	}
	for (int i = 0; i < bytes; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (i * 8 + j < strlen(bitstring) && bitstring[i * 8 + j] == '1') this->bits[i] |= (1 << (7 - j));
		}
	}
}

void bitstream::Free()
{
	length = 0;
	free(bits);
}

bitstream bitstream::slice(int left, int right, int unit_len)
{
	bitstream ret;
	ret.length = (right - left) * unit_len;
	ret.bits = (char*)malloc(ret.length / 8+1);
	for (int i = 0; i < ret.length / 8; i++)
	{
		ret.bits[i] = bits[(i + left) * unit_len / 8];
	}
	return ret;
}

int bitstream::write(const char* filename)
{
	int bytes = ceil((float)length / 8);
	FILE* outfile;
	outfile = fopen(filename, "ab");
	if (outfile == NULL) return 0;
	fwrite(bits, bytes, 1, outfile);
	fclose(outfile);
	return 1;
}

void bitstream::append(bitstream b)
{
	int bytes = ceil((float)b.length / 8 + (float)length / 8);
	if (length == 0) bits = (char*)malloc(bytes);
	else
		bits = (char*)realloc(bits, bytes);
	for (int i = 0; i < b.length; i++) setbit(length + i, b.getbit(i));
	length += b.length;
}

long bitstream::read(const char* filename, int offset, int bytes)
{
	FILE* infile;
	long _offset;
	infile = fopen(filename, "rb");
	if (infile == NULL) return -1;
	fseek(infile, offset, SEEK_SET);
	int size;
	if (bytes == -1)
	{
		size = _filelength(_fileno(infile)) - offset;
	}
	else
	{
		size = bytes;
	}
	bits = (char*)malloc(size);
	fread(bits, size, 1, infile);
	_offset = ftell(infile);
	fclose(infile);
	length = size * 8;
	return _offset;
}

bitstream bitstream::operator+(const bitstream& b)
{
	bitstream ret;
	int bytes = ceil((float)b.length / 8 + (float)this->length / 8);
	ret.bits = (char*)malloc(bytes);
	ret.length = this->length + b.length;
	for (int i = 0; i < this->length / 8; i++) ret.bits[i] = this->bits[i];
	for (int i = 0; i < this->length % 8; i++) ret.setbit(this->length / 8 * 8 + i, this->getbit(this->length / 8 * 8 + i));
	for (int i = 0; i < b.length; i++) ret.setbit(this->length + i, b.getbit(i));
	return ret;
}

char bitstream::getbit(int i) const
{
	return (bits[i / 8] >> (7 - i % 8)) & 0x1;
}

bool bitstream::operator<(const bitstream& b)
{
	if (length < b.length) return true;
	if (length > b.length) return false;
	for (int i = 0; i < length / 8; i++)
	{
		if (bits[i] < b.bits[i]) return true;
		if (bits[i] > b.bits[i]) return false;
	}
	for (int i = 0; i < length % 8; i++)
	{
		if (getbit(length / 8 * 8 + i) < b.getbit(length / 8 * 8 + i)) return true;
		if (getbit(length / 8 * 8 + i) > b.getbit(length / 8 * 8 + i)) return false;
	}
	return false;
}

void bitstream::setbit(int i, int n)
{
	if (n == 0) bits[i / 8] &= 255 - (1 << (7 - i % 8));
	else bits[i / 8] |= 1 << (7 - i % 8);
}

void bitstream::operator=(const bitstream& b)
{
	if (this->length == 0) this->Free();
	this->length = b.length;
	int bytes = ceil((float)this->length / 8);
	this->bits = (char*)malloc(bytes);
	for (int i = 0; i < bytes; i++)
	{
		bits[i] = 0;
	}
	for (int i = 0; i < bytes; i++) this->bits[i] = b.bits[i];
}

void bitstream::operator=(const char* b)
{
	if (this->length == 0) this->Free();
	this->length = strlen(b);
	int bytes = ceil((float)this->length / 8);
	this->bits = (char*)malloc(bytes);
	for (int i = 0; i < bytes; i++)
	{
		this->bits[i] = 0;
	}
	for (int i = 0; i < bytes; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (i * 8 + j < strlen(b) && b[i * 8 + j] == '1') this->bits[i] |= (1 << (7 - j));
		}
	}
}

bool bitstream::operator==(const bitstream& b)
{
	if (this->length != b.length) return false;
	for (int i = 0; i < this->length / 8; i++)
	{
		if (this->bits[i] != b.bits[i]) return false;
	}
	for (int i = 0; i < this->length % 8; i++)
	{
		if (this->getbit(this->length / 8 * 8 + i) != b.getbit(b.length / 8 * 8 + i)) return false;
	}
	return true;
}

bool bitstream::operator==(const char* b)
{
	if (this->length != strlen(b)) return false;
	for (int i = 0; i < this->length / 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (i * 8 + j < strlen(b) && ((this->bits[i] & (1 << j)) != b[i * 8 + j] - '0')) return false;
		}
	}
	return true;
}

ostream& operator<<(ostream &output, const bitstream& b)
{
	int bytes = ceil((float)b.length / 8);
	for (int i = 0; i < bytes; i++)
	{
		for (int j = 0; j <= 7; j++)
		{
			if (i * 8 + j < b.length)output << (char)(b.getbit(i * 8 + j) + '0');
		}
	}
	return output;
}
