#include <iostream>
#include <simulib>

using namespace Eigen;
using namespace std;

int main() {
    VectorXd v = GenStepVector(1, 1, 10);
    cout << v << endl;
    return 0;
}