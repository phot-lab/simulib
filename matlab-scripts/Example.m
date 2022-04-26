%   ex17: Optical propagation within an optical fiber impaired by only
%   group-velocity dispersion (GVD).

function example()

clear
clc

%% Global parameters

Nsymb = 1024;           % number of symbols
Nt = 32;                % number of discrete points per symbol

%% Tx parameters
symbrate = 10;          % symbol rate [Gbaud].
tx.rolloff = 0.2;       % pulse roll-off
tx.emph = 'asin';       % digital-premphasis type
modfor = 'qpsk';         % modulation format
PdBm = 0;               % power [dBm]
lam = 1550;             % carrier wavelength [nm]

%% Channel parameters

% Transmission fiber
ft.length     = 10E3;       % length [m]
ft.lambda     = 1550;       % wavelength [nm] of fiber parameters
ft.alphadB    = 0;          % attenuation [dB/km]
ft.disp       = 17;         % dispersion [ps/nm/km] @ ft.lambda
ft.slope      = 0;          % slope [ps/nm^2/km] @ ft.lambda
ft.n2         = 0;          % nonlinear index [m^2/W]
ft.aeff       = 80;         % effective area [um^2]

%% Rx parameters
rx.modformat = modfor;      % modulation format
rx.sync.type = 'da';        % time-recovery method
rx.oftype = 'gauss';        % optical filter type
rx.obw = Inf;               % optical filter bandwidth normalized to symbrate
rx.eftype = 'rootrc';       % optical filter type
rx.ebw = 0.5;               % electrical filter bandwidth normalized to symbrate
rx.epar = tx.rolloff;       % electrical filter extra parameters

%% Init
Nsamp = Nsymb*Nt;           % overall number of samples
fs = symbrate*Nt;           % sampling rate [GHz]
inigstate(Nsamp,fs);        % initialize global variables: Nsamp and fs.

%% Tx side

Plin = 10.^(PdBm/10);   % [mW]
E = lasersource(Plin,lam,struct('pol','single'));  % y-pol does not exist

rng(1);
[patx, patbinx] = pattern2(Nsymb,'rand',struct('format',modfor));

[sigx, normx] = digitalmod(patx,modfor,symbrate,'rootrc',tx);

E   = mzmodulator(E, sigx);

%% Channel

E = fiber(E,ft);

rsig = rxfrontend(E,lam,symbrate,rx);  % front-end

eyeop = evaleye(patx,angle(rsig),symbrate,modfor,struct('plot',true));
