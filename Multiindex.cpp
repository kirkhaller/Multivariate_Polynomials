//
// Created by Kirk Haller on 6/25/20.
//

#include "Multiindex.h"
#include <cassert>
#include <math.h>

Multi_index::Multi_index() {

}

std::string Multi_index::description() const {
    // Needs to be implemented
    return "";
}

int Multi_index::get_value(const int index) const {
    assert(index > 0 && index < dimension());
    return 0; // Needs to be implemented
}


