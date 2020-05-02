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
		se.import_text(str_filepath_settings);
	}

	// Test the settings of the file.  Add properties that don't exist.
	bool b_change = false;

	if (se.get_prop_int("gen") == 0) {
		b_change = true;
		se.set_prop_int("gen", 0);
	}

	if (se.get_prop_int("mleck_count") == 0) {
		b_change = true;
		se.set_prop_int("mleck_count", 100);
	}

	if (se.get_prop_int("mleck_id_highest") == 0) {
		b_change = true;
		se.set_prop_int("mleck_id_highest", 0);
	}

	if (se.get_prop_dbl("mleck_random_PC") == 0.0) {
		b_change = true;
		se.set_prop_dbl("mleck_random_PC", 5.0);
	}

	if (se.get_prop_int("cur_id_last") == 0) {
		b_change = true;
		se.set_prop_int("cur_id_last", 1);
	}

	if (se.get_prop_int("cur_id_step") == 0) {
		b_change = true;
		se.set_prop_int("cur_id_step", 1);
	}

	if (se.get_prop_int("gen_proc") == 0) {
		b_change = true;
		se.set_prop_int("gen_proc", 5);
	}

	if (se.get_prop_int("mleck_start_jewel_min") == 0) {
		b_change = true;
		se.set_prop_int("mleck_start_jewel_min", 5);
	}

	if (se.get_prop_int("mleck_start_jewel_max") == 0) {
		b_change = true;
		se.set_prop_int("mleck_start_jewel_max", 10);
	}

	if (b_change == true) {
		// Save the new settings file
		se.export_text(str_filepath_settings);
	}
}


void farm::init(int i_farm_in
	, string str_path_bin_in
	, string str_path_farm_in
							) {
	str_path_farm = str_path_farm_in;
	str_filepath_settings = str_path_farm_in + "\\" + filename_settings;
	load_settings();

	// Seed the random number generator
	srand((unsigned)time(NULL));

	// Make sure there is a folder for the dead mlecks.  Files are copied from 
	str_filepath_farm_alive = str_path_farm_in + "\\alive";
	if (boost::filesystem::is_directory(str_filepath_farm_alive) == false) {
		// The farm folder doesn't exist.  Create it.
		//https://stackoverflow.com/questions/50960492/creating-folders-in-c/56262869#56262869
		std::experimental::filesystem::create_directories(str_filepath_farm_alive);
	}

	// Make sure there is a folder for the dead mlecks.  Files are copied from alive to dead
	// when they have expired.
	str_filepath_farm_dead = str_path_farm_in + "\\dead";
	if (boost::filesystem::is_directory(str_filepath_farm_dead) == false) {
		// The farm folder doesn't exist.  Create it.
		//https://stackoverflow.com/questions/50960492/creating-folders-in-c/56262869#56262869
		std::experimental::filesystem::create_directories(str_filepath_farm_dead);
	}

	// initialize the cbh object, so that it has data to traverse
	cbh.init(str_path_bin_in);

	// the cur_bin_handler changes state by the load_cur_bin function.  But that should
	// be the last time it is directly accessed from here.  It should be accessed via
	//	the jewel_handler and jewel objects as a pointer.
	jh.init(& cbh);

	// Had this backward before.  The cur_bin_handler and more importantly jewel_handler,
	//	need to be available, because they'll be used when populating the mlecks.
	populate_mlecks();

}
void farm::run(int i_gen_count_in) {
	std::cout << "farm run start" << std::endl;

	int i_gen_proc_total = se.get_prop_int("gen_proc");

	if (cbh.is_valid() == false) {
		std::cout << "NO CUR ####.BIN FILES!  EXITING" << std::endl;
	}
	else {
		int i_cur_id = -1;

		srand((unsigned)time(0));

		for (size_t i_gen_proc = 0; i_gen_proc < i_gen_proc_total; ++i_gen_proc) {
			std::cout << "\tfarm gen start:" << (i_gen_proc + 1) << std::endl;

			//	int	cur_id_step				1 = random.  2 = sequential.
			//										If it meets the end of the sequence, it quits
			if (se.get_prop_int("cur_id_step") == 1) {
				// select a random cur_id
				i_cur_id = cbh.load_cur_bin(1, 0);
			}
			else {
				if (i_cur_id == -1) {
					i_cur_id = cbh.load_cur_bin(1, 0);
				}
				else {
					i_cur_id = cbh.load_cur_bin(2, i_cur_id);
				}
			}

			std::cout << "\t\tcur id\t: " << i_cur_id << std::endl;

			std::cout << "\tfarm gen end:  " << (i_gen_proc + 1) << std::endl;
		}
	}


	std::cout << "farm run end" << std::endl;
}


