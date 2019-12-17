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

			// The files are read into vectors of their lines
			vector<string> vec_data_situation;
			// Only lines that meet the vec_instruments critiria are included.
			vector<string> vec_data_instruments;
			map<string, map<string, double>> map_map_ticker;

			map<string, vector<tuple<double, double, double, double, double, double, double, double>>> map_vec_candle300;
			map<string, vector<tuple<double, double, double, double, double, double, double, double>>> map_vec_candle14400;
			map<string, vector<tuple<double, double, short, double, double, double>>> map_vec_history;
			//map<string, vector<tuple<double, double, double, double, double, double, double, double>>> map_vec_orderbook;

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
					std::string str_line2;
					std::ifstream ifs(str_filepath_instruments);
					while (std::getline(ifs, str_line2)) {
						if (str_line2.size() > 0) {
							if (i_instrument_count == 0) {
								vec_data_instruments.push_back(str_line2);
								vec_instrument.push_back(str_line2);
							} else {
								// For some reason, the tradional find method didn't work on this one.  Meh.  Algorithm works.
								for (size_t i_gen_nth2 = 0; i_gen_nth2 < vec_instrument_user.size(); ++i_gen_nth2) {
									if (str_line2.compare(vec_instrument_user.at(i_gen_nth2)) == 0) {
										std::cout << " found instrument : " << str_line2 << "\n";
										vec_data_instruments.push_back(str_line2);
										vec_instrument.push_back(str_line2);
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
								bool b_found_map = false;
								if (map_map_ticker.size() > 0) {
									if (map_map_ticker.count(vec_ticker.at(0)) > 0) { b_found_map = true; }
								}

								map<string, double> map_ticker;
								if (b_found_map == true) {
									map_ticker = map_map_ticker[vec_ticker.at(0)];
								}

								double d_val1 = 0.0;
								map_ticker[vec_ticker.at(1)] = std::stod(vec_ticker.at(2), nullptr);
								map_map_ticker[vec_ticker.at(0)] = map_ticker;
							}
						}
					}
				}

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
							vector<tuple<double, double, double, double, double, double, double, double>> vec_candle;
							vec_candle.reserve(500);
							process_file_candle(str_filepath_candle, map_map_ticker[vec_data_instruments.at(i)]["last"], &vec_candle);
							map_vec_candle300.emplace(vec_data_instruments.at(i), std::move(vec_candle));
						}

						// Read the candle data 14400 into an a map vs instruments
						str_filepath_candle = str_path_scripture + "\\" + str_gen + "\\"
							+ str_filename_candle_prefix
							+ vec_data_instruments.at(i)
							+ str_filename_candle_14400_suffix;
						if (boost::filesystem::is_regular_file(str_filepath_candle) == true) {
							vector<tuple<double, double, double, double, double, double, double, double>> vec_candle;
							vec_candle.reserve(500);
							process_file_candle(str_filepath_candle, map_map_ticker[vec_data_instruments.at(i)]["last"], &vec_candle);
							map_vec_candle14400.emplace(vec_data_instruments.at(i), std::move(vec_candle));
						}

						//	vector<string> vec_files_hist;
						str_filepath_history = str_path_scripture + "\\" + str_gen + "\\"
							+ str_filename_hist_prefix
							+ vec_data_instruments.at(i)
							+ str_filename_hist_suffix;
						if (boost::filesystem::is_regular_file(str_filepath_history) == true) {
							vector<tuple<double, double, short, double, double, double>> vec_tup;
							process_file_history(str_filepath_history, map_map_ticker[vec_data_instruments.at(i)]["last"], &vec_tup);
							map_vec_history.emplace(vec_data_instruments.at(i), std::move(vec_tup));
						}

						//	
						//	vector<string> vec_files_hist;




					}
				}

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

