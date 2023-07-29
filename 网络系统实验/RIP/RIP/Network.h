#pragma once
#include "Interface.h"
#include "Pipe.h"
#include "Router.h"
#include <vector>
#include <unordered_set>
using namespace std;
class Network
{
public:
	unordered_set<Router*> routers;
	unordered_set<Pipe*> pipes;
	map<string, Router*> router_map;
	map<pair<string, string>, Pipe*> names_pipe_map;
	map<Interface*, string> interface_router_map; //used for debug
	void refresh();
	bool update();
	void print_tables();
	void add_router(string name);
	void add_subnet(string router, string subnet);
	void add_link(string router1, string router2, int cost);
	void modify_link(string router1, string router2, int cost);
	void update_until_converge();
};

