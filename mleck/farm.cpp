/* refer farm.h for details.


*/

#include "farm.h"

// constructor
farm::farm() {

}

void farm::load_settings() {
	//https://stackoverflow.com/questions/50960492/creating-folders-in-c/56262869#56262869
	bool b_exists = std::experimental::filesystem::exists(str_filepath_settings);

	if (b_exists == true) {
		// Load the existing settings file
		s.import_text(str_filepath_settings);
	}

	// Test the settings of the file.  Add properties that don't exist.
	bool b_change = false;

	if (s.get_prop_int("gen") == 0) {
		b_change = true;
		s.set_prop_int("gen", 0);
	}

	if (s.get_prop_int("mleck_count") == 0) {
		b_change = true;
		s.set_prop_int("mleck_count", 100);
	}

	if (s.get_prop_int("mleck_id_highest") == 0) {
		b_change = true;
		s.set_prop_int("mleck_id_highest", 0);
	}

	if (s.get_prop_dbl("mleck_random_PC") == 0.0) {
		b_change = true;
		s.set_prop_dbl("mleck_random_PC", 5.0);
	}

	if (s.get_prop_int("cur_id_last") == 0) {
		b_change = true;
		s.set_prop_int("cur_id_last", 1);
	}

	if (s.get_prop_int("cur_id_step") == 0) {
		b_change = true;
		s.set_prop_int("cur_id_step", 1);
	}


	if (s.get_prop_int("gen_proc") == 0) {
		b_change = true;
		s.set_prop_int("gen_proc", 5);
	}



	if (b_change == true) {
		// Save the new settings file
		s.export_text(str_filepath_settings);
	}
}


void farm::init(int i_farm_in
	, string str_path_bin_in
	, string str_path_farm_in
) {
	str_path_farm = str_path_farm_in;
	str_filepath_settings = str_path_farm_in + "\\" + filename_settings;
	load_settings();

	populate_mlecks();

	// The mleck objects are ready.  Now it's time to prep the currency data available.

	// initialize the cbh object, so that it has data to traverse
	cbh.init(str_path_bin_in);

}
void farm::run(int i_gen_count_in
					) {
	std::cout << "farm run start" << std::endl;

	int i_gen_proc_total = s.get_prop_int("gen_proc");

	if (cbh.is_valid() == false) {
		std::cout << "NO CUR ####.BIN FILES!  EXITING" << std::endl;
	} else {
		for (size_t i_gen_proc = 0; i_gen_proc < i_gen_proc_total; ++i_gen_proc) {
			std::cout << "\tfarm gen start:" << i_gen_proc << std::endl;

			int i_cur_id = -1;

			//	int	cur_id_step				1 = random.  2 = sequential.
			//										If it meets the end of the sequence, it quits
			if (s.get_prop_int("cur_id_step") == 1) {
				// select a random cur_id
				i_cur_id = cbh.load_cur_bin(1, 0);
			} else {
				if (i_cur_id == -1) {
					i_cur_id = cbh.load_cur_bin(1, 0);
				} else {
					i_cur_id = cbh.load_cur_bin(2, i_cur_id);
				}
			}
			



			std::cout << "\tfarm gen end:  " << i_gen_proc << std::endl;
		}

	}


	std::cout << "farm run end" << std::endl;
}




void farm::populate_mlecks() {

	// Load existing mlecks that are supposed to be available.
	string str_m_list = s.get_prop_str("mlecks");
	vector<string> vec_m_str;
	boost::split(vec_m_str, str_m_list, boost::is_any_of("~"));
	if (vec_m_str.size() > 0) {
		for (size_t i_vec = 0; i_vec < vec_m_str.size(); ++i_vec) {
			string str_m = vec_m_str.at(i_vec);
			boost::algorithm::trim(str_m);
			if (str_m.compare("") != 0) {
				int i_m = std::stoi(str_m);
				mleck m(i_m, str_path_farm, s.get_prop_int("gen"));
				vec_m.push_back(m);
			}
		}
	}

	// Populate the mleck list with mlecks so that it meets the mleck count.
	if (vec_m.size() < s.get_prop_int("mleck_count")) {
		size_t i_new = boost::lexical_cast<size_t>(s.get_prop_int("mleck_count") - vec_m.size());
		int i_mleck_id_highest = s.get_prop_int("mleck_id_highest");
		int i_gen = s.get_prop_int("gen");
		for (size_t i1 = 0; i1 < i_new; ++i1) {
			++i_mleck_id_highest;
			mleck m(i_mleck_id_highest, str_path_farm, i_gen);
			vec_m.push_back(m);
			if (str_m_list.compare("") != 0) {
				str_m_list = str_m_list + "~" + std::to_string(i_mleck_id_highest);
			} else {
				str_m_list = std::to_string(i_mleck_id_highest);

			}
		}
		// Write out the data.
		s.set_prop_int("mleck_id_highest", i_mleck_id_highest);
		s.set_prop_str("mlecks", str_m_list);
		s.export_text(str_filepath_settings);
	}
}


bool exists_test(const std::string& name) {
	ifstream f(name.c_str());
	return f.good();
}


