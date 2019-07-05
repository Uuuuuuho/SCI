#pragma once


#ifndef MAPPER_H
#define MAPPER_H
#include "Rayleigh.h"
#include "complex_new.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <malloc.h>
#include "Equations.h"

using namespace std;


#define PI 3.1415926535897932384626
#define MOD_SUPER_16QAM 9
#define MOD_QUAD_4PAM	8
#define MOD_INP_4PAM	7
#define MOD_IQ			6
#define	MOD_QAM64		5
#define	MOD_QAM16		4
#define	MOD_PSK8		3
#define	MOD_QPSK		2
#define	MOD_BPSK		1



//local Complex
void 	mapping_qam(Complex<double> *signal, double a_s, int M, double shift);
void 	mapping_IQqam(Complex<double> *signal, double a_s, int M, double shift);
void 	mapping_psk(Complex<double> *signal, double a_s, int M, int d);
void 	mapping_InPhase_pam(Complex<double> *signal, double a_s, int M, int d);
void 	mapping_Quad_pam(Complex<double> *signal, double a_s, int M, int d);
void	mapping_SUPER_16QAM(Complex<double>* signal, double a_s, int M, double shift);

class Mapper
{
public:
	int	m_mod_type;
	int	m_N_1d_const;
	int	m_dim;
	int	m_N_2d_const;
	int	m_Ninfo;
	int	m_Ninfobit;
	double	m_avg_sig_energy;

	double	*m_nd_map_tab;
	Complex<double>	*m_2d_map_tab;
	Complex<double>*m_rot_map_tab;
	Mapper();




	void init(int mod_type);
	void map_tab_gen(double target_sig_energy);
	void PSK8_Mapping(vector<bool>, vector<bool>, vector<Complex<double>>&); 
    void PSK8_Demapping(vector<bool>, vector<Complex<double>> &);
    void PSK16_Mapping(vector<bool>, vector<bool>, vector<Complex<double>>&);
    void PSK16_Phase_Extension(vector<bool>, vector<Complex<double>> &);
	void BPSK_Mapping(vector<bool>, vector<Complex<double>>&);
	void QPSK_Mapping(vector<bool>, vector<Complex<double>>&);
	void QPSK_Mapping(vector<bool>, vector<Complex<double>>&, double val);
	void PAM_InPhase_Mapping(vector<bool>, vector<Complex<double>>&, double val);
	void PAM_Quad_Mapping(vector<bool>, vector<Complex<double>>&, double val);
	void QAM_Mapping(vector<bool>, vector<Complex<double>>&);
	void QAM64_Mapping(vector<bool>, vector<Complex<double>>&);
	void BPSK_Demapping(vector<Complex<double>>, int, vector<bool>&);
	void QPSK_Demapping(vector<Complex<double>>, int, vector<bool>&);
	void QAM_Demapping(vector<Complex<double>>, int, vector<bool>&);
	void QAM64_Demapping(vector<Complex<double>>, int, vector<bool>&);

	void IQ_Mapping(vector<bool> inPhase, double p_alpha, vector<bool> Quad, double p_beta, vector<Complex<double>>&);

	//========================llr=========================================================
	void llr_generation(FADING FAD_MOD, vector<Complex<double>> rx_buf, vector<double> llr_wgt_buf, double* rx_llr0, double*rx_llr1, Mapper *M_MAP, int SP_NDCARperSYM);
	void bit2symLLR_generation(double* rx_llr0_buf, double *rx_llr1_buf, double **LLR, int SP_NCODEBITperSYM, int SP_NCODEBIT, int SP_NCODE);
	void llr_weighting(vector<double>&llr0, vector<double>&llr1, vector<double>llr_wgt, Mapper *M_Map);
	void llr_Ca(double* llr1, double *ll2, double *llr3, int Bsize);
	//====================================================================================
	
	//=========================JTC========================================================
	void vec2array(vector<Complex<double>> in, Complex<double> *output,int size);
	void array2vec(Complex<double>*in,vector<Complex<double>> &output,int size);
	//====================================================================================

	//=========================ssd========================================================
	void ssd_init(Mapper *M_map,double degree);
	void rotator(Complex<double> *input, double degree);
	void rot(vector<Complex<double>>&input, double degree);
    void rotate(Complex<double> &input, double degree);
	void interleave(vector<Complex<double>>&input);
	void ssd_ML(vector<Complex<double>>rx, vector<bool>&decoded, Mapper *M_MAP,vector<double>wgt,int size);
	void SSD_llr_generation(vector<Complex<double>> rx_buf, vector<double> llr_wgt_buf, double* rx_llr0, double*rx_llr1, Mapper *M_MAP, int SP_NDCARperSYM);
    //=====================================================================================	
	double *PSKLUT;
	int **QAMLUT, **QAMLUT2;
	vector<vector<double>> PAMLUT;
    int ***QAM64LUT;
    vector<vector<vector<vector<Complex<double>>>>> PSK8LUT;
	~Mapper();

    //Superposition
    void Super(vector<Complex<double>> a, vector<Complex<double>> &b);
    void Super_Sub(vector<Complex<double>> a, vector<Complex<double>> &b);
	void Interference(vector<Complex<double>> a, vector<Complex<double>>& b);
};


#endif // !MAPPER_H
