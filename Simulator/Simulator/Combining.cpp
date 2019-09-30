#include "Combining.h"

//Combining class linker error occurs

Comb::Comb()
{
}

Comb::~Comb()
{
}

void Comb::test()
{
}

void Comb::EGC(FADING FAD_MOD, vector<Complex<double>>& rx_source, vector<double>& h_sd, vector<Complex<double>> RD_rx, vector<double> h_rd)
{
	//combining over block fading channel
	vector<double> rate1, rate2;
	int pkt_size = rx_source.size();

	switch (FAD_MOD) {
	case Rayl:
		rate1.resize(pkt_size), rate2.resize(pkt_size);

		for (int i = 0; i < pkt_size; i++) {
			rate1[i] = h_sd[i] / (h_sd[i] + h_rd[i]);
			rate2[i] = h_rd[i] / (h_sd[i] + h_rd[i]);

			h_sd[i] = rate1[i] * h_sd[i] + rate2[i] * h_rd[i];
			rx_source[i] = rx_source[i] * rate1[i] + RD_rx[i] * rate2[i];
		}
		break;

	case Rayl_Quasi:
		rate1.resize(1), rate2.resize(1);
		rate1[0] = 0.5;
		rate2[0] = 0.5;
		h_sd[0] = rate1[0] * h_sd[0] + rate2[0] * h_rd[0];
		for (int i = 0; i < pkt_size; i++) {

			rx_source[i] = rx_source[i] * rate1[0] + RD_rx[i] * rate2[0];
		}
		break;

	}
}

void Comb::MRC(FADING FAD_MOD, vector<Complex<double>>& rx_source, vector<double>& h_sd, vector<Complex<double>> RD_rx, vector<double> h_rd)
{
	//combining over block fading channel
	vector<double> rate1, rate2;
	int pkt_size = rx_source.size();

	switch (FAD_MOD) {
	case Rayl:
		rate1.resize(pkt_size), rate2.resize(pkt_size);

		for (int i = 0; i < pkt_size; i++) {
			rate1[i] = h_sd[i] / (h_sd[i] + h_rd[i]);
			rate2[i] = h_rd[i] / (h_sd[i] + h_rd[i]);

			h_sd[i] = rate1[i] * h_sd[i] + rate2[i] * h_rd[i];
			rx_source[i] = rx_source[i] * rate1[i] + RD_rx[i] * rate2[i];
		}
		break;

	case Rayl_Quasi:
		rate1.resize(1), rate2.resize(1);
		rate1[0] = h_sd[0] / (h_sd[0] + h_rd[0]);
		rate2[0] = h_rd[0] / (h_sd[0] + h_rd[0]);
		h_sd[0] = rate1[0] * h_sd[0] + rate2[0] * h_rd[0];
		for (int i = 0; i < pkt_size; i++) {

			rx_source[i] = rx_source[i] * rate1[0] + RD_rx[i] * rate2[0];
		}
		break;

	}
}

void Comb::COOP_MRC(FADING FAD_MOD, double SNR_SD, vector<Complex<double>>& rx_source, vector<double>& h_sd, double SNR_RD, vector<Complex<double>> RD_rx, vector<double> h_rd)
{
	vector<double> rate1, rate2;
	int pkt_size = rx_source.size();

	double tmp1 = pow(10, (SNR_SD / 10)), tmp2 = pow(10, (SNR_RD / 10));

	switch (FAD_MOD) {
	case Rayl:
		rate1.resize(pkt_size), rate2.resize(pkt_size);

		for (int i = 0; i < pkt_size; i++) {
			rate1[i] = (h_sd[i] * tmp1) / (h_sd[i] * tmp1 + h_rd[i] * tmp2);
			rate2[i] = (h_rd[i] * tmp2) / (h_sd[i] * tmp1 + h_rd[i] * tmp2);

			h_sd[i] = rate1[i] * h_sd[i] + rate2[i] * h_rd[i];
			rx_source[i] = rx_source[i] * rate1[i] + RD_rx[i] * rate2[i];
		}
		break;

	case Rayl_Quasi:
		rate1.resize(1), rate2.resize(1);

		rate1[0] = (h_sd[0] * tmp1) / (h_sd[0] * tmp1 + h_rd[0] * tmp2);
		rate2[0] = (h_rd[0] * tmp2) / (h_sd[0] * tmp1 + h_rd[0] * tmp2);

		h_sd[0] = rate1[0] * h_sd[0] + rate2[0] * h_rd[0];

		for (int i = 0; i < pkt_size; i++) {
			rx_source[i] = rx_source[i] * rate1[0] + RD_rx[i] * rate2[0];
		}
		break;

	}
}

