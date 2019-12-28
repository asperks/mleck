// utility for recording settings related to an application.  Just use it as a bunch of
//	maps, so it doesn't pollute the program.

#pragma once

#ifndef SETTINGS_H
#define SETTINGS_H

using namespace std;

#include <string>
#include <iostream>
#include <map>

class settings {
	private:
		// Data Members 
		string str_class_name = "settings";

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
		
		void to_string();

};

#endif //SETTINGS_H