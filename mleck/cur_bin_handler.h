/*
A class that handles all of the interface between the mleck engine and the cur
objects.  I'm tossing up whether I should rename this class as 'shape' at some
point.  It is about the shape of a number of currency points, and they get
transformed into other representations.

This class creates BIN2 files.  These read cur_bin bin files, uses one in order to
compare the previous bin to this one, and the next one in order to measure
performance from the current one selected.  So it requires three sequential bin
files in order to develop a BIN2 file, which is used in further processing.

If this were to be used live, of course, the NEXT one wouldn't be necessary, only
the post one.  A part of logic I'd need to change in order to change to make it
perform live.

Created 2019-12-29

*/

#pragma once

#ifndef CUR_BIN_HANDLER_H
#define CUR_BIN_HANDLER_H

using namespace std;

#include "cur_bin.h"
#include "cur_bin2.h"

#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <experimental/filesystem>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

class cur_bin_handler {
private:
	bool b_valid = false;
	string str_path_bin;
	string str_path_bin2;

	// The id extents of the bins available for processing.
	int i_cur_id_min = -1;
	int i_cur_id_max = -1;

	int i_cur_id_open = -1;
	cur_bin cb_open;
	cur_bin2 cb2_open;

	// list of bins, id and filepath
	map<int, string> map_bin;
	map<int, string> map_bin2;

	// returns the next id from the parameter that meets the criteria for
	//	creating a bin2 file.  It will start at the beginning of the cohort if it
	//	goes past the max number.
	int get_next_id(int id_in);

	// returns a random id selected from the bin map that meets the criteria
	// for creating a bin2 file.
	int get_random_id();

	bool is_valid_id(int id_in);

public:
	//getter/setters
	bool is_valid() { return b_valid; }

	// Constructor : blank.
	cur_bin_handler();

	// Constructor : Pass data loc.
	cur_bin_handler(string str_path_bin_in);

	// This reads in a map of bin and bin2 files, which are the read-only cur
	// data, and the processing of that data at that point.  
	// This utility creates the bin2 files the first time it reads them. The
	//	next time it processes the same gen, it re-loads the bin2 files into mem.
	void init(string str_path_bin_in);

	// Set this to 1 in order to randomize.  2 to do sequentially.  If random,
	//	ignore the second parameter.  If sequential start sequence at that id.
	// If that id is above max, it starts at min.
	int load_cur_bin(int i_cur_step_type, int i_cur_id);


};



#endif