#ifndef OPTICALAB_FIBER_TYPES_H
#define OPTICALAB_FIBER_TYPES_H

#include <complex>
#include <vector>

using namespace std;

struct Linear {
    bool is_scalar;
    bool is_unique;
};

struct VectorLinear : Linear {
    vector<complex<double>[2][2]> matin;
    double db[][2];
};

// Scalar Linear effect
struct ScalarLinear : Linear {
    double matin;
    double db;
};

struct E {
    double lambda;
    VectorXcd field;
};

/*
%       X.length:
%       X.alphadB:    fiber attenuation [dB/km]
%       X.aeff:       fiber effective area [um^2] of the fundamental mode
%       X.n2:         fiber nonlinear index [m^2/W]
%       X.lambda:     wavelength [nm] of fiber parameters
%       X.disp:       fiber chromatic dispersion coefficient [ps/nm/km]
%                     @ X.lambda.
%       X.slope:      fiber slope [ps/nm^2/km], i.e. derivative of X.disp
%                     @ X.lambda.
 */
struct Fiber {
    double length; // fiber length [m]
    double lambda;
    double attenuation;
    double dispersion;
    double slope;
    double nonlinear_index;
    double effective_area;
    double pmdpar;  // Polarization-mode dispersion (PMD) parameter [ps/sqrt(km)] in strong-coupling mode.
    double beat_length; // Beat length [m]
    double nplates; // Number of waveplates when birefringence is on
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
    bool dphi_fwm; // FWM criterion for the first step setup.

    string coupling;
    string step_update;
    string step_type;

    Fiber() {
        length = 0;
        beat_length = 20;
        nplates = 100;
        trace = false;
        dphi_fwm = true;
    }
};


#endif //OPTICALAB_FIBER_TYPES_H
