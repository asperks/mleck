// convert_to_bin.cpp : Convert polo data files in text format to binary files.
//
//	This file contains the 'main' function. Program execution begins and ends there.
//
//
//	--path_scripture	Path where the source text files from poloniex have been stored.
//
//	--path_bin			Path where the binary files created from the scripture is stored.
//
//	--gen					A pipe (|) delimited string of all gens to process
//							if empty all of the folders in the path_scripture
//							directory
//
//	--instruments		A pipe (|) delimited string of all instruments to process into
//							binaries.
//

#include "settings.h"
#include "polo_gen.h"


#include <iostream>
#include <iterator>
#include <map>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string.hpp> 


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

		} else if (vec_args.at(i) == "--instruments") {
			string str_temp = vec_args.at(i + 1);
			// Remove quotes from paths
			str_temp.erase(remove(str_temp.begin(), str_temp.end(), '\"'), str_temp.end());
			s.set_prop_str(vec_args.at(i), str_temp);
		}
	}


	string str_instruments = s.get_prop_str("--instruments");
	vector<string> vec_instrument;
	if (str_instruments != "") {
		boost::split(vec_instrument, str_instruments, boost::is_any_of("|"));
	}


	string str_gens = s.get_prop_str("--gen");
	vector<string> vec_gen;
	if (str_gens != "") {
		boost::split(vec_gen, str_gens, boost::is_any_of("|"));
	}

	polo_gen pg;
	pg.process(vec_gen
					, vec_instrument
					, s.get_prop_str("--path_scripture")
					, s.get_prop_str("--path_bin")
					);



	std::cout << "program end\n";
}
