/*
A class that parses currency data, and formats it so that it provides a
unique view of the properties it is viewing.

Refer to the jewel_relations*.txt files for details of the relationships
between values.  These items determine which items can match up with which
items.

*/

#pragma once

#ifndef JEWEL_C_RAND_H
#define JEWEL_C_RAND_H

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

class jewel_c_rand : public jewel {

private:
	// there aren't any in this implementation.

public:
	//getter/setters 

	jewel_c_rand();

	void calc_return();

	string create(int id_mleck_in);

};



#endif