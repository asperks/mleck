/*
A class that links cur_bin objects.  A single cur_bin object is a single point of instrument
data.  The cur_bin_link object will load one, and link it to its surrounding cur_bin objects
so that the performance between elemenets can be recorded.  It will then write out this
performance into a bin2 file so that it can be used quickly in future.


*/

#pragma once

#ifndef CUR_BIN_LINK_H
#define CUR_BIN_LINK_H

using namespace std;

#include "cur_bin.h"

#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <boost/algorithm/string.hpp>

class cur_bin_link {
private:

public:

};



#endif