void Comb::COOP_EGC(FADING FAD_MOD, double SNR_SD, vector<Complex<double>>& rx_source, vector<double>& h_sd, double SNR_RD, vector<Complex<double>> RD_rx, vector<double> h_rd)
{
	vector<double> rate1, rate2;
	int pkt_size = rx_source.size();

	double tmp1 = pow(10, (SNR_SD / 10)), tmp2 = pow(10, (SNR_RD / 10));

	switch (FAD_MOD) {
	case Rayl:
		rate1.resize(pkt_size), rate2.resize(pkt_size);

		for (int i = 0; i < pkt_size; i++) {
			rate1[i] = (h_sd[i] * tmp1) / (h_sd[i] * tmp1 + h_rd[i] * tmp2);
			rate2[i] = (h_rd[i] * tmp2) / (h_sd[i] * tmp1 + h_rd[i] * tmp2);

			h_sd[i] = rate1[i] * h_sd[i] + rate2[i] * h_rd[i];
			rx_source[i] = rx_source[i] * rate1[i] + RD_rx[i] * rate2[i];
		}
		break;

	case Rayl_Quasi:
		rate1.resize(1), rate2.resize(1);

		rate1[0] = 0.5;
		rate2[0] = 0.5;

		h_sd[0] = rate1[0] * h_sd[0] + rate2[0] * h_rd[0];

		for (int i = 0; i < pkt_size; i++) {
			rx_source[i] = rx_source[i] * rate1[0] + RD_rx[i] * rate2[0];
		}
		break;

	}
}

void Comb::SUPER_MRC(FADING FAD_MOD, double SNR_SD, vector<Complex<double>>& rx_source, vector<double>& h_sd, double SNR_RD, vector<Complex<double>> RD_rx, vector<double> h_rd)
{
	vector<double> rate1, rate2;
	int pkt_size = rx_source.size();

	double tmp1 = pow(10, (SNR_SD / 10)), tmp2 = pow(10, (SNR_RD / 10));

	switch (FAD_MOD) {
	case Rayl:
		rate1.resize(pkt_size), rate2.resize(pkt_size);

		for (int i = 0; i < pkt_size; i++) {
			rate1[i] = (h_sd[i] * tmp1) / (h_sd[i] * tmp1 + h_rd[i] * tmp2);
			rate2[i] = (h_rd[i] * tmp2) / (h_sd[i] * tmp1 + h_rd[i] * tmp2);

			h_sd[i] = rate1[i] * h_sd[i] + rate2[i] * h_rd[i];
			rx_source[i] = rx_source[i] * rate1[i] + RD_rx[i] * rate2[i];
		}
		break;

	case Rayl_Quasi:
		rate1.resize(1), rate2.resize(1);

		rate1[0] = (h_sd[0] * tmp1) / (h_sd[0] * tmp1 + h_rd[0] * tmp2);
		rate2[0] = (h_rd[0] * tmp2) / (h_sd[0] * tmp1 + h_rd[0] * tmp2);

		h_sd[0] = rate1[0] * h_sd[0] + rate2[0] * h_rd[0];

		for (int i = 0; i < pkt_size; i++) {
			rx_source[i] = rx_source[i] * rate1[0] + RD_rx[i] * (rate2[0] * 4);
			//rx_source[i] = rx_source[i] * rate1[0] + RD_rx[i] * (rate2[0] * 4/3);
		}
		break;

	}
}

