/*



*/

#pragma once

#ifndef MLECK_H
#define MLECK_H

using namespace std;

#include "settings.h"

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
	int id;
	int i_gen_birth;

	// The total amount of position points.  + = in profit.
	double d_score = 0.0;

	string str_path_farm;
	string str_filepath_settings;

	settings s;
	const char * filename_settings = "id.settings";
	/*	The settings file will allow me to flexibly add settings to the program,
	store them in an external text file, and modify them if I want to change
	them for different effects.

	int	id
	int	gen_birth


	// Used for debugging.
	int	test_int
	dbl	test_dbl
	str	test_str

	*/


public:
	mleck(int id_in
		, string str_path_farm_in
		, int i_gen_in
	);



	// init will open an existing mleck if one exists, or create a randomized one
	//	if one doesn't.
	void init(int id_in
				, string str_path_farm_in
				, int i_gen_in
	);


	void load_settings();

	// These are for runtime values.  The other values get set on creation and are for
	//	administration.  These are for if you clone a mleck, this sets them 
	void reset_settings();

	// Still thinking about how I'm going to handle this one.
	void process();


	void mutate();


	// Create a new mleck settings file based upon the old mleck.
	void clone(mleck m
					, int i_gen_in
					);


	// Delete the mleck settings file.
	void purge(int id);

};



#endif