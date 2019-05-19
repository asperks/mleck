// Class for a snapshot of poloniex data.  The data from poloniex is retrieved using
//	a separate method.
//		Reads the poloniex data text files.
//		Creates binary representations of that data, one for each instrument.
//		Populates a class for access.
//
//	There is a single function that sucks the data out of the folders.  I might want
//	to change that later.  
//

#pragma once

#ifndef POLO_GEN_H
#define POLO_GEN_H

using namespace std;


#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string.hpp> 

class polo_gen {
private:
	// Data Members 
	string str_class_name = "polo_gen";

	bool b_exists_scripture = false;
	bool b_loaded = false;

	string str_gen = "";
	string str_path_scripture = "";

	string str_filename_situation = "0.situation.asc";
	string str_filename_ticker = "1.ticker.asc";
	string str_filename_instruments = "3.instruments_btc.asc";

	// From the instrument, the filename to read is constructed using
	//	the prefix and suffix.
	string str_filename_candle_prefix = "4.instrument.candle_chart.";
	string str_filename_candle_300_suffix = ".300.asc";
	string str_filename_candle_14400_suffix = ".14400.asc";

	string str_filename_hist_prefix = "4.instrument.trade_hist.";
	string str_filename_hist_suffix = ".asc";
	string str_filename_orderbook_prefix = "4.instrument.order_book.";
	string str_filename_orderbook_suffix = ".asc";

	string str_ticker_record = "last|quoteVolume|high24hr|highestBid|percentChange|low24hr|lowestAsk";
	vector<string> vec_ticker_record;

public:

	// Member Functions() 

	// Read all of the files links for the scripture folder into 
	// memory.
	//	if vec_instruments is empty, read all, if not, only read
	//	in the instruments in the vector.
	int init(vector<string> vec_gen
		, vector<string> vec_instrument
		, string str_path_scripture
	);


	string to_string();

};


#endif //POLO_GEN_H