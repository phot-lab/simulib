#ifndef OPTICALAB_FIBER_TYPES_H
#define OPTICALAB_FIBER_TYPES_H

#include <complex>
#include <vector>
#include "Eigen/Core"

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

struct Out{
    double time;
    double first_dz;
    unsigned long ncycle;
};

struct E {
    double lambda;
    VectorXcd field;
};

struct Fiber {
    double length;           // fiber length [m]
    double lambda;           // lambda: wavelength [nm] of fiber parameters
    double attenuation;      // alphadB: fiber attenuation [dB/km]
    double dispersion;       // fiber chromatic dispersion coefficient [ps/nm/km]
    double slope;            // fiber slope [ps/nm^2/km], i.e. derivative of X.disp
    double nonlinear_index;  // n2: fiber nonlinear index [m^2/W]
    double effective_area;   // aeff: fiber effective area [um^2] of the fundamental mode
    double pmdpar;           // Polarization-mode dispersion (PMD) parameter [ps/sqrt(km)] in strong-coupling mode.
    double beat_length;      // Beat length [m]
    double nplates;          // Number of waveplates when birefringence is on
    double dzmax;
    double chlambda;
    double dphi_max;
    double alpha_lin;
    double bandwidth;
    double gam;
    bool is_unique;
    bool is_kerr;
    bool is_dual;
    bool is_manakov;
    bool is_cle;
    bool is_sym;
    bool trace;
    bool dphi_fwm;  // FWM criterion for the first step setup.

    string coupling;
    string step_update;
    string step_type;

    Fiber() {
        is_unique   = false;
        length      = 0;
        beat_length = 20;
        nplates     = 100;
        trace       = false;
        dphi_fwm    = true;
    }
};


#endif  // OPTICALAB_FIBER_TYPES_H
