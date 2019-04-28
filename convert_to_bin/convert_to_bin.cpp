// convert_to_bin.cpp : Convert polo data files in text format to binary files.
//
//	This file contains the 'main' function. Program execution begins and ends there.
//
//
//	--path_scripture	Path where the source text files from poloniex have been stored.
//
//	--path_bin			Path where the binary files created from the scripture is stored.
//
//	--gen					If the user passes a gen, the program builds that bin,
//							instead of processing all of the folders in the path_scripture
//							directory
//

#include "settings.h"

#include <iostream>
#include <iterator>
#include <map>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>


int main(int argc, char** argv) {
	std::cout << "program start : " << argc << " arguments\n";

	vector<string> vec_args;
	for (int i = 0; i < argc; ++i) {
		vec_args.push_back(argv[i]);
	}

	// Add arguments to a settings class for access later on.
	settings s;
	for (int i = 0; i < vec_args.size(); ++i) {
		if (vec_args.at(i) == "--path_scripture") {
			string str_temp = vec_args.at(i + 1);
			// Remove quotes from paths
			str_temp.erase(remove(str_temp.begin(), str_temp.end(), '\"'), str_temp.end());
			s.set_prop_str(vec_args.at(i), str_temp);

		} else if (vec_args.at(i) == "--path_bin") {
			string str_temp = vec_args.at(i + 1);
			// Remove quotes from paths
			str_temp.erase(remove(str_temp.begin(), str_temp.end(), '\"'), str_temp.end());
			s.set_prop_str(vec_args.at(i), str_temp);

		} else if (vec_args.at(i) == "--gen") {
			string str_temp = vec_args.at(i + 1);
			// Remove quotes from paths
			str_temp.erase(remove(str_temp.begin(), str_temp.end(), '\"'), str_temp.end());
			s.set_prop_str(vec_args.at(i), str_temp);
		}
	}

	// List all of the directories in the scripture directory
	std::cout << "read folders\n";
	vector<string> vec_dir;
	if (boost::filesystem::is_directory(s.get_prop_str("--path_scripture"))) {
		for (auto& di : boost::make_iterator_range(boost::filesystem::directory_iterator(s.get_prop_str("--path_scripture")), {})) {
			vec_dir.push_back(di.path().string());
		}
	}

	if (s.get_prop_str("--gen") == "") {
		// create bins for every folder in the scripture
		if (vec_dir.size() > 0) {
			for (int i = 0; i < vec_dir.size(); ++i) {
				// Loop through each folder in the scripture directory.
				boost::filesystem::path p(vec_dir.at(i));
				boost::filesystem::path dir = p.filename();
				string str_dir = dir.string();

				std::cout << "process scripture = " + str_dir + "\n";
			}
		}
	} else {
		// create bins for only the --gen folder.
		if (std::find(vec_dir.begin(), vec_dir.end(), s.get_prop_str("--gen")) != vec_dir.end()) {
			// This folder exists in the scriptures directory list.

		}
	}

	std::cout << "program end\n";
}