void farm::populate_mlecks() {
	// Load existing mlecks that are supposed to be available.  This will be done by looking for 
	// idmleck_*.settings files in the farm folder.
	vector<string> vec_m_str;

	// Read the name of all of the bin files in the bin folder, and index them in a map.
	std::cout << "read farm alive mlecks folder\n";
	if (boost::filesystem::is_directory(str_filepath_farm_alive) == true) {
		for (auto& p : std::experimental::filesystem::recursive_directory_iterator(str_filepath_farm_alive)) {
			boost::filesystem::path fsp(p.path().string());
			string str_mleck_test = fsp.stem().string();
			mleck m;

			m.init(str_mleck_test
				, str_path_farm
				, tuple<int, int>(se.get_prop_int("mleck_start_jewel_min"), se.get_prop_int("mleck_start_jewel_max"))
				, & jh
			);

			if (m.get_id() > 0) {
				// If it is zero it means the operation of reading the mleck file failed.
				vec_m.push_back(m);
			}
		}
	} else {
		// This shouldn't happen.  These folders are created during the program and farm init functions.
	}



	if (vec_m_str.size() > 0) {
		for (size_t i_vec = 0; i_vec < vec_m_str.size(); ++i_vec) {
			string str_m = vec_m_str.at(i_vec);
			boost::algorithm::trim(str_m);
			if (str_m.compare("") != 0) {
				int i_m = std::stoi(str_m);
				mleck m(i_m
					, str_path_farm
					, se.get_prop_int("gen")
					, tuple<int, int>(se.get_prop_int("mleck_start_jewel_min"), se.get_prop_int("mleck_start_jewel_max"))
					, & jh
				);
				vec_m.push_back(m);
			}
		}
	}


	//// Populate the mleck list with mlecks so that it meets the mleck count.
	//if (vec_m.size() < se.get_prop_int("mleck_count")) {
	//	size_t i_new = boost::lexical_cast<size_t>(se.get_prop_int("mleck_count") - vec_m.size());
	//	int i_mleck_id_highest = se.get_prop_int("mleck_id_highest");
	//	int i_gen = se.get_prop_int("gen");
	//	for (size_t i1 = 0; i1 < i_new; ++i1) {
	//		++i_mleck_id_highest;
	//		mleck m(i_mleck_id_highest
	//			, str_path_farm
	//			, i_gen
	//			, tuple<int, int>(se.get_prop_int("mleck_start_jewel_min"), se.get_prop_int("mleck_start_jewel_max"))
	//			, & jh
	//		);
	//		vec_m.push_back(m);
	//		if (str_m_list.compare("") != 0) {
	//			str_m_list = str_m_list + "~" + std::to_string(i_mleck_id_highest);
	//		}
	//		else {
	//			str_m_list = std::to_string(i_mleck_id_highest);

	//		}
	//	}
	//	// Write out the data.
	//	se.set_prop_int("mleck_id_highest", i_mleck_id_highest);
	//	se.set_prop_str("mlecks", str_m_list);
	//	se.export_text(str_filepath_settings);
	//}
}


bool exists_test(const std::string& name) {
	ifstream f(name.c_str());
	return f.good();
}


