// refer cur_bin.h for details.
//
//	
//

#include "cur_bin.h"

void cur_bin::add_double_ticker(string str_ticker, string str_key, double d_val) {
	bool b_found_map = false;

	// Find if there is already a map for this ticker.
	if (map_map_ticker.size() > 0) {
		if (map_map_ticker.count(str_ticker) > 0) { b_found_map = true; }
	}

	map<string, double> map_ticker;
	// get the map from the map of maps if it already exists.
	if (b_found_map == true) {	map_ticker = map_map_ticker[str_ticker];}

	map_ticker[str_key] = d_val;
	map_map_ticker[str_ticker] = map_ticker;

}


void cur_bin::set_tup_candle300(string str_ticker, vector<tuple<int, double, double, double, double, double, double, double>> vec) {
	map_vec_candle300.emplace(str_ticker, std::move(vec));
}


void cur_bin::set_tup_candle14400(string str_ticker, vector<tuple<int, double, double, double, double, double, double, double>> vec) {
	map_vec_candle14400.emplace(str_ticker, std::move(vec));
}


void cur_bin::set_tup_history(string str_ticker, vector<tuple<int, double, bool, double>> vec) {
	map_vec_history.emplace(str_ticker, std::move(vec));
}


void cur_bin::set_tup_orderbook(string str_ticker, vector<tuple<double, double, double, double>> vec) {
	map_vec_orderbook.emplace(str_ticker, std::move(vec));
}


void cur_bin::clear() {
	vec_data_situation.clear();
	vec_data_instruments.clear();
	map_map_ticker.clear();
	map_vec_candle300.clear();
	map_vec_candle14400.clear();
	map_vec_history.clear();
	map_vec_orderbook.clear();
	i_status = 0;
}