void Comb::SUPER_EGC(FADING FAD_MOD, double SNR_SD, vector<Complex<double>>& rx_source, vector<double>& h_sd, double SNR_RD, vector<Complex<double>> RD_rx, vector<double> h_rd)
{
	vector<double> rate1, rate2;
	int pkt_size = rx_source.size();

	double tmp1 = pow(10, (SNR_SD / 10)), tmp2 = pow(10, (SNR_RD / 10));

	switch (FAD_MOD) {
	case Rayl:
		rate1.resize(pkt_size), rate2.resize(pkt_size);

		for (int i = 0; i < pkt_size; i++) {
			rate1[i] = (h_sd[i] * tmp1) / (h_sd[i] * tmp1 + h_rd[i] * tmp2);
			rate2[i] = (h_rd[i] * tmp2) / (h_sd[i] * tmp1 + h_rd[i] * tmp2);

			h_sd[i] = rate1[i] * h_sd[i] + rate2[i] * h_rd[i];
			rx_source[i] = rx_source[i] * rate1[i] + RD_rx[i] * rate2[i];
		}
		break;

	case Rayl_Quasi:
		rate1.resize(1), rate2.resize(1);

		rate1[0] = 0.5;
		rate2[0] = 0.5;

		h_sd[0] = rate1[0] * h_sd[0] + rate2[0] * h_rd[0];

		for (int i = 0; i < pkt_size; i++) {
			rx_source[i] = rx_source[i] * rate1[0] + RD_rx[i] * (rate2[0] * 4);
			//rx_source[i] = rx_source[i] * rate1[0] + RD_rx[i] * (rate2[0] * 4/3);
		}
		break;

	}
}

void Comb::BLOCK_MRC(FADING FAD_MOD, double SNR_SD, vector<Complex<double>>& rx_source, vector<double>& h_sd, double SNR_RD, vector<Complex<double>> RD_rx, vector<double> h_rd)
{
	vector<double> rate1, rate2;
	int pkt_size = rx_source.size(), output_size = RD_rx.size();
	rx_source.resize(output_size);
	double tmp1 = pow(10, (SNR_SD / 10)), tmp2 = pow(10, (SNR_RD / 10));

	int i = 0;

	switch (FAD_MOD) {	//FAST Fading은 나중에 고치기로...
	case Rayl:
		rate1.resize(pkt_size), rate2.resize(pkt_size);

		for (; i < pkt_size; i++) {
			rate1[i] = (h_sd[i] * tmp1) / (h_sd[i] * tmp1 + h_rd[i] * tmp2);
			rate2[i] = (h_rd[i] * tmp2) / (h_sd[i] * tmp1 + h_rd[i] * tmp2);

			h_sd[i] = rate1[i] * h_sd[i] + rate2[i] * h_rd[i];
			rx_source[i] = rx_source[i] * rate1[i] + RD_rx[i] * rate2[i];
		}
		break;

	case Rayl_Quasi:
		rate1.resize(1), rate2.resize(1);

		rate1[0] = (h_sd[0] * tmp1) / (h_sd[0] * tmp1 + h_rd[0] * tmp2);
		rate2[0] = (h_rd[0] * tmp2) / (h_sd[0] * tmp1 + h_rd[0] * tmp2);

		h_sd[0] = (rate1[0] * h_sd[0] + rate2[0] * h_rd[0]);
		h_sd.push_back(h_rd[0]);
		for (; i < pkt_size; i++) 
			rx_source[i] = rx_source[i] * rate1[0] + RD_rx[i] * rate2[0];

		for(;i < output_size; i++)
			rx_source[i] = RD_rx[i];

		break;

	}
}

