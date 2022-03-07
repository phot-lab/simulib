#include "Dense"
#include "fiber_tools.h"
#include "fiber_types.h"
#include "globals.h"
#include "tools.h"
#include <complex>
#include <cstring>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

using namespace std;
using namespace Eigen;

struct Test {
    int a;
    double b;
};

std::tuple<double, char, std::string> get_student(int id) {
    if (id == 0) return std::make_tuple(3.8, 'A', "Lisa Simpson");
    if (id == 1) return std::make_tuple(2.9, 'C', "Milhouse Van Houten");
    if (id == 2) return std::make_tuple(1.7, 'D', "Ralph Wiggum");
    throw std::invalid_argument("id");
}

int main() {

    return 0;
}
