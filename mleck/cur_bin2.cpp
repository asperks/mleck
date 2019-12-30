/* refer cur_bin2.h for details.


*/

#include "cur_bin2.h"

cur_bin2::cur_bin2() {

}

void cur_bin2::init(string str_path_bin, int id_cur, string str_filepath_bin2, cur_bin cb_in) {
	b_valid = false;
	cb_base = cb_in;

	// Find if there is an existing BIN2 file.  If not, create it.  If it
	// exists, load it.
	
	//https://stackoverflow.com/questions/50960492/creating-folders-in-c/56262869#56262869
	if (std::experimental::filesystem::exists(str_filepath_bin2)) {
		// Load the BIN2 file.
		import_bin(str_filepath_bin2);

	} else {
		// We have already validated that there is a preceding and succeeding bin
		//	file for this element during the is_valid_id test in the cur_bin_handler

		string str_filepath_bin_prev = str_path_bin + "\\" + std::to_string(id_cur - 1) + ".bin";
		string str_filepath_bin_next = str_path_bin + "\\" + std::to_string(id_cur + 1) + ".bin";

		cur_bin cb_prev;
		cur_bin cb_next;

		cb_prev.import_bin(str_filepath_bin_prev);
		cb_next.import_bin(str_filepath_bin_next);

		if (cb_base.get_map_struct_ticker().size() > 0) {
			//for (it = cb_base.get_map_struct_ticker().begin(); it != cb_base.get_map_struct_ticker().end(); it++) {
			for (auto it = cb_base.get_map_struct_ticker().begin(); it != cb_base.get_map_struct_ticker().end(); ++it) {

				string str_instrument = it->first;
				vector<double> vec_dbl;

				double d_val = 0.0;

				// Extract data from the candle300
				vector<Candle_line> vec_candle300 = cb_base.get_struct_candle300(str_instrument);

				// Extract data from the candle14400
				vector<Candle_line> vec_candle14400 = cb_base.get_struct_candle14400(str_instrument);


				// Extract data from the history


				// Extract data from the orderbook

			}
		}


	

		

		cur_bin2::export_bin(str_filepath_bin2);
	}

}


// Export in BIN2 format.
void cur_bin2::export_bin(string str_filepath) {
	// Write out an ascii file of the performance of each instrument.




}

// Import BIN2 format.
void cur_bin2::import_bin(string str_filepath) {

}