void Comb::LLR_MRC(FADING FAD_MOD, double SNR_SD, double * llr0, double * llr1, vector<double>& h_sd, double SNR_RD, 
	double * SUPER_llr0, double * SUPER_llr1, vector<double>& h_rd, int SIZE)
{
	vector<double> rate1, rate2;

	double tmp1 = pow(10, (SNR_SD / 10)), tmp2 = pow(10, (SNR_RD / 10));

	switch (FAD_MOD) {
	case Rayl:
		rate1.resize(SIZE), rate2.resize(SIZE);

		for (int i = 0; i < SIZE; i++) {
			rate1[i] = (h_sd[i] * tmp1) / (h_sd[i] * tmp1 + h_rd[i] * tmp2);
			rate2[i] = (h_rd[i] * tmp2) / (h_sd[i] * tmp1 + h_rd[i] * tmp2);

			h_sd[i] = rate1[i] * h_sd[i] + rate2[i] * h_rd[i];
			llr0[i] = llr0[i] * rate1[i] + SUPER_llr0[i] * rate2[i];
		}
		break;

	case Rayl_Quasi:
		rate1.resize(1), rate2.resize(1);

		//rate1[0] = (h_sd[0] * tmp1) / (h_sd[0] * tmp1 + h_rd[0] * tmp2);
		//rate2[0] = (h_rd[0] * tmp2) / (h_sd[0] * tmp1 + h_rd[0] * tmp2);

		//h_sd[0] = rate1[0] * h_sd[0] + rate2[0] * h_rd[0];

		//EGC EXPERIMENT
		rate1[0] = 0.5;
		rate2[0] = 0.5;

		for (int i = 0; i < SIZE; i++) {
			llr0[i] = llr0[i] * rate1[0] + SUPER_llr0[i] * rate2[0];
			llr1[i] = llr1[i] * rate1[0] + SUPER_llr1[i] * rate2[0];
		}
		break;

	}
}

void Comb::LLR_MRC(FADING FAD_MOD, double SNR_SD, double* llr0, double* llr1, vector<double>& h_sd, double SNR_RD, vector<double> SUPER_llr0, vector<double> SUPER_llr1, vector<double>& h_rd, int SIZE)
{
	vector<double> rate1, rate2;

	double tmp1 = pow(10, (SNR_SD / 10)), tmp2 = pow(10, (SNR_RD / 10));

	switch (FAD_MOD) {
	case Rayl:
		rate1.resize(SIZE), rate2.resize(SIZE);

		for (int i = 0; i < SIZE; i++) {
			rate1[i] = (h_sd[i] * tmp1) / (h_sd[i] * tmp1 + h_rd[i] * tmp2);
			rate2[i] = (h_rd[i] * tmp2) / (h_sd[i] * tmp1 + h_rd[i] * tmp2);

			h_sd[i] = rate1[i] * h_sd[i] + rate2[i] * h_rd[i];
			llr0[i] = llr0[i] * rate1[i] + SUPER_llr0[i] * rate2[i];
		}
		break;

	case Rayl_Quasi:
		rate1.resize(1), rate2.resize(1);

		rate1[0] = (h_sd[0] * tmp1) / (h_sd[0] * tmp1 + h_rd[0] * tmp2);
		rate2[0] = (h_rd[0] * tmp2) / (h_sd[0] * tmp1 + h_rd[0] * tmp2);

		h_sd[0] = rate1[0] * h_sd[0] + rate2[0] * h_rd[0];

		for (int i = 0; i < SIZE; i++) {
			llr0[i] = llr0[i] * rate1[0] + SUPER_llr0[i] * rate2[0];
			llr1[i] = llr1[i] * rate1[0] + SUPER_llr1[i] * rate2[0];
		}
		break;

	}
}

