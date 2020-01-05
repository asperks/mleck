/*
A class that parses currency data, and formats it so that it provides a
unique view of the properties it is viewing.

Refer to the jewel_relations*.txt files for details of the relationships
between values.  These items determine which items can match up with which
items.

*/

#pragma once

#ifndef JEWEL_PT_VAL_H
#define JEWEL_PT_VAL_H

using namespace std;

#include "jewel.h"

#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <experimental/filesystem>

#include <boost/algorithm/string.hpp>

class jewel_pt_val : public jewel {

private:
	int i_jewel_relation_lhs = -1;
	int i_jewel_relation_rhs = -1;

	double d_calc_return = 0.0;

	vector<double> vec_dbl;

	// This is used to ensure that new jewels created aren't duplicates of existing
	// jewels.  By using this reference, the hash of any new jewel can be tested
	// to find out whether another jewel should be pointed to instead of the one
	// created.
	map<string, int> * ptr_map_hash_jewel;

public:
	//getter/setters 

	// These will be necessary to parse into a binary file.
	int get_jewel_relation_lhs() { return i_jewel_relation_lhs; }
	int get_jewel_relation_rhs() { return i_jewel_relation_rhs; }

	// This is used only at runtime.
	double get_calc_return() { return d_calc_return; }


	jewel_pt_val();

	void init(map<string, int> * map_hash_jewel_in);

	int create();


};



#endif