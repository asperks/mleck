// refer instrument_gen.h for details.


#include "instrument_gen.h"

int instrument_gen::init(string str_gen
								, vector<string> vec_instruments
								, string str_path_scripture
								, string str_filepath_bin
								) {
	vector<string> vec_errors;




	if (vec_errors.size() > 0) {
		std::cout << "Errors found in " << str_class_name << "[" << __func__ << "]" << " :\n";
		for (int i = 0; i < vec_errors.size(); ++i) {
			std::cout << "\t" << vec_errors.at(i);
		}
	}
	// return 0 if no errors.
	return static_cast<int>(vec_errors.size());
}


int instrument_gen::read_bin() {
	vector<string> vec_errors;




	if (vec_errors.size() > 0) {
		std::cout << "Errors found in " << str_class_name << "[" << __func__ << "]" << " :\n";
		for (int i = 0; i < vec_errors.size(); ++i) {
			std::cout << "\t" << vec_errors.at(i);
		}
	}
	// return 0 if no errors.
	return static_cast<int>(vec_errors.size());
}

int instrument_gen::write_bin() {
	vector<string> vec_errors;




	if (vec_errors.size() > 0) {
		std::cout << "Errors found in " << str_class_name << "[" << __func__ << "]" << " :\n";
		for (int i = 0; i < vec_errors.size(); ++i) {
			std::cout << "\t" << vec_errors.at(i);
		}
	}
	// return 0 if no errors.
	return static_cast<int>(vec_errors.size());
}

string instrument_gen::to_string() {
	string str_return = "";

	return str_return;
}
