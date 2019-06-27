#include "Rayleigh.h"

Rayleigh::Rayleigh()
{
}

void Rayleigh::Rayleigh_Fading(vector<Complex<double>> data, vector<Complex<double>>& RayFading, vector<Complex<double>>&ndata, vector<double>&h_gain)
{
	int m = data.size();
	RayFading.resize(m);
	ndata.resize(m);
	h_gain.resize(m);

	for (int i = 0; i < m; i++) {
		RayFading[i] = Complex<double>(Gaussian(0, 0.5), Gaussian(0, 0.5));
		ndata[i] = data[i] * RayFading[i];
		h_gain[i] = (RayFading[i].re*RayFading[i].re) + (RayFading[i].im*RayFading[i].im);

	}
}

void Rayleigh::slow_Rayleigh_Fading(vector<Complex<double>> data, vector<Complex<double>>& RayFading, vector<Complex<double>>&ndata, vector<double>&h_gain)
{
	int m = data.size();
	
	ndata.resize(m);
	RayFading.resize(1);
	h_gain.resize(1);

	Complex<double> slowfactor = Complex<double>(Gaussian(0, 0.5), Gaussian(0, 0.5));
	h_gain[0] = (slowfactor.re*slowfactor.re) + (slowfactor.im*slowfactor.im);
	RayFading[0] = slowfactor;

	for (int i = 0; i < m; i++) {
		
		ndata[i] = data[i] * slowfactor;

	}

}

void Rayleigh::Coherent(vector<Complex<double>> RayFading, vector<Complex<double>>&ndata, vector<double>h_gain)
{
	unsigned int m = ndata.size();
	for (int i = 0; i < m; i++) {
		ndata[i] = RayFading[i].conjugate() * ndata[i];
		ndata[i].re = ndata[i].re / h_gain[i];
		ndata[i].im = ndata[i].im / h_gain[i];

	}
}

void Rayleigh::Quasi_Coherent(vector<Complex<double>> RayFading, vector<Complex<double>>& ndata, vector<double> h_gain)
{
	int m = ndata.size();
	Complex<double> conj = RayFading[0].conjugate();

	for (int i = 0; i < m; i++) {
		ndata[i] =  conj * ndata[i];
		ndata[i] = ndata[i] / h_gain[0];
	}

}


void Rayleigh::SSD_Coherent(vector<Complex<double>> RayFading, vector<Complex<double>>&ndata, vector<double>h_gain)
{
	unsigned int m = ndata.size();
	for (int i = 0; i < m; i++) {
		ndata[i] = RayFading[i].conjugate() * ndata[i];
		ndata[i].re = (ndata[i].re / sqrt(h_gain[i]));
		ndata[i].im = (ndata[i].im / sqrt(h_gain[i]));

	}
}



double Rayleigh::Gaussian(double mu, double var)
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

void Rayleigh::channel_estimator(vector<Complex<double>>rx_signal, vector<Complex<double>>tx_signal, vector<Complex<double>> &h, vector<double>&hgain, int size)
{
	h.resize(size);
	hgain.resize(size);
	double scale;
	for (int i = 0; i < size; ++i)
	{
		h[i] = rx_signal[i] * (tx_signal[i].conjugate());
		scale = masq(tx_signal[i].re, tx_signal[i].im);
		h[i].re = h[i].re / scale;
		h[i].im = h[i].im / scale;

		hgain[i] = masq(h[i].re, h[i].im);

	}
}

void Rayleigh::STBC_channel_estimator(vector<Complex<double>>rx_signal, vector<Complex<double>>tx_signal, vector<Complex<double>> &h, int size)
{
	h.resize(size);
	double scale;
	for (int i = 0; i < size; ++i)
	{
		h[i] = rx_signal[i] * (tx_signal[i].conjugate());
		scale = masq(tx_signal[i].re, tx_signal[i].im);
		h[i].re = h[i].re / scale;
		h[i].im = h[i].im / scale;


	}

}




void Rayleigh::JTC_STBC_Fading(vector<Complex<double>> RayFading, vector<Complex<double>> data, vector<double> &h_gain, vector<Complex<double>> &ndata)
{
	int m = data.size();
	ndata.resize(m);
	h_gain.resize(m);
	for (int i = 0; i < m / 2; i++) {

		h_gain[2 * i] = h_gain[2 * i + 1] = masq(RayFading[2 * i].re, RayFading[2 * i].im) + masq(RayFading[2 * i + 1].re, RayFading[2 * i + 1].im);


		ndata[2 * i] = data[2 * i] * RayFading[2 * i]
			+ data[2 * i + 1] * RayFading[2 * i + 1];
		ndata[2 * i + 1] = RayFading[2 * i + 1].conjugate() * data[2 * i]
			- RayFading[2 * i].conjugate() * data[2 * i + 1];
	}
}


