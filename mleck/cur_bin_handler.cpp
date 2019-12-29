/* refer cur_bin_handler.h for details.


*/


#include "cur_bin_handler.h"

cur_bin_handler::cur_bin_handler(string str_path_bin_in) {
	init(str_path_bin_in);
	}

void cur_bin_handler::init(string str_path_bin_in) {
	b_valid = false;
	str_path_bin = str_path_bin_in;

	vec_i_bin.clear();

	// Read the name of all of the bin files in the bin folder, and index them in a vector.





}