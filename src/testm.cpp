#include "testm.h"
#include <vector>


void test_modify(double& e, std::vector<double>& v){
    e += 1.0;
    v[0] += 1.0;
}