/* refer jewel_c_rand.h for details.


*/

#include "jewel_c_rand.h"

string jewel_c_rand::create() {
	jewel::b_valid = false;
	string str_id = "";

	jewel::jt = Jewel_type::J_RAND;

	str_id = "J_RAND";

	double d_rand_stochastic = randZeroToOne();
	double d_rand_min = (randZeroToOne() * 20.0) - 10.0;
	double d_rand_max = (randZeroToOne() * 20.0) - 10.0;
	
	if (d_rand_min > d_rand_max) { d_rand_min = d_rand_max; }
	if (d_rand_max < d_rand_min) { d_rand_max = d_rand_min; }

	jewel::vec_param1.push_back(d_rand_stochastic);
	jewel::vec_param1.push_back(d_rand_min);
	jewel::vec_param1.push_back(d_rand_max);

	str_id = str_id + "!param1";
	str_id = str_id + "`" + std::to_string(d_rand_stochastic);
	str_id = str_id + "`" + std::to_string(d_rand_min);
	str_id = str_id + "`" + std::to_string(d_rand_max);

	jewel::str_id = str_id;
	return str_id;
}


void jewel_c_rand::load() {
	vector<string> vec_line;
	boost::split(vec_line, jewel::str_id, boost::is_any_of("!"));
	bool b_found_param1 = false;

	if (vec_line.size() >= 2) {
		if (vec_line.at(0).compare("J_RAND") == 0) {
			vector<string> vec_param;
			boost::split(vec_param, vec_line.at(1), boost::is_any_of("`"));
			if (vec_param.size() >= 1) {
				if (vec_param.at(0).compare("param1") == 0) {
					jewel::vec_param1.clear();
					if (vec_param.size() >= 4) {
						b_found_param1 = true;
						jewel::vec_param1.push_back(std::stod(vec_param.at(1)));
						jewel::vec_param1.push_back(std::stod(vec_param.at(2)));
						jewel::vec_param1.push_back(std::stod(vec_param.at(3)));
					}
				}
			}
		}
	}

	if (b_found_param1 == false) {
		create();
	}
}



void jewel_c_rand::calc_return() {

	jewel::vec_calc_return.clear();

	for (int i = 0; i < jewel::ptr_cbh->get_cur_bin_open().get_map_ticker_count(); ++i) {
		// This steps through all of the valid instruments of this currency set.
		// https://stackoverflow.com/questions/1340729/how-do-you-generate-a-random-double-uniformly-distributed-between-0-and-1-from-c/26853142#26853142
		double d_calc = (randZeroToOne() * (d_rand_max * 2.0)) - d_rand_max;

		if (d_calc > jewel::vec_param1.at(2)) { d_calc = jewel::vec_param1.at(2); }
		if (d_calc < jewel::vec_param1.at(1)) { d_calc = jewel::vec_param1.at(1); }

		// If that parameter is very low, it means the stochastic element will be very low.
		// This means that ALL decisions have a stochastic element applied, and if it is not
		// desired, it will be bred out.
		d_calc = d_calc * (randZeroToOne() * jewel::vec_param1.at(0));

		//	PT_RAND		:	random double between -10 and 10
		// push back the calc calue of this instrument to the parent class return vector.
		vec_calc_return.push_back(d_calc);
	}

}

