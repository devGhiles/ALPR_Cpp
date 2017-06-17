//
// Created by Nabil on 11/05/2017.
//

#include <sstream>
#include "strings.h"


string to_string(int i){
    std::ostringstream ss;
    ss << i;
    return ss.str();
}