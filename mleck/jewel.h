/*
A class that parses currency data, and formats it so that it provides a
unique view of the properties it is viewing.

Refer to the jewel_relations*.txt files for details of the relationships
between values.  These items determine which items can match up with which
items.

*/

#pragma once

#ifndef JEWEL_H
#define JEWEL_H

using namespace std;


#include "cur_bin_handler.h"

#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <experimental/filesystem>

#include <boost/algorithm/string.hpp>

class jewel {

private:


	bool b_valid = false;
	string str_path_bin;

	// This forms the left and right hand side of ratios.  These are jewel_relations*
	// ids as defined in the text files.
	//
	//	All returned values are differences.
	//
	//	PT_RAND		:	random number between -10 and 10
	//	PT_DIFF		:	sum of two ratios
	//	PT_RATIO		:	ratio between two values returned as a PT_DIFF
	//	PT_VOL			:	ratio between a value and a volume.
	//	PT_JEWEL		:	the returned value of another jewel as a DIFF
	int i_jewel_type_count = 5;

	enum Jewel_type { PT_RAND, PT_DIFF, PT_RATIO, PT_VOL, PT_JEWEL };

	int i_jewel_relation_lhs = -1;
	int i_jewel_relation_rhs = -1;

	double d_calc_return = 0.0;

	vector<double> vec_dbl;

	// This is used to access other jewel returns.
	map<string, int> * ptr_map_hash_jewel;

	// This is used to access the currency data.
	cur_bin_handler * ptr_cbh;

public:
	jewel() {}

	jewel(const jewel &jewel_from) {}

	~jewel() {}

	//getter/setters 

	// These will be necessary to parse into a binary file.
	int get_jewel_relation_lhs() { return i_jewel_relation_lhs; }
	int get_jewel_relation_rhs() { return i_jewel_relation_rhs; }

	// This is used only at runtime.
	double get_calc_return() { return d_calc_return; }

	double calc_return();

	void init(map<string, int> * map_hash_jewel_in);

	void set_cbh(cur_bin_handler * ptr_cbh_in) { ptr_cbh = ptr_cbh_in; }

	int create(int id_mleck_in);

	string to_string();

};



#endif