void Comb::LLR_COMB(FADING FAD_MOD, double SNR_SD, vector<double> h_sd, vector<vector<double>> LLR_FIRST,
	double SNR_RD, vector<double> h_rd, vector<vector<double>> &LLR_SECOND){	//only consider case of Quasi static rayleigh fading channel
    int size = LLR_FIRST.size();
	vector<double> rate1, rate2;
	double tmp1 = pow(10, (SNR_SD / 10)), tmp2 = pow(10, (SNR_RD / 10)),  tmpSum0 = 0, tmpSum1 = 0, tmpSecond0 = 0, tmpFirst0 = 0;

	rate1.resize(1), rate2.resize(1);

	rate1[0] = (h_sd[0] * tmp1) / (h_sd[0] * tmp1 + h_rd[0] * tmp2);
	rate2[0] = (h_rd[0] * tmp2) / (h_sd[0] * tmp1 + h_rd[0] * tmp2);

	//EGC
	//rate1[0] = 0.5;
	//rate2[0] = 0.5;

	for (int i = 0; i < size; i++) {//과정을 단순화 해볼까?
		//tmpSum0 = (LLR_FIRST[i][0] + LLR_SECOND[i][0]), tmpSum1 = (LLR_FIRST[i][1] + LLR_SECOND[i][1]);

		//tmpSecond0 = LLR_SECOND[i][0] / tmpSum0;

		//LLR_SECOND[i][0] = LLR_FIRST[i][0] / tmpSum0 * rate1[0] + LLR_SECOND[i][0] / tmpSum0 * rate2[0];
		//LLR_SECOND[i][1] = LLR_FIRST[i][1] / tmpSum1 * rate1[0] + LLR_SECOND[i][1] / tmpSum1 * rate2[0];

		//LLR_SECOND[i][0] *= tmpSum0;
		//LLR_SECOND[i][1] *= tmpSum1;

		LLR_SECOND[i][0] = LLR_FIRST[i][0] * rate1[0] + LLR_SECOND[i][0] * rate2[0];
		LLR_SECOND[i][1] = LLR_FIRST[i][1] * rate1[0] + LLR_SECOND[i][1] * rate2[0];
	}
}

void Comb::LLR_COMB2(FADING FAD_MOD, double SNR_SD, vector<double> &h_sd, double *llr0, double *llr1,
	double SNR_RD, vector<double> h_rd, double *HARQ_llr0, double *HARQ_llr1, int size)
{
	//only consider case of Quasi static rayleigh fading channel
	
	vector<double> rate1, rate2;
	double tmp1 = pow(10, (SNR_SD / 10)), tmp2 = pow(10, (SNR_RD / 10));

	rate1.resize(1), rate2.resize(1);

	rate1[0] = (h_sd[0] * tmp1) / (h_sd[0] * tmp1 + h_rd[0] * tmp2);
	rate2[0] = (h_rd[0] * tmp2) / (h_sd[0] * tmp1 + h_rd[0] * tmp2);

	//rate1[0] = 0.5;
	//rate2[0] = 0.5;

	for (int i = 0; i < size; i++) {
		llr0[i] = llr0[i] * rate1[0] + HARQ_llr0[i] * rate2[0];
		llr1[i] = llr1[i] * rate1[0] + HARQ_llr1[i] * rate2[0];
	}
}

void Comb::LLR_COMB3(double ** LLR1, double ** LLR2, double ** HARQ_LLR1, double ** HARQ_LLR2, int col, int row)
{
	for (int i = 0; i < col; i++) {
		for (int j = 0; j < row; j++) {//firstly, EGC..., change to MRC later
			LLR1[i][j] = 0.5 * LLR1[i][j] + 0.5 * HARQ_LLR1[i][j];
			LLR2[i][j] = 0.5 * LLR2[i][j] + 0.5 * HARQ_LLR2[i][j];
		}
	}
}

