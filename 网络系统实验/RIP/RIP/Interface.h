#pragma once
#include <string>
#include <map>
using namespace std;
class Interface
{
public:
	map<string, int> information_out; //distance vector to send
	map<string, int> information_in; //distance vector that would receive
	void inform(string dest, int dist);
	void receive(string dest, int dist);
	void clear_information_out();
	void clear_information_in();
};

