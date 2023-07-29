#include "router.hh"

#include <iostream>

using namespace std;

// Dummy implementation of an IP router

// Given an incoming Internet datagram, the router decides
// (1) which interface to send it out on, and
// (2) what next hop address to send it to.

// For Lab 6, please replace with a real implementation that passes the
// automated checks run by `make check_lab6`.

// You will need to add private members to the class declaration in `router.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

std::string getBinaryString(uint32_t ip, uint8_t length)
{
    std::string binaryString;

    for (uint8_t i = 0; i < length; ++i) {
        uint8_t bit = (ip >> (31 - i)) & 1;
        binaryString += std::to_string(bit);
    }
    return binaryString;
}

//! \param[in] route_prefix The "up-to-32-bit" IPv4 address prefix to match the datagram's destination address against
//! \param[in] prefix_length For this route to be applicable, how many high-order (most-significant) bits of the route_prefix will need to match the corresponding bits of the datagram's destination address?
//! \param[in] next_hop The IP address of the next hop. Will be empty if the network is directly attached to the router (in which case, the next hop address should be the datagram's final destination).
//! \param[in] interface_num The index of the interface to send the datagram out on.
void Router::add_route(const uint32_t route_prefix,
                       const uint8_t prefix_length,
                       const optional<Address> next_hop,
                       const size_t interface_num) {
    cerr << "DEBUG: adding route " << Address::from_ipv4_numeric(route_prefix).ip() << "/" << int(prefix_length)
         << " => " << (next_hop.has_value() ? next_hop->ip() : "(direct)") << " on interface " << interface_num << "\n";

    DUMMY_CODE(route_prefix, prefix_length, next_hop, interface_num);
    string binary_string = getBinaryString(route_prefix, prefix_length);
    RouterItem _routerItem{next_hop, interface_num};
    _radixTree.insert(binary_string, _routerItem);
}

//! \param[in] dgram The datagram to be routed
void Router::route_one_datagram(InternetDatagram &dgram) {
    DUMMY_CODE(dgram);
    string binary_string = getBinaryString(dgram.header().dst, 32);
    RouterItem selected_item = _radixTree.search(binary_string);
    if(dgram.header().ttl <= 1)
    {
        return;
    }
    dgram.header().ttl--;
    AsyncNetworkInterface& _interface = interface(selected_item.interface_num); //remember to use the reference
    if (selected_item.next_hop.has_value())
    {
        _interface.send_datagram(dgram, selected_item.next_hop.value());
    } 
    else
    {
        _interface.send_datagram(dgram, Address::from_ipv4_numeric(dgram.header().dst));
    }
}

void Router::route() {
    // Go through all the interfaces, and route every incoming datagram to its proper outgoing interface.
    for (auto &interface : _interfaces) {
        auto &queue = interface.datagrams_out();
        while (not queue.empty()) {
            route_one_datagram(queue.front());
            queue.pop();
        }
    }
}

RouterItem RadixTree::search(const std::string& path)
{
    Node* node = root.get();
    size_t index = 0;
    string _path = path;
    Node* chosen = NULL;

    while (index < node->children.size()) {
        Node* child = node->children[index].get();
        if (child->path == _path) {
            return child->value;
        }
        if (child->path.length() < _path.length() && node->children[index].get()->path == _path.substr(0, child->path.length())) {
            node = child;
            _path = _path.substr(child->path.length());
            chosen = child;
            index = 0;
            continue;
        }
        else{
            index++;
        }
    }
	if(chosen != NULL && chosen->has_value) return chosen->value;
    return default_item;
}


void RadixTree::insert(const std::string& path, const RouterItem& value)
{
    if (path.empty()) {
        default_item = value;
        return;
    }

    Node* node = root.get();
    size_t index = 0;
    std::string common_prefix = "";
    Node* common_prefix_node = root.get();
    string _path = path;

    while (index < node->children.size()) {
        common_prefix_node = node;
        Node* child = node->children[index].get();
        if (child->path == _path) {
            common_prefix = _path;
            common_prefix_node = child;
            break;
        }
        if (child->path.length() < _path.length() && _path.substr(0, child->path.length()) == child->path) {
        	_path = _path.substr(child->path.length());
            common_prefix += child->path;
            node = child;
            index = 0;
            continue;
        }
        else {
            index++;
        }
    }

    if (common_prefix == path) {
        common_prefix_node->has_value = true;
        common_prefix_node->value = value;
        return;
    }

    std::string remaining_path = path.substr(common_prefix.length());

    for (size_t idx = 0; idx < common_prefix_node->children.size(); idx++) {
        Node* child = common_prefix_node->children[idx].get();
        if (child->path[0] == remaining_path[0]) {
            size_t i = 0;
            while (i < child->path.length() && i < remaining_path.length() && child->path[i] == remaining_path[i]) {
                i++;
            }

            auto new_child = std::make_unique<Node>();
            new_child->path = child->path.substr(0,i);
            auto grandson1 = std::make_unique<Node>();
            grandson1->path = child->path.substr(i);
            grandson1->has_value = child->has_value;
            grandson1->value = child->value;
            grandson1->children = std::move(child->children);

			auto grandson2 = std::make_unique<Node>();
            grandson2->path = remaining_path.substr(i);
            grandson2->has_value = true;
            grandson2->value =value;

            new_child->children.push_back(std::move(grandson1));
            new_child->children.push_back(std::move(grandson2));
            common_prefix_node->children[idx] = std::move(new_child);
            return;
        }
    }
    auto new_node = std::make_unique<Node>();
    new_node->path = remaining_path;
    new_node->has_value = true;
    new_node->value = value;
    common_prefix_node->children.push_back(std::move(new_node));
}