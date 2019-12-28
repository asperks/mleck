/*
A class that handles all of the interface between the mleck engine and the cur
objects.

*/

#pragma once

#ifndef CUR_BIN_HANDLER_H
#define CUR_BIN_HANDLER_H

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

class cur_bin_handler {
private:
	bool b_valid = false;
	string str_path_bin;

	vector<int> vec_i_bin;

public:

	cur_bin_handler(string str_path_bin_in);

	void init(string str_path_bin_in);

};



#endif