#include "Pipe.h"

void Pipe::transfer()
{
	interface2->clear_information_in(); //1->2
	for (auto msg : interface1->information_out)
	{
		interface2->receive(msg.first, msg.second + cost);
	}
	interface1->clear_information_out();
	interface1->clear_information_in(); //2->1
	for (auto msg : interface2->information_out)
	{
		interface1->receive(msg.first, msg.second + cost);
	}
	interface2->clear_information_out();
}
