#pragma once



#ifndef AWGN_H
#define AWGN_H


#include "Complex_new.h"
#include <math.h>
#include <random>
#include <iostream>
#include <vector>

using namespace std;
class AWGN
{
public:
	AWGN();
	void Awgn(double SNR, double Eb, vector<Complex<double>>& ndata);
	vector<vector<Complex<double>>> HAwgnC(double SNR, double Eb, int m, int n);
	double Gaussian(double mu, double var);
	double sigma2;
	vector<vector<Complex<double>>> noise;
	~AWGN();
};


#endif // !AWGN_H