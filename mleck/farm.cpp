/* refer farm.h for details.


*/

#include "farm.h"

void farm::load_settings() {
	//https://stackoverflow.com/questions/50960492/creating-folders-in-c/56262869#56262869
	bool b_exists = std::experimental::filesystem::exists(str_filepath_settings);

	if (b_exists == true) {
		// Load the existing settings file
		s.import_text(str_filepath_settings);

	} else {
		// Create a new settings file.  Add these properties.
		s.set_prop_int("gen", 0);
		s.set_prop_int("mleck_count", 100);

		//// Used for debugging
		//s.set_prop_int("test_int", 999);
		//s.set_prop_dbl("test_dbl", 123.456789);
		//s.set_prop_str("test_str", "test_string");

		// Save the new settings file
		s.export_text(str_filepath_settings);
	}
}


void farm::init(int i_farm_in
					, string str_path_bin_in
					, string str_path_farm_in
					) {

	str_filepath_settings = str_path_farm_in + "\\" + filename_settings;
	load_settings();


}
	

void farm::run(int i_gen_count_in
					) {



}


bool exists_test(const std::string& name) {
	ifstream f(name.c_str());
	return f.good();
}


