/* refer cur_bin_handler.h for details.


*/

#include "jewel_handler.h"


void jewel_handler::init(cur_bin_handler * ptr_cbh_in) {
	b_valid = false;

	std::cout << "jewel_handler.init \t:" << "\n";

	set_cbh(ptr_cbh_in);

	map_up_jewel.clear();

	std::cout << "jewel_handler.init \t:END" << "\n";
}


string jewel_handler::get_jewel_id(int i_new_threshold) {
	bool b_new = true;
	string str_jewel_id = "";

	if (map_up_jewel.size() < i_new_threshold) {
		b_new = true;
	} else {
		int i_rand = static_cast<int>(rand() % (static_cast<int>(map_up_jewel.size()) + 2));

		//int i_rand = boost::lexical_cast<int>(randMToN(0.0, static_cast<double>(map_up_jewel.size()) + 2.0));

		if (i_rand >= map_up_jewel.size()) {
			b_new = true;
		} else {
			// Select a random jewel from the jewel map.
			//https://stackoverflow.com/questions/15425442/retrieve-random-key-element-for-stdmap-in-c/15425535#15425535
			auto it = map_up_jewel.begin();
			std::advance(it, rand() % map_up_jewel.size());
			str_jewel_id = it->first;
		}

	}

	if (b_new == true) {

		Jewel_type jt = static_cast<Jewel_type>(rand() % (static_cast<int>(Jewel_type::MAX)));

		//Jewel_type jt_max{ Jewel_type::MAX };
		//size_t i_jt = boost::lexical_cast<size_t>(randMToN(0.0, static_cast<double>(jt_max)));
		//Jewel_type jt = static_cast<Jewel_type>(i_jt);

		if (jt == Jewel_type::J_RAND) {
			auto up_jc = std::make_unique<jewel_c_rand>(cbh_ptr);
			up_jc->create();
			str_jewel_id = up_jc->to_string();
			map_up_jewel.emplace(str_jewel_id, std::move(up_jc));

		} else if (jt == Jewel_type::J_DIFF) {
			auto up_jc = std::make_unique<jewel_c_diff>(cbh_ptr);
			up_jc->create();
			str_jewel_id = up_jc->to_string();
			map_up_jewel.emplace(str_jewel_id, std::move(up_jc));

		} else {
			//populate with the random type if the type is not found.
			// This shouldn't happen.
			auto up_jc = std::make_unique<jewel_c_rand>(cbh_ptr);
			up_jc->create();
			str_jewel_id = up_jc->to_string();
			map_up_jewel.emplace(str_jewel_id, std::move(up_jc));

		}

		
	}
	



	return str_jewel_id;
}


std::unique_ptr<jewel>* jewel_handler::jewel_link(string str_jewel_id) {
	if (map_up_jewel.count(str_jewel_id) > 0) {
		// It found the jewel in the map
		auto it = map_up_jewel.find(str_jewel_id);
		if (it != map_up_jewel.end()) {
			return std::move(&(it->second));
		}

	} else {
		bool b_found = false;
		// It DIDN'T find the jewel in the map
		auto j = jewel(cbh_ptr);
		j.set_id(str_jewel_id);
		Jewel_type jt_test = j.get_jewel_type();
		j = NULL;

		if (jt_test == Jewel_type::J_RAND) {
			//unique_ptr<jewel_c_rand> up_jc(static_cast<jewel_c_rand*>(up_j.release()));
			auto up_jc = std::make_unique<jewel_c_rand>(cbh_ptr);
			up_jc->set_id(str_jewel_id);
			up_jc->load();
			map_up_jewel.emplace(str_jewel_id, std::move(up_jc));

			auto it = map_up_jewel.find(str_jewel_id);
			if (it != map_up_jewel.end()) {
				return std::move(&(it->second));
			}

		} else if (jt_test == Jewel_type::J_DIFF) {
			//unique_ptr<jewel_c_diff> up_jc(static_cast<jewel_c_diff*>(up_j.release()));
			auto up_jc = std::make_unique<jewel_c_diff>(cbh_ptr);
			up_jc->set_id(str_jewel_id);
			up_jc->load();
			map_up_jewel.emplace(str_jewel_id, std::move(up_jc));

			auto it = map_up_jewel.find(str_jewel_id);
			if (it != map_up_jewel.end()) {
				return std::move(&(it->second));
			}





		//} else if (up_j->get_jewel_type() == Jewel_type::J_JEWEL) {
		//	// not complete yet.  

		//} else if (up_j->get_jewel_type() == Jewel_type::J_RATIO) {
		//	// not complete yet.  populate with the random type 


		//} else if (up_j->get_jewel_type() == Jewel_type::J_VOL) {


		//} else {
		//	//populate with the random type if the type is not found.
		//	auto up_jc = std::make_unique<jewel_c_rand>(cbh_ptr);
		//	up_jc->create();
		//	map_up_jewel.emplace(str_jewel_id, std::move(up_jc));
		//	//ptr_return = &map_up_jewel.at(str_jewel_id);

		}

	}

	return NULL;
}