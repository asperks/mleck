/*
A class that parses currency data, and formats it so that it provides a
unique view of the properties it is viewing.

*/

#pragma once

#ifndef JEWEL_H
#define JEWEL_H

using namespace std;

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



public:

	jewel();

	void init();

};



#endif