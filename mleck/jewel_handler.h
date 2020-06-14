/*
A class that parses currency data, and formats it so that it provides a
unique view of the properties it is viewing.

*/

#pragma once

#ifndef JEWEL_HANDLER_H
#define JEWEL_HANDLER_H

#include "cur_bin_handler.h"
#include "jewel.h"
#include "jewel_c_rand.h"
#include "jewel_c_diff.h"


#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include <experimental/filesystem>

#include <boost/algorithm/string.hpp>

using namespace std;

class jewel_handler {
private:
	bool b_valid = false;
	string str_path_bin;

	int id_jewel_max = 0;
	
	// This is the map of the jewels.  When a new jewel is created, the id_jewel_max
	// variable increases.  The mlecks store a vector of jewels that they use in order
	// to arrive
	// This will be populated by objects inherited from the jewel class.
	std::map<string, std::unique_ptr<jewel>>  map_up_jewel;

	cur_bin_handler * cbh_ptr = nullptr;

public:

	jewel_handler() {};

	~jewel_handler() {
		//map_up_jewel.clear();
		//delete cbh_ptr; 
	};

	// Load the existing jewels.
	void init(cur_bin_handler * ptr_cbh_in);

	void set_cbh(cur_bin_handler * cbh_ptr_in) { cbh_ptr = cbh_ptr_in; }

	// Will create a new, or 
	string get_jewel_id(int i_new_threshold);

	// This either either accesses a unique ptr of a jewel in the map, or if it doesn't exist
	//	 creates the element, and then returns it.  Either way, it returns a pointer to the 
	// instantiated jewl in memory for speedy access.
	std::unique_ptr<jewel> * jewel_link(string str_jewel_id);

	void export_text(string str_filepath_jewel);

	//void export_bin(string str_filepath_jewel);

	//void import_bin(string str_filepath_jewel);


	//https://stackoverflow.com/questions/1340729/how-do-you-generate-a-random-double-uniformly-distributed-between-0-and-1-from-c/26853142#26853142
	inline  double randZeroToOne() { return rand() / (RAND_MAX + 1.); }
	inline  double randMToN(double M, double N) { return M + (rand() / (RAND_MAX / (N - M))); }


};



#endif