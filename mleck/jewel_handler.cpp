/* refer cur_bin_handler.h for details.


*/

#include "jewel_handler.h"


void jewel_handler::init(cur_bin_handler * ptr_cbh_in) {
	b_valid = false;

	set_cbh(ptr_cbh_in);



}


std::unique_ptr<jewel>* jewel_handler::jewel_link(string str_jewel_id) {
	std::unique_ptr<jewel>* ptr_return = nullptr;
	if (map_up_jewel.count(str_jewel_id) > 0) {
		// It found the jewel in the map
		ptr_return = & map_up_jewel.at(str_jewel_id);
	} else {
		bool b_found = false;
		// It DIDN'T find the jewel in the map
		auto up_j = std::make_unique<jewel>(ptr_cbh);
		up_j->set_id(str_jewel_id);

		if (up_j->get_jewel_type() == Jewel_type::J_RAND) {
			auto up_jc = std::make_unique<jewel_c_rand>(ptr_cbh);
			up_jc->load();
			map_up_jewel.emplace(str_jewel_id, std::move(up_jc));
			ptr_return = &map_up_jewel.at(str_jewel_id);

		} else if (up_j->get_jewel_type() == Jewel_type::J_DIFF) {
			auto up_jc = std::make_unique<jewel_c_diff>(ptr_cbh);
			up_jc->load();
			map_up_jewel.emplace(str_jewel_id, std::move(up_jc));
			ptr_return = &map_up_jewel.at(str_jewel_id);

		//} else if (up_j->get_jewel_type() == Jewel_type::J_JEWEL) {
		//	// not complete yet.  

		//} else if (up_j->get_jewel_type() == Jewel_type::J_RATIO) {
		//	// not complete yet.  populate with the random type 


		//} else if (up_j->get_jewel_type() == Jewel_type::J_VOL) {


		} else {
			//populate with the random type if the type is not found.
			auto up_jc = std::make_unique<jewel_c_rand>(ptr_cbh);
			up_jc->load();
			map_up_jewel.emplace(str_jewel_id, std::move(up_jc));
			ptr_return = &map_up_jewel.at(str_jewel_id);

		}

	}

	return ptr_return;
}