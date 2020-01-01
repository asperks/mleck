/*
A class that parses currency data, and formats it so that it provides a
unique view of the properties it is viewing.

The cur_comparator enum defines the methods in which different data points are
compared and then used for analysis.  Mleck always requires at least two points,
and at a minimum compares the two points, and returns a value based upon their
ratios.

	pt2			-	return a ratio between two points as a double.



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
	enum cur_comparator { pt2, pt3, vec_comp1};
	const int CUR_COMPARATOR_ITEMS = 3;

	bool b_valid = false;
	string str_path_bin;

	vector<double> vec_dbl;

	cur_bin_handler * ptr_cbh;

public:

	jewel();

	void init();

	void set_cbh(cur_bin_handler cbh_in) {ptr_cbh = &cbh_in;}

};



#endif