#ifndef OPTICALAB_FIBER_TYPES_H
#define OPTICALAB_FIBER_TYPES_H

#include "Eigen/Core"
#include <complex>
#include <vector>
#include "LaserSource.h"

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

struct Fiber {
    double length         = 10000;  // fiber length [m]
    double lambda         = 1550;   // lambda: wavelength [nm] of fiber parameters
    double attenuation    = 0;      // alphadB: fiber attenuation [dB/km]
    double dispersion     = 17;     // fiber chromatic dispersion coefficient [ps/nm/km]
    double slope          = 0;      // fiber slope [ps/nm^2/km], i.e. derivative of X.disp
    double nonlinearIndex = 0;      // n2: fiber nonlinear index [m^2/W]
    double effectiveArea  = 80;     // aeff: fiber effective area [um^2] of the fundamental mode
    double pmdpar         = 0;      // Polarization-mode dispersion (PMD) parameter [ps/sqrt(km)] in strong-coupling mode.
    double beatLength     = 20;     // Beat length [m]
    double nplates        = 100;    // Number of waveplates when birefringence is on
    double dzmax          = length;
    double chlambda       = 0;
    double dphiMax        = 20;
    double alphaLinear    = 0;
    double bandwidth      = 0;
    double gam            = 0;
    bool isUnique         = true;
    bool is_kerr          = false;
    bool is_dual          = false;
    bool is_manakov       = false;
    bool is_cle           = true;
    bool is_sym           = false;
    bool trace            = false;
    bool dphi_fwm         = true;  // FWM criterion for the first step setup.
    string coupling       = "none";
    string step_update    = "cle";
    string step_type;
};

tuple<Out, E> fiberTransmit(E &e, Fiber fiber);


#endif  // OPTICALAB_FIBER_TYPES_H