void Comb::LLR_COMB4(double LC_Comb, double rate1, double rate2, double * llr0, double * llr1, double * HARQ_llr0, double *HARQ_llr1, int size)
{
	//only consider case of Quasi static rayleigh fading channel

	rate1 = 0.9999;
	rate2 = 0.0001;

	for (int i = 0; i < size; i++) {
		llr0[i] = (llr0[i] * rate1 + HARQ_llr0[i] * rate2) * LC_Comb;
		llr1[i] = (llr1[i] * rate1 + HARQ_llr1[i] * rate2) * LC_Comb;
	}
}

double Comb::Rate(double SNR, double RD_Gain, vector<double> h_sd, vector<double> h_rd, double LC_SD, double LC_RD, double & rate1, double & rate2)
{
	double tmp1 = pow(10, (SNR / 10)), tmp2 = pow(10, ((SNR + RD_Gain) / 10)),
		result = (h_sd[0] * tmp1) / (h_sd[0] * tmp1 + h_rd[0] * tmp2) * LC_SD + (h_rd[0] * tmp2) / (h_sd[0] * tmp1 + h_rd[0] * tmp2) * LC_RD;

	rate1 = (h_sd[0] * tmp1) / (h_sd[0] * tmp1 + h_rd[0] * tmp2) / LC_SD;
	rate2 = (h_rd[0] * tmp2) / (h_sd[0] * tmp1 + h_rd[0] * tmp2) / LC_RD;

	return result;
}

void Comb::Picking_EVEN(vector < vector < double >> &LLR_SECOND, vector < vector < double >> &LLR_THIRD){
    int size_col = LLR_SECOND.size(), size_row = LLR_SECOND[0].size();
    LLR_THIRD.resize(size_col/2);
	int i = 0;
    for(; i < size_col/2; i++){
        LLR_THIRD[i].resize(size_row);
    }

    for(i = 0; i < size_col/4; i ++){
        LLR_THIRD[i][0] = LLR_SECOND[4 * i][0];
        LLR_THIRD[i][1] = LLR_SECOND[4 * i][1];
        LLR_THIRD[i + 1][0] = LLR_SECOND[4 * i + 1][0];
        LLR_THIRD[i + 1][1] = LLR_SECOND[4 * i + 1][1];
    }
}

void Comb::Picking_FIRSTPAIR(vector < vector < double >> &LLR_SECOND, vector < vector < double >> &LLR_THIRD){
    int size_col = LLR_SECOND.size(), size_row = LLR_SECOND[0].size();
    LLR_THIRD.resize(size_col/2);
	int i = 0;
    for(; i < size_col/2; i++){
        LLR_THIRD[i].resize(size_row);
    }

    for(i = 0; i < size_col/2; i ++){
        LLR_THIRD[i][0] = LLR_SECOND[i][0];
        LLR_THIRD[i][1] = LLR_SECOND[i][1];
    }
}


void Comb::Picking_SECONDPAIR(vector < vector < double >> &LLR_SECOND, vector < vector < double >> &LLR_THIRD){
    int size_col = LLR_SECOND.size(), size_row = LLR_SECOND[0].size();
    LLR_THIRD.resize(size_col/2);
	int i = 0, j = 0;
    for(; i < size_col/2; i++){
        LLR_THIRD[i].resize(size_row);
    }

    for(i = 0, j = size_col/2; i < size_col/2; i++, j++){
        LLR_THIRD[i][0] = LLR_SECOND[j][0];
        LLR_THIRD[i][1] = LLR_SECOND[j][1];
    }
}

vector<bool> Comb::Comnining_PAIRS(vector < bool > encoded_source, vector < bool > encoded_relay){
    int size = encoded_source.size();
    vector<bool> result(size * 2);

    for(int i = 0; i < size/2; i++){
        result[4 * i] = encoded_relay[2 * i];
        result[4 * i + 1] = encoded_relay[2 * i + 1];
        result[4 * i + 2] = encoded_source[2 * i];
        result[4 * i + 3] = encoded_source[2 * i + 1];        
    }
	return result;
}

