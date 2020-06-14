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


#include "jewel.h"

using namespace std;

class jewel_c_rand : public jewel {

private:
	const double d_rand_max = 10.0;

public:
	//getter/setters 

	jewel_c_rand() { jewel::ptr_cbh = nullptr; }
	jewel_c_rand(cur_bin_handler* ptr_cbh_in) { jewel::ptr_cbh = ptr_cbh_in; }
	~jewel_c_rand() { delete jewel::ptr_cbh; }

	void calc_return();

	string create();

	// read the string id, and populate the parameters for the object from
	// those parameters.
	void load();

	void set_id(string str_id_in) { jewel::set_id(str_id_in); };

	string to_string() { return jewel::str_id; }
};



#endif