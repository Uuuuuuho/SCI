#pragma once
#ifndef COMBINING_H
#define COMBINING_H

#include "complex_new.h"
#include "Rayleigh.h"
#include <vector>
using namespace std;

class Comb {
public:
	Comb();
	~Comb();
	void test();
	void EGC(FADING FAD_MOD, vector<Complex<double>>& rx_source, vector<double>& h_sd, vector<Complex<double>> RD_rx, vector<double> h_rd);
	void MRC(FADING FAD_MOD, vector<Complex<double>> &rx_source, vector<double> &h_sd, vector<Complex<double>> RD_rx, vector<double> h_rd);
	void COOP_MRC(FADING FAD_MOD, double SNR_SD, vector<Complex<double>> &rx_source, vector<double> &h_sd,
		double SNR_RD, vector<Complex<double>> RD_rx, vector<double> h_rd);
	void COOP_EGC(FADING FAD_MOD, double SNR_SD, vector<Complex<double>>& rx_source, vector<double>& h_sd,
		double SNR_RD, vector<Complex<double>> RD_rx, vector<double> h_rd);
	void SUPER_MRC(FADING FAD_MOD, double SNR_SD, vector<Complex<double>> &rx_source, vector<double> &h_sd,
		double SNR_RD, vector<Complex<double>> RD_rx, vector<double> h_rd);
	void SUPER_EGC(FADING FAD_MOD, double SNR_SD, vector<Complex<double>>& rx_source, vector<double>& h_sd,
		double SNR_RD, vector<Complex<double>> RD_rx, vector<double> h_rd);
	void BLOCK_MRC(FADING FAD_MOD, double SNR_SD, vector<Complex<double>> &rx_source, vector<double> &h_sd,
		double SNR_RD, vector<Complex<double>> RD_rx, vector<double> h_rd);
	void LLR_MRC(FADING FAD_MOD, double SNR_SD, double* llr0, double* llr1, vector<double> &h_sd,
		double SNR_RD, double* SUPER_llr0, double* SUPER_llr1, vector<double> &h_rd, int SIZE);
	void LLR_COMB(FADING FAD_MOD, double SNR_SD, vector<double> &h_sd, vector<vector<double>> LLR_FIRST, 
		double SNR_RD, vector<double> h_rd, vector<vector<double>> &LLR_SECOND);
	void LLR_COMB2(FADING FAD_MOD, double SNR_SD, vector<double> &h_sd, double *llr0, double *llr1,
		double SNR_RD, vector<double> h_rd, double *HARQ_llr0, double *HARQ_llr1, int size);
	void LLR_COMB3(double** LLR1, double** LLR2, double** HARQ_LLR1, double** HARQ_LLR2, int col, int row);
	void LLR_COMB4(double LC_Comb, double rate1, double rate2, double *llr0, double *llr1, double *HARQ_llr0, double *HARQ_llr1, int size);
	double Rate(double SNR, double RD_Gain, vector<double> h_sd, vector<double> h_rd, double LC_SD, double LC_RD, double &rate1, double &rate2);

    void Picking_EVEN(vector<vector<double>> &LLR_SECOND, vector<vector<double>> &LLR_THIRD);
    void Picking_FIRSTPAIR(vector<vector<double>> &LLR_SECOND, vector<vector<double>> &LLR_THIRD);
    void Picking_SECONDPAIR(vector<vector<double>> &LLR_SECOND, vector<vector<double>> &LLR_THIRD);
};

#endif
