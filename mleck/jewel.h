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

#include <stdlib.h>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <fstream>
#include <random>
#include <string>
#include <map>
#include <vector>


#include <experimental/filesystem>

#include <boost/algorithm/string.hpp>

#include "cur_bin_handler.h"
//#include "jewel_handler.h"

using namespace std;


class jewel_handler;

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
//
//	 I'll move the MAX tag as I develop the subequent elements.
enum class Jewel_type {
	J_RAND
	, J_DIFF
	, MAX
	, J_JEWEL
	, J_RATIO
	, J_VOL
};


class jewel {

private:
	string str_path_bin;

protected:
	// This is a unique string that defines the parameters of the jewel.
	Jewel_type jt;
	string str_id = "";

	bool b_valid = false;

	// different child.  e.g. Each jewel has a 
	//		randomly assigned stochastic value (applied to all calculations)
	//		randomly assigned min
	//		randomly assigned max 
	vector<double> vec_param1;

	// A double value for every instrument.
	vector<double> vec_calc_return;

	double d_calc_return = 0.0;

	// This is used to access the currency data.
	cur_bin_handler* ptr_cbh = nullptr;


public:
	jewel() {ptr_cbh = nullptr;}
	jewel(cur_bin_handler* ptr_cbh_in) { ptr_cbh = ptr_cbh_in; }
	~jewel() {delete ptr_cbh;	}

	//getter/setters 
	Jewel_type get_jewel_type() { return jt; }
	void set_id(string str_id_in) { str_id = str_id_in; }
	string to_string() { return str_id; }

	// This is used only at runtime.
	vector<double>* get_calc_return() { return &vec_calc_return; }

	void init(cur_bin_handler* ptr_cbh_in);

	void set_cbh(cur_bin_handler* ptr_cbh_in) { ptr_cbh = ptr_cbh_in; }


	// These functions are implemented in the inherited classes.
	void calc_return() {};

	string create() { return ""; };


	//https://stackoverflow.com/questions/1340729/how-do-you-generate-a-random-double-uniformly-distributed-between-0-and-1-from-c/26853142#26853142
	inline double randZeroToOne() { return rand() / (RAND_MAX + 1.); }

	inline double randMToN(double M, double N) { return M + (rand() / (RAND_MAX / (N - M))); }

};



#endif