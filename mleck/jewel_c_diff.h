/*
A class that parses currency data, and formats it so that it provides a
unique view of the properties it is viewing.

Refer to the jewel_relations*.txt files for details of the relationships
between values.  These items determine which items can match up with which
items.

*/

#pragma once

#ifndef JEWEL_C_DIFF_H
#define JEWEL_C_DIFF_H

#include "jewel.h"

using namespace std;

class jewel_c_diff : public jewel {

private:
	// there aren't any in this implementation.
	const double d_instrument_range = 150.0;

	//	The first element doesn't relate to the text elements.  If it is non-zero, it's a modulo
	// allowing the instrument to use.  I'll set the value to a random number between 
	// -50 to 50, and limit the minimum to zero.  Which means all elements would mostly
	// be self referential.  But it does mean that it can view the contents of other instruments.
	// the two elements are the left and right elements.
	vector<int64_t> vec_instrument_source_nth;

	// These are the markers that determine which currency elements are pointed to.
	// The details of the elements to selected are in : 
	//							jewel_relations_pt_diff_leftright.txt
	vector<tuple<int64_t, int64_t, int64_t, double>> vec_opt;



protected:

	// These are the parameters like the links to the fields for which
	// this caculation is achieved.
	vector<double> vec_param2;


public:
	jewel_c_diff() { jewel::ptr_cbh = nullptr; }
	jewel_c_diff(cur_bin_handler* ptr_cbh_in) { jewel::ptr_cbh = ptr_cbh_in; }
	~jewel_c_diff() { delete jewel::ptr_cbh; }

	//getter/setters 


	void calc_return();

	string create();

	// read the string id, and populate the parameters for the object from
	// those parameters.
	void load();


};



#endif