/* refer mleck.h for details.


*/
#include "mleck.h"

mleck::mleck(int id_in
	, string str_path_farm_in
	, int i_gen_in
	, tuple<int, int> mleck_jewel_range
	, jewel_handler * ptr_jh_in
	) {
	init(id_in, str_path_farm_in, i_gen_in, mleck_jewel_range, ptr_jh_in);
}


void mleck::load_settings() {
	//https://stackoverflow.com/questions/50960492/creating-folders-in-c/56262869#56262869
	bool b_exists = std::experimental::filesystem::exists(str_filepath_settings);
	bool b_change = false;

	if (b_exists == true) {
		// Load the existing settings file
		se.import_text(str_filepath_settings);
	} else {
		reset_settings();
		b_change = true;
	}

	// Test the settings of the file.  Add properties that don't exist.
	if ((se.get_prop_int("id") != id) && (id != 0)) {
		b_change = true;
		se.set_prop_int("id", id);
	} else {
		id = se.get_prop_int("id");
	}

	if ((se.get_prop_int("gen_birth") != i_gen_birth) && (i_gen_birth != 0)) {
		b_change = true;
		se.set_prop_int("gen_birth", i_gen_birth);
	} else {
		i_gen_birth = se.get_prop_int("gen_birth");
	}

	if ((se.get_prop_int("i_jewel_count") != i_jewels) && (i_jewels != 0)) {
		b_change = true;
		se.set_prop_int("i_jewel_count", i_jewels);
	} else {
		i_jewels = se.get_prop_int("i_jewel_count");
	}

	if (se.get_prop_str("delim_jewels") == "") {
		b_change = true;
		se.set_prop_str("delim_jewels", " ");
	} else {
		string str_jewels = se.get_prop_str("delim_jewels");
		boost::algorithm::trim(str_jewels);
		if (str_jewels != "") {
			vector<string> vec_str_jewel;
			boost::split(vec_str_jewel, str_jewels, boost::is_any_of("~"));
			if (vec_str_jewel.size() > 0) {
				for (size_t i = 0; i < vec_str_jewel.size(); ++i) {
					string str_id = vec_str_jewel.at(i);
					boost::algorithm::trim(str_id);
					if (str_id != "") {
						vec_id_jewel.push_back(str_id);


						//// Convert a string to a uint64_t
						//// https://stackoverflow.com/questions/42356939/c-convert-string-to-uint64-t/42357045#42357045
						//uint64_t u64_v;
						//std::istringstream iss(str_id);
						//iss >> u64_v;
						//// This populates the vector with the index of the jewel.  During the processing stage,
						//// this data will be matched with the jewel_handler so that it can be directly accessed.
						//vec_id_jewel.push_back(u64_v);
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
	str_filepath_settings = str_path_farm + "\\" + filename_settings_prefix + std::to_string(id) + filename_settings_suffix;
	// Save the settings file
	se.export_text(str_filepath_settings);
}


void mleck::reset_settings() {
	d_score = 0.0;
	d_val = 100.0;

	se.set_prop_dbl("score", d_score);

	// This really isn't used in processing.  It's really more just for working out what cumulative
	// values would be.  Score is the evolutionary test, and it is additive.
	se.set_prop_dbl("val", d_val);

	// Save the settings file
	se.export_text(str_filepath_settings);
}


void mleck::init(int id_in
	, string str_path_farm_in
	, int i_gen_birth_in
	, tuple<int, int> mleck_jewel_range_in
	, jewel_handler * ptr_jh_in
	) {
	set_jewel_handler(ptr_jh_in);
	mleck_jewel_range = mleck_jewel_range_in;
	str_path_farm = str_path_farm_in;
	id = id_in;
	i_gen_birth = i_gen_birth_in;

	str_filepath_settings = str_path_farm + "\\" + std::to_string(id) + "_" + filename_settings_suffix;

	load_settings();

	check_jewels();
}


void mleck::init(string str_filepath_in
	, string str_path_farm_in
	, tuple<int, int> mleck_jewel_range_in
	, jewel_handler * ptr_jh_in
	) {

	set_jewel_handler(ptr_jh_in);
	mleck_jewel_range = mleck_jewel_range_in;
	str_path_farm = str_path_farm_in;
	str_filepath_settings = str_filepath_in;
	load_settings();

	id = se.get_prop_int("id");
	i_gen_birth = se.get_prop_int("gen_birth");

	check_jewels();
}


void mleck::check_jewels() {
	bool b_change = false;
	if (i_jewels == 0) {
		b_change = true;
		// set a random number between the tuple min max extents.
		i_jewels = std::get<0>(mleck_jewel_range) + (std::rand() % (std::get<1>(mleck_jewel_range) - std::get<0>(mleck_jewel_range) + 1));
		se.set_prop_int("i_jewel_count", i_jewels);
	}

	if (vec_id_jewel.size() < i_jewels) {
		b_change = true;
		// Create/Link to jewels.
		for (int i = 0; i < (i_jewels - vec_id_jewel.size()); ++i) {
			// This passes the jewel id string (a string of characters that defines the jewel.
			//vec_up_jewel.push_back(ptr_jh->get_jewel_ptr(vec_id_jewel.at(i)));
		}
	}

	if (b_change == true) {
		save_settings();
	}

	// Check the pointers
	if (vec_id_jewel.size() != vec_up_jewel.size()) {

	}
}