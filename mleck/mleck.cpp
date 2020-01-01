/* refer mleck.h for details.


*/
#include "mleck.h"

mleck::mleck(int id_in
	, string str_path_farm_in
	, int i_gen_in
	, tuple<int, int> mleck_jewel_range
	, jewel_handler jh_in

) {
	init(id_in, str_path_farm_in, i_gen_in, mleck_jewel_range, jh_in);
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
	if ((s.get_prop_int("id") != id) && (id != 0)) {
		b_change = true;
		s.set_prop_int("id", id);
	} else {
		id = s.get_prop_int("id");
	}

	if ((s.get_prop_int("gen_birth") != i_gen_birth) && (i_gen_birth != 0)) {
		b_change = true;
		s.set_prop_int("gen_birth", i_gen_birth);
	} else {
		i_gen_birth = s.get_prop_int("gen_birth");
	}

	if ((s.get_prop_int("i_jewel_count") != i_jewels) && (i_jewels != 0)) {
		b_change = true;
		s.set_prop_int("i_jewel_count", i_jewels);
	} else {
		i_jewels = s.get_prop_int("i_jewel_count");
	}

	if (s.get_prop_str("delim_jewels") == "") {
		b_change = true;
		s.set_prop_str("delim_jewels", " ");
	} else {
		string str_jewels = s.get_prop_str("delim_jewels");
		boost::algorithm::trim(str_jewels);
		if (str_jewels != "") {
			vector<string> vec_str_jewel;
			boost::split(vec_str_jewel, str_jewels, boost::is_any_of("~"));
			if (vec_str_jewel.size() > 0) {
				for (size_t i = 0; i < vec_str_jewel.size(); ++i) {
					string str_id = vec_str_jewel.at(i);
					boost::algorithm::trim(str_id);
					if (str_id != "") {
						vec_id_jewel.push_back(stoi(str_id));
					}
				}
			}
		}
	}

	if (b_change == true) {
		// Make sure the settings file aligns with the id.
		save_settings();
	}
}

void mleck::save_settings() {
	str_filepath_settings = str_path_farm + "\\" + std::to_string(id) + "_" + filename_settings;
	// Save the settings file
	s.export_text(str_filepath_settings);
}

void mleck::reset_settings() {
	d_score = 0.0;
	d_val = 100.0;

	s.set_prop_dbl("score", d_score);

	// This really isn't used in processing.  It's really more just for working out what cumulative
	// values would be.  Score is the evolutionary test, and it is additive.
	s.set_prop_dbl("val", d_val);

	// Save the settings file
	s.export_text(str_filepath_settings);
}



void mleck::init(int id_in
	, string str_path_farm_in
	, int i_gen_birth_in
	, tuple<int, int> mleck_jewel_range_in
	, jewel_handler jh_in
) {
	set_jewel_handler(jh_in);
	mleck_jewel_range = mleck_jewel_range_in;
	str_path_farm = str_path_farm_in;
	id = id_in;
	i_gen_birth = i_gen_birth_in;

	str_filepath_settings = str_path_farm + "\\" + std::to_string(id) + "_" + filename_settings;


	load_settings();

	check_jewels();


}

void mleck::check_jewels() {
	bool b_change = false;
	if (i_jewels == 0) {
		b_change = true;
		// set a random number between the tuple min max extents.
		i_jewels = std::get<0>(mleck_jewel_range) + (std::rand() % (std::get<1>(mleck_jewel_range) - std::get<0>(mleck_jewel_range) + 1));
		s.set_prop_int("i_jewel_count", i_jewels);
	}

	if (vec_id_jewel.size() < i_jewels) {
		b_change = true;
		// Create/Link to jewels.
		for (int i = 0; i < (i_jewels - vec_id_jewel.size()); ++i) {
			int id_jewel = ptr_jh->create_jewel_link(id);
			vec_id_jewel.push_back(id_jewel);
		}
	}

	if (b_change == true) {
		save_settings();
	}

	// Check the pointers
	if (vec_id_jewel.size() != vec_jewel.size()) {

	}
}