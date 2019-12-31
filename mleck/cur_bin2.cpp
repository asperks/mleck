/* refer cur_bin2.h for details.


*/

#include "cur_bin2.h"

cur_bin2::cur_bin2() {

}

void cur_bin2::init(string str_path_bin_in, int id_cur, string str_filepath_bin2, cur_bin cb_in) {
	b_valid = false;
	str_path_bin = str_path_bin_in;
	cb_base = cb_in;

	// Find if there is an existing BIN2 file.  If not, create it.  If it
	// exists, load it.
	//https://stackoverflow.com/questions/50960492/creating-folders-in-c/56262869#56262869
	if (std::experimental::filesystem::exists(str_filepath_bin2 + ".bin2")) {
		// Load the BIN2 file.
		import_bin(str_filepath_bin2);

	} else {
		// We have already validated that there is a preceding and succeeding bin
		//	file for this element during the is_valid_id test in the cur_bin_handler
		// These don't have extensions.
		string str_filepath_bin_prev = str_path_bin + "\\" + std::to_string(id_cur - 1) + ".bin";
		string str_filepath_bin_next = str_path_bin + "\\" + std::to_string(id_cur + 1) + ".bin";

		cur_bin cb_prev;
		cur_bin cb_next;

		cb_prev.import_bin(str_filepath_bin_prev);
		cb_next.import_bin(str_filepath_bin_next);

		if (cb_base.get_map_struct_ticker().size() > 0) {

			for (auto const& imap : cb_base.get_map_struct_ticker()) {
				//for (auto it = cb_base.get_map_struct_ticker().begin(); it != cb_base.get_map_struct_ticker().end(); ++it) {
				string str_instrument = imap.first;
				Ticker t = imap.second;

				Ticker t_prev = cb_prev.get_map_struct_ticker()[str_instrument];
				Ticker t_next = cb_next.get_map_struct_ticker()[str_instrument];

				Ticker t_prev_change;
				Ticker t_next_change;

				if (t_prev.last != 0.0) {
					t_prev_change.last = (t.last / t_prev.last) - 1.0;
				} else { t_prev_change.last = 0.0;}
				if (t_prev.highestbid != 0.0) {
					t_prev_change.highestbid = (t.highestbid / t_prev.highestbid) - 1.0;
				} else { t_prev_change.highestbid = 0.0; }
				if (t_prev.percentchange != 0.0) {
					t_prev_change.percentchange = (t.percentchange / t_prev.percentchange) - 1.0;
				} else { t_prev_change.percentchange = 0.0; }
				if (t_prev.lowestask != 0.0) {
					t_prev_change.lowestask = (t.lowestask / t_prev.lowestask) - 1.0;
				} else { t_prev_change.lowestask = 0.0; }

				if (t.last != 0.0) {
					t_next_change.last = (t_next.last / t.last) - 1.0;
				} else { t_next_change.last = 0.0; }
				if (t.highestbid != 0.0) {
					t_next_change.highestbid = (t_next.highestbid / t.highestbid) - 1.0;
				} else { t_next_change.highestbid = 0.0; }
				if (t.percentchange != 0.0) {
					t_next_change.percentchange = (t_next.percentchange / t.percentchange) - 1.0;
				} else { t_next_change.percentchange = 0.0; }
				if (t.lowestask != 0.0) {
					t_next_change.lowestask = (t_next.lowestask / t.lowestask) - 1.0;
				} else { t_next_change.lowestask = 0.0; }

				map_struct_ticker_next[str_instrument] = t_next_change;
				map_struct_ticker_prev[str_instrument] = t_prev_change;

				// This is how to process that information for usage in comparisons.  I've included the raw data, because
				//	some data points are useless for total analysis

				// Extract data from the candle300
				// from USDT_ETH
				//	1509254100 | 294.99999541 | 294.00000168 | 294.99999493 | 294.00000168 | 294.00000647 | 3127.14211448
				//	1509253800 | 294.99999768 | 294.00000017 | 294.99999728 | 294.00000017 | 294.19053401 | 1518.57383018
				//	skip			 avg,mm			 avg,mm			 avg,mm			 avg,mm			 avg,mm			 avg,tot,mm
				Candle_line cl300_instrument_avg;
				Candle_line cl300_instrument_tot;
				Candle_line cl300_instrument_min;
				Candle_line cl300_instrument_max;

				vector<Candle_line> vec_candle300 = cb_base.get_struct_candle300(str_instrument);
				if (vec_candle300.size() > 0) {
					for (size_t i = 0; i < vec_candle300.size(); ++i) {
						cl300_instrument_tot.val1 = cl300_instrument_tot.val1 + vec_candle300.at(i).val1;
						cl300_instrument_tot.val2 = cl300_instrument_tot.val2 + vec_candle300.at(i).val2;
						cl300_instrument_tot.val3 = cl300_instrument_tot.val3 + vec_candle300.at(i).val3;
						cl300_instrument_tot.val4 = cl300_instrument_tot.val4 + vec_candle300.at(i).val4;
						cl300_instrument_tot.val5 = cl300_instrument_tot.val5 + vec_candle300.at(i).val5;
						cl300_instrument_tot.vol = cl300_instrument_tot.vol + vec_candle300.at(i).vol;

						// min/max calc
						if (i == 0) {
							cl300_instrument_min.val1 = vec_candle300.at(i).val1;
							cl300_instrument_min.val2 = vec_candle300.at(i).val2;
							cl300_instrument_min.val3 = vec_candle300.at(i).val3;
							cl300_instrument_min.val4 = vec_candle300.at(i).val4;
							cl300_instrument_min.val5 = vec_candle300.at(i).val5;
							cl300_instrument_min.vol = vec_candle300.at(i).vol;

							cl300_instrument_max.val1 = vec_candle300.at(i).val1;
							cl300_instrument_max.val2 = vec_candle300.at(i).val2;
							cl300_instrument_max.val3 = vec_candle300.at(i).val3;
							cl300_instrument_max.val4 = vec_candle300.at(i).val4;
							cl300_instrument_max.val5 = vec_candle300.at(i).val5;
							cl300_instrument_max.vol = vec_candle300.at(i).vol;
						} else {
							if (vec_candle300.at(i).val1 < cl300_instrument_min.val1) { cl300_instrument_min.val1 = vec_candle300.at(i).val1; }
							if (vec_candle300.at(i).val2 < cl300_instrument_min.val2) { cl300_instrument_min.val2 = vec_candle300.at(i).val2; }
							if (vec_candle300.at(i).val3 < cl300_instrument_min.val3) { cl300_instrument_min.val3 = vec_candle300.at(i).val3; }
							if (vec_candle300.at(i).val4 < cl300_instrument_min.val4) { cl300_instrument_min.val4 = vec_candle300.at(i).val4; }
							if (vec_candle300.at(i).val5 < cl300_instrument_min.val5) { cl300_instrument_min.val5 = vec_candle300.at(i).val5; }
							if (vec_candle300.at(i).vol < cl300_instrument_min.vol) { cl300_instrument_min.vol = vec_candle300.at(i).vol; }

							if (vec_candle300.at(i).val1 > cl300_instrument_max.val1) { cl300_instrument_max.val1 = vec_candle300.at(i).val1; }
							if (vec_candle300.at(i).val2 > cl300_instrument_max.val2) { cl300_instrument_max.val2 = vec_candle300.at(i).val2; }
							if (vec_candle300.at(i).val3 > cl300_instrument_max.val3) { cl300_instrument_max.val3 = vec_candle300.at(i).val3; }
							if (vec_candle300.at(i).val4 > cl300_instrument_max.val4) { cl300_instrument_max.val4 = vec_candle300.at(i).val4; }
							if (vec_candle300.at(i).val5 > cl300_instrument_max.val5) { cl300_instrument_max.val5 = vec_candle300.at(i).val5; }
							if (vec_candle300.at(i).vol > cl300_instrument_max.vol) { cl300_instrument_max.vol = vec_candle300.at(i).vol; }
						}

					}
					double d_size = boost::lexical_cast<double>(vec_candle300.size());
					cl300_instrument_avg.val1 = cl300_instrument_tot.val1 / d_size;
					cl300_instrument_avg.val2 = cl300_instrument_tot.val2 / d_size;
					cl300_instrument_avg.val3 = cl300_instrument_tot.val3 / d_size;
					cl300_instrument_avg.val4 = cl300_instrument_tot.val4 / d_size;
					cl300_instrument_avg.val5 = cl300_instrument_tot.val5 / d_size;
					cl300_instrument_avg.vol = cl300_instrument_tot.vol / d_size;

					cl_prev_300_tot.vol = cl_prev_300_tot.vol + cl300_instrument_tot.vol;
				}
				map_cl_prev_300_avg.emplace(str_instrument, std::move(cl300_instrument_avg));
				map_cl_prev_300_tot.emplace(str_instrument, std::move(cl300_instrument_tot));
				map_cl_prev_300_min.emplace(str_instrument, std::move(cl300_instrument_min));
				map_cl_prev_300_max.emplace(str_instrument, std::move(cl300_instrument_max));

				// Extract data from the candle14400
				//	from USDT_BTC
				//	1509249600 | 5775.50000000 | 5702.28462698 | 5742.99999998 | 5726.00000000 | 5754.48812736 | 856007.03235443
				//	1509235200 | 5730.97601203 | 5670.00000000 | 5728.06090088 | 5719.00000000 | 5700.16508200 | 1247850.47603730
				//	skip			 avg,mm			 avg,mm				avg,mm			 avg,mm			  avg,mm				avg,tot,mm
				Candle_line cl14400_instrument_avg;
				Candle_line cl14400_instrument_tot;
				Candle_line cl14400_instrument_min;
				Candle_line cl14400_instrument_max;

				vector<Candle_line> vec_candle14400 = cb_base.get_struct_candle14400(str_instrument);
				if (vec_candle300.size() > 0) {
					for (size_t i = 0; i < vec_candle14400.size(); ++i) {
						cl14400_instrument_tot.val1 = cl14400_instrument_tot.val1 + vec_candle14400.at(i).val1;
						cl14400_instrument_tot.val2 = cl14400_instrument_tot.val2 + vec_candle14400.at(i).val2;
						cl14400_instrument_tot.val3 = cl14400_instrument_tot.val3 + vec_candle14400.at(i).val3;
						cl14400_instrument_tot.val4 = cl14400_instrument_tot.val4 + vec_candle14400.at(i).val4;
						cl14400_instrument_tot.val5 = cl14400_instrument_tot.val5 + vec_candle14400.at(i).val5;
						cl14400_instrument_tot.vol = cl14400_instrument_tot.vol + vec_candle14400.at(i).vol;

						// min/max calc
						if (i == 0) {
							cl14400_instrument_min.val1 = vec_candle14400.at(i).val1;
							cl14400_instrument_min.val2 = vec_candle14400.at(i).val2;
							cl14400_instrument_min.val3 = vec_candle14400.at(i).val3;
							cl14400_instrument_min.val4 = vec_candle14400.at(i).val4;
							cl14400_instrument_min.val5 = vec_candle14400.at(i).val5;
							cl14400_instrument_min.vol = vec_candle14400.at(i).vol;

							cl14400_instrument_max.val1 = vec_candle14400.at(i).val1;
							cl14400_instrument_max.val2 = vec_candle14400.at(i).val2;
							cl14400_instrument_max.val3 = vec_candle14400.at(i).val3;
							cl14400_instrument_max.val4 = vec_candle14400.at(i).val4;
							cl14400_instrument_max.val5 = vec_candle14400.at(i).val5;
							cl14400_instrument_max.vol = vec_candle14400.at(i).vol;
						} else {
							if (vec_candle14400.at(i).val1 < cl14400_instrument_min.val1) { cl14400_instrument_min.val1 = vec_candle14400.at(i).val1; }
							if (vec_candle14400.at(i).val2 < cl14400_instrument_min.val2) { cl14400_instrument_min.val2 = vec_candle14400.at(i).val2; }
							if (vec_candle14400.at(i).val3 < cl14400_instrument_min.val3) { cl14400_instrument_min.val3 = vec_candle14400.at(i).val3; }
							if (vec_candle14400.at(i).val4 < cl14400_instrument_min.val4) { cl14400_instrument_min.val4 = vec_candle14400.at(i).val4; }
							if (vec_candle14400.at(i).val5 < cl14400_instrument_min.val5) { cl14400_instrument_min.val5 = vec_candle14400.at(i).val5; }
							if (vec_candle14400.at(i).vol < cl14400_instrument_min.vol) { cl14400_instrument_min.vol = vec_candle14400.at(i).vol; }

							if (vec_candle14400.at(i).val1 > cl14400_instrument_max.val1) { cl14400_instrument_max.val1 = vec_candle14400.at(i).val1; }
							if (vec_candle14400.at(i).val2 > cl14400_instrument_max.val2) { cl14400_instrument_max.val2 = vec_candle14400.at(i).val2; }
							if (vec_candle14400.at(i).val3 > cl14400_instrument_max.val3) { cl14400_instrument_max.val3 = vec_candle14400.at(i).val3; }
							if (vec_candle14400.at(i).val4 > cl14400_instrument_max.val4) { cl14400_instrument_max.val4 = vec_candle14400.at(i).val4; }
							if (vec_candle14400.at(i).val5 > cl14400_instrument_max.val5) { cl14400_instrument_max.val5 = vec_candle14400.at(i).val5; }
							if (vec_candle14400.at(i).vol > cl14400_instrument_max.vol) { cl14400_instrument_max.vol = vec_candle14400.at(i).vol; }
						}
					}
					double d_size = boost::lexical_cast<double>(vec_candle14400.size());
					cl14400_instrument_avg.val1 = cl14400_instrument_tot.val1 / d_size;
					cl14400_instrument_avg.val2 = cl14400_instrument_tot.val2 / d_size;
					cl14400_instrument_avg.val3 = cl14400_instrument_tot.val3 / d_size;
					cl14400_instrument_avg.val4 = cl14400_instrument_tot.val4 / d_size;
					cl14400_instrument_avg.val5 = cl14400_instrument_tot.val5 / d_size;
					cl14400_instrument_avg.vol = cl14400_instrument_tot.vol / d_size;

					cl_prev_14400_tot.vol = cl_prev_14400_tot.vol + cl14400_instrument_tot.vol;
				}
				map_cl_prev_14400_avg.emplace(str_instrument, std::move(cl14400_instrument_avg));
				map_cl_prev_14400_tot.emplace(str_instrument, std::move(cl14400_instrument_tot));
				map_cl_prev_14400_min.emplace(str_instrument, std::move(cl14400_instrument_min));
				map_cl_prev_14400_max.emplace(str_instrument, std::move(cl14400_instrument_max));

				// Extract data from the history
				// from USDT_LTC
				//		Important to record this as avg,tot elements as both total and price*value.  
				//		So that it can be compared as a percentage between	all instruments.
				//	1509254600 | 54.31142791 | 543.11427910 |  1
				//	1509254008 | 54.31142791 | 37.46402297 | -1
				//	skip			 avg,mm			avg,tot,mm	  avg,tot
				History_line hl_instrument_avg;
				History_line hl_instrument_tot;
				History_line hl_instrument_min;
				History_line hl_instrument_max;

				vector<History_line> vec_history = cb_base.get_struct_history(str_instrument);
				if (vec_candle300.size() > 0) {
					for (size_t i = 0; i < vec_history.size(); ++i) {
						hl_instrument_tot.priceusd = hl_instrument_tot.priceusd + vec_history.at(i).priceusd;
						hl_instrument_tot.amountusd = hl_instrument_tot.amountusd + vec_history.at(i).amountusd;
						hl_instrument_tot.action = hl_instrument_tot.action + vec_history.at(i).action;

						// min/max calc
						if (i == 0) {
							hl_instrument_min.priceusd = vec_history.at(i).priceusd;
							hl_instrument_min.amountusd = vec_history.at(i).amountusd;

							hl_instrument_max.priceusd = vec_history.at(i).priceusd;
							hl_instrument_max.amountusd = vec_history.at(i).amountusd;

						} else {
							if (vec_history.at(i).priceusd < hl_instrument_min.priceusd) { hl_instrument_min.priceusd = vec_history.at(i).priceusd; }
							if (vec_history.at(i).amountusd < hl_instrument_min.amountusd) { hl_instrument_min.amountusd = vec_history.at(i).amountusd; }

							if (vec_history.at(i).priceusd > hl_instrument_max.priceusd) { hl_instrument_max.priceusd = vec_history.at(i).priceusd; }
							if (vec_history.at(i).amountusd > hl_instrument_max.amountusd) { hl_instrument_max.amountusd = vec_history.at(i).amountusd; }
						}
					}
					double d_size = boost::lexical_cast<double>(vec_history.size());
					hl_instrument_avg.priceusd = hl_instrument_tot.priceusd / d_size;
					hl_instrument_avg.amountusd = hl_instrument_tot.amountusd / d_size;
					hl_instrument_avg.action = boost::lexical_cast<int>(1000.0 * (boost::lexical_cast<double>(hl_instrument_tot.action) / d_size));

					hl_prev_history_tot.amountusd = hl_prev_history_tot.amountusd + hl_instrument_tot.amountusd;
					hl_prev_history_tot.action = hl_prev_history_tot.action + hl_instrument_tot.action;
				}
				map_hl_prev_avg.emplace(str_instrument, std::move(hl_instrument_avg));
				map_hl_prev_tot.emplace(str_instrument, std::move(hl_instrument_tot));
				map_hl_prev_min.emplace(str_instrument, std::move(hl_instrument_min));
				map_hl_prev_max.emplace(str_instrument, std::move(hl_instrument_max));


				// Extract data from the orderbook
				// from USDT_XMR
				//		Important to record this as avg,tot elements as both total and price*value.  
				//		So that it can be compared as a percentage between	all instruments.
				//	86.74652466 | 3.69240923 | 86.80009774 | 0.00311432
				//	86.56134530 | 0.01563150 | 86.87378847 | 0.05743694
				//	avg,mm		  avg,tot,mm	avg,mm		  avg,tot,mm
				Orderbook_line ol_instrument_avg;
				Orderbook_line ol_instrument_tot;
				Orderbook_line ol_instrument_min;
				Orderbook_line ol_instrument_max;

				vector<Orderbook_line> vec_orderbook = cb_base.get_struct_orderbook(str_instrument);
				if (vec_orderbook.size() > 0) {
					for (size_t i = 0; i < vec_orderbook.size(); ++i) {
						ol_instrument_tot.val1 = ol_instrument_tot.val1 + vec_orderbook.at(i).val1;
						ol_instrument_tot.val2 = ol_instrument_tot.val2 + vec_orderbook.at(i).val2;
						ol_instrument_tot.val3 = ol_instrument_tot.val3 + vec_orderbook.at(i).val3;
						ol_instrument_tot.val4 = ol_instrument_tot.val4 + vec_orderbook.at(i).val4;

						// min/max calc
						if (i == 0) {
							ol_instrument_min.val1 = vec_orderbook.at(i).val1;
							ol_instrument_min.val2 = vec_orderbook.at(i).val2;
							ol_instrument_min.val3 = vec_orderbook.at(i).val3;
							ol_instrument_min.val4 = vec_orderbook.at(i).val4;

							ol_instrument_max.val1 = vec_orderbook.at(i).val1;
							ol_instrument_max.val2 = vec_orderbook.at(i).val2;
							ol_instrument_max.val3 = vec_orderbook.at(i).val3;
							ol_instrument_max.val4 = vec_orderbook.at(i).val4;
						} else {
							if (vec_orderbook.at(i).val1 < ol_instrument_min.val1) { ol_instrument_min.val1 = vec_orderbook.at(i).val1; }
							if (vec_orderbook.at(i).val2 < ol_instrument_min.val2) { ol_instrument_min.val2 = vec_orderbook.at(i).val2; }
							if (vec_orderbook.at(i).val3 < ol_instrument_min.val3) { ol_instrument_min.val3 = vec_orderbook.at(i).val3; }
							if (vec_orderbook.at(i).val4 < ol_instrument_min.val4) { ol_instrument_min.val4 = vec_orderbook.at(i).val4; }

							if (vec_orderbook.at(i).val1 > ol_instrument_max.val1) { ol_instrument_max.val1 = vec_orderbook.at(i).val1; }
							if (vec_orderbook.at(i).val2 > ol_instrument_max.val2) { ol_instrument_max.val2 = vec_orderbook.at(i).val2; }
							if (vec_orderbook.at(i).val3 > ol_instrument_max.val3) { ol_instrument_max.val3 = vec_orderbook.at(i).val3; }
							if (vec_orderbook.at(i).val4 > ol_instrument_max.val4) { ol_instrument_max.val4 = vec_orderbook.at(i).val4; }
						}
					}
					double d_size = boost::lexical_cast<double>(vec_orderbook.size());
					ol_instrument_avg.val1 = ol_instrument_tot.val1 / d_size;
					ol_instrument_avg.val2 = ol_instrument_tot.val2 / d_size;
					ol_instrument_avg.val3 = ol_instrument_tot.val3 / d_size;
					ol_instrument_avg.val4 = ol_instrument_tot.val4 / d_size;

					ol_prev_orderbook_tot.val2 = ol_prev_orderbook_tot.val2 + ol_instrument_tot.val2;
					ol_prev_orderbook_tot.val4 = ol_prev_orderbook_tot.val4 + ol_instrument_tot.val4;

					ol_prev_orderbook_totcalc.val2 = ol_prev_orderbook_totcalc.val2 + (ol_instrument_tot.val2 * ol_instrument_avg.val1);
					ol_prev_orderbook_totcalc.val4 = ol_prev_orderbook_totcalc.val4 + (ol_instrument_tot.val4 * ol_instrument_avg.val3);
				}
				map_ol_prev_avg.emplace(str_instrument, std::move(ol_instrument_avg));
				map_ol_prev_tot.emplace(str_instrument, std::move(ol_instrument_tot));
				map_ol_prev_min.emplace(str_instrument, std::move(ol_instrument_min));
				map_ol_prev_max.emplace(str_instrument, std::move(ol_instrument_max));
			}
		}

		cur_bin2::export_bin(str_filepath_bin2);

		// Used for testting.
		//cur_bin2::import_bin(str_filepath_bin2);

		cur_bin2::export_text(str_filepath_bin2);

		b_valid = true;
	}
}