void Rayleigh::STBC_Coherent(vector<Complex<double>> RayFading, vector<Complex<double>>&ndata, vector<double> h_gain)
{
	vector<Complex<double>> tmp;
	tmp = ndata;
	int m = ndata.size() / 2;
	for (int i = 0; i < m; i++) { //i+=2로 수정...
		ndata[2 * i] = RayFading[2 * i].conjugate() * tmp[2 * i]
			+ RayFading[2 * i + 1] * tmp[2 * i + 1];
		ndata[2 * i + 1] = RayFading[2 * i + 1].conjugate() * tmp[2 * i]
			- RayFading[2 * i] * tmp[2 * i + 1];

		ndata[2 * i].re = ndata[2 * i].re / h_gain[2 * i];
		ndata[2 * i].im = ndata[2 * i].im / h_gain[2 * i];

		ndata[2 * i + 1].re = ndata[2 * i + 1].re / h_gain[2 * i + 1];
		ndata[2 * i + 1].im = ndata[2 * i + 1].im / h_gain[2 * i + 1];
	}
}


void Rayleigh::STBC_Fading(vector<Complex<double>> &RayFading, vector<Complex<double>> data, vector<double> &h_gain, vector<Complex<double>> &ndata)
{
	int m = data.size(), n = m / 2;
	RayFading.resize(m);
	ndata.resize(m);
	h_gain.resize(m);
	for (int i = 0; i < n; i++) { //fast Rayleigh fading
		RayFading[2 * i] = Complex<double>(Gaussian(0, 0.5), Gaussian(0, 0.5));
		RayFading[2 * i + 1] = Complex<double>(Gaussian(0, 0.5), Gaussian(0, 0.5));
		h_gain[2 * i] = h_gain[2 * i + 1] = masq(RayFading[2 * i].re, RayFading[2 * i].im) + masq(RayFading[2 * i + 1].re, RayFading[2 * i + 1].im);

		ndata[2 * i] = data[2 * i] * RayFading[2 * i]
			+ data[2 * i + 1] * RayFading[2 * i + 1];
		ndata[2 * i + 1] = RayFading[2 * i + 1].conjugate() * data[2 * i]
			- RayFading[2 * i].conjugate() * data[2 * i + 1];
	}
}


void Rayleigh::STBC_Quasi(vector<Complex<double>> &RayFading, vector<Complex<double>> data, vector<double> &h_gain, vector<Complex<double>> &ndata)
{
	int m = data.size(), n = m / 2;
	RayFading.resize(m);
	ndata.resize(m);
	h_gain.resize(m);
	Complex<double>tmp1, tmp2;
	tmp1 = Complex<double>(Gaussian(0, 0.5), Gaussian(0, 0.5));
	tmp2 = Complex<double>(Gaussian(0, 0.5), Gaussian(0, 0.5));

	for (int i = 0; i < n; i++) { //fast Rayleigh fading
		RayFading[2 * i] = tmp1;
		RayFading[2 * i + 1] = tmp2;
		h_gain[2 * i] = h_gain[2 * i + 1] = masq(RayFading[2 * i].re, RayFading[2 * i].im) + masq(RayFading[2 * i + 1].re, RayFading[2 * i + 1].im);

		ndata[2 * i] = data[2 * i] * RayFading[2 * i]
			+ data[2 * i + 1] * RayFading[2 * i + 1];
		ndata[2 * i + 1] = RayFading[2 * i + 1].conjugate() * data[2 * i]
			- RayFading[2 * i].conjugate() * data[2 * i + 1];
	}
}


void Rayleigh::CIOD_Rotate(vector<Complex<double>> &data) {
	//should angle1 be 10, angle2 be 41.7195.. take angle var in case of other options
	int m = data.size();

	double angle1 = 31.7175, angle2 = 31.7175;

	double c = cos(2 * PI*angle1 / 180), s = sin(2 * PI*angle2 / 180);
	Complex<double> tmp1(c, s);
	for (int i = 0; i < m; i++)
		data[i] = tmp1 * data[i];

}

void Rayleigh::CIOD_Interleave(vector<Complex<double>> &data) {
	vector<Complex<double>> tmp = data;
	int m = data.size();
	m = m - (m % 4);
	for (int i = 0; i < m; i += 4) {
		data[i].im = tmp[i + 2].im;
		data[i + 1].im = tmp[i + 3].im;
		data[i + 2].im = tmp[i].im;
		data[i + 3].im = tmp[i + 1].im;
	}
}

void Rayleigh::CIOD_Derotate(vector<Complex<double>> &ndata) {
	int m = ndata.size();
	double angle1 = 31.7175, angle2 = 31.7175;
	double c = cos(2 * PI*angle1 / 180), s = -sin(2 * PI*angle2 / 180);
	Complex<double> tmp1(c, s);
	for (int i = 0; i < m; i++) {
		ndata[i] = tmp1 * ndata[i];
	}
}

