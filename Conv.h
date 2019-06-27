#ifndef CONV_H
#define CONV_H

#include <vector>
#include "complex_new.h"
#include <math.h>



using namespace std;

struct	branch {
	// next_state is used for forward recursion(trellis)
	// prev_state is used for backward recursion(trellis)
	union {
		int	next_state;
		int	prev_state;
	};
	int		info_bit;
	int		code_bit;
	//complex<double>	map ;	// I am not quite sure whether this is required or not.
};

class Conv {
public:
	~Conv();
	int Nstate, Nmemory, Ninfo, Ncode, Bsize;
	double  EbN0, rate;
	int	 m_enc_prev_state;
	vector < vector<branch>>FTrellis, BTrellis;
	vector<vector<double>>uc;
	vector<vector<double>>m_log_alpa;
	vector<vector<int>>m_traceback_mem;


	//trellis
	void trellis_gen(int *generate, int Bsize, int Nmemory, int Ncodebit, int Ninfobit);
	void Trellis_clear();

	//encoding
	void encoder_init();
	vector<bool> Conv_encoder(vector<bool> &input, double rate);

	//decoding
	void init_dec_state();
	
	vector<bool> decoding(double** LLR);
	void weighting(vector<double>h_gain);

	//추가 구현
	




};



#endif


//======================================================================================================================
