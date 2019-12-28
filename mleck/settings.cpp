// refer settings.h for details
//
//	Forked from convert_to_bin utility 2019-12-28

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
	if (map_str.find(key) != map_str.end()) { str_return = map_str.at(key); }
	return str_return;
}

int settings::get_prop_int(string key) {
	int i_return = 0;
	if (map_int.find(key) != map_int.end()) { i_return = map_int.at(key); }
	return i_return;
}

double settings::get_prop_dbl(string key) {
	double d_return = 0.0F;
	if (map_dbl.find(key) != map_dbl.end()) { d_return = map_dbl.at(key); }
	return d_return;
}

void settings::export_text(string str_filepath_settings) {
	ofstream ofs;
	ofs.open(str_filepath_settings);

	if (map_int.size() > 0) {
		for (auto const& x : map_int) {
			ofs << "int|" << x.first  // string (key)
				 << '|'
				 << x.second // int's value 
				 << std::endl;
		}
	}

	if (map_dbl.size() > 0) {
		for (auto const& x : map_dbl) {
			ofs << "dbl|" << x.first  // string (key)
				 << '|'
				 << fixed << setprecision(8) << x.second // dbl's value 
				 << std::endl;
		}
	}

	if (map_str.size() > 0) {
		for (auto const& x : map_str) {
			ofs << "str|" << x.first  // string (key)
				 << '|'
				 << x.second // string's value 
				 << std::endl;
		}
	}

	ofs.close();
}


void settings::import_text(string str_filepath_settings) {
	string str_line;
	ifstream ifs(str_filepath_settings);
	while (getline(ifs, str_line)) {
		vector<string> vec_line;
		boost::split(vec_line, str_line, boost::is_any_of("|"));

		if (vec_line.size() >= 3) {
			//https://www.techiedelight.com/convert-string-to-int-cpp/
			if (vec_line.at(0).compare("str") == 0) {
				map_str[vec_line.at(1)] = vec_line.at(2);
			} else if (vec_line.at(0).compare("int") == 0) {
				map_int[vec_line.at(1)] = std::stoi(vec_line.at(2));
			} else if (vec_line.at(0).compare("dbl") == 0) {
				map_dbl[vec_line.at(1)] = std::stod(vec_line.at(2));
			}
		}
	}
	ifs.close();
}


void settings::to_string() {
	cout << "Class Name = " << str_class_name;
}