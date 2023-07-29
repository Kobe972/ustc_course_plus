#pragma once
#include "Interface.h"
class Pipe
{
public:
	Interface* interface1;
	Interface* interface2;
	int cost;
	void transfer();
};

