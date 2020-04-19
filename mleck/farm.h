// utility for recording settings related to an application.  Just use it as a bunch of
//	maps, so it doesn't pollute the program.
//
//	Forked from convert_to_bin utility 2019-12-28

#pragma once

#ifndef FARM_H
#define FARM_H

#include "settings.h"
#include "mleck.h"
#include "cur_bin_handler.h"
#include "jewel_handler.h"

#include <iomanip>
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <experimental/filesystem>

using namespace std;


class farm {
private:
	const char * str_class_name = "farm";

	int id;
	int i_gen_count;
	int i_cur_bin;

	bool b_loaded = false;

	string str_path_bin;
	string str_path_farm;
	string str_filepath_settings;
	string str_filepath_farm_alive;
	string str_filepath_farm_dead;

	settings se;
	const char * filename_settings = "mleck.settings";
	/*	The settings file will allow me to flexibly add settings to the program,
	store them in an external text file, and modify them if I want to change
	them for different effects.
	
	int	gen								the current gen
	int	mleck_count				the mleck cohort count
	int	mleck_id_highest	record of the id of the last mleck created
	int	gen_proc					generations to process before quitting.

	int	cur_id_last				The last cur_id that was processed.  Useful for
												sequential stepping
	int	cur_id_step				1 = random.  2 = sequential.
												If it meets the end of the sequence, it quits

	dbl	mleck_random_PC		the percentage of replacement

	*/

	// The farm object owns the vector of mlecks, and the currency and jewel handlers.
	vector<mleck> vec_m;
	cur_bin_handler cbh;
	jewel_handler jh;


public:

	farm();

	void init(int id_in
				, string str_path_bin_in
				, string str_path_farm_in
				);

	// Load the settings from an external settings file for a farm, or populate
	//	the settings file with default values.  They can be modified if required.
	void load_settings();

	// Ensure all of the available slots for mlecks are filled so the full 
	//	cohort is available.
	void populate_mlecks();


	void run(int i_gen_count_in
				);


};



#endif