// refer polo_cur.h for details.
//
//	
//
#include "polo_cur.h"

int polo_cur::process(vector<string> vec_gen
							, vector<string> vec_instrument_user
							, string str_path_scripture
							, string str_path_bin
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
		vec_errors.push_back("No cur id's found for processing!");
	} else {
		for (size_t i_gen_nth = 0; i_gen_nth < vec_gen.size(); ++i_gen_nth) {
			string str_cur_id = vec_gen.at(static_cast<int>(i_gen_nth));
			string str_cur_id_path = str_path_scripture + "\\" + str_cur_id;
			string str_bin_filepath = str_path_bin + "\\" + str_cur_id + ".bin";

			std::cout << "process scripture cur path = " + str_cur_id_path + "\n";

			cur_bin cb = cur_bin();

			cb.set_status(1);

			// The files are read into vectors of their lines
			vector<string> vec_data_situation;
			// Only lines that meet the vec_instruments critiria are included.
			vector<string> vec_data_instruments;
			map<string, map<string, double>> map_map_ticker;

			boost::split(vec_ticker_record, str_ticker_record, boost::is_any_of("|"));

			if (boost::filesystem::is_directory(str_cur_id_path) == false) {
				vec_errors.push_back("folder [" + str_cur_id_path + "] not found!");
				cb.set_status(2);
			} else {

				// Read the situation file.
				string str_filepath_situation = str_path_scripture + "\\" + str_cur_id + "\\" + str_filename_situation;
				if (boost::filesystem::is_regular_file(str_filepath_situation) == false) {
					vec_errors.push_back("file [" + str_filepath_situation + "] not found!");
					cb.set_status(2);
				} else {
					std::string str_line;
					std::ifstream ifs(str_filepath_situation);
					while (std::getline(ifs, str_line)) {
						if (str_line.size() > 0) {
							// Changing of terminology.  Yeah, I know, bad design.  But I need to make it more consistent
							// cur_id's are not gens.  Gens relate to the generations of the mlecks.
							boost::replace_all(str_line, "Gen|", "cur_id|");
							// Add it to the cur_bin object
							cb.add_string_situation(str_line);
						}
					}
				}

				// read the vec_data_instruments file, and remove elements
				//	if they don't exist from a user list, or add elements if no
				//	instruments have been provided.
				string str_filepath_instruments = str_path_scripture + "\\" + str_cur_id + "\\" + str_filename_instruments;
				if (boost::filesystem::is_regular_file(str_filepath_instruments) == false) {
					vec_errors.push_back("file [" + str_filepath_instruments + "] not found!");
					cb.set_status(2);
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
					if (i_instrument_count > 0) {
						if (vec_data_instruments.size() != vec_instrument_user.size()) {
							std::cout << "\tTICKER NOT FOUND\n";
							cb.set_status(3);
						}
					}
				}

				if (cb.get_status() == 1) {
					// Read the ticker file, and populate the ticker file with
					//	the data of valid instruments.  This will need to be a dictionary of dictionaries.
					// read the vec_data_instruments file, and remove elements
					//	if they don't exist from a user list, or add elements if no
					//	instruments have been provided.
					string str_filepath_ticker = str_path_scripture + "\\" + str_cur_id + "\\" + str_filename_ticker;

					if (boost::filesystem::is_regular_file(str_filepath_instruments) == false) {
						vec_errors.push_back("file [" + str_filepath_ticker + "] not found!");
						cb.set_status(2);
					} else {
						std::string str_line3;
						std::ifstream ifs(str_filepath_ticker);
						while (std::getline(ifs, str_line3)) {
							if (str_line3.size() > 0) {
								vector<string> vec_ticker;
								boost::split(vec_ticker, str_line3, boost::is_any_of("|"));

								bool b_add_value = false;
								if (i_instrument_count == 0) {
									if (cb.is_valid_instrument(vec_ticker.at(0))) {
										b_add_value = true;
									} else {
										b_add_value = false;
									}
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
									cb.set_ticker_double(vec_ticker.at(0), vec_ticker.at(1), std::stod(vec_ticker.at(2), nullptr));
								}
							}
						}
					}

					if (cb.get_map_ticker_count() != vec_data_instruments.size()) {
						std::cout << "\tTICKER DATA NOT FOUND\n";
						cb.set_status(4);
					}
				}

				if (cb.get_status() == 1) {
					//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
					// Read all of the data files into memory
					string str_filepath_candle = "";
					string str_filepath_history = "";
					string str_filepath_orderbook = "";

					if (vec_data_instruments.size() > 0) {
						for (size_t i = 0; i < vec_data_instruments.size(); ++i) {
							Ticker t = cb.get_ticker(vec_data_instruments.at(i));

							// Read the candle data 300 into an a map vs instruments
							str_filepath_candle = str_path_scripture + "\\" + str_cur_id + "\\"
								+ str_filename_candle_prefix
								+ vec_data_instruments.at(i)
								+ str_filename_candle_300_suffix;

							if (boost::filesystem::is_regular_file(str_filepath_candle) == true) {
								vector<Candle_line> vec_struct;
								vec_struct.reserve(500);
								process_file_candle(str_filepath_candle, t.last, &vec_struct);
								cb.set_struct_candle300(vec_data_instruments.at(i), vec_struct);
							} else {
								cb.set_status(5);
							}

							// Read the candle data 14400 into an a map vs instruments
							str_filepath_candle = str_path_scripture + "\\" + str_cur_id + "\\"
								+ str_filename_candle_prefix
								+ vec_data_instruments.at(i)
								+ str_filename_candle_14400_suffix;

							if (boost::filesystem::is_regular_file(str_filepath_candle) == true) {
								vector<Candle_line> vec_struct;
								vec_struct.reserve(500);
								process_file_candle(str_filepath_candle, t.last, &vec_struct);
								cb.set_struct_candle14400(vec_data_instruments.at(i), vec_struct);
							} else {
								cb.set_status(6);
							}

							//	read the trade history file into an a map vs instruments
							str_filepath_history = str_path_scripture + "\\" + str_cur_id + "\\"
								+ str_filename_hist_prefix
								+ vec_data_instruments.at(i)
								+ str_filename_hist_suffix;

							if (boost::filesystem::is_regular_file(str_filepath_history) == true) {
								vector<History_line> vec_struct;
								vec_struct.reserve(500);
								process_file_history(str_filepath_history, t.last, &vec_struct);
								cb.set_struct_history(vec_data_instruments.at(i), vec_struct);
							} else {
								cb.set_status(7);
							}

							//	read the order book file into an a map vs instruments
							str_filepath_orderbook = str_path_scripture + "\\" + str_cur_id + "\\"
								+ str_filename_orderbook_prefix
								+ vec_data_instruments.at(i)
								+ str_filename_orderbook_suffix;

							if (boost::filesystem::is_regular_file(str_filepath_orderbook) == true) {
								vector<Orderbook_line> vec_struct;
								vec_struct.reserve(500);
								process_file_orderbook(str_filepath_orderbook, t.last, &vec_struct);
								cb.set_struct_orderbook(vec_data_instruments.at(i), vec_struct);
							} else {
								cb.set_status(8);
							}
						}
					}
					// Read all of the data files into memory
					//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
				}


				if (cb.get_status() == 1) {

					//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
					//	Write out the data to a BIN file.
					//
					string str_bin_filepath = str_path_bin + "\\" + str_cur_id + ".bin";
					string str_ascii_filepath = str_path_bin + "\\" + str_cur_id + ".asc";

					//// testing ascii writing.
					cb.export_text(str_ascii_filepath);

					cb.export_bin(str_bin_filepath);

					//// testing bin reading.
					cb.import_bin(str_bin_filepath);
					if (cb.get_status() == 1) {
						std::cout << "\tBIN successfully read\n";
					}
					//
					//	Write out the data to a BIN file.
					//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

					/*
					//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
					//	Test the data read/write process from BIN
					//
					cur_bin cb_test = cur_bin();
					cb_test.import_bin(str_bin_filepath);
					cb_test.export_text(str_ascii_filepath);
					//
					//	Test the data read/write process from BIN
					//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
					*/
					std::cout << "\tCur id Complete : " << str_cur_id << "\n";
				} else {
					std::cout << "\tCur id DISCARDED : " << str_cur_id << "\n";

				}
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
void polo_cur::process_file_candle(string str_filepath
												, double d_val_last
												, vector<Candle_line> * ptr_vec_output
												) {
	vector<Candle_line> vec_struct_file;
	vec_struct_file.reserve(500);

	int i_val = -1, i_start = -1, i_end = -1;

	std::string str_line;
	std::ifstream ifs(str_filepath);
	while (std::getline(ifs, str_line)) {
		if (str_line.size() > 0) {
			vector<string> vec_line;
			boost::split(vec_line, str_line, boost::is_any_of("|"));
			if (vec_line.size() >= 8) {
				Candle_line cl;

				cl.unix_time = std::stoi(vec_line.at(0), nullptr, 10);
				cl.val1 = std::stod(vec_line.at(1), nullptr);
				cl.val2 = std::stod(vec_line.at(2), nullptr);
				cl.val3 = std::stod(vec_line.at(3), nullptr);
				cl.val4 = std::stod(vec_line.at(4), nullptr);
				cl.val5 = std::stod(vec_line.at(5), nullptr);
				cl.vol = std::stod(vec_line.at(6), nullptr);
				//cl.vol = std::stod(vec_line.at(7), nullptr);

				vec_struct_file.push_back(cl);
			}
		}
	}

	// Reverse the list.
	if (vec_struct_file.size() > 0) {
		for (int i = static_cast<int>(vec_struct_file.size()) - 1; i >= 0; --i) {
			ptr_vec_output->push_back(vec_struct_file.at(static_cast<size_t>(i)));
		}
	}
}


// Record vector with elements newest (lower) to latest (higher)
//		1					2				3				4
//137.33920319|16.63857248|137.40000000|23.79240309
//137.33920318|0.00728780|137.45367868|1.13800000
//137.27802432|0.00729104|137.47839826|72.64250000
//137.23912591|0.07842815|137.57818032|31.71775437
void polo_cur::process_file_orderbook(string str_filepath
													, double d_val_last
													, vector<Orderbook_line> * ptr_vec_output
													) {
	std::string str_line;
	std::ifstream ifs(str_filepath);
	while (std::getline(ifs, str_line)) {
		if (str_line.size() > 0) {
			vector<string> vec_line;
			boost::split(vec_line, str_line, boost::is_any_of("|"));
			if (vec_line.size() >= 4) {
				Orderbook_line ol;

				ol.val1 = std::stod(vec_line.at(0), nullptr);
				ol.val2 = std::stod(vec_line.at(1), nullptr);
				ol.val3 = std::stod(vec_line.at(2), nullptr);
				ol.val4 = std::stod(vec_line.at(3), nullptr);

				ptr_vec_output->push_back(ol);
				//std::cout << "\tline : " << str_line << "\n";
			}
		}
	}
}


// Read the text data file into memory
// Record vector with elements newest (lower) to latest (higher)   
//	0(skip)	1(skip)			2					3		4		5			6(skip)
//411121485|26532891|2019-03-1812:52:20|3980.92148989|sel|2184.04682637|0.54862846
//411121484|26532890|2019-03-1812:52:20|3981.27192630|sel|8.78997159|0.00220783
//411121478|26532889|2019-03-1812:52:06|3983.29999997|buy|187.88003226|0.04716693
void polo_cur::process_file_history(string str_filepath
												, double d_val_last
												, vector<History_line> * ptr_vec_output
												) {
	int i_epoch_start = -1, i_epoch_end = -1;
	double d_vol_total = 0.0;

	std::string str_line;
	std::ifstream ifs(str_filepath);
	while (std::getline(ifs, str_line)) {
		if (str_line.size() > 0) {
			vector<string> vec_line;
			boost::split(vec_line, str_line, boost::is_any_of("|"));
			if (vec_line.size() >= 7) {
				History_line hl;

				hl.unix_time = seconds_from_epoch(vec_line.at(2));

				bool b_buysell = false;
				if (vec_line.at(4).compare("buy") == 0) {
					hl.action = 1;
				} else {
					hl.action = -1;
				}

				hl.priceusd = std::stod(vec_line.at(3), nullptr);
				hl.amountusd = std::stod(vec_line.at(5), nullptr);

				// This is the amount of units in the crypto.  Not used.
				//hl.amountunit = std::stod(vec_line.at(6), nullptr);

				ptr_vec_output->push_back(hl);
				//std::cout << "\tline : " << str_line << "\n";
			}
		}
	}
}