// Export in ascii format.
void cur_bin2::export_text(string str_filepath) {
	// Write out an ascii file of the performance of each instrument.
	ofstream ofs(str_filepath + ".asc");

	if (ofs.is_open()) {

		ofs << "SITUATION" << std::endl;
		if (cb_base.get_vec_situation().size() > 0) {
			for (size_t i_situation = 0; i_situation < cb_base.get_vec_situation().size(); ++i_situation) {
				ofs << "\t" << cb_base.get_vec_situation().at(i_situation).size() << "|" << cb_base.get_vec_situation().at(i_situation) << std::endl;
			}
		}

		ofs << "TOTALS" << std::endl;

		ofs << "\tCandle300" << std::endl;
		ofs << "\t\tvol\t\t\t\t:\t" << fixed << setprecision(8) << cl_prev_300_tot.vol << std::endl;

		ofs << "\tCandle14400" << std::endl;
		ofs << "\t\tvol\t\t\t\t:\t" << fixed << setprecision(8) << cl_prev_14400_tot.vol << std::endl;

		ofs << "\tHistory" << std::endl;
		ofs << "\t\tavg amountusd \t:\t" << fixed << setprecision(8) << hl_prev_history_tot.amountusd << std::endl;
		ofs << "\t\tavg action*1K \t:\t" << hl_prev_history_tot.action << std::endl;

		ofs << "\tOrderbook" << std::endl;
		ofs << "\t\tval2     \t\t:\t" << fixed << setprecision(8) << ol_prev_orderbook_tot.val2 << std::endl;
		ofs << "\t\tval4     \t\t:\t" << fixed << setprecision(8) << ol_prev_orderbook_tot.val4 << std::endl;
		ofs << "\t\tval1*val2\t\t:\t" << fixed << setprecision(8) << ol_prev_orderbook_totcalc.val2 << std::endl;
		ofs << "\t\tval3*val4\t\t:\t" << fixed << setprecision(8) << ol_prev_orderbook_totcalc.val4 << std::endl;


		ofs << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
		ofs << "TICKER" << std::endl;
		if (cb_base.get_map_struct_ticker().size() > 0) {

			for (auto const& imap : cb_base.get_map_struct_ticker()) {
				string str_instrument = imap.first;
				Ticker t = imap.second;

				Ticker t_prev_change = map_struct_ticker_prev[str_instrument];
				Ticker t_next_change = map_struct_ticker_next[str_instrument];

				ofs << "INSTRUMENT\t:\t" << str_instrument << std::endl;

				ofs << "\tTicker\t\t\t\tPrevious\t\tBase\t\t\t\tNext" << std::endl;
				
				ofs << "\tlast" << "\t\t\t\t" << std::right << std::setw(10) << fixed << setprecision(4) << t_prev_change.last
					<< "\t" << std::right << std::setw(14) << fixed << setprecision(8) << t.last
					<< "\t" << std::right << std::setw(10) << fixed << setprecision(4) << t_next_change.last << std::endl;
				ofs << "\thighestbid" << "\t\t" << std::right << std::setw(10) << fixed << setprecision(4) << t_prev_change.highestbid
					<< "\t" << std::right << std::setw(14) << fixed << setprecision(8) << t.highestbid
					<< "\t" << std::right << std::setw(10) << fixed << setprecision(4) << t_next_change.highestbid << std::endl;
				ofs << "\tlowestask" << "\t\t" << std::right << std::setw(10) << fixed << setprecision(4) << t_prev_change.lowestask
					<< "\t" << std::right << std::setw(14) << fixed << setprecision(8) << t.lowestask
					<< "\t" << std::right << std::setw(10) << fixed << setprecision(4) << t_next_change.lowestask << std::endl;
				ofs << "\tpercentchange" << "\t" << std::right << std::setw(10) << fixed << setprecision(4) << t_prev_change.percentchange
					<< "\t" << std::right << std::setw(14) << fixed << setprecision(8) << t.percentchange
					<< "\t" << std::right << std::setw(10) << fixed << setprecision(4) << t_next_change.percentchange << std::endl;
				ofs << "\thigh24hr" << "\t" << "       " 
					<< "\t\t\t\t" << std::right << std::setw(14) << fixed << setprecision(8) << t.high24hr << std::endl;
				ofs << "\tlow24hr" << "\t" << "        "
					<< "\t\t\t\t" << std::right << std::setw(14) << fixed << setprecision(8) << t.low24hr << std::endl;
				ofs << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
			}
		}

		ofs << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
		ofs << "TOTALS, AVERAGES, MIN, MAX" << std::endl;
		if (cb_base.get_map_struct_ticker().size() > 0) {

			for (auto const& imap : cb_base.get_map_struct_ticker()) {
				string str_instrument = imap.first;
				Ticker t = imap.second;
				ofs << "INSTRUMENT\t:\t" << str_instrument << std::endl;

				//	1509254100 | 294.99999541 | 294.00000168 | 294.99999493 | 294.00000168 | 294.00000647 | 3127.14211448
				//	1509253800 | 294.99999768 | 294.00000017 | 294.99999728 | 294.00000017 | 294.19053401 | 1518.57383018
				//	skip			 avg,mm			 avg,mm			 avg,mm			 avg,mm			 avg,mm			 avg,tot,mm

				ofs << "300 Candle" << std::endl;
				ofs << "\tType\t" << std::left << std::setw(18) << "\t     val1";
				ofs << "\t" << std::left << std::setw(18) << "     val2";
				ofs << "\t" << std::left << std::setw(18) << "     val3";
				ofs << "\t" << std::left << std::setw(18) << "     val4";
				ofs << "\t" << std::left << std::setw(18) << "     val5";
				ofs << "\t" << std::left << std::setw(18) << "     vol";
				ofs << std::endl;

				ofs << "\tmin\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_min[str_instrument].val1;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_min[str_instrument].val2;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_min[str_instrument].val3;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_min[str_instrument].val4;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_min[str_instrument].val5;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_min[str_instrument].vol;
				ofs << std::endl;

				ofs << "\tmax\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_max[str_instrument].val1;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_max[str_instrument].val2;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_max[str_instrument].val3;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_max[str_instrument].val4;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_max[str_instrument].val5;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_max[str_instrument].vol;
				ofs << std::endl;

				ofs << "\tavg\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_avg[str_instrument].val1;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_avg[str_instrument].val2;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_avg[str_instrument].val3;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_avg[str_instrument].val4;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_avg[str_instrument].val5;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_avg[str_instrument].vol;
				ofs << std::endl;

				ofs << "\ttot\t" << std::right << std::setw(18) << " ";
				ofs << "\t" << std::right << std::setw(18) << " ";
				ofs << "\t" << std::right << std::setw(18) << " ";
				ofs << "\t" << std::right << std::setw(18) << " ";
				ofs << "\t" << std::right << std::setw(18) << " ";
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_300_tot[str_instrument].vol;
				ofs << std::endl;


				ofs << "14400 Candle" << std::endl;
				ofs << "\tType\t" << std::left << std::setw(18) << "\t     val1";
				ofs << "\t" << std::left << std::setw(18) << "     val2";
				ofs << "\t" << std::left << std::setw(18) << "     val3";
				ofs << "\t" << std::left << std::setw(18) << "     val4";
				ofs << "\t" << std::left << std::setw(18) << "     val5";
				ofs << "\t" << std::left << std::setw(18) << "      vol";
				ofs << std::endl;

				ofs << "\tmin\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_min[str_instrument].val1;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_min[str_instrument].val2;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_min[str_instrument].val3;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_min[str_instrument].val4;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_min[str_instrument].val5;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_min[str_instrument].vol;
				ofs << std::endl;

				ofs << "\tmax\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_max[str_instrument].val1;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_max[str_instrument].val2;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_max[str_instrument].val3;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_max[str_instrument].val4;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_max[str_instrument].val5;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_max[str_instrument].vol;
				ofs << std::endl;

				ofs << "\tavg\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_avg[str_instrument].val1;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_avg[str_instrument].val2;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_avg[str_instrument].val3;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_avg[str_instrument].val4;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_avg[str_instrument].val5;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_avg[str_instrument].vol;
				ofs << std::endl;

				ofs << "\ttot\t" << std::right << std::setw(18) << " ";
				ofs << "\t" << std::right << std::setw(18) << " ";
				ofs << "\t" << std::right << std::setw(18) << " ";
				ofs << "\t" << std::right << std::setw(18) << " ";
				ofs << "\t" << std::right << std::setw(18) << " ";
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_cl_prev_14400_tot[str_instrument].vol;
				ofs << std::endl;

				// Extract data from the history
				// from USDT_LTC
				//		Important to record this as avg,tot elements as both total and price*value.  
				//		So that it can be compared as a percentage between	all instruments.
				//	1509254600 | 54.31142791 | 543.11427910 |  1
				//	1509254008 | 54.31142791 | 37.46402297 | -1
				//	skip			 avg,mm			avg,tot,mm	  avg,tot

				ofs << "History" << std::endl;
				ofs << "\tType\t" << std::left << std::setw(18) << "\t" << "priceusd";
				ofs << "\t" << std::left << std::setw(18) << "amountusd";
				ofs << "\t" << std::left << std::setw(18) << "action";
				ofs << std::endl;

				ofs << "\tmin\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_hl_prev_min[str_instrument].priceusd;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_hl_prev_min[str_instrument].amountusd;
				ofs << "\t" << std::right << std::setw(18) << " ";

				ofs << std::endl;

				ofs << "\tmax\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_hl_prev_max[str_instrument].priceusd;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_hl_prev_max[str_instrument].amountusd;
				ofs << "\t" << std::right << std::setw(18) << " ";

				ofs << std::endl;

				ofs << "\tavg\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_hl_prev_avg[str_instrument].priceusd;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_hl_prev_avg[str_instrument].amountusd;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_hl_prev_avg[str_instrument].action;

				ofs << std::endl;

				ofs << "\ttot\t" << std::right << std::setw(18) << " ";
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_hl_prev_avg[str_instrument].amountusd;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_hl_prev_avg[str_instrument].action;
				ofs << std::endl;

				// Extract data from the orderbook
				// from USDT_XMR
				//		Important to record this as avg,tot elements as both total and price*value.  
				//		So that it can be compared as a percentage between	all instruments.
				//	86.74652466 | 3.69240923 | 86.80009774 | 0.00311432
				//	86.56134530 | 0.01563150 | 86.87378847 | 0.05743694
				//	avg,mm		  avg,tot,mm	avg,mm		  avg,tot,mm

				ofs << "orderbook" << std::endl;
				ofs << "\tType\t" << std::left << std::setw(18) << "\t     val1";
				ofs << "\t" << std::left << std::setw(18) << "     val2";
				ofs << "\t" << std::left << std::setw(18) << "     val3";
				ofs << "\t" << std::left << std::setw(18) << "     val4";

				ofs << std::endl;

				ofs << "\tmin\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_ol_prev_min[str_instrument].val1;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_ol_prev_min[str_instrument].val2;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_ol_prev_min[str_instrument].val3;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_ol_prev_min[str_instrument].val4;
				ofs << std::endl;

				ofs << "\tmax\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_ol_prev_max[str_instrument].val1;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_ol_prev_max[str_instrument].val2;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_ol_prev_max[str_instrument].val3;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_ol_prev_max[str_instrument].val4;
				ofs << std::endl;

				ofs << "\tavg\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_ol_prev_avg[str_instrument].val1;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_ol_prev_avg[str_instrument].val2;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_ol_prev_avg[str_instrument].val3;
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_ol_prev_avg[str_instrument].val4;
				ofs << std::endl;

				ofs << "\ttot\t" << std::right << std::setw(18) << " ";
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_ol_prev_avg[str_instrument].val2;
				ofs << "\t" << std::right << std::setw(18) << " ";
				ofs << "\t" << std::right << std::setw(18) << fixed << setprecision(8) << map_ol_prev_avg[str_instrument].val4;
				ofs << std::endl;

				ofs << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
			}
		}
		ofs.close();
	}
}


