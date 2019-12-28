/* refer mleck.h for details.


*/
#include "mleck.h"

mleck::mleck(int id_in
				, string str_path_farm_in
				, int i_gen_in
				) {
	init(id_in, str_path_farm_in, i_gen_in);
}


void mleck::load_settings() {
	//https://stackoverflow.com/questions/50960492/creating-folders-in-c/56262869#56262869
	bool b_exists = std::experimental::filesystem::exists(str_filepath_settings);
	bool b_change = false;

	if (b_exists == true) {
		// Load the existing settings file
		s.import_text(str_filepath_settings);
	} else {
		reset_settings();
		b_change = true;
	}

	// Test the settings of the file.  Add properties that don't exist.
	if (s.get_prop_int("id") != id) {
		b_change = true;
		s.set_prop_int("id", id);
	} else {
		id = s.get_prop_int("id");
	}

	if (s.get_prop_int("gen_birth") != i_gen_birth) {
		b_change = true;
		s.set_prop_int("gen_birth", i_gen_birth);
	} else {
		i_gen_birth = s.get_prop_int("gen_birth");
	}

	if (b_change == true) {
		// Make sure the settings file aligns with the id.
		str_filepath_settings = str_path_farm + "\\" + std::to_string(id) + "_" + filename_settings;
		// Save the settings file
		s.export_text(str_filepath_settings);
	}
}

void mleck::reset_settings() {
	d_score = 0.0;

	s.set_prop_dbl("score", d_score);

	// Save the settings file
	s.export_text(str_filepath_settings);
}


void mleck::init(int id_in
						, string str_path_farm_in
						, int i_gen_birth_in
						) {
	str_path_farm = str_path_farm_in;
	id = id_in;
	i_gen_birth = i_gen_birth_in;

	str_filepath_settings = str_path_farm + "\\" + std::to_string(id) + "_" + filename_settings;

	load_settings();




}