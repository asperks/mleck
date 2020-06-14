/* The Mleck object.  This is the unique holder



*/

#pragma once

#ifndef MLECK_H
#define MLECK_H

using namespace std;

#include "settings.h"
#include "jewel_handler.h"
#include "jewel.h"

#include <stdlib.h>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <fstream>
#include <memory>
#include <random>
#include <string>
#include <map>
#include <vector>

#include <experimental/filesystem>

#include <boost/algorithm/string.hpp>

class mleck {
private:
	int id = 0;
	int i_gen_birth;

	string str_filename_settings;
	string str_path_farm_alive;
	string str_path_farm_dead;
	string str_filepath_settings;

	// The total amount of position points.  + = in profit.
	double d_score = 0.0;

	// This really isn't used in processing.  It's really more just for working out what cumulative
	// values would be.  Score is the evolutionary test, and it is additive.
	double d_val = 100.0;

	// The number of jewels, the vector of id's that is loaded from the settings, and pointers to
	//	jewels themselves.  During runtime, the pointers will be loaded, and when the processing is
	// undertaken, the values will be returned by the jewels by directly referencing the pointers.
	int i_jewels;
	vector<string> vec_id_jewel;
	vector<std::unique_ptr<jewel> *> vec_up_jewel;



	settings se;
	const char* filename_settings_prefix = "idmleck_";
	const char * filename_settings_suffix = ".settings";
	/*	The settings file will allow me to flexibly add settings to the program,
	store them in an external text file, and modify them if I want to change
	them for different effects.

	int	id
	int	i_gen_birth
	int	i_jewels

	// Used for debugging.
	int	test_int
	dbl	test_dbl
	str	test_str

	*/

	jewel_handler * jh_ptr = nullptr;


public:

	mleck(string str_path_farm_alive_in, string str_path_farm_dead_in);

	~mleck() {
		//vec_up_jewel.clear();
		//vec_id_jewel.clear();
		//delete jh_ptr;
	}

	// init will open an existing mleck if one exists, or create a randomized one
	//	if one doesn't.
	void init(int id_in
				, int i_gen_in
				, jewel_handler * ptr_jh_in
				
	);

	// init the mleck object 
	void init(string str_filepath_in
		, jewel_handler * ptr_jh_in
	);

	

	int get_id() { return id; }


	void set_jewel_handler(jewel_handler * ptr_jh_in) { jh_ptr = ptr_jh_in; }


	void load_settings();
	void save_settings();

	// These are for runtime values.  The other values get set on creation and are for
	//	administration.  These are for if you clone a mleck, this sets them 
	void reset_settings();

	// This function reads the mleck jewels, and updates the jewel_handler
	void jewel_load();

	// This function ensures a mleck has the valid number of jewls, and if not, creates/links more.
	void jewel_check(tuple<int, int> jewel_range, int i_new_threshold);



	// Still thinking about how I'm going to handle this one.
	void process();

	// reset the jewel pointer vector
	void mutate();


	// Create a new mleck settings file based upon the old mleck.
	// reset the jewel pointer vector
	void clone(mleck m
					, int i_gen_in
					);


	// Delete the mleck settings file.
	void purge(int id);

};



#endif