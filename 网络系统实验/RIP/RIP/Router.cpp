#include "Router.h"

void Router::add_subnet(string name)
{
	table[name] = make_pair(0, nullptr);
}

bool Router::process_message()
{
	bool ret = false;
	for (auto interface : interfaces)
	{
		for (auto msg : interface->information_in)
		{
			if (!table.count(msg.first))
			{
				table[msg.first] = make_pair(msg.second, interface);
				ret = true;
			}
			else if (table[msg.first].first > msg.second || table[msg.first].second == interface && table[msg.first].first != msg.second)
			{
				table[msg.first].first = msg.second;
				table[msg.first].second = interface;
				ret = true;
			}
		}
	}
	return ret;
}

void Router::refresh()
{
	for (auto item : table)
	{
		inform(item.first, item.second.first);
	}
}

void Router::inform(string dest, int dist)
{
	for (auto interface :interfaces)
	{
		interface->inform(dest, dist);
	}
}
