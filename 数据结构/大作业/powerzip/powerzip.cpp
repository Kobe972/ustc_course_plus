#include"huffman.h"

// A simple arguments parser
std::pair<bool, string> getCmdOption(char** begin, char** end, const std::string& option) {
    char** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) {
        return std::make_pair(true, *itr);
    }
    return std::make_pair(false, "");
}
bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}
int main(int argc, char** argv)
{
    if (cmdOptionExists(argv, argv + argc, "-h")) {
        std::cout << "This is a powerful compression software" << std::endl << std::endl;
        std::cout << "Usage" << std::endl;
        std::cout << "./powerzip [OPTION] ... [FILE] ..." << std::endl << std::endl;
        std::cout << "Options" << std::endl;
        std::cout << "-h : print out help information" << std::endl;
        std::cout << "-f : the path for the input file" << std::endl;
        std::cout << "-z : zip" << std::endl;
        std::cout << "-u : unzip" << std::endl;
        return 0;
    }
    auto input_info = getCmdOption(argv, argv + argc, "-f");
    string input_filename = input_info.second;
    if (cmdOptionExists(argv, argv + argc, "-z")) {
        Zip(input_filename.c_str());
    }
    else if (cmdOptionExists(argv, argv + argc, "-u")) {
        UnZip(input_filename.c_str());
    }
	return 0;
}
