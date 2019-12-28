// utility for recording settings related to an application.  Just use it as a bunch of
//	maps, so it doesn't pollute the program.
//
//	Forked from convert_to_bin utility 2019-12-28

#pragma once

#ifndef FARM_H
#define FARM_H

using namespace std;

#include "settings.h"

#include <iomanip>
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <experimental/filesystem>


class farm {
private:
	const char * str_class_name = "farm";

	int i_farm;
	int i_gen_count;
	int i_cur_bin;

	string str_path_bin;
	string str_path_farm;
	string str_filepath_settings;

	settings s;
	const char * filename_settings = "mleck.settings";
	/*	The settings file will allow me to flexibly add settings to the program,
	store them in an external text file, and modify them if I want to change
	them for different effects.
	
	int	gen
	int	mleck_count


	// Used for debugging.
	int	test_int
	dbl	test_dbl
	str	test_str
	
	*/





public:

	void init(int i_farm_in
				, string str_path_bin_in
				, string str_path_farm_in
				);

	void run(int i_gen_count_in
				);


	// Load the settings from an external settings file for a farm, or populate
	//	the settings file with default values.  They can be modified if required.
	void load_settings();

};



#endif