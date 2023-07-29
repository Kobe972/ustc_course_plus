#include "Network.h"
#include <iostream>
using namespace std;
int main()
{
	Network network;
	network.add_router("A");
	network.add_router("B");
	network.add_router("C");
	network.add_link("A", "B", 21);
	network.add_link("B", "C", 1);
	network.add_link("C", "A", 7);
	network.add_subnet("A", "net_A");
	network.add_subnet("B", "net_B");
	network.add_subnet("C", "net_C");
	cout << "Initial tables:" << endl;
	network.print_tables();
	network.update_until_converge();
	cout << "Cost between A and B modified to 3." << endl;
	network.modify_link("A", "B", 3);
	network.update_until_converge();
	cout << "Cost between A and B modified to 10." << endl;
	network.modify_link("A", "B", 10);
	network.update_until_converge();
	return 0;
}