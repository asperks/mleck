// refer polo_gen.h for details.

#include "polo_gen.h"


int polo_gen::init(vector<string> vec_gen
	, vector<string> vec_instrument_user
	, string str_path_scripture
) {
	vector<string> vec_errors;

	int i_instrument_count = static_cast<int>(vec_instrument_user.size());
	vector<string> vec_instrument;

	// List all of the directories in the scripture directory
	std::cout << "read scripture folders\n";
	vector<string> vec_dir;
	if (boost::filesystem::is_directory(str_path_scripture) == true) {
		for (auto& di : boost::make_iterator_range(boost::filesystem::directory_iterator(str_path_scripture), {})) {
			vec_dir.push_back(di.path().string());
		}
	}

	if (vec_gen.size() == 0) {
		for (int i = 0; i < vec_dir.size(); ++i) {
			// Loop through each folder in the scripture directory.
			boost::filesystem::path p(vec_dir.at(i));
			boost::filesystem::path dir = p.filename();
			string str_dir = dir.string();
			vec_gen.push_back(str_dir);
		}
	}

	if (vec_gen.size() == 0) {
		vec_errors.push_back("No gens found for processing!");
	} else {
		for (int i_gen_nth = 0; i_gen_nth < vec_gen.size(); ++i_gen_nth) {
			string str_gen = vec_gen.at(i_gen_nth);
			string str_gen_path = str_path_scripture + "\\" + str_gen;

			std::cout << "process scripture gen path = " + str_gen_path + "\n";

			// The files are read into vectors of their lines
			vector<string> vec_data_situation;
			// Only lines that meet the vec_instruments critiria are included.
			vector<string> vec_data_ticker;
			vector<string> vec_data_instruments;

			// The valid files for processing are loaded into vectors
			vector<string> vec_files_candle_300;
			vector<string> vec_files_candle_14400;
			vector<string> vec_files_hist;
			vector<string> vec_files_orderbook;

			if (boost::filesystem::is_directory(str_gen_path) == false) {
				vec_errors.push_back("folder [" + str_gen_path + "] not found!");
			} else {

				// Read the situation file.
				string str_filepath_situation = str_path_scripture + "\\" + str_gen + "\\" + str_filename_situation;
				if (boost::filesystem::is_regular_file(str_filepath_situation) == false) {
					vec_errors.push_back("file [" + str_filepath_situation + "] not found!");
				} else {
					std::string str_line; std::ifstream ifs(str_filepath_situation);
					while (std::getline(ifs, str_line)) {
						if (str_line.size() > 0) {
							vec_data_situation.push_back(str_line);
						}
					}
				}

				// read the vec_data_instruments file, and remove elements
				//	if they don't exist from a user list, or add elements if no
				//	instruments have been provided.
				string str_filepath_instruments = str_path_scripture + "\\" + str_gen + "\\" + str_filename_instruments;
				if (boost::filesystem::is_regular_file(str_filepath_instruments) == false) {
					vec_errors.push_back("file [" + str_filepath_instruments + "] not found!");
				} else {
					std::string str_line; std::ifstream ifs(str_filepath_instruments);
					while (std::getline(ifs, str_line)) {
						if (str_line.size() > 0) {
							if (i_instrument_count == 0) {
								vec_data_instruments.push_back(str_line);
								vec_instrument.push_back(str_line);
							} else {
								if (std::find(vec_instrument_user.begin(), vec_instrument_user.end(), str_line) != vec_instrument_user.end()) {
									vec_data_instruments.push_back(str_line);
									vec_instrument.push_back(str_line);
								}
							}
						}
					}
				}

				// Read the ticker file, and populate the ticker file with
				//	the data of valid instruments.



				// Loop through the vec_instruments file to determine which
				//	files should be included in the vectors of files.  If files
				//	are missing, the instruments are removed from the 
				//	vec_instruments vector, and an error is returned.
			}
		}
	}




	if (vec_errors.size() > 0) {
		std::cout << "Errors found in " << str_class_name << "[" << __func__ << "]" << " :\n";
		for (int i = 0; i < vec_errors.size(); ++i) {
			std::cout << "\t" << vec_errors.at(i);
		}
	}
	// return 0 if no errors.
	return static_cast<int>(vec_errors.size());
}


string polo_gen::to_string() {
	string str_return = "";

	return str_return;
}
