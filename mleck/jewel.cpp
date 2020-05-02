/* refer jewel.h for details.


*/

#include "jewel.h"


void jewel::init(cur_bin_handler* ptr_cbh_in) {
	b_valid = false;

	set_cbh(ptr_cbh_in);

	b_valid = true;
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

