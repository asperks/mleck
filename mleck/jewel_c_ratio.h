/*
A class that parses currency data, and formats it so that it provides a
unique view of the properties it is viewing.

Refer to the jewel_relations*.txt files for details of the relationships
between values.  These items determine which items can match up with which
items.

*/

#pragma once

#ifndef JEWEL_C_RATIO_H
#define JEWEL_C_RATIO_H

#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <experimental/filesystem>

#include <boost/algorithm/string.hpp>

#include "jewel.h"

using namespace std;

class jewel_c_ratio : public jewel {

private:
	int i_jewel_relation_lhs = -1;
	int i_jewel_relation_rhs = -1;

	vector<double> vec_dbl;

public:
	//getter/setters 

	jewel_c_ratio() {};

	// These will be necessary to parse into a binary file.
	int get_jewel_relation_lhs() { return i_jewel_relation_lhs; }
	int get_jewel_relation_rhs() { return i_jewel_relation_rhs; }

	void calc_return();

	string create(int id_mleck_in);

};



#endif