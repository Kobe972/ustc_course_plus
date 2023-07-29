#pragma once
#include "Interface.h"
#include <unordered_set>
#include <map>
using namespace std;
class Router
{
public:
	unordered_set<Interface*> interfaces;
	string name;
	map<string, pair<int, Interface*>> table; // the last string is the next hop
	void add_subnet(string name);
	bool process_message();
	void refresh();
	void inform(string dest, int dist);
};

