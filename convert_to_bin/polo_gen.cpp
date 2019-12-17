// refer polo_gen.h for details.
//
//	
//

#include "polo_gen.h"


int polo_gen::process(vector<string> vec_gen
	, vector<string> vec_instrument_user
	, string str_path_scripture
) {
	vector<string> vec_errors;

	int i_instrument_count = static_cast<int>(vec_instrument_user.size());

	// List all of the directories in the scripture directory
	std::cout << "read scripture folders\n";
	vector<string> vec_dir;
	if (boost::filesystem::is_directory(str_path_scripture) == true) {
		for (auto& di : boost::make_iterator_range(boost::filesystem::directory_iterator(str_path_scripture), {})) {
			vec_dir.push_back(di.path().string());
		}
	}

	if (vec_gen.size() == 0) {
		for (size_t i = 0; i < vec_dir.size(); ++i) {
			// Loop through each folder in the scripture directory.
			boost::filesystem::path p(vec_dir.at(static_cast<int>(i)));
			boost::filesystem::path dir = p.filename();
			string str_dir = dir.string();
			vec_gen.push_back(str_dir);
		}
	}

	if (vec_gen.size() == 0) {
		vec_errors.push_back("No gens found for processing!");
	} else {
		for (size_t i_gen_nth = 0; i_gen_nth < vec_gen.size(); ++i_gen_nth) {
			string str_gen = vec_gen.at(static_cast<int>(i_gen_nth));
			string str_gen_path = str_path_scripture + "\\" + str_gen;

			std::cout << "process scripture gen path = " + str_gen_path + "\n";

			cur_bin cb = cur_bin();



			// The files are read into vectors of their lines
			vector<string> vec_data_situation;
			// Only lines that meet the vec_instruments critiria are included.
			vector<string> vec_data_instruments;
			map<string, map<string, double>> map_map_ticker;

			boost::split(vec_ticker_record, str_ticker_record, boost::is_any_of("|"));

			if (boost::filesystem::is_directory(str_gen_path) == false) {
				vec_errors.push_back("folder [" + str_gen_path + "] not found!");
			} else {

				// Read the situation file.
				string str_filepath_situation = str_path_scripture + "\\" + str_gen + "\\" + str_filename_situation;
				if (boost::filesystem::is_regular_file(str_filepath_situation) == false) {
					vec_errors.push_back("file [" + str_filepath_situation + "] not found!");
				} else {
					std::string str_line;
					std::ifstream ifs(str_filepath_situation);
					while (std::getline(ifs, str_line)) {
						if (str_line.size() > 0) {
							// Add it to the cur_bin object
							cb.add_string_situation(str_line);
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
					std::string str_line2;
					std::ifstream ifs(str_filepath_instruments);
					while (std::getline(ifs, str_line2)) {
						if (str_line2.size() > 0) {
							if (i_instrument_count == 0) {
								vec_data_instruments.push_back(str_line2);
								cb.add_string_instrument(str_line2);
							} else {
								// For some reason, the tradional find method didn't work on this one.  Meh.  Algorithm works.
								for (size_t i_gen_nth2 = 0; i_gen_nth2 < vec_instrument_user.size(); ++i_gen_nth2) {
									if (str_line2.compare(vec_instrument_user.at(i_gen_nth2)) == 0) {
										std::cout << " found instrument : " << str_line2 << "\n";
										vec_data_instruments.push_back(str_line2);
										// Add it to the cur_bin object
										cb.add_string_instrument(str_line2);
										break;
									}
								}
							}
						}
					}
				}

				// Read the ticker file, and populate the ticker file with
				//	the data of valid instruments.  This will need to be a dictionary of dictionaries.
				// read the vec_data_instruments file, and remove elements
				//	if they don't exist from a user list, or add elements if no
				//	instruments have been provided.
				string str_filepath_ticker = str_path_scripture + "\\" + str_gen + "\\" + str_filename_ticker;

				if (boost::filesystem::is_regular_file(str_filepath_instruments) == false) {
					vec_errors.push_back("file [" + str_filepath_ticker + "] not found!");
				} else {
					std::string str_line3;
					std::ifstream ifs(str_filepath_ticker);
					while (std::getline(ifs, str_line3)) {
						if (str_line3.size() > 0) {
							vector<string> vec_ticker;
							boost::split(vec_ticker, str_line3, boost::is_any_of("|"));

							bool b_add_value = false;
							if (i_instrument_count == 0) {
								b_add_value = true;
							} else {
								for (size_t i_gen_nth2 = 0; i_gen_nth2 < vec_instrument_user.size(); ++i_gen_nth2) {
									if (vec_ticker.at(0).compare(vec_instrument_user.at(i_gen_nth2)) == 0) {
										b_add_value = true;
										break;
									}
								}
							}

							bool b_found2 = false;
							for (size_t i = 0; i < vec_ticker_record.size(); ++i) {
								if (vec_ticker.at(1).compare(vec_ticker_record.at(i)) == 0) {
									b_found2 = true;
									break;
								}
							}

							if (b_found2 == false) { b_add_value = false; }

							if (b_add_value == true) {
								// Add it to the cur_bin object
								cb.add_double_ticker(vec_ticker.at(0), vec_ticker.at(1), std::stod(vec_ticker.at(2), nullptr));
							}
						}
					}
				}

				//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
				// Read all of the data files into memory
				string str_filepath_candle = "";
				string str_filepath_history = "";
				string str_filepath_orderbook = "";

				if (vec_data_instruments.size() > 0) {
					for (size_t i = 0; i < vec_data_instruments.size(); ++i) {
						// Read the candle data 300 into an a map vs instruments
						str_filepath_candle = str_path_scripture + "\\" + str_gen + "\\"
							+ str_filename_candle_prefix
							+ vec_data_instruments.at(i)
							+ str_filename_candle_300_suffix;
						if (boost::filesystem::is_regular_file(str_filepath_candle) == true) {
							vector<tuple<int, double, double, double, double, double, double, double>> vec_candle;
							vec_candle.reserve(500);
							process_file_candle(str_filepath_candle, map_map_ticker[vec_data_instruments.at(i)]["last"], &vec_candle);
							cb.set_tup_candle300(vec_data_instruments.at(i), vec_candle);
						}


						// Read the candle data 14400 into an a map vs instruments
						str_filepath_candle = str_path_scripture + "\\" + str_gen + "\\"
							+ str_filename_candle_prefix
							+ vec_data_instruments.at(i)
							+ str_filename_candle_14400_suffix;
						if (boost::filesystem::is_regular_file(str_filepath_candle) == true) {
							vector<tuple<int, double, double, double, double, double, double, double>> vec_candle;
							vec_candle.reserve(500);
							process_file_candle(str_filepath_candle, map_map_ticker[vec_data_instruments.at(i)]["last"], &vec_candle);
							cb.set_tup_candle14400(vec_data_instruments.at(i), vec_candle);
						}


						//	read the trade history file into an a map vs instruments
						str_filepath_history = str_path_scripture + "\\" + str_gen + "\\"
							+ str_filename_hist_prefix
							+ vec_data_instruments.at(i)
							+ str_filename_hist_suffix;
						if (boost::filesystem::is_regular_file(str_filepath_history) == true) {
							vector<tuple<int, double, bool, double>> vec_tup;
							process_file_history(str_filepath_history, map_map_ticker[vec_data_instruments.at(i)]["last"], &vec_tup);
							cb.set_tup_history(vec_data_instruments.at(i), vec_tup);
						}


						//	read the order book file into an a map vs instruments
						str_filepath_orderbook = str_path_scripture + "\\" + str_gen + "\\"
							+ str_filename_orderbook_prefix
							+ vec_data_instruments.at(i)
							+ str_filename_orderbook_suffix;
						if (boost::filesystem::is_regular_file(str_filepath_history) == true) {
							vector<tuple<double, double, double, double>> vec_tup;
							process_file_orderbook(str_filepath_orderbook, map_map_ticker[vec_data_instruments.at(i)]["last"], &vec_tup);
							cb.set_tup_orderbook(vec_data_instruments.at(i), vec_tup);
						}
					}
				}
				// Read all of the data files into memory
				//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

				//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
				//	Write out the data to a BIN file.
				//


				//
				//	Write out the data to a BIN file.
				//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


				std::cout << "\tGen Complete : " << str_gen << "\n";
			}
		}
	}



	if (vec_errors.size() > 0) {
		std::cout << "Errors found in " << str_class_name << "[" << __func__ << "]" << " :\n";
		for (size_t i = 0; i < vec_errors.size(); ++i) {
			std::cout << "\t" << vec_errors.at(i);
		}
	}
	// return 0 if no errors.
	return static_cast<int>(vec_errors.size());
}


// Read the text data file into memory
// Record vector with elements newest (lower) to latest (higher)     (the file must be reversed)
//			0			1				2					3			 4			5				6			7
//	1548777600|0.00000017|0.00000016|0.00000016|0.00000016|0.00000016|5.82696703|34300966.899461
//	1548792000|0.00000017|0.00000016|0.00000017|0.00000016|0.00000016|2.57156965|15154582.862649
//	1548806400|0.00000017|0.00000016|0.00000017|0.00000016|0.00000016|0.0729452|453722.45582316
void polo_gen::process_file_candle(string str_filepath, double d_val_last, vector<tuple<int, double, double, double, double, double, double, double>> * ptr_vec_output) {
	vector<tuple<int, double, double, double, double, double, double, double>> vec_tup_file;
	vec_tup_file.reserve(500);

	int i_val = -1, i_start = -1, i_end = -1;

	std::string str_line;
	std::ifstream ifs(str_filepath);
	while (std::getline(ifs, str_line)) {
		if (str_line.size() > 0) {
			vector<string> vec_line;
			boost::split(vec_line, str_line, boost::is_any_of("|"));
			if (vec_line.size() >= 8) {

				tuple<int, double, double, double, double, double, double, double> tup = std::make_tuple(std::stoi(vec_line.at(0), nullptr, 10)
					, std::stod(vec_line.at(1), nullptr)
					, std::stod(vec_line.at(2), nullptr)
					, std::stod(vec_line.at(3), nullptr)
					, std::stod(vec_line.at(4), nullptr)
					, std::stod(vec_line.at(5), nullptr)
					, std::stod(vec_line.at(6), nullptr)
					, std::stod(vec_line.at(7), nullptr)
				);
				if (get<0>(tup) > 0) {
					i_val = get<0>(tup);
					if (i_start == -1) {
						i_start = i_val;
					}
					i_end = i_val;
					vec_tup_file.push_back(tup);
				}
			}
		}
	}

	// Reverse the list.
	if (vec_tup_file.size() > 0) {
		for (int i = static_cast<int>(vec_tup_file.size()) - 1; i >= 0; --i) {
			ptr_vec_output->push_back(vec_tup_file.at(static_cast<size_t>(i)));
		}
	}
}


// Record vector with elements newest (lower) to latest (higher)
//		1					2				3				4
//137.33920319|16.63857248|137.40000000|23.79240309
//137.33920318|0.00728780|137.45367868|1.13800000
//137.27802432|0.00729104|137.47839826|72.64250000
//137.23912591|0.07842815|137.57818032|31.71775437
void polo_gen::process_file_orderbook(string str_filepath, double d_val_last, vector<tuple<double, double, double, double>> * ptr_vec_output) {
	std::string str_line;
	std::ifstream ifs(str_filepath);
	while (std::getline(ifs, str_line)) {
		if (str_line.size() > 0) {
			vector<string> vec_line;
			boost::split(vec_line, str_line, boost::is_any_of("|"));
			if (vec_line.size() >= 4) {
				tuple<double, double, double, double> tup = std::make_tuple(
					std::stod(vec_line.at(0), nullptr)
					, std::stod(vec_line.at(1), nullptr)
					, std::stod(vec_line.at(2), nullptr)
					, std::stod(vec_line.at(3), nullptr)
				);

				ptr_vec_output->push_back(tup);
				//std::cout << "\tline : " << str_line << "\n";
			}
		}
	}
}


// Read the text data file into memory
// Record vector with elements newest (lower) to latest (higher)   
//	0(skip)	1(skip)			2						3			 4			5				6(skip)
//411121485|26532891|2019-03-1812:52:20|3980.92148989|sel|2184.04682637|0.54862846
//411121484|26532890|2019-03-1812:52:20|3981.27192630|sel|8.78997159|0.00220783
//411121478|26532889|2019-03-1812:52:06|3983.29999997|buy|187.88003226|0.04716693
void polo_gen::process_file_history(string str_filepath, double d_val_last, vector<tuple<int, double, bool, double>> * ptr_vec_output) {
	int i_epoch_start = -1, i_epoch_end = -1;
	double d_vol_total = 0.0;

	std::string str_line;
	std::ifstream ifs(str_filepath);
	while (std::getline(ifs, str_line)) {
		if (str_line.size() > 0) {
			vector<string> vec_line;
			boost::split(vec_line, str_line, boost::is_any_of("|"));
			if (vec_line.size() >= 7) {

				int i_epoch = seconds_from_epoch(vec_line.at(2));

				bool b_buysell = false;
				if (vec_line.at(4).compare("buy") == 0) {
					b_buysell = true;
				}

				tuple<int, double, bool, double> tup = std::make_tuple(i_epoch
					, std::stod(vec_line.at(3), nullptr)
					, b_buysell
					, std::stod(vec_line.at(5), nullptr)
				);

				if (i_epoch_start < 0) {
					i_epoch_start = i_epoch;
				}

				i_epoch_end = i_epoch;
				d_vol_total += std::stod(vec_line.at(5), nullptr);
				ptr_vec_output->push_back(tup);
				//std::cout << "\tline : " << str_line << "\n";
			}
		}
	}
}


string polo_gen::to_string() {
	string str_return = "";

	return str_return;
}
