#ifndef OPTICALAB_FIBER_TYPES_H
#define OPTICALAB_FIBER_TYPES_H

#include "Eigen/Core"
#include <complex>
#include <vector>

using namespace std;
using namespace Eigen;

struct Linear {
    bool is_scalar;
    bool is_unique;
};

struct NonScalarLinear : Linear {
    vector<MatrixXcd> matin;
    MatrixXd db;
};

struct ScalarLinear : Linear {
    double matin;
    double db;
};

struct Out {
    double time;
    double first_dz;
    unsigned long ncycle;
};

struct E {
    double lambda = 1550; // lambda: wavelength [nm] of wave
    VectorXcd field; // time samples of the electric field along rows
};

struct Fiber {
    double length          = 10000;  // fiber length [m]
    double lambda          = 1550;  // lambda: wavelength [nm] of fiber parameters
    double attenuation     = 0;     // alphadB: fiber attenuation [dB/km]
    double dispersion      = 17;    // fiber chromatic dispersion coefficient [ps/nm/km]
    double slope           = 0;     // fiber slope [ps/nm^2/km], i.e. derivative of X.disp
    double nonlinear_index = 0;     // n2: fiber nonlinear index [m^2/W]
    double effective_area  = 80;    // aeff: fiber effective area [um^2] of the fundamental mode
    double pmdpar          = 0;     // Polarization-mode dispersion (PMD) parameter [ps/sqrt(km)] in strong-coupling mode.
    double beat_length     = 20;    // Beat length [m]
    double nplates         = 100;   // Number of waveplates when birefringence is on
    double dzmax           = length;
    double chlambda        = 0;
    double dphi_max        = 20;
    double alpha_lin       = 0;
    double bandwidth       = 0;
    double gam             = 0;
    bool is_unique         = true;
    bool is_kerr           = false;
    bool is_dual           = false;
    bool is_manakov        = false;
    bool is_cle            = true;
    bool is_sym            = false;
    bool trace             = false;
    bool dphi_fwm          = true;  // FWM criterion for the first step setup.
    string coupling        = "none";
    string step_update     = "cle";
    string step_type;
};


#endif  // OPTICALAB_FIBER_TYPES_H
