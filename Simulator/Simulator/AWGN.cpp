#include "AWGN.h"

AWGN::AWGN()
{
}

void AWGN::Awgn(double SNR, double Eb, vector<Complex<double>> &ndata)
{
	int m = ndata.size();

	sigma2 = 0.5* Eb  * pow(10, (-0.1*SNR));	//calcualte

	for (int i = 0; i < m; i++) {
		ndata[i] = Complex<double>(Gaussian(0, sigma2), Gaussian(0, sigma2)) + ndata[i];
	}
}


vector<vector<Complex<double>>> AWGN::HAwgnC(double SNR, double Eb, int m, int n)
{
	vector<vector<Complex<double>>> result;
	double sigma2;

	sigma2 = Eb * 0.5 * pow(10, (-SNR) / 10);	//calcualte
	sigma2 = sigma2 * 7 / 4;
	result.resize(m);

	for (int i = 0; i < m; i++) {
		result[i].resize(n);
		for (int j = 0; j < n; j++) {
			result[i][j] = Complex<double>(Gaussian(0, sigma2), Gaussian(0, sigma2));
		}
	}
	return result;
}

double AWGN::Gaussian(double mu, double var)
{
	double U1, W, mult;
	static double U2;
	static double X1, X2;
	static int call = 0;
	double sigma = sqrt(var);

	if (call == 1)
	{
		call = !call;
		return (mu + sigma * (double)X2);
	}

	do
	{
		U1 = -1 + ((double)rand() / RAND_MAX) * 2;	//-1~1사이의 U1
		U2 = -1 + ((double)rand() / RAND_MAX) * 2;	//-1~1사이의 U2
		W = pow(U1, 2) + pow(U2, 2);
	} while (W >= 1 || W == 0);	//call이 0이면 0~1사이의 W 생성

	mult = sqrt((-2 * log(W)) / W);
	X1 = U1 * mult;
	X2 = U2 * mult;

	call = !call;

	return (mu + sigma * (double)X1);
}


AWGN::~AWGN()
{
}
