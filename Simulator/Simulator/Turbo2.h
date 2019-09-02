#ifndef TURBO2_H
#define TURBO2_H



#include <vector>
#include "complex_new.h"
#include <math.h>
#include "Mapper.h"

#define	pInfty	100000000000000.0
#define	mInfty	-100000000000000.0

using namespace std;



struct	branch2 {
	// next_state is used for forward recursion(trellis)
	// prev_state is used for backward recursion(trellis)
	union {
		int	next_state;
		int	prev_state;
	};
	int		info_bit;
	int		code_bit;
	
};


class Turb {
public:
	~Turb();
	int m_Nstate, m_Nmemory, m_Ninfo, m_Ncode, m_Bsize,Bsize;
	int m_g0, m_g1,f1,f2;
	vector < vector<branch2>>FTrellis, BTrellis;
	double	m_log_sum_exp_correction_lut[256];
	vector<bool>tb;

	double	 **PU_I1, **PU_O1, **PU_I2, **PU_O2, **L1, **L2;
	//decoding variables
	double *info0, *info1, *parity1_0, *parity1_1, *parity2_0, *parity2_1,
		*info0_int, *info1_int, *tail1_bit0, *tail1_bit1, *tail2_bit0, *tail2_bit1;

    vector<double> info0_v, info1_v, parity1_0_v, parity1_1_v, parity2_0_v, parity2_1_v,
		info0_int_v, info1_int_v, tail1_bit0_v, tail1_bit1_v, tail2_bit0_v, tail2_bit1_v;
	//===============================================================================
	double	**log_alpa, **log_beta;			// feedforward and feedback recursion
											//=====================================================
											//				Temporary Variables
											//=====================================================

	double	*tot_log_a;
	double	*tmp1, *tmp2, **tmp3, **tmp33, *tmp4, *tmp44;
	int		*tmp5;


	void turbo_trellis_gen(int *generate, int Bsize, int Nmemory, int Ncodebit, int Ninfobit);
	void int_init();//trellis_gen다음에 사용할것!
	
	//encoding
	vector <bool> encode(vector <bool> source, vector<bool> source_int);
	vector<bool> interleaver(vector<bool> source);
	
	int* inter_pattern,*deinter_pattern;

	
	//decoding parameter & method

	void turbo_llr_generation(FADING FAD_MOD, vector<Complex<double>> rx_buf, vector<double> llr_wgt_buf, double* rx_llr0, double*rx_llr1, Mapper *M_MAP, int SP_NDCARperSYM, double LC);
	void turbo_llr_generation(FADING FAD_MOD, vector<Complex<double>> rx_buf, vector<double> llr_wgt_buf, vector<double>& rx_llr0, vector<double>& rx_llr1, Mapper *M_MAP, int SP_NDCARperSYM, double LC);
    
	void turbo_llr_generation_InPhase(FADING FAD_MOD, vector<Complex<double>> rx_buf, vector<double> llr_wgt_buf, double* rx_llr0, double* rx_llr1, Mapper* M_MAP, int SP_NDCARperSYM, double LC);
	void turbo_llr_generation_Quad(FADING FAD_MOD, vector<Complex<double>> rx_buf, vector<double> llr_wgt_buf, double* rx_llr0, double* rx_llr1, Mapper* M_MAP, int SP_NDCARperSYM, double LC);
	void turbo_bit2sym(double* rx_llr0_buf, double *rx_llr1_buf, double **LLR1,double **LLR2, int SP_NCODEBITperSYM, int SP_NCODEBIT, int SP_NCODE);
	void turbo_bit2sym(vector<double> rx_llr0_buf, vector<double>rx_llr1_buf, vector<vector<double>>& LLR1,vector<vector<double>>& LLR2, int SP_NCODEBITperSYM, int SP_NCODEBIT, int SP_NCODE);
	void	log_sum_exp_lut_generation();
	double	log_sum_exp(double *din, int num);
	double	log_sum_exp(double din1, double din2);

    void llr_segment(double *llr0, double *llr1, double *source_llr0, double *source_llr1, double *relay_llr0, double *relay_llr1, int size);
    void llr_segment(vector<double>llr0, vector<double>llr1, vector<double>&source_llr0, vector<double>&source_llr1, vector<double>&relay_llr0, vector<double>&relay_llr1, int size);
	
	vector<bool> turbo_decoding(double** LLR1, double**LLR2, int turbo_iter_num);
    vector<bool> turbo_decoding(vector<vector<double>> LLR1, vector<vector<double>> LLR2, int turbo_iter_num);
	vector<vector<double>>  ExportLLR_turbo_decoding(double** LLR1, double**LLR2, int turbo_iter_num);
	vector<vector<double>>  ExportLLR_turbo_decoding(vector<vector<double>> LLR1, vector<vector<double>> LLR2, int turbo_iter_num);
    vector<vector<double>>  ExportLLR_turbo_decoding_excluding(double** LLR1, double**LLR2, int turbo_iter_num);
    void Decision(vector<vector<double>> LLR, vector<bool> &result);
	void decoding(double	**PC_I, double	**PU_I, double	**PU_O);
	void decoding(vector<vector<double>>& PC_I, double** PU_I, double** PU_O);
	vector<bool> Attach(vector<bool>, vector<bool>);


	//LLR combining after iteration
	void LLR_combining_after_iteration(double** LLR1, double** LLR2, double** DF_LLR1, double** DF_LLR2);
	//member var for copy LLR after iteration
	//vector<vector<double>> 
};







#endif
