%%Electrc Amplifier
% data = input E
% Gain_EA： 增益倍数 
% powerW: 输入信号功率
% one_sided_spectral_density：单边功率谱密度
%%
function [E,Gain] = Electric_Amplifier(data,Gain_EA,powerW,one_sided_spectral_density)

powerW_After_EA = powerW*10^(Gain_EA/10);
E = data.*sqrt(powerW_After_EA);
SNR_GS = 10*log10((powerW)/(one_sided_spectral_density/2));
E = awgn(E,SNR_GS,'measured');
Gain = Gain_EA;
%%