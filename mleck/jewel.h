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

#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <experimental/filesystem>

#include <boost/algorithm/string.hpp>

#include "cur_bin_handler.h"
#include "jewel_handler.h"

using namespace std;

class jewel {

private:
	string str_path_bin;

protected:
	// This forms the left and right hand side of ratios.  These are jewel_relations*
	// ids as defined in the text files.
	//
	//	All returned values are differences.
	//
	//	PT_RAND		:	random double between -10 and 10
	//	PT_DIFF		:	sum of two ratios in %
	//	PT_JEWEL		:	the returned value of another JEWEL as a DIFF
	//	PT_RATIO		:	ratio between two values returned as a PT_DIFF
	//								This value will always be a difference between the current point
	//								and the point previous.
	//	PT_VOL			:	ratio between a value and a volume.
	//								This value will always be a difference between the current point
	//								and the point previous.
	int i_jewel_type_count = 1;

	enum Jewel_type { J_RAND, J_DIFF, J_JEWEL, J_RATIO, J_VOL };

	// This is a unique string that defines the parameters of the jewel.
	Jewel_type jt;
	string str_id = "";

	bool b_valid = false;

	// A double value for every instrument.
	vector<double> vec_calc_return;

	double d_calc_return = 0.0;

	// This is used to access other jewel returns.
	jewel_handler * ptr_jh;

	// This is used to access the currency data.
	cur_bin_handler * ptr_cbh;


public:
	jewel() {}

	jewel(const jewel &jewel_from) {}

	~jewel() {}

	//getter/setters 

	// This is used only at runtime.
	vector<double> * get_calc_return() { return & vec_calc_return; }

	void init(jewel_handler * ptr_jh_in, cur_bin_handler* ptr_cbh_in);

	void set_jh(jewel_handler * ptr_jh_in) { ptr_jh = ptr_jh_in; }

	void set_cbh(cur_bin_handler * ptr_cbh_in) { ptr_cbh = ptr_cbh_in; }

	string to_string() { return str_id; }

	// These functions are implemented in the inherited classes.
	void calc_return() {};

	string create(int id_mleck_in) { return ""; };




};



#endif