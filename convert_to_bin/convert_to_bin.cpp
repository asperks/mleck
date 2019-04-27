// convert_to_bin.cpp : Convert polo data files in text format to binary files.
//
//	This file contains the 'main' function. Program execution begins and ends there.
//


#include "settings.h"

#include <iostream>
#include <map>
#include <vector>

//#include <boost/filesystem.hpp>


int main(int argc, char** argv) {
	std::cout << "program start : " << argc << " arguments:" << "\n";

	vector<string> vec_args;
	for (int i = 0; i < argc; ++i) {
		vec_args.push_back(argv[i]);
		std::cout << argv[i] << "\n";
	}

	settings s;
	for (int i = 0; i < vec_args.size(); ++i) {
		if (vec_args.at(i) == "--path_scripture") { s.set_prop_str(vec_args.at(i), vec_args.at(i + 1)); }
		if (vec_args.at(i) == "--path_bin") { s.set_prop_str(vec_args.at(i), vec_args.at(i + 1)); }
	}



	//std::vector <boost::filesystem::directory_entry> ItDir;

	//std::copy(boost::filesystem::directory_iterator(DirectoryPath, boost::filesystem::directory_iterator(), back_inserter(ItDir));


	std::cout << "program end";
}





// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
