/* refer cur_bin_handler.h for details.


*/

#include "cur_bin_handler.h"

cur_bin_handler::cur_bin_handler() {

}


cur_bin_handler::cur_bin_handler(string str_path_bin_in) {
	init(str_path_bin_in);
}


// Set this to 1 in order to randomize.  2 to do sequentially.  If random,
//	ignore the second parameter.  If sequential start sequence at that id.
int cur_bin_handler::load_cur_bin(int i_cur_step_type, int id_cur_in) {
	id_cur_open = id_cur_in;

	if (i_cur_step_type == 1) {
		id_cur_open = get_random_id();
	} else if (i_cur_step_type == 2) {
		if (!is_valid_id(id_cur_open)) {
			id_cur_open = get_next_id(id_cur_open);
		}
	}

	cb_open.import_bin(map_bin[id_cur_open]);
	cb2_open.init(str_path_bin, id_cur_open, str_path_bin2 + "\\" + std::to_string(id_cur_open), cb_open);

	return id_cur_open;
}


bool cur_bin_handler::is_valid_id(int id_in) {
	if ((map_bin.find((id_in - 1)) != map_bin.end()) && (map_bin.find((id_in + 1)) != map_bin.end())) {
		// found one element before and after this one.  Mleck requires three sequential data
		// points in order to create bin2 files.
		return true;
	} else {
		return false;
	}
}


int cur_bin_handler::get_next_id(int id_in) {
	int id = id_in;
	bool b_end = false;
	while (b_end == false) {
		++id;
		if (map_bin.find(id) != map_bin.end()) {
			// found
			b_end = is_valid_id(id);
		}
		if (id > id_cur_max) {
			// start at the beginning element.  There will never a bin2 object for the 
			//	first element, because mleck requires two sequential data points in order
			//	to create bin2 files.
			id = id_cur_min;
		}
	}
	return id;
}


int cur_bin_handler::get_random_id() {
	int id = 0;
	bool b_end = false;
	while (b_end == false) {
		auto it = map_bin.begin();
		std::advance(it, rand() % map_bin.size());
		id = it->first;
		b_end = is_valid_id(id);
	}
	return id;
}


void cur_bin_handler::init(string str_path_bin_in) {
	b_valid = false;
	str_path_bin = str_path_bin_in;
	str_path_bin2 = str_path_bin_in + "2";

	map_bin.clear();
	map_bin2.clear();
	id_cur_min = -1;
	id_cur_max = -1;
	id_cur_open = -1;

	std::cout << "cur_bin_handler.init \t: \t" << str_path_bin << "\n";

	// Read the name of all of the bin files in the bin folder, and index them in a map.
	std::cout << "read bin folder\n";
	if (boost::filesystem::is_directory(str_path_bin) == true) {
		for (auto& p : std::experimental::filesystem::recursive_directory_iterator(str_path_bin)) {
			boost::filesystem::path fsp(p.path().string());
			string str_bin = fsp.stem().string();
			char* c;
			long converted = strtol(str_bin.c_str(), &c, 10);
			if (*c) {
				// conversion failed because the input wasn't a number
			} else {
				// this validates that the file queried is a number ###, without the extension.
				// I don't make a .bin test.  Not necessary.
				int id_cur = atoi(fsp.stem().string().c_str());

				if (id_cur_min == -1) {
					id_cur_min = id_cur;
				} else {
					if (id_cur < id_cur_min) {
						id_cur_min = id_cur;
					}
				}

				if (id_cur_max == -1) {
					id_cur_max = id_cur;
				} else {
					if (id_cur > id_cur_max) {
						id_cur_max = id_cur;
					}
				}

				map_bin.emplace(id_cur, std::move(p.path().string()));
			}
		}
	}

	if (boost::filesystem::is_directory(str_path_bin2) == false) {
		// These bin files are created within this class when a cur_id is selected.
		std::cout << "create bin2 folder\n";
		std::experimental::filesystem::create_directories(str_path_bin2);
	}

	if (map_bin.size() > 0) {
		b_valid = true;
	}

	std::cout << "cur_bin_handler.init \tEND\n";
}

