//
// Created by Kirk Haller on 7/27/20.
//

#include "kbnsum.h"
#include <math.h>


KBNSum::KBNSum(double value_in) : sum(value_in), error_sum(0.0) {}

void KBNSum::add(double value_in) {
    double new_sum = sum + value_in;
    if (abs(sum) > abs(value_in)) {
        error_sum += ((sum - new_sum) + value_in);
    } else {
        error_sum += ((value_in - new_sum) + sum);
    }
    sum = new_sum;
}

