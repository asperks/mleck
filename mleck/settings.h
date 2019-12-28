// utility for recording settings related to an application.  Just use it as a bunch of
//	maps, so it doesn't pollute the program.
//
//	Added import/export functions 2019-12-28
//	Forked from convert_to_bin utility 2019-12-28
//

#pragma once

#ifndef SETTINGS_H
#define SETTINGS_H

using namespace std;

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string.hpp> 

#include <boost/date_time.hpp>

class settings {
	private:
		// Data Members 
		const char * str_class_name = "settings";

		map<string, string> map_str;
		map<string, int> map_int;
		map<string, double> map_dbl;


	public:
		// Member Functions() 

		void set_prop_str(string, string);
		
		void set_prop_int(string, int);

		void set_prop_dbl(string, double);

		string get_prop_str(string);
		
		int get_prop_int(string);

		double get_prop_dbl(string);

		// Export the maps to an external filename provided.  It is text
		//	editable.
		void export_text(string);

		// Import the settings from an external file.
		void import_text(string);
		
		void to_string();

};

#endif //SETTINGS_H