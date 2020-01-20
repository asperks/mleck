/*
A class that parses currency data, and formats it so that it provides a
unique view of the properties it is viewing.

*/

#pragma once

#ifndef JEWEL_HANDLER_H
#define JEWEL_HANDLER_H

using namespace std;

#include "jewel.h"
#include "jewel_pt_val.h"

#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <experimental/filesystem>

#include <boost/algorithm/string.hpp>

class jewel_handler {
private:
	bool b_valid = false;
	string str_path_bin;

	int id_jewel_max = 0;
	
	// This is the map of the jewels.  When a new jewel is created, the id_jewel_max
	// variable increases.  The mlecks store a vector of jewels that they use in order
	// to arrive
	//map<int, std::unique_ptr<jewel>> map_up_jewel;


	// When a new jewel is created, it will have a hash created of its core data;
	// If a new mutation is created of a jewel hash that already exists, instead
	// of creating a new jewel, it will adopt the jewel it finds.
	map<string, int> map_hash_jewel;

	cur_bin_handler * ptr_cbh;

public:

	jewel_handler();

	// Load the existing jewels.
	void init(cur_bin_handler cbh_in);

	void set_cbh(cur_bin_handler cbh_in) { ptr_cbh = & cbh_in; }

	int create_jewel_link(int id_mleck);

	std::unique_ptr<jewel> * get_ptr_jewel(int id);

	void export_bin(string str_filepath_jewel);

	void import_bin(string str_filepath_jewel);

};



#endif