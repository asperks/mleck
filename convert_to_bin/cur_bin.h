/*
A class for handling poloniex data.  But it can be tailored to anything.  What's important
is that it can read the base data files, convert them to a BIN format, and then load
them from that format quickly when required.  This will be the core object loaded into memory
when processing crypto data at a specific point in time.

2019-12-21	ASP
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
	double last;
	double high24hr;
	//double isfrozen;
	double highestbid;
	double percentchange;
	double low24hr;
	double lowestask;
	//double basevolume;
};

struct Candle_line {
	int unix_time;
	double val1;
	double val2;
	double val3;
	double val4;
	double val5;
	//double val6;
	double vol;
};

struct History_line {
	int unix_time;
	double val1;
	double val2;
	bool buy;
};

struct Orderbook_line {
	double val1;
	double val2;
	double val3;
	double val4;
};


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
	map<string, Ticker> map_struct_ticker;

	// Key: Instrument
	map <string, vector<Candle_line>> map_vec_candle300;
	map <string, vector<Candle_line>> map_vec_candle14400;
	map <string, vector<History_line>> map_vec_history;
	map <string, vector<Orderbook_line>> map_vec_orderbook;

public:

	// Getter / Setters
	int get_status() { return i_status; }
	Ticker get_ticker(string str_ticker) { return map_struct_ticker[str_ticker]; };

	void add_string_situation(string str_val) { vec_data_situation.push_back(str_val); };
	void add_string_instrument(string str_val) { vec_data_instruments.push_back(str_val); };

	bool is_valid_instrument(string);

	// This is set by reading the ticker file, and populating the struct for this instrument
	void set_ticker_double(string, string, double);

	void set_struct_candle300(string, vector<Candle_line>);
	void set_struct_candle14400(string, vector<Candle_line>);
	void set_struct_history(string, vector<History_line>);
	void set_struct_orderbook(string, vector<Orderbook_line>);

	void clear();

	void export_text(string);

	void export_bin(string);

	void import_bin(string);


};

#endif //CUR_BIN_H