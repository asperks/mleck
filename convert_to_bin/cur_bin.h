#pragma once

#ifndef CUR_BIN_H
#define CUR_BIN_H

using namespace std;


#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>


class cur_bin {
private:
	int i_status = 0;

	// Data defining the generation of this particular mleck situation.
	vector<string> vec_data_situation;

	// Only lines that meet the vec_instruments critiria are included.
	// This is a vector of all instruments in this structure.
	vector<string> vec_data_instruments;

	// A map of all instrument specific data at this snapshot.
	// Key: Instrument
	map<string, map<string, double>> map_map_ticker;

	// Key: Instrument
	map<string, vector<tuple<int, double, double, double, double, double, double, double>>> map_vec_candle300;
	map<string, vector<tuple<int, double, double, double, double, double, double, double>>> map_vec_candle14400;
	map<string, vector<tuple<int, double, bool, double>>> map_vec_history;
	map<string, vector<tuple<double, double, double, double>>> map_vec_orderbook;

public:

	// Getter / Setters
	int get_status() { return i_status; }

	void add_string_situation(string str_val) { vec_data_situation.push_back(str_val); };
	void add_string_instrument(string str_val) { vec_data_instruments.push_back(str_val); };
	void add_double_ticker(string, string, double);

	void set_tup_candle300(string, vector<tuple<int, double, double, double, double, double, double, double>>);
	void set_tup_candle14400(string, vector<tuple<int, double, double, double, double, double, double, double>>);
	void set_tup_history(string, vector<tuple<int, double, bool, double>>);
	void set_tup_orderbook(string, vector<tuple<double, double, double, double>>);
	
	void clear();

	void export_all_text(string);


};

#endif //CUR_BIN_H