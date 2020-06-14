/*
A class for handling poloniex data.  But it can be tailored to anything.  What's important
is that it can read the base data files, convert them to a BIN format, and then load
them from that format quickly when required.  This will be the core object loaded into memory
when processing crypto data at a specific point in time.

2019-12-21	ASP

Merged back into mleck utility 2019-12-30

Merged into convert_to_bin utility 2019-12-30

Forked from convert_to_bin utility 2019-12-28
*/

#pragma once

#ifndef CUR_BIN_H
#define CUR_BIN_H

using namespace std;

#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <boost/algorithm/string.hpp>

struct Ticker {
	double last = 0.0;
	double high24hr = 0.0;
	double highestbid = 0.0;
	double percentchange = 0.0;
	double low24hr = 0.0;
	double lowestask = 0.0;
};

struct Candle_line {
	unsigned long unix_time = 0;
	double val1 = 0.0;
	double val2 = 0.0;
	double val3 = 0.0;
	double val4 = 0.0;
	double val5 = 0.0;
	double vol = 0.0;
};

struct History_line {
	unsigned long unix_time = 0;
	double priceusd = 0.0;
	double amountusd = 0.0;
	// +1 buy, -1 sell
	long int action = 0;
};

struct Orderbook_line {
	double val1 = 0.0;
	double val2 = 0.0;
	double val3 = 0.0;
	double val4 = 0.0;
};

class cur_bin {
private:
	const char * str_class_name = "cur_bin";

	int i_status = 0;

	// Data defining the generation of this particular mleck situation.
	vector<string> vec_data_situation;

	// Only lines that meet the vec_instruments critiria are included.
	// This is a vector of all instruments in this structure.
	vector<string> vec_data_instruments;

	// A map of all instrument specific data at this snapshot.
	// Key: Instrument
	map<string, Ticker> map_struct_ticker;

	// Key: Instrument
	map <string, vector<Candle_line>> map_vec_candle300;
	map <string, vector<Candle_line>> map_vec_candle14400;
	map <string, vector<History_line>> map_vec_history;
	map <string, vector<Orderbook_line>> map_vec_orderbook;

public:

	// Getter / Setters
	int get_status() { return i_status; }
	void set_status(int i_status_in) { i_status = i_status_in; }

	size_t get_map_ticker_count() { return map_struct_ticker.size(); }

	Ticker get_ticker(string str_ticker) { return map_struct_ticker[str_ticker]; };

	Ticker * get_ticker_ptr(string str_ticker) { return &map_struct_ticker[str_ticker]; };

	Ticker get_ticker(int64_t nth);

	string get_instrument(int64_t nth);

	void add_string_situation(string str_val) { vec_data_situation.push_back(str_val); };
	void add_string_instrument(string str_val) { vec_data_instruments.push_back(str_val); };

	bool is_valid_instrument(string);

	vector<string> get_vec_situation() { return vec_data_situation; }

	// This is set by reading the ticker file, and populating the struct for this instrument
	// Pass instrument, ticker property, property value.
	void set_ticker_double(string, string, double);
	map<string, Ticker> get_map_struct_ticker() { return map_struct_ticker; }

	void set_struct_candle300(string, vector<Candle_line>);
	void set_struct_candle14400(string, vector<Candle_line>);
	void set_struct_history(string, vector<History_line>);
	void set_struct_orderbook(string, vector<Orderbook_line>);
	vector<Candle_line> get_struct_candle300(string str_instrument) { return map_vec_candle300[str_instrument]; }
	vector<Candle_line> get_struct_candle14400(string str_instrument) { return map_vec_candle14400[str_instrument]; }
	vector<History_line> get_struct_history(string str_instrument) { return map_vec_history[str_instrument]; }
	vector<Orderbook_line> get_struct_orderbook(string str_instrument) { return map_vec_orderbook[str_instrument]; }

	vector<Candle_line> * get_struct_candle300_ptr(string str_instrument) { return &map_vec_candle300[str_instrument]; }
	vector<Candle_line>* get_struct_candle14400_ptr(string str_instrument) { return &map_vec_candle14400[str_instrument]; }
	vector<History_line>* get_struct_history_ptr(string str_instrument) { return &map_vec_history[str_instrument]; }
	vector<Orderbook_line>* get_struct_orderbook_ptr(string str_instrument) { return &map_vec_orderbook[str_instrument]; }


	// Clear all maps and vectors.
	void clear();

	// Export as a formatted text file.
	void export_text(string);

	// Export in BIN format.
	void export_bin(string);

	// Import BIN format.
	void import_bin(string);
	
};

#endif //CUR_BIN_H