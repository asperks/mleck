// Class for a snapshot of poloniex data.  The data from poloniex is retrieved using
//	a separate method.
//		Reads the poloniex data text files.
//		Populates a class for access.  That class reads/writes binary for future
//		processing.
//
//	There is a single function that sucks the data out of the folders and uses that
//	data in memory.  I might want to change that later.  
//

#pragma once

#ifndef POLO_GEN_H
#define POLO_GEN_H

using namespace std;

// serialization requirements
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <time.h>
#include "cur_bin.h"

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string.hpp> 

#include <boost/date_time.hpp>
namespace bt = boost::posix_time;



// Convert date strings to unix epochs in seconds.
// https://stackoverflow.com/a/3787188
const std::locale formats[] = {
	std::locale(std::locale::classic(),new bt::time_input_facet("%Y-%m-%d %H:%M:%S")),
	std::locale(std::locale::classic(),new bt::time_input_facet("%Y/%m/%d %H:%M:%S")),
	std::locale(std::locale::classic(),new bt::time_input_facet("%d.%m.%Y %H:%M:%S")),
	std::locale(std::locale::classic(),new bt::time_input_facet("%Y-%m-%d")) };
const size_t formats_n = sizeof(formats) / sizeof(formats[0]);


class polo_gen {
private:
	// Data Members 
	string str_class_name = "polo_gen";

	bool b_exists_scripture = false;
	bool b_loaded = false;

	string str_gen = "";
	string str_path_scripture = "";

	const string str_filename_situation = "0.situation.asc";
	const string str_filename_ticker = "1.ticker.asc";
	const string str_filename_instruments = "3.instruments_btc.asc";

	// From the instrument, the filename to read is constructed using
	//	the prefix and suffix.
	const string str_filename_candle_prefix = "4.instrument.candle_chart.";
	const string str_filename_candle_300_suffix = ".300.asc";
	const string str_filename_candle_14400_suffix = ".14400.asc";

	const string str_filename_hist_prefix = "4.instrument.trade_hist.";
	const string str_filename_hist_suffix = ".asc";
	const string str_filename_orderbook_prefix = "4.instrument.order_book.";
	const string str_filename_orderbook_suffix = ".asc";

	const string str_ticker_record = "last|quoteVolume|high24hr|highestBid|percentChange|low24hr|lowestAsk";
	vector<string> vec_ticker_record;

public:

	// Member Functions() 

	// Read all of the files links for the scripture folder into 
	// memory.
	//	if vec_instruments is empty, read all, if not, only read
	//	in the instruments in the vector.
	int process(vector<string> vec_gen
		, vector<string> vec_instrument
		, string str_path_scripture
		, string str_path_bin
	);

	// Record vector with elements newest (lower) to latest (higher)
//			0			1				2					3			 4			5				6			7
//	1548777600|0.00000017|0.00000016|0.00000016|0.00000016|0.00000016|5.82696703|34300966.899461
//	1548792000|0.00000017|0.00000016|0.00000017|0.00000016|0.00000016|2.57156965|15154582.862649
//	1548806400|0.00000017|0.00000016|0.00000017|0.00000016|0.00000016|0.0729452|453722.45582316
	void process_file_candle(string, double, vector<Candle_line> *);

	// Record vector with elements newest (lower) to latest (higher)
	//	0(skip)	1(skip)			2						3			 4			5				6(skip)
	//411121485|26532891|2019-03-18 12:52:20|3980.92148989|sel|2184.04682637|0.54862846
	//411121484|26532890|2019-03-18 12:52:20|3981.27192630|sel|8.78997159|0.00220783
	//411121478|26532889|2019-03-18 12:52:06|3983.29999997|buy|187.88003226|0.04716693
	void process_file_history(string, double, vector<History_line> *);

	// Record vector with elements newest (lower) to latest (higher)
	//		1					2				3				4
	//137.33920319|16.63857248|137.40000000|23.79240309
	//137.33920318|0.00728780|137.45367868|1.13800000
	//137.27802432|0.00729104|137.47839826|72.64250000
	//137.23912591|0.07842815|137.57818032|31.71775437
	void process_file_orderbook(string, double, vector<Orderbook_line> *);


	// Convert date strings to unix epochs in seconds.
	// https://stackoverflow.com/a/3787188
	std::time_t pt_to_time_t(const bt::ptime& pt) {
		bt::ptime timet_start(boost::gregorian::date(1970, 1, 1));
		bt::time_duration diff = pt - timet_start;
		return diff.ticks() / bt::time_duration::rep_type::ticks_per_second;
	}

	unsigned long seconds_from_epoch(const std::string& s) {
		bt::ptime pt;
		for (size_t i = 0; i < formats_n; ++i) {
			std::istringstream is(s);
			is.imbue(formats[i]);
			is >> pt;
			if (pt != bt::ptime()) break;
		}
		//std::cout << " ptime is " << pt << '\n';
		//std::cout << " seconds from epoch are " << pt_to_time_t(pt) << '\n';
		return static_cast<unsigned long>(pt_to_time_t(pt));
	}

	string to_string();
};

#endif //POLO_GEN_H