void Rayleigh::CIOD_RayFading(vector<Complex<double>> RayFading, vector<Complex<double>> data, vector<double> &h_gain, vector<Complex<double>> &ndata) {
	int m = data.size();

	RayFading.resize(m), ndata.resize(m), h_gain.resize(m / 2);
	int n = m / 4;
	for (int i = 0; i < n; i++) {
		h_gain[2 * i] = masq(RayFading[4 * i].re, RayFading[4 * i].im) + masq(RayFading[4 * i + 1].re, RayFading[4 * i + 1].im);
		h_gain[2 * i + 1] = masq(RayFading[4 * i + 2].re, RayFading[4 * i + 2].im) + masq(RayFading[4 * i + 3].re, RayFading[4 * i + 3].im);



		ndata[4 * i] = RayFading[4 * i] * data[4 * i] + RayFading[4 * i + 1] * data[4 * i + 1];
		ndata[4 * i + 1] = RayFading[4 * i + 1].conjugate() * data[4 * i] - RayFading[4 * i].conjugate() * data[4 * i + 1];
		ndata[4 * i + 2] = RayFading[4 * i + 2] * data[4 * i + 2] + RayFading[4 * i + 3] * data[4 * i + 3];
		ndata[4 * i + 3] = RayFading[4 * i + 3].conjugate() * data[4 * i + 2] - RayFading[4 * i + 2].conjugate() * data[4 * i + 3];
	}
}



void Rayleigh::CIOD_Coherent(vector<Complex<double>> RayFading, vector<Complex<double>>&ndata, vector<double>h_gain)
{
	vector<Complex<double>> tmp;
	tmp = ndata;
	int m = ndata.size() / 4;
	for (int i = 0; i < m; i++) {
		ndata[4 * i] = RayFading[4 * i].conjugate() * tmp[4 * i] + RayFading[4 * i + 1] * tmp[4 * i + 1];
		ndata[4 * i + 1] = RayFading[4 * i + 1].conjugate() * tmp[4 * i] - RayFading[4 * i] * tmp[4 * i + 1];
		ndata[4 * i + 2] = RayFading[4 * i + 2].conjugate() * tmp[4 * i + 2] + RayFading[4 * i + 3] * tmp[4 * i + 3];
		ndata[4 * i + 3] = RayFading[4 * i + 3].conjugate() * tmp[4 * i + 2] - RayFading[4 * i + 2] * tmp[4 * i + 3];

		//try to make this simple

		ndata[4 * i].re = ndata[4 * i].re / h_gain[2 * i];
		ndata[4 * i].im = ndata[4 * i].im / h_gain[2 * i];
		ndata[4 * i + 1].re = ndata[4 * i + 1].re / h_gain[2 * i];
		ndata[4 * i + 1].im = ndata[4 * i + 1].im / h_gain[2 * i];
		ndata[4 * i + 2].re = ndata[4 * i + 2].re / h_gain[2 * i + 1];
		ndata[4 * i + 2].im = ndata[4 * i + 2].im / h_gain[2 * i + 1];
		ndata[4 * i + 3].re = ndata[4 * i + 3].re / h_gain[2 * i + 1];
		ndata[4 * i + 3].im = ndata[4 * i + 3].im / h_gain[2 * i + 1];
	}
}



void Rayleigh::CIOD_ML(vector <double> h_sd, vector<Complex<double>> &ndata) {  //only for QPSK
	int sz = h_sd.size();
	vector<Complex<double>> tmp(2 * sz), tmp2(4);
	vector<Complex<double>> Table{ Complex<double>(-1,-1),Complex<double>(-1,1),Complex<double>(1,1),Complex<double>(1,-1) };
	vector<double> ed(4);
	int index = 0;
	int m = sz / 2;
	for (int i = 0; i < m; i++) {
		h_sd[2 * i] = -h_sd[2 * i];
		tmp[4 * i] = Complex<double>(Table[0].re*(h_sd[2 * i]), Table[0].im*h_sd[2 * i + 1]);
		tmp[4 * i + 1] = Complex<double>(Table[1].re*(h_sd[2 * i]), Table[1].im*h_sd[2 * i + 1]);
		tmp[4 * i + 2] = Complex<double>(Table[2].re*(h_sd[2 * i]), Table[2].im*h_sd[2 * i + 1]);
		tmp[4 * i + 3] = Complex<double>(Table[3].re*(h_sd[2 * i]), Table[3].im*h_sd[2 * i + 1]);
	}

	int n = ndata.size() / 4;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 4; j++) {
			tmp2[0] = ndata[4 * i + j] + tmp[4 * i];
			tmp2[1] = ndata[4 * i + j] + tmp[4 * i + 1];
			tmp2[2] = ndata[4 * i + j] + tmp[4 * i + 2];
			tmp2[3] = ndata[4 * i + j] + tmp[4 * i + 3];
			ed[0] = masq(tmp2[0].re, tmp2[0].im);
			ed[1] = masq(tmp2[1].re, tmp2[1].im);
			ed[2] = masq(tmp2[2].re, tmp2[2].im);
			ed[3] = masq(tmp2[3].re, tmp2[3].im);
			ed[0] > ed[1] ? index = 1 : index;
			ed[index] > ed[2] ? index = 2 : index;
			ed[index] > ed[3] ? index = 3 : index;
			ndata[4 * i + j] = Table[index];
			index = 0;
		}
	}

}






Rayleigh::~Rayleigh()
{
}
