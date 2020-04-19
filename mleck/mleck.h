/* The Mleck object.  This is the unique holder



*/

#pragma once

#ifndef MLECK_H
#define MLECK_H

using namespace std;

#include "settings.h"
#include "jewel_handler.h"
#include "jewel.h"

#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <experimental/filesystem>

#include <boost/algorithm/string.hpp>

class mleck {
private:
	int id = 0;
	int i_gen_birth;

	// The total amount of position points.  + = in profit.
	double d_score = 0.0;

	// This really isn't used in processing.  It's really more just for working out what cumulative
	// values would be.  Score is the evolutionary test, and it is additive.
	double d_val = 100.0;

	// The number of jewels, the vector of id's that is loaded from the settings, and pointers to
	//	jewels themselves.  During runtime, the pointers will be loaded, and when the processing is
	// undertaken, the values will be returned by the jewels by directly referencing the pointers.
	int i_jewels;
	vector<uint64_t> vec_id_jewel;
	vector<std::unique_ptr<jewel> *> vec_up_jewel;

	string str_path_farm;
	string str_filepath_settings;

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

	// When a new mleck is created, this is the range of min/max
	tuple<int, int> mleck_jewel_range;

	jewel_handler * ptr_jh;

public:

	mleck() {};

	mleck(int id_in
		, string str_path_farm_in
		, int i_gen_in
		, tuple<int, int> mleck_jewel_range_in
		, jewel_handler * ptr_jh_in
	);



	// init will open an existing mleck if one exists, or create a randomized one
	//	if one doesn't.
	void init(int id_in
				, string str_path_farm_in
				, int i_gen_in
				, tuple<int, int> mleck_jewel_range_in
				, jewel_handler * ptr_jh_in
				
	);

	// init the mleck objet 
	void init(string str_filepath_in
		, string str_path_farm_in
		, tuple<int, int> mleck_jewel_range_in
		, jewel_handler * ptr_jh_in

	);

	int get_id() { return id; }


	void set_jewel_handler(jewel_handler * ptr_jh_in) { ptr_jh = ptr_jh_in; }


	void load_settings();
	void save_settings();

	// These are for runtime values.  The other values get set on creation and are for
	//	administration.  These are for if you clone a mleck, this sets them 
	void reset_settings();

	void check_jewels();



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