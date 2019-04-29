// Class for a snapshot of poloniex data.  The data from poloniex is retrieved using
//	a separate method.
//		Reads the poloniex data text files.
//		Creates a binary representation of that data
//		Reads the poloniex data binary files.
//		Populates a class for access.

#pragma once

#ifndef INSTRUMENT_GEN_H
#define INSTRUMENT_GEN_H

using namespace std;


#include <string>
#include <iostream>
#include <map>
#include <vector>


class instrument_gen {
private:
	// Data Members 
	string str_class_name = "instrument_gen";

	bool b_exists_scripture = false;
	bool b_exists_bin = false;
	bool b_loaded = false;

	string str_instrument = "";
	string str_gen = "";
	string str_path_scripture = "";
	string str_filepath_bin = "";



public:

	// Member Functions() 
	int init(string str_gen
				, vector<string> vec_instruments
				, string str_path_scripture
				, string str_filepath_bin
				);


	int read_bin();

	int write_bin();

	string to_string();

};


#endif //INSTRUMENT_GEN_H