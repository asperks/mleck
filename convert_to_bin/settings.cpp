// logic

#include "settings.h"


void settings::set_prop_str(string key, string val) {
	map_str[key] = val;
}

void settings::set_prop_int(string key, int val) {
	map_int[key] = val;
}

void settings::set_prop_dbl(string key, double val) {
	map_dbl[key] = val;
}


string settings::get_prop_str(string key) {
	string str_return = "";
	if (map_str.find(key) != map_str.end()) {str_return = map_str.at(key);}
	return str_return;
}


int settings::get_prop_int(string key) {
	int i_return = 0;
	if (map_int.find(key) != map_int.end()) {	i_return = map_int.at(key);}
	return i_return;
}


double settings::get_prop_dbl(string key) {
	double d_return = 0.0F;
	if (map_dbl.find(key) != map_dbl.end()) {d_return = map_dbl.at(key);}
	return d_return;
}



void settings::to_string() {
	cout << "Class Name = " << str_class_name;
}