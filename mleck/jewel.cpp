/* refer jewel.h for details.


*/

#include "jewel.h"


void jewel::init() {
	b_valid = false;




	b_valid = true;
}



void jewel::set_id(string str_id_in) { 
	str_id = str_id_in;
	b_valid = false;

	vector<string> vec_line;
	boost::split(vec_line, jewel::str_id, boost::is_any_of("!"));

	if (vec_line.at(0).compare("J_RAND") == 0) {
		jt = Jewel_type::J_RAND;
		b_valid = true;
	} else if (vec_line.at(0).compare("J_DIFF") == 0) {
		jt = Jewel_type::J_DIFF;
		b_valid = true;
	}

}

// This is implemented in child classes.
//void jewel::calc_return() {
//
//	jewel::vec_calc_return.clear();
//
//	for (int i = 0; i < jewel::ptr_cbh->get_cur_bin_open().get_map_ticker_count(); ++i) {
//		// This steps through all of the valid instruments of this currency set.
//		double d_calc = 0.0;
//
//		 
//
//		// push back the calc calue of this instrument to the parent class return vector.
//		vec_calc_return.push_back(d_calc);
//	}
//
//}

