#include "Interface.h"

void Interface::inform(string dest, int dist)
{
	information_out[dest] = dist;
}

void Interface::receive(string dest, int dist)
{
	information_in[dest] = dist;
}

void Interface::clear_information_out()
{
	information_out.clear();
}

void Interface::clear_information_in()
{
	information_in.clear();
}
