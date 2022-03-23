#include "MatlabDataArray.hpp"
#include "MatlabEngine.hpp"
#include <fstream>
#include <SimuLib>

// Reference
// https://www.mathworks.com/help/matlab/matlab_external/pass-variables-from-matlab-to-c.html

using namespace matlab::engine;
using namespace std;
using namespace Eigen;

int main() {

    // Start MATLAB engine synchronously
    std::unique_ptr<MATLABEngine> matlabPtr = startMATLAB();

    // Evaluate MATLAB statement
    matlabPtr->eval(u"cd /Users/lichunyu/Documents/MATLAB/OptiluX/Examples");
    matlabPtr->eval(u"ex01_inigstate");
    matlabPtr->eval(u"result = ex17_opt_fiber_GVD();");

    // Get the result from MATLAB
    matlab::data::Array result = matlabPtr->getVariable(u"result");
    auto elements              = matlab::data::getReadOnlyElements<complex<double>>(result);

    ofstream outfile;
    outfile.open("../field.txt");
    for (const auto &item: elements) {
        outfile << item << endl;
    }
    outfile.close();

    return 0;
}
