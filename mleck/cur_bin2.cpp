/* refer cur_bin2.h for details.


*/

#include "cur_bin2.h"

cur_bin2::cur_bin2() {

}

void cur_bin2::init(int cur_id, string str_filepath_bin2, cur_bin cb_in) {
	b_valid = false;
	cb_base = cb_in;

	// Find if there is an existing BIN2 file.  If not, create it.  If it
	// exists, load it.
	// We have already validated that there is a preceding and succeeding bin
	//	file for this element during the is_valid_id test in the cur_bin_handler


}