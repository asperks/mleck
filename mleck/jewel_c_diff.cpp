/* refer jewel_c_diff.h for details.


*/

#include "jewel_c_diff.h"

string jewel_c_diff::create() {
	jewel::b_valid = false;
	string str_id = "";

	jewel::jt = Jewel_type::J_DIFF;

	str_id = "J_DIFF";

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

	int64_t i_instrument_left = static_cast<int64_t>(randZeroToOne() * d_instrument_range) - (d_instrument_range / 3.0) -1;
	int64_t i_instrument_right = static_cast<int64_t>(randZeroToOne() * d_instrument_range) - (d_instrument_range / 3.0) -1;
	if (i_instrument_left < 0) { i_instrument_left = -1; }
	if (i_instrument_right < 0) { i_instrument_right = -1; }
	str_id = str_id + "!instrument";
	str_id = str_id + "`" + std::to_string(i_instrument_left);
	str_id = str_id + "`" + std::to_string(i_instrument_right);

	/*
	//  The details of the elements to selected are in :
	//							jewel_relations_pt_diff_leftright.txt
cur_bin
	single struct per instrument
		map <string, Ticker> map_struct_ticker;
0,0,0				percentchange

	vector per instrument
		map <string, vector<History_line>> map_vec_history;
1,0,0,%			action


cur_bin2
   		map <string, History_line> map_hl_prev_avg;
2,0,0				action
			map <string, History_line> map_hl_prev_mid;
2,1,0				action
			map <string, History_line> map_hl_prev_tot;
2,2,0				action

	*/

	int64_t i_opt_left1 = static_cast<int64_t>(randMToN(0.0, 3.0));
	int64_t i_opt_left2 = 0;
	if (i_opt_left1 >= 2) {	i_opt_left2 = static_cast<int>(randMToN(0.0, 3.0));	}
	int i_opt_left3 = 0;
	double d_opt_left1 = sqrt(sqrt(randMToN(0.0, 16.0))) - 1.0;
	if (d_opt_left1 < 0.0) { d_opt_left1 = 0.0; }
	if (d_opt_left1 > 1.0) { d_opt_left1 = 1.0; }
	str_id = str_id + "!opt_left";
	str_id = str_id + "`" + std::to_string(i_opt_left1);
	str_id = str_id + "`" + std::to_string(i_opt_left2);
	str_id = str_id + "`" + std::to_string(i_opt_left3);
	str_id = str_id + "`" + std::to_string(d_opt_left1);

	int64_t i_opt_right1 = static_cast<int64_t>(randMToN(0.0, 3.0));
	int64_t i_opt_right2 = 0;
	if (i_opt_right1 >= 2) {	i_opt_right2 = static_cast<int>(randMToN(0.0, 3.0));}
	int64_t i_opt_right3 = 0;
	double d_opt_right1 = sqrt(sqrt(randMToN(0.0, 16.0))) - 1.0;
	if (d_opt_right1 < 0.0) {d_opt_right1 = 0.0; }
	if (d_opt_right1 > 1.0) { d_opt_right1 = 1.0; }
	str_id = str_id + "!opt_right";
	str_id = str_id + "`" + std::to_string(i_opt_right1);
	str_id = str_id + "`" + std::to_string(i_opt_right2);
	str_id = str_id + "`" + std::to_string(i_opt_right3);
	str_id = str_id + "`" + std::to_string(d_opt_right1);


	vec_opt.push_back(tuple<int, int, int, double>(i_opt_left1, i_opt_left2, i_opt_left3, d_opt_left1));
	vec_opt.push_back(tuple<int, int, int, double>(i_opt_right1, i_opt_right2, i_opt_right3, d_opt_right1));


	jewel::str_id = str_id;
	return str_id;
}


void jewel_c_diff::load() {
	vector<string> vec_line;
	boost::split(vec_line, jewel::str_id, boost::is_any_of("!"));

	bool b_found_param1 = false;
	bool b_found_instrument = false;
	bool b_found_left = false;
	bool b_found_right = false;

	if (vec_line.size() >= 3) {
		if (vec_line.at(0).compare("J_DIFF") == 0) {
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

					if (vec_param.at(0).compare("instrument") == 0) {
						jewel::vec_param1.clear();
						if (vec_param.size() >= 3) {
							b_found_instrument = true;
							vec_instrument_source_nth.push_back(std::stoi(vec_param.at(1)));
							vec_instrument_source_nth.push_back(std::stoi(vec_param.at(2)));
						}
					}

					if (vec_param.at(0).compare("left") == 0) {
						jewel::vec_param1.clear();
						if (vec_param.size() >= 5) {
							b_found_left = true;
							int i_opt1 = std::stoi(vec_param.at(1));
							int i_opt2 = std::stoi(vec_param.at(2));
							int i_opt3 = std::stoi(vec_param.at(3));
							double d_opt1 = std::stod(vec_param.at(4));
							vec_opt.push_back(tuple<int, int, int, double>(i_opt1, i_opt2, i_opt3, d_opt1));
						}
					}

					if (vec_param.at(0).compare("right") == 0) {
						jewel::vec_param1.clear();
						if (vec_param.size() >= 5) {
							b_found_right = true;
							int i_opt1 = std::stoi(vec_param.at(1));
							int i_opt2 = std::stoi(vec_param.at(2));
							int i_opt3 = std::stoi(vec_param.at(3));
							double d_opt1 = std::stod(vec_param.at(4));
							vec_opt.push_back(tuple<int, int, int, double>(i_opt1, i_opt2, i_opt3, d_opt1));
						}
					}

					if (vec_param.at(0).compare("right") == 0) {
						jewel::vec_param1.clear();
					}
				}
			}
		}

		if ((b_found_param1 == false)
			|| (b_found_instrument == false)
			|| (b_found_left == false)
			|| (b_found_right == false)
			) {
			// if a the parameter count is incorrect, re-create the jewel..
			// This shouldn't really happen.
			create();
		}
	}
}



void jewel_c_diff::calc_return() {

	jewel::vec_calc_return.clear();

	for (int i = 0; i < jewel::ptr_cbh->get_cur_bin_open().get_map_ticker_count(); ++i) {
		// This steps through all of the valid instruments of this currency set.
		double d_calc = 0.0;

		int64_t nth_instrument_left = vec_instrument_source_nth.at(0);
		if (nth_instrument_left >= 0) {
			nth_instrument_left = ptr_cbh->get_cur_bin_open().get_map_ticker_count() % vec_instrument_source_nth.at(0);
		}
			
		int64_t nth_instrument_right = vec_instrument_source_nth.at(1);
		if (nth_instrument_right >= 0) {
			nth_instrument_right = ptr_cbh->get_cur_bin_open().get_map_ticker_count() % vec_instrument_source_nth.at(1);
		}

			




		if (d_calc < jewel::vec_param1.at(1)) { d_calc = jewel::vec_param1.at(1); }
		if (d_calc > jewel::vec_param1.at(2)) { d_calc = jewel::vec_param1.at(2); }

		// If that parameter is very low, it means the stochastic element will be very low.
		// This means that ALL decisions have a stochastic element applied, and if it is not
		// desired, it will be bred out.
		d_calc = d_calc * (randZeroToOne() * jewel::vec_param1.at(0));

		// push back the calc calue of this instrument to the parent class return vector.
		vec_calc_return.push_back(d_calc);
	}

}

