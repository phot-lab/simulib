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

int main() {
    VectorXd v= GenVector(1,1,10);
    cout << v << endl;
    cout << v << endl;
    cout << nbThreads() << endl;
    return 0;
}
