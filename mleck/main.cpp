// mleck.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//	Program points to a bin path.  That is the data-set it iterates through.
//



#include "settings.h"
#include "farm.h"

#include <experimental/filesystem>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string.hpp> 
#include <boost/lexical_cast.hpp>

int main(int argc, char** argv) {
	std::cout << "program start : " << argc << " arguments" << std::endl;

	vector<string> vec_args;
	for (int i = 0; i < argc; ++i) {
		vec_args.push_back(argv[i]);
	}

	int i_farm_id = 0;
	int i_gen_iterations = 10;
	int i_mleck_count = 0;


	// Add arguments to a settings class for access later on.
	settings se;
	for (int i = 0; i < vec_args.size(); ++i) {
		if (vec_args.at(i) == "--path_bin") {
			string str_temp = vec_args.at(i + 1);
			// Remove quotes from paths
			str_temp.erase(remove(str_temp.begin(), str_temp.end(), '\"'), str_temp.end());
			se.set_prop_str(vec_args.at(i), str_temp);

		} else if (vec_args.at(i) == "--path_farm") {
			string str_temp = vec_args.at(i + 1);
			// Remove quotes from paths
			str_temp.erase(remove(str_temp.begin(), str_temp.end(), '\"'), str_temp.end());
			se.set_prop_str(vec_args.at(i), str_temp);

		} else if (vec_args.at(i) == "--farm_id") {
			string str_temp = vec_args.at(i + 1);
			// Remove quotes from paths
			str_temp.erase(remove(str_temp.begin(), str_temp.end(), '\"'), str_temp.end());
			se.set_prop_str(vec_args.at(i), str_temp);

		} else if (vec_args.at(i) == "--gen_iterations") {
			string str_temp = vec_args.at(i + 1);
			// Remove quotes from paths
			str_temp.erase(remove(str_temp.begin(), str_temp.end(), '\"'), str_temp.end());
			se.set_prop_str(vec_args.at(i), str_temp);
		}
	}

	string str_gen_iterations = se.get_prop_str("--gen_iterations");
	if (str_gen_iterations != "") {
		i_gen_iterations = boost::lexical_cast<int>(str_gen_iterations);
	}

	string str_farm_id = se.get_prop_str("--farm_id");
	if (str_gen_iterations != "") {
		i_farm_id = boost::lexical_cast<int>(str_farm_id);
	}

	if (boost::filesystem::is_directory(se.get_prop_str("--path_bin")) == false) {
		std::cout << "BIN directory NOT FOUND!  EXITING!" << std::endl;
	} else {
		string str_filepath_farm_id = se.get_prop_str("--path_farm") + "\\" + std::to_string(i_farm_id);
		if (boost::filesystem::is_directory(se.get_prop_str("--path_farm")) == false) {
			// The farm folder doesn't exist.  Create it.
			//https://stackoverflow.com/questions/50960492/creating-folders-in-c/56262869#56262869
			std::experimental::filesystem::create_directories(se.get_prop_str("--path_farm"));
			std::experimental::filesystem::create_directories(str_filepath_farm_id);
		}

		if (boost::filesystem::is_directory(str_filepath_farm_id) == false) {
			// The farm folder doesn't exist.  Create it.
			//https://stackoverflow.com/questions/50960492/creating-folders-in-c/56262869#56262869
			std::experimental::filesystem::create_directories(str_filepath_farm_id);
		}


		if (boost::filesystem::is_directory(str_filepath_farm_id) == false) {
			std::cout << "FARM directory CANNOT BE CREATED!  EXITING!" << std::endl;
		} else {
			farm fa;

			fa.init(i_farm_id, se.get_prop_str("--path_bin"), str_filepath_farm_id);

			fa.run(i_gen_iterations);
		}

	}


	std::cout << "program end" << std::endl;
}

