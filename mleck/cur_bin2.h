/*
A class that parses currency data, and formats it so that it provides a
unique view of the properties it is viewing.

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

public:

	cur_bin2();

	void init(int cur_id, string str_filepath_bin2, cur_bin cb_in);

	// Export in BIN2 format.
	void export_bin(string);

	// Import BIN2 format.
	void import_bin(string);

};



#endif