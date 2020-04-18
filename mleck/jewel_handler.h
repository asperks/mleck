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
#include <unordered_map>
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
	// NO idea why this didn't work.  But the using command changed it.
	// https://stackoverflow.com/questions/34770359/compile-error-trying-to-put-a-unique-ptr-into-a-map/34770634#34770634
	using map_up_jewel = std::unordered_map<uint64_t, std::unique_ptr<jewel>>;


	// When a new jewel is created, it will have a hash created of its core data;
	// If a new mutation is created of a jewel hash that already exists, instead
	// of creating a new jewel, it will adopt the jewel it finds.
	using map_hash_jewel = map<string, int>;

	cur_bin_handler * ptr_cbh;

public:

	jewel_handler() {};

	~jewel_handler() {};

	// Load the existing jewels.
	void init(cur_bin_handler * ptr_cbh_in);

	void set_cbh(cur_bin_handler * ptr_cbh_in) { ptr_cbh = ptr_cbh_in; }

	// This either adds an existing jewel link to a mleck, or creates a new jewel,
	//	and links the mleck to that jewel.
	int create_jewel_link(int id_mleck);

	std::unique_ptr<jewel> * get_ptr_jewel(int id);

	//void export_bin(string str_filepath_jewel);

	//void import_bin(string str_filepath_jewel);

};



#endif