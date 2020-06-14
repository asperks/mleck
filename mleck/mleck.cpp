/* refer mleck.h for details.


*/
#include "mleck.h"

mleck::mleck(string str_path_farm_alive_in, string str_path_farm_dead_in) {
	str_path_farm_alive = str_path_farm_alive_in;
	str_path_farm_dead = str_path_farm_dead_in;
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
						// Create the unique pointer to the jewel handler.
						std::unique_ptr<jewel>* up_jewel_ptr = jh_ptr->jewel_link(str_id);
						vec_up_jewel.push_back(std::move(up_jewel_ptr));
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

// Save the settings file.  Save settings only works for live mlecks.  
void mleck::save_settings() {
	// create the jewel settings string.
	bool b_first = true;
	string str_jewels = "";
	if (vec_id_jewel.size() > 0) {

		for (size_t i = 0; i < vec_id_jewel.size(); ++i) {
			if (b_first == false) {
				str_jewels = str_jewels + "~";
			}
			str_jewels = str_jewels + vec_id_jewel.at(i);
			b_first = false;
		}
	}
	se.set_prop_str("delim_jewels", str_jewels);

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
	, int i_gen_birth_in
	, jewel_handler* ptr_jh_in
) {
	set_jewel_handler(ptr_jh_in);
	id = id_in;
	i_gen_birth = i_gen_birth_in;

	str_filename_settings = filename_settings_prefix + std::to_string(id) + filename_settings_suffix;

	// The assumption for now is that loading a mleck will always be from the alive section.
	str_filepath_settings = str_path_farm_alive + "\\" + str_filename_settings;

	load_settings();

	jewel_load();
}


void mleck::init(string str_filename_in
	, jewel_handler* ptr_jh_in
) {
	str_filename_settings = str_filename_in;

	set_jewel_handler(ptr_jh_in);

	// The assumption for now is that loading a mleck will always be from the alive section.
	str_filepath_settings = str_path_farm_alive + "\\" + str_filename_settings;

	load_settings();

	id = se.get_prop_int("id");
	i_gen_birth = se.get_prop_int("gen_birth");



}


void mleck::jewel_load() {
	std::cout << "mleck::jewel_load \t: \t" << get_id() << "\n";

	if (vec_id_jewel.size() > 0) {
		vec_up_jewel.clear();
		for (int i = 0; i < vec_id_jewel.size(); ++i) {
			unique_ptr<jewel>* up_j_ptr = jh_ptr->jewel_link(vec_id_jewel.at(i));
			//std::cout << "jewel_load \t: \t" << up_j_ptr->get()->get_id()  << "\n";
			vec_up_jewel.push_back(std::move(up_j_ptr));
		}
	}
	std::cout << "mleck::jewel_load \t: END" << "\n";
}



void mleck::jewel_check(tuple<int, int> tup_jewel_range, int i_new_threshold) {
	std::cout << "mleck::jewel_check \t: \t" << get_id() << "\n";
	bool b_change = false;

	if (i_jewels == 0) {
		b_change = true;
		// set a random number between the tuple min max extents.
		i_jewels = std::get<0>(tup_jewel_range) + (std::rand() % (std::get<1>(tup_jewel_range) - std::get<0>(tup_jewel_range) + 1));
		se.set_prop_int("i_jewel_count", i_jewels);
	}

	std::cout << "mleck::jewel_check \t: jewels = " << i_jewels << "\n";

	if (vec_id_jewel.size() < i_jewels) {
		// This means the jewel count of the mleck is less than the number it should have.
		b_change = true;
		// Create/Link to jewels.
		for (int i = 0; i < (i_jewels - vec_id_jewel.size()); ++i) {
			// This passes the jewel id string (a string of characters that defines the jewel.
			vec_id_jewel.push_back(jh_ptr->get_jewel_id(i_new_threshold));
		}
	}

	if (b_change == true) {
		save_settings();
	}

	jewel_load();
	std::cout << "mleck::jewel_check \t: END" << "\n";
}