void polo_gen::process_file_candle(string str_filepath, double d_val_last, vector<tuple<double, double, double, double, double, double, double, double>> * ptr_vec_output) {
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

	if (vec_tup_file.size() == 0) {
		ptr_vec_output->push_back(tuple<double, double, double, double, double, double, double, double>(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
		ptr_vec_output->push_back(tuple<double, double, double, double, double, double, double, double>(100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
	} else {
		if (vec_tup_file.size() == 1) {
			ptr_vec_output->push_back(tuple<double, double, double, double, double, double, double, double>(100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
		} else {
			if (d_val_last == 0.0) {
				ptr_vec_output->push_back(tuple<double, double, double, double, double, double, double, double>(100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
			} else {
				double d_diff_total = static_cast<double>(i_end - i_start);
				for (int i = static_cast<int>(vec_tup_file.size()) - 1; i >= 0; --i) {
					double d_diff = static_cast<double>(i_end - get<0>(vec_tup_file.at(static_cast<size_t>(i))));
					double d_pc = (d_diff / d_diff_total) * 100.0;

					tuple<double, double, double, double, double, double, double, double> tup = std::make_tuple(
						d_pc
						, -1.0 * (d_val_last - get<1>(vec_tup_file.at(i)))
						, -1.0 * (d_val_last - get<2>(vec_tup_file.at(i)))
						, -1.0 * (d_val_last - get<3>(vec_tup_file.at(i)))
						, -1.0 * (d_val_last - get<4>(vec_tup_file.at(i)))
						, -1.0 * (d_val_last - get<5>(vec_tup_file.at(i)))
						, get<6>(vec_tup_file.at(i))
						, get<7>(vec_tup_file.at(i))
					);

					ptr_vec_output->push_back(tup);
				}
			}
		}
	}
}


void polo_gen::process_file_history(string str_filepath, double d_val_last, vector<tuple<double, double, short, double, double, double>> * vec_tup_output) {
	vector<tuple<int, double, short, double>> vec_tup_file;
	vec_tup_file.reserve(500);

	int i_epoch_start = -1, i_epoch_end = -1;
	double d_vol_total = 0.0;

	std::string str_line;
	std::ifstream ifs(str_filepath);
	while (std::getline(ifs, str_line)) {
		if (str_line.size() > 0) {
			vector<string> vec_line;
			boost::split(vec_line, str_line, boost::is_any_of("|"));
			if (vec_line.size() >= 7) {

				//	0(n reqd)	1(not reqd)			2						3			 4			5					6(not reqd)
				//411121485 | 26532891 | 2019-03-18 12:52:20 | 3980.92148989 | sel | 2184.04682637 | 0.54862846
				//411121484 | 26532890 | 2019-03-18 12:52:20 | 3981.27192630 | sel | 8.78997159 | 0.00220783
				//411121478 | 26532889 | 2019-03-18 12:52:06 | 3983.29999997 | buy | 187.88003226 | 0.04716693
				//
				//				turn to
				//
				//	 dbl%			dbl%						t/f					dbl,		dbl%					dbl%
				//	timepc, price vs start pc, buy/sell true/false,		vol,	vol vs max,	vol running total vs total,

				int i_epoch = seconds_from_epoch(vec_line.at(2));

				short buysell = -1;
				if (vec_line.at(4).compare("buy") == 0) {
					buysell = 1;
				} else {
					buysell = -1;
				}


				tuple<int, double, short, double> tup = std::make_tuple(i_epoch
					, std::stod(vec_line.at(3), nullptr)
					, buysell
					, std::stod(vec_line.at(5), nullptr)
				);

				if (i_epoch_start < 0) {
					i_epoch_start = i_epoch;
				}

				i_epoch_end = i_epoch;
				d_vol_total += std::stod(vec_line.at(5), nullptr);
				vec_tup_file.push_back(tup);

				std::cout << "\tline : " << str_line << "\n";
			}
		}
	}

	/*
	if (vec_candle_backward.size() == 0) {
		ptr_vec_candle->push_back(tuple<double, double, double, double, double, double, double, double>(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
		ptr_vec_candle->push_back(tuple<double, double, double, double, double, double, double, double>(100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
	} else {
		if (vec_candle_backward.size() == 1) {
			ptr_vec_candle->push_back(tuple<double, double, double, double, double, double, double, double>(100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
		} else {
			double d_diff_total = static_cast<double>(i_end - i_start);

			for (int i = static_cast<int>(vec_candle_backward.size()) - 1; i >= 0; --i) {
				double d_diff = static_cast<double>(i_end - get<0>(vec_candle_backward.at(static_cast<size_t>(i))));
				double d_pc = (d_diff / d_diff_total) * 100.0;

				tuple<double, double, double, double, double, double, double, double> tup = std::make_tuple(
					d_pc
					, get<1>(vec_candle_backward.at(i))
					, get<2>(vec_candle_backward.at(i))
					, get<3>(vec_candle_backward.at(i))
					, get<4>(vec_candle_backward.at(i))
					, get<5>(vec_candle_backward.at(i))
					, get<6>(vec_candle_backward.at(i))
					, get<7>(vec_candle_backward.at(i))
				);

				ptr_vec_candle->push_back(tup);
			}
		}
	}
	*/







}



string polo_gen::to_string() {
	string str_return = "";

	return str_return;
}