void cur_bin2::clear() {
	cl_prev_300_tot = {};
	cl_prev_14400_tot = {};
	hl_prev_history_tot = {};
	ol_prev_orderbook_tot = {};
	ol_prev_orderbook_totcalc = {};

	map_struct_ticker_next.clear();
	map_struct_ticker_prev.clear();

	map_cl_prev_300_avg.clear();
	map_cl_prev_300_tot.clear();
	map_cl_prev_300_min.clear();
	map_cl_prev_300_max.clear();

	map_cl_prev_14400_avg.clear();
	map_cl_prev_14400_tot.clear();
	map_cl_prev_14400_min.clear();
	map_cl_prev_14400_max.clear();

	map_hl_prev_avg.clear();
	map_hl_prev_tot.clear();
	map_hl_prev_min.clear();
	map_hl_prev_max.clear();

	map_ol_prev_avg.clear();
	map_ol_prev_tot.clear();
	map_ol_prev_min.clear();
	map_ol_prev_max.clear();
}


// Export in BIN2 format.
void cur_bin2::export_bin(string str_filepath) {
	// Write out an ascii file of the performance of each instrument.
	fstream fs;
	fs.open(str_filepath + ".bin2", ios::out | ios::binary);

	if (fs.is_open()) {
		// Write out the totals data
		fs.write((char*)&cl_prev_300_tot, sizeof(Candle_line));
		fs.write((char*)&cl_prev_14400_tot, sizeof(Candle_line));
		fs.write((char*)&hl_prev_history_tot, sizeof(History_line));
		fs.write((char*)&ol_prev_orderbook_tot, sizeof(Orderbook_line));
		fs.write((char*)&ol_prev_orderbook_totcalc, sizeof(Orderbook_line));

		// Gonna cheat with this one.  These BIN2 files are discardable, and they really DO need
		// to be aligned with the BIN files.  So I'm not going to transcribe collection sizes,
		// because the maps are fixed length.
		if (cb_base.get_map_struct_ticker().size() > 0) {

			// loop through the instruments.
			for (auto const& imap : cb_base.get_map_struct_ticker()) {
				string str_instrument = imap.first;

				fs.write((char*)&map_struct_ticker_next[str_instrument], sizeof(Ticker));
				fs.write((char*)&map_struct_ticker_prev[str_instrument], sizeof(Ticker));

				fs.write((char*)&map_cl_prev_300_avg[str_instrument], sizeof(Candle_line));
				fs.write((char*)&map_cl_prev_300_tot[str_instrument], sizeof(Candle_line));
				fs.write((char*)&map_cl_prev_300_min[str_instrument], sizeof(Candle_line));
				fs.write((char*)&map_cl_prev_300_max[str_instrument], sizeof(Candle_line));

				fs.write((char*)&map_cl_prev_14400_avg[str_instrument], sizeof(Candle_line));
				fs.write((char*)&map_cl_prev_14400_tot[str_instrument], sizeof(Candle_line));
				fs.write((char*)&map_cl_prev_14400_min[str_instrument], sizeof(Candle_line));
				fs.write((char*)&map_cl_prev_14400_max[str_instrument], sizeof(Candle_line));

				fs.write((char*)&map_hl_prev_avg[str_instrument], sizeof(History_line));
				fs.write((char*)&map_hl_prev_tot[str_instrument], sizeof(History_line));
				fs.write((char*)&map_hl_prev_min[str_instrument], sizeof(History_line));
				fs.write((char*)&map_hl_prev_max[str_instrument], sizeof(History_line));

				fs.write((char*)&map_ol_prev_avg[str_instrument], sizeof(Orderbook_line));
				fs.write((char*)&map_ol_prev_tot[str_instrument], sizeof(Orderbook_line));
				fs.write((char*)&map_ol_prev_min[str_instrument], sizeof(Orderbook_line));
				fs.write((char*)&map_ol_prev_max[str_instrument], sizeof(Orderbook_line));
			}
		}
		fs.close();
	}
}


