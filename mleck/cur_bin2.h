/*
A class that parses currency data, and formats it so that it provides a
unique view of the properties it is viewing.

The cur_bin2 class measure the ticker performance between a sequence of three
sequential cur_bin objects, and writes this data out to an external BIN2 file
so that it can be quickly loaded for future processing.

*/

#pragma once

#ifndef CUR_BIN2_H
#define CUR_BIN2_H

using namespace std;

#include "cur_bin.h"

#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <experimental/filesystem>

#include <boost/algorithm/string.hpp>

class cur_bin2 {
private:
	bool b_valid = false;
	string str_path_bin;

	cur_bin cb_base;

	// a ratio between the previous ticker and the base
	map<string, Ticker> map_struct_ticker_prev;

	// a ratio between the base ticker and the next.
	//	This would be removed if this were to be a live implementation.  It is 
	// implemented here so that I can quickly measure performance for back testing.
	map<string, Ticker> map_struct_ticker_next;

	// There will be X elements for each of these vectors.  These are values that are
	//	derived from reading the They will contain :
	//		Average
	//		Total
	//		Middle
	map <string, vector<double>> map_vec_dbl_prev;
	map <string, vector<double>> map_vec_dbl_prev_shared;

	vector<double > vec_dbl_prev_shared_totals;

	// There is no _next requirement for this


public:

	cur_bin2();

	void init(string str_path_bin, int cur_id, string str_filepath_bin2, cur_bin cb_in);

	// Export in BIN2 format.
	void export_bin(string);

	// Import BIN2 format.
	void import_bin(string);

};



#endif