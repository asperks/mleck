/* refer cur_bin.h for details.

Forked from convert_to_bin utility 2019-12-28
*/


#include "cur_bin.h"

bool cur_bin::is_valid_instrument(string str_instrument) {
	if (std::find(vec_data_instruments.begin(), vec_data_instruments.end(), str_instrument) != vec_data_instruments.end()) {
		return true;
	} else {
		return false;
	}
}


void cur_bin::set_ticker_double(string str_ticker, string str_key, double d_val) {
	bool b_found_map = false;
	Ticker ticker{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	Ticker * ptr_t = &ticker;

	// modifies str to lower case.
	// https://stackoverflow.com/a/313988
	boost::algorithm::to_lower(str_key);

	// Find if there is already a map reference for this ticker.
	if (map_struct_ticker.size() > 0) {
		if (map_struct_ticker.count(str_ticker) > 0) {
			ptr_t = &map_struct_ticker[str_ticker];
			b_found_map = true;
		}
	}

	// If the map reference wasn't found.  Create a new entry in the map.
	if (b_found_map == false) {
		map_struct_ticker[str_ticker] = ticker;
		ptr_t = &map_struct_ticker[str_ticker];
	}

	// Populate the properties of the ticker struct.
	if (str_key.compare("last") == 0) {
		ptr_t->last = d_val;
	} else if (str_key.compare("high24hr") == 0) {
		ptr_t->high24hr = d_val;
		//	} else if (str_key.compare("isfrozen") == 0) {
		//		ptr_t->isfrozen = d_val;
	} else if (str_key.compare("highestbid") == 0) {
		ptr_t->highestbid = d_val;
	} else if (str_key.compare("percentchange") == 0) {
		ptr_t->percentchange = d_val;
	} else if (str_key.compare("low24hr") == 0) {
		ptr_t->low24hr = d_val;
	} else if (str_key.compare("lowestask") == 0) {
		ptr_t->lowestask = d_val;
		//	} else if (str_key.compare("basevolume") == 0) {
		//		ptr_t->basevolume = d_val;
	}
}


void cur_bin::set_struct_candle300(string str_ticker, vector<Candle_line> vec) {
	map_vec_candle300.emplace(str_ticker, std::move(vec));
}


void cur_bin::set_struct_candle14400(string str_ticker, vector<Candle_line> vec) {
	map_vec_candle14400.emplace(str_ticker, std::move(vec));
}


void cur_bin::set_struct_history(string str_ticker, vector<History_line> vec) {
	map_vec_history.emplace(str_ticker, std::move(vec));
}


void cur_bin::set_struct_orderbook(string str_ticker, vector<Orderbook_line> vec) {
	map_vec_orderbook.emplace(str_ticker, std::move(vec));
}


void cur_bin::clear() {
	vec_data_situation.clear();
	vec_data_instruments.clear();
	map_struct_ticker.clear();
	map_vec_candle300.clear();
	map_vec_candle14400.clear();
	map_vec_history.clear();
	map_vec_orderbook.clear();
	i_status = 0;
}


void cur_bin::import_bin(string str_filepath) {
	fstream fs;
	fs.open(str_filepath, ios::in | ios::binary);

	if (fs.is_open()) {
		cur_bin::clear();
		this->set_status(1);

		size_t i_cnt_situation;
		// Get the count of the number of situation elements.
		fs.read((char*)&i_cnt_situation, sizeof(i_cnt_situation));
		if (i_cnt_situation > 0) {
			cur_bin::vec_data_situation.reserve(i_cnt_situation);
			for (size_t i_situation = 0; i_situation < i_cnt_situation; ++i_situation) {
				size_t i_strlen;
				// Get the string length
				fs.read((char*)&i_strlen, sizeof(i_strlen));
				std::string str_situation = "";
				for (size_t i_str = 0; i_str < i_strlen; ++i_str) {
					char c;
					// Get the string char
					fs.read((char*)&c, sizeof(c));
					// push back the char on the string
					str_situation.push_back(c);
				}
				// add the string to the vector.
				vec_data_situation.push_back(str_situation);
			}
		}

		size_t i_cnt_instrument;
		// Get the count of the number of situation elements.
		fs.read((char*)&i_cnt_instrument, sizeof(i_cnt_instrument));
		if (i_cnt_instrument > 0) {
			cur_bin::vec_data_instruments.reserve(i_cnt_instrument);

			for (size_t i_instrument = 0; i_instrument < i_cnt_instrument; ++i_instrument) {
				size_t i_instrument_strlen;
				// Get the string length
				fs.read((char*)&i_instrument_strlen, sizeof(i_instrument_strlen));
				std::string str_instrument = "";
				for (size_t i_str = 0; i_str < i_instrument_strlen; ++i_str) {
					char c;
					// Get the string char
					fs.read((char*)&c, sizeof(c));
					// push back the char on the string
					str_instrument.push_back(c);
				}
				// add the string to the vector.
				vec_data_instruments.push_back(str_instrument);

				Ticker t;
				fs.read((char*)&t, sizeof(t));
				map_struct_ticker[str_instrument] = t;

				size_t i_vector_loop = 0;
				
				// Check to make sure that this 
				if (this->get_status() == 1) {
					// Read the Candle300 data
					i_vector_loop = 0;
					fs.read((char*)&i_vector_loop, sizeof(i_vector_loop));
					map_vec_candle300[str_instrument].reserve(i_vector_loop);
					if (i_vector_loop == 0) {
						this->set_status(2);
					} else {
						for (size_t i_vec = 0; i_vec < i_vector_loop; ++i_vec) {
							Candle_line struct_extract;
							fs.read((char*)&struct_extract, sizeof(struct_extract));
							map_vec_candle300[str_instrument].push_back(struct_extract);
						}
					}

					// Read the Candle14400 data
					i_vector_loop = 0;
					fs.read((char*)&i_vector_loop, sizeof(i_vector_loop));
					if (i_vector_loop == 0) {
						this->set_status(2);
					} else {
						map_vec_candle14400[str_instrument].reserve(i_vector_loop);
						for (size_t i_vec = 0; i_vec < i_vector_loop; ++i_vec) {
							Candle_line struct_extract;
							fs.read((char*)&struct_extract, sizeof(struct_extract));
							map_vec_candle14400[str_instrument].push_back(struct_extract);
						}
					}

					// Read the History data
					i_vector_loop = 0;
					fs.read((char*)&i_vector_loop, sizeof(i_vector_loop));
					if (i_vector_loop == 0) {
						this->set_status(2);
					} else {
						map_vec_history[str_instrument].reserve(i_vector_loop);
						for (size_t i_vec = 0; i_vec < i_vector_loop; ++i_vec) {
							History_line struct_extract;
							fs.read((char*)&struct_extract, sizeof(struct_extract));
							map_vec_history[str_instrument].push_back(struct_extract);
						}
					}

					// Read the Orderbook data
					i_vector_loop = 0;
					fs.read((char*)&i_vector_loop, sizeof(i_vector_loop));
					if (i_vector_loop == 0) {
						this->set_status(2);
					} else {
						map_vec_orderbook[str_instrument].reserve(i_vector_loop);
						for (size_t i_vec = 0; i_vec < i_vector_loop; ++i_vec) {
							Orderbook_line struct_extract;
							fs.read((char*)&struct_extract, sizeof(struct_extract));
							map_vec_orderbook[str_instrument].push_back(struct_extract);
						}
					}
				}
			}
		}

		fs.close();
	}
}


void cur_bin::export_bin(string str_filepath) {
	fstream fs;
	fs.open(str_filepath, ios::out | ios::binary);

	if (fs.is_open()) {
		// Write out the situation data
		if (vec_data_situation.size() > 0) {
			size_t i_cnt = static_cast<size_t>(vec_data_situation.size());
			// Write the number of situation elements.
			fs.write((char*)&i_cnt, sizeof(i_cnt));
			for (size_t i_situation = 0; i_situation < vec_data_situation.size(); ++i_situation) {
				size_t i_strlen = static_cast<size_t>(vec_data_situation.at(i_situation).size());
				const char * cstr = vec_data_situation.at(i_situation).c_str();
				// Write the string length and the situation string of that length.
				fs.write((char*)&i_strlen, sizeof(i_strlen));
				fs.write((char*)cstr, i_strlen);
			}
		}

		// Write out the instrument name
		if (vec_data_instruments.size() > 0) {
			size_t i_cnt = static_cast<size_t>(vec_data_instruments.size());
			// Write the number of instrument elements.
			fs.write((char*)&i_cnt, sizeof(i_cnt));

			for (size_t i_instrument = 0; i_instrument < vec_data_instruments.size(); ++i_instrument) {
				size_t i_strlen = static_cast<size_t>(vec_data_instruments.at(i_instrument).size());
				const char * cstr = vec_data_instruments.at(i_instrument).c_str();
				// Write the string length and the instrument string of that length.
				fs.write((char*)&i_strlen, sizeof(i_strlen));
				fs.write((char*)cstr, i_strlen);

				std::cout << "cur bin write = #" << to_string(i_instrument) << "\t" << vec_data_instruments.at(i_instrument) << "\n";

				Ticker t = map_struct_ticker[vec_data_instruments.at(i_instrument)];
				// Write the ticker for this element to the binary file.
				fs.write((char*)&t, sizeof(t));

				size_t i_cnt_loop = 0;
				// Write the Candle300 data
				i_cnt_loop = map_vec_candle300[vec_data_instruments.at(i_instrument)].size();
				fs.write((char*)&i_cnt_loop, sizeof(i_cnt_loop));
				if (i_cnt_loop > 0) {
					for (size_t i_vec = 0; i_vec < i_cnt_loop; ++i_vec) {
						Candle_line cl = map_vec_candle300[vec_data_instruments.at(i_instrument)].at(i_vec);
						fs.write((char*)&cl, sizeof(cl));
					}
				}

				// Write the Candle14400 data
				i_cnt_loop = map_vec_candle14400[vec_data_instruments.at(i_instrument)].size();
				fs.write((char*)&i_cnt_loop, sizeof(i_cnt_loop));
				if (i_cnt_loop > 0) {
					for (size_t i_vec = 0; i_vec < i_cnt_loop; ++i_vec) {
						Candle_line cl = map_vec_candle14400[vec_data_instruments.at(i_instrument)].at(i_vec);
						fs.write((char*)&cl, sizeof(cl));
					}
				}

				// Write the History data
				i_cnt_loop = map_vec_history[vec_data_instruments.at(i_instrument)].size();
				fs.write((char*)&i_cnt_loop, sizeof(i_cnt_loop));
				if (i_cnt_loop > 0) {
					for (size_t i_vec = 0; i_vec < i_cnt_loop; ++i_vec) {
						History_line hl = map_vec_history[vec_data_instruments.at(i_instrument)].at(i_vec);
						fs.write((char*)&hl, sizeof(hl));
					}
				}

				// Write the Orderbook data
				i_cnt_loop = map_vec_orderbook[vec_data_instruments.at(i_instrument)].size();
				fs.write((char*)&i_cnt_loop, sizeof(i_cnt_loop));
				if (i_cnt_loop > 0) {
					for (size_t i_vec = 0; i_vec < i_cnt_loop; ++i_vec) {
						Orderbook_line ol = map_vec_orderbook[vec_data_instruments.at(i_instrument)].at(i_vec);
						fs.write((char*)&ol, sizeof(ol));
					}
				}
			}
		}

		fs.close();
	}
}


void cur_bin::export_text(string str_filepath) {
	ofstream ofs(str_filepath);

	if (ofs.is_open()) {
		//ofs << "This is a line.\n";
		//ofs << "This is another line.\n";

		ofs << "situation elements:" << vec_data_situation.size() << "\n";

		if (vec_data_situation.size() > 0) {
			for (size_t i_situation = 0; i_situation < vec_data_situation.size(); ++i_situation) {
				ofs << "\t" << vec_data_situation.at(i_situation).size() << "|" << vec_data_situation.at(i_situation) << "\n";
			}
		}

		if (vec_data_instruments.size() > 0) {
			ofs << "\t\t" << "instrument count:" << vec_data_instruments.size() << "\n";
			for (size_t i_instrument = 0; i_instrument < vec_data_instruments.size(); ++i_instrument) {
				ofs << "\t\t" << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
				ofs << "\t\t\t" << "instrument:";
				ofs << vec_data_instruments.at(i_instrument).size() << "|" << vec_data_instruments.at(i_instrument) << "\n";

				ofs << "\t\t\t" << "last:" << fixed << setprecision(8) << map_struct_ticker[vec_data_instruments.at(i_instrument)].last << "\n";
				ofs << "\t\t\t" << "high24hr:" << fixed << setprecision(8) << map_struct_ticker[vec_data_instruments.at(i_instrument)].high24hr << "\n";
				ofs << "\t\t\t" << "highestbid:" << fixed << setprecision(8) << map_struct_ticker[vec_data_instruments.at(i_instrument)].highestbid << "\n";
				ofs << "\t\t\t" << "percentchange:" << fixed << setprecision(8) << map_struct_ticker[vec_data_instruments.at(i_instrument)].percentchange << "\n";
				ofs << "\t\t\t" << "low24hr:" << fixed << setprecision(8) << map_struct_ticker[vec_data_instruments.at(i_instrument)].low24hr << "\n";
				ofs << "\t\t\t" << "lowestask:" << fixed << setprecision(8) << map_struct_ticker[vec_data_instruments.at(i_instrument)].lowestask << "\n";

				ofs << "\t\t\t" << "candle300|" << map_vec_candle300[vec_data_instruments.at(i_instrument)].size() << "\n";
				for (size_t i_vec = 0; i_vec < map_vec_candle300[vec_data_instruments.at(i_instrument)].size(); ++i_vec) {
					ofs << "\t\t\t\t" << "" << map_vec_candle300[vec_data_instruments.at(i_instrument)].at(i_vec).unix_time;
					ofs << "|" << fixed << setprecision(8) << map_vec_candle300[vec_data_instruments.at(i_instrument)].at(i_vec).val1;
					ofs << "|" << fixed << setprecision(8) << map_vec_candle300[vec_data_instruments.at(i_instrument)].at(i_vec).val2;
					ofs << "|" << fixed << setprecision(8) << map_vec_candle300[vec_data_instruments.at(i_instrument)].at(i_vec).val3;
					ofs << "|" << fixed << setprecision(8) << map_vec_candle300[vec_data_instruments.at(i_instrument)].at(i_vec).val4;
					ofs << "|" << fixed << setprecision(8) << map_vec_candle300[vec_data_instruments.at(i_instrument)].at(i_vec).val5;
					ofs << "|" << fixed << setprecision(8) << map_vec_candle300[vec_data_instruments.at(i_instrument)].at(i_vec).vol;
					ofs << "\n";
				}
				ofs << "\t\t\txxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";

				ofs << "\t\t\t" << "candle14400|" << map_vec_candle14400[vec_data_instruments.at(i_instrument)].size() << "\n";
				for (size_t i_vec = 0; i_vec < map_vec_candle14400[vec_data_instruments.at(i_instrument)].size(); ++i_vec) {
					ofs << "\t\t\t\t" << "" << map_vec_candle14400[vec_data_instruments.at(i_instrument)].at(i_vec).unix_time;
					ofs << "|" << fixed << setprecision(8) << map_vec_candle14400[vec_data_instruments.at(i_instrument)].at(i_vec).val1;
					ofs << "|" << fixed << setprecision(8) << map_vec_candle14400[vec_data_instruments.at(i_instrument)].at(i_vec).val2;
					ofs << "|" << fixed << setprecision(8) << map_vec_candle14400[vec_data_instruments.at(i_instrument)].at(i_vec).val3;
					ofs << "|" << fixed << setprecision(8) << map_vec_candle14400[vec_data_instruments.at(i_instrument)].at(i_vec).val4;
					ofs << "|" << fixed << setprecision(8) << map_vec_candle14400[vec_data_instruments.at(i_instrument)].at(i_vec).val5;
					ofs << "|" << fixed << setprecision(8) << map_vec_candle14400[vec_data_instruments.at(i_instrument)].at(i_vec).vol;
					ofs << "\n";
				}
				ofs << "\t\t\txxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";

				ofs << "\t\t\t" << "history|" << map_vec_history[vec_data_instruments.at(i_instrument)].size() << "\n";
				for (size_t i_vec = 0; i_vec < map_vec_history[vec_data_instruments.at(i_instrument)].size(); ++i_vec) {
					ofs << "\t\t\t\t" << "" << map_vec_history[vec_data_instruments.at(i_instrument)].at(i_vec).unix_time;
					ofs << "|" << fixed << setprecision(8) << map_vec_history[vec_data_instruments.at(i_instrument)].at(i_vec).priceusd;
					ofs << "|" << fixed << setprecision(8) << map_vec_history[vec_data_instruments.at(i_instrument)].at(i_vec).amountusd;
					ofs << "|" << map_vec_history[vec_data_instruments.at(i_instrument)].at(i_vec).action;
					ofs << "\n";
				}
				ofs << "\t\t\txxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";

				ofs << "\t\t\t" << "orderbook|" << map_vec_orderbook[vec_data_instruments.at(i_instrument)].size() << "\n";
				for (size_t i_vec = 0; i_vec < map_vec_orderbook[vec_data_instruments.at(i_instrument)].size(); ++i_vec) {
					ofs << "\t\t\t\t" << fixed << setprecision(8) << map_vec_orderbook[vec_data_instruments.at(i_instrument)].at(i_vec).val1;
					ofs << "|" << fixed << setprecision(8) << map_vec_orderbook[vec_data_instruments.at(i_instrument)].at(i_vec).val2;
					ofs << "|" << fixed << setprecision(8) << map_vec_orderbook[vec_data_instruments.at(i_instrument)].at(i_vec).val3;
					ofs << "|" << fixed << setprecision(8) << map_vec_orderbook[vec_data_instruments.at(i_instrument)].at(i_vec).val4;
					ofs << "\n";
				}
				ofs << "\t\t\txxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";

				ofs << "\t\t" << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
			}
		}

		ofs.close();
	}
}
