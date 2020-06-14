/*
A class that parses currency data, and formats it so that it provides a
unique view of the properties it is viewing.

The cur_bin2 class measure the ticker performance between a sequence of three
sequential cur_bin objects, and writes this data out to an external BIN2 file
so that it can be quickly loaded for future processing.

*/

#pragma once

#ifndef CUR_BIN2_H
#define CUR_BIN2_H

using namespace std;

#include "cur_bin.h"

#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <experimental/filesystem>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

class cur_bin2 {
private:
	bool b_valid = false;
	string str_path_bin;

	cur_bin cb_base;

	// a ratio between the base ticker and the next.
	//	This would be removed if this were to be a live implementation.  It is 
	// implemented here so that I can quickly measure performance for back testing.
	map<string, Ticker> map_struct_ticker_next;

	// a ratio between the previous ticker and the base
	map<string, Ticker> map_struct_ticker_prev;

	// There will be X elements for each of these vectors.  These are values that are
	//	derived from reading the They will contain :
	//		Average
	//		Total
	// If the cur_bin data isn't available (no entries) the numbers will be zero'd


	//0 = min;
	//1 = max;
	//2 = avg;
	//3 = mid;
	//4 = tot;
	map <string, vector<Candle_line>> map_cl_prev_300;
	map <string, vector<Candle_line>> map_cl_prev_14400;
	map <string, vector<History_line>> map_hl_prev;
	map <string, vector<Orderbook_line>> map_ol_prev;


	// This will have as many elements in it as the map_vec_dbl_prev_tot vector does.  
	Candle_line cl_prev_300_tot;
	Candle_line cl_prev_14400_tot;
	History_line hl_prev_history_tot;
	Orderbook_line ol_prev_orderbook_tot;

	Orderbook_line ol_prev_orderbook_totcalc;


public:

	cur_bin2();

	void clear();

	void init(string str_path_bin, int cur_id, string str_filepath_bin2, cur_bin cb_in);

	void set_base(cur_bin cb_in) { cb_base = cb_in; }
	void set_path_bin(string str_path_bin_in) { str_path_bin = str_path_bin_in; }

	// Export in ascii format.
	void export_text(string);

	// Export in BIN2 format.
	void export_bin(string);

	// Import BIN2 format.
	void import_bin(string);


	//0 = min;
	//1 = max;
	//2 = avg;
	//3 = mid;
	//4 = tot;
	Candle_line get_cl_300_line(string str_instrument, int nth) { return map_cl_prev_300[str_instrument].at(nth); }
	Candle_line get_cl_14400_line(string str_instrument, int nth) { return map_cl_prev_14400[str_instrument].at(nth); }
	History_line get_hl_line(string str_instrument, int nth) { return map_hl_prev[str_instrument].at(nth); }
	Orderbook_line get_ol_line(string str_instrument, int nth) { return map_ol_prev[str_instrument].at(nth); }


	Candle_line get_mid_cl(vector<Candle_line> vec);
	History_line get_mid_hl(vector<History_line> vec);
	Orderbook_line get_mid_ol(vector<Orderbook_line> vec);

};



#endif