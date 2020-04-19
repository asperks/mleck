/* refer jewel_c_rand.h for details.


*/

#include "jewel_c_rand.h"

string jewel_c_rand::create(int id_mleck_in) {
	jewel::b_valid = false;
	string str_id = "";

	jewel::jt = J_RAND;


	jewel::str_id = str_id;
	return str_id;
}


void jewel_c_rand::calc_return() {

	jewel::vec_calc_return.clear();

	for (int i = 0; i < jewel::ptr_cbh->get_cur_bin_open().get_map_ticker_count(); ++i) {
		// This steps through all of the valid instruments of this currency set.
		double d_calc = 0.0;

		 

		// push back the calc calue of this instrument to the parent class return vector.
		vec_calc_return.push_back(d_calc);
	}

}

