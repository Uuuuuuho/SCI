#pragma once

#ifndef RAYLEIGH_H
#define RAYLEIGH_H

//#include "Mapper.h"
#include "Complex_new.h"
#include "Equations.h"
#include <vector>
#include <math.h>


typedef enum mod { BPSK = 1, QPSK = 2, PSK8 = 3, QAM16 = 4, QAM64 = 5, IQQAM = 6, INP_4PAM = 7, QUAD_4PAM = 8, MOD_SUPER_16QAM = 9 } MOD;
typedef enum fading { No, Rayl, Rayl_Quasi, STBC, STBC_Quasi, QOSTBC, CIOD, CIOD_Quasi, JTC } FADING;

using namespace std;


class Rayleigh
{
public:
	Rayleigh();
	Complex<double> m_hGaus;
	void Rayleigh_Fading(vector<Complex<double>> data, vector<Complex<double>>&RayFading, vector<Complex<double>> &ndata, vector<double>&h_gain);
	void slow_Rayleigh_Fading(vector<Complex<double>> data, vector<Complex<double>>&RayFading, vector<Complex<double>> &ndata, vector<double>&h_gain);
	void Coherent(vector<Complex<double>> RayFading, vector<Complex<double>>&ndata, vector<double>h_gain);
	void Quasi_Coherent(vector<Complex<double>> RayFading, vector<Complex<double>>&ndata, vector<double>h_gain);

	void channel_estimator(vector<Complex<double>>rx_signal,vector<Complex<double>>tx_siganl, vector<Complex<double>> &h,vector<double>&hgain,int size);
	void STBC_channel_estimator(vector<Complex<double>>rx_signal, vector<Complex<double>>tx_signal, vector<Complex<double>> &h, int size);
	double Gaussian(double mu, double var);
	void STBC_Fading(vector<Complex<double>> &RayFading, vector<Complex<double>> data, vector<double> &h_gain, vector<Complex<double>> &ndata);
	void JTC_STBC_Fading(vector<Complex<double>> RayFading, vector<Complex<double>> data, vector<double> &h_gain, vector<Complex<double>> &ndata);
	void STBC_Coherent(vector<Complex<double>> RayFading, vector<Complex<double>>&ndata, vector<double>h_gain);
	void STBC_Quasi(vector<Complex<double>> &RayFading, vector<Complex<double>> data, vector<double> &h_gain, vector<Complex<double>> &ndata);

	void SSD_Coherent(vector<Complex<double>> RayFading, vector<Complex<double>>&ndata, vector<double>h_gain);
	void CIOD_Rotate( vector<Complex<double>> &data);
	void CIOD_Interleave(vector<Complex<double>> &data);
	void CIOD_RayFading(vector<Complex<double>> RayFading, vector<Complex<double>> data, vector<double> &h_gain, vector<Complex<double>> &ndata);
	void CIOD_Coherent(vector<Complex<double>> RayFading, vector<Complex<double>>&ndata, vector<double>h_gain);
	void CIOD_Derotate(vector<Complex<double>> &ndata);
	void CIOD_ML(vector <double> h_sd, vector<Complex<double>> &ndata);

	
	~Rayleigh();
};


#endif // !RAYLEIGH_H