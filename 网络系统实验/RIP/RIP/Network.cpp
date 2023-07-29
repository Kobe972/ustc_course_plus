#include "Network.h"
#include <iostream>
#include <unordered_set>
#include <queue>
using namespace std;

void Network::refresh()
{
	for (auto router : routers)
	{
		router->refresh();
	}
}

bool Network::update()
{
	bool ret = false;
	for (auto pipe : pipes)
	{
		pipe->transfer();
	}
	for (auto router : routers)
	{
		ret |= router->process_message();
	}
	return ret;
}

void Network::print_tables()
{
	for (auto router : routers)
	{
		cout << "============Router " << router->name << " ============" << endl;
		cout << "Destination\tDistance\tNext-Hop" << endl;
		for (auto item : router->table)
		{
			if (item.second.second == nullptr)
			{
				cout << item.first << "\t\t" << item.second.first << "\t\t-" << endl;
			}
			else
			{
				cout << item.first << "\t\t" << item.second.first << "\t\t" << interface_router_map[item.second.second] << endl;
			}
		}
	}
	cout << endl;
}

void Network::add_router(string name)
{
	Router* router = new Router();
	router->name = name;
	routers.insert(router);
	router_map[name] = router;
}

void Network::add_subnet(string router, string subnet)
{
	router_map[router]->add_subnet(subnet);
}

void Network::add_link(string router1, string router2, int cost)
{
	if (names_pipe_map.count(make_pair(router1, router2)))
	{
		cout << "Link already exists!" << endl;
		return;
	}
	if (!router_map.count(router1))
	{
		cout << "Router " << router1 << " does not exist!" << endl;
		return;
	}
	if (!router_map.count(router2))
	{
		cout << "Router " << router2 << " does not exist!" << endl;
		return;
	}
	Interface* interface_router1 = new Interface();
	router_map[router1]->interfaces.insert(interface_router1);
	Interface* interface_router2 = new Interface();
	router_map[router2]->interfaces.insert(interface_router2);
	Pipe* pipe = new Pipe();
	pipe->interface1 = interface_router1;
	pipe->interface2 = interface_router2;
	pipe->cost = cost;
	names_pipe_map[make_pair(router1, router2)] = pipe;
	names_pipe_map[make_pair(router2, router1)] = pipe;
	pipes.insert(pipe);
	interface_router_map[interface_router1] = router_map[router2]->name;
	interface_router_map[interface_router2] = router_map[router1]->name;
}

void Network::modify_link(string router1, string router2, int cost)
{
	if (!names_pipe_map.count(make_pair(router1, router2)))
	{
		cout << "Link between " << router1 << " and " << router2 << " does not exist!" << endl;
		return;
	}
	if (!router_map.count(router1))
	{
		cout << "Router " << router1 << " does not exist!" << endl;
		return;
	}
	if (!router_map.count(router2))
	{
		cout << "Router " << router2 << " does not exist!" << endl;
		return;
	}
	names_pipe_map[make_pair(router1, router2)]->cost = cost;
}

void Network::update_until_converge()
{
	bool changed;
	int iteration = 0;
	do
	{
		refresh();
		changed = update();
		iteration++;
		cout << iteration << "th iteration:" << endl;
		print_tables();
	} while (changed == true);
}