// Import BIN2 format.
void cur_bin2::import_bin(string str_filepath) {
	fstream fs;
	fs.open(str_filepath + ".bin2", ios::in | ios::binary);

	if (fs.is_open()) {
		cur_bin2::clear();

		fs.read((char*)&cl_prev_300_tot, sizeof(Candle_line));
		fs.read((char*)&cl_prev_14400_tot, sizeof(Candle_line));
		fs.read((char*)&hl_prev_history_tot, sizeof(History_line));
		fs.read((char*)&ol_prev_orderbook_tot, sizeof(Orderbook_line));
		fs.read((char*)&ol_prev_orderbook_totcalc, sizeof(Orderbook_line));

		// Gonna cheat with this one.  These BIN2 files are discardable, and they really DO need
		// to be aligned with the BIN files.  So I'm not going to transcribe collection sizes,
		// because the maps are fixed length.
		if (cb_base.get_map_struct_ticker().size() > 0) {
			// loop through the instruments.
			for (auto const& imap : cb_base.get_map_struct_ticker()) {
				string str_instrument = imap.first;

				Ticker t_next;
				Ticker t_prev;
				fs.read((char*)&t_next, sizeof(Ticker));
				fs.read((char*)&t_prev, sizeof(Ticker));
				map_struct_ticker_next.emplace(str_instrument, std::move(t_next));
				map_struct_ticker_prev.emplace(str_instrument, std::move(t_prev));

				Candle_line cl_300_avg;
				Candle_line cl_300_tot;
				Candle_line cl_300_min;
				Candle_line cl_300_max;
				fs.read((char*)&cl_300_avg, sizeof(Candle_line));
				fs.read((char*)&cl_300_tot, sizeof(Candle_line));
				fs.read((char*)&cl_300_min, sizeof(Candle_line));
				fs.read((char*)&cl_300_max, sizeof(Candle_line));
				map_cl_prev_300_avg.emplace(str_instrument, std::move(cl_300_avg));
				map_cl_prev_300_tot.emplace(str_instrument, std::move(cl_300_tot));
				map_cl_prev_300_min.emplace(str_instrument, std::move(cl_300_min));
				map_cl_prev_300_max.emplace(str_instrument, std::move(cl_300_max));

				Candle_line cl_14400_avg;
				Candle_line cl_14400_tot;
				Candle_line cl_14400_min;
				Candle_line cl_14400_max;
				fs.read((char*)&cl_14400_avg, sizeof(Candle_line));
				fs.read((char*)&cl_14400_tot, sizeof(Candle_line));
				fs.read((char*)&cl_14400_min, sizeof(Candle_line));
				fs.read((char*)&cl_14400_max, sizeof(Candle_line));
				map_cl_prev_14400_avg.emplace(str_instrument, std::move(cl_14400_avg));
				map_cl_prev_14400_tot.emplace(str_instrument, std::move(cl_14400_tot));
				map_cl_prev_14400_min.emplace(str_instrument, std::move(cl_14400_min));
				map_cl_prev_14400_max.emplace(str_instrument, std::move(cl_14400_max));

				History_line hl_avg;
				History_line hl_tot;
				History_line hl_min;
				History_line hl_max;
				fs.read((char*)&hl_avg, sizeof(History_line));
				fs.read((char*)&hl_tot, sizeof(History_line));
				fs.read((char*)&hl_min, sizeof(History_line));
				fs.read((char*)&hl_max, sizeof(History_line));
				map_hl_prev_avg.emplace(str_instrument, std::move(hl_avg));
				map_hl_prev_tot.emplace(str_instrument, std::move(hl_tot));
				map_hl_prev_min.emplace(str_instrument, std::move(hl_min));
				map_hl_prev_max.emplace(str_instrument, std::move(hl_max));

				Orderbook_line ol_avg;
				Orderbook_line ol_tot;
				Orderbook_line ol_min;
				Orderbook_line ol_max;
				fs.read((char*)&ol_avg, sizeof(Orderbook_line));
				fs.read((char*)&ol_tot, sizeof(Orderbook_line));
				fs.read((char*)&ol_min, sizeof(Orderbook_line));
				fs.read((char*)&ol_max, sizeof(Orderbook_line));
				map_ol_prev_avg.emplace(str_instrument, std::move(ol_avg));
				map_ol_prev_tot.emplace(str_instrument, std::move(ol_tot));
				map_ol_prev_min.emplace(str_instrument, std::move(ol_min));
				map_ol_prev_max.emplace(str_instrument, std::move(ol_max));
			}
		}
		fs.close();
	}
}