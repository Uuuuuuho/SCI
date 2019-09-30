#include "Mapper.h"

Mapper::Mapper() {};


void Mapper::init(int mod_type)
{
	m_mod_type = mod_type;
	switch (mod_type) {
	case	MOD_BPSK:
		m_dim = 1;
		m_N_1d_const = 2;
		m_N_2d_const = 2; //meaningless
		m_2d_map_tab = new Complex<double>[m_N_1d_const];
		m_rot_map_tab = new Complex<double>[m_N_1d_const];
		m_Ninfobit = 1;
		m_Ninfo = 2;
		break;
	case	MOD_QPSK:
		m_dim = 2;
		m_N_1d_const = 2;
		m_N_2d_const = 4;
		m_Ninfobit = 2;
		m_Ninfo = 4;
		m_2d_map_tab = new Complex<double>[m_N_2d_const];
		m_rot_map_tab = new Complex<double>[m_N_2d_const];
		break;
	case	MOD_INP_4PAM:
		m_dim = 2;
		m_N_1d_const = 2;
		m_N_2d_const = 4;
		m_Ninfobit = 2;
		m_Ninfo = 4;
		m_2d_map_tab = new Complex<double>[m_N_2d_const];
		m_rot_map_tab = new Complex<double>[m_N_2d_const];
		break;
	case	MOD_QUAD_4PAM:
		m_dim = 2;
		m_N_1d_const = 2;
		m_N_2d_const = 4;
		m_Ninfobit = 2;
		m_Ninfo = 4;
		m_2d_map_tab = new Complex<double>[m_N_2d_const];
		m_rot_map_tab = new Complex<double>[m_N_2d_const];
		break;
	case	MOD_PSK8:
		m_dim = 2;
		m_N_1d_const = 2; //meaningless
		m_N_2d_const = 8;
		m_Ninfo = 8;
		m_Ninfobit = 3;
		m_2d_map_tab = new Complex<double>[m_N_2d_const];
		m_rot_map_tab = new Complex<double>[m_N_2d_const];
		break;
	case	MOD_QAM16:
		m_dim = 2;
		m_N_1d_const = 4;
		m_N_2d_const = 16;
		m_Ninfo = 16;
		m_Ninfobit = 4;
		m_2d_map_tab = new Complex<double>[m_N_2d_const];
		m_rot_map_tab = new Complex<double>[m_N_2d_const];
		break;
	case	MOD_SUPER_16QAM:
		m_dim = 2;
		m_N_1d_const = 4;
		m_N_2d_const = 16;
		m_Ninfo = 16;
		m_Ninfobit = 4;
		m_2d_map_tab = new Complex<double>[m_N_2d_const];
		m_rot_map_tab = new Complex<double>[m_N_2d_const];
		break;        
	case	MOD_SUPER_16QAM2:
		m_dim = 2;
		m_N_1d_const = 4;
		m_N_2d_const = 16;
		m_Ninfo = 16;
		m_Ninfobit = 4;
		m_2d_map_tab = new Complex<double>[m_N_2d_const];
		m_rot_map_tab = new Complex<double>[m_N_2d_const];
		break;        

	case	MOD_IQ:
		m_dim = 2;
		m_N_1d_const = 4;
		m_N_2d_const = 16;
		m_Ninfo = 16;
		m_Ninfobit = 4;
		m_2d_map_tab = new Complex<double>[m_N_2d_const];
		m_rot_map_tab = new Complex<double>[m_N_2d_const];
		break;
	case    MOD_QAM64:
		m_dim = 2;
		m_N_1d_const = 8;
		m_N_2d_const = 64;
		m_Ninfo = 64;
		m_Ninfobit = 6;
		m_2d_map_tab = new Complex<double>[m_N_2d_const];
		m_rot_map_tab = new Complex<double>[m_N_2d_const];
		QAM64LUT = (int ***)malloc(2 * sizeof(int ***));
		for (int l = 0; l < 2; l++)
			QAM64LUT[l] = (int **)malloc(2 * sizeof(int **));

		for (int j = 0; j < 2; j++)
		{
			for (int h = 0; h < 2; h++) {
				QAM64LUT[j][h] = (int*)malloc(2 * sizeof(int *));
			}

		}
		break;

	default:
		printf("This mod type is not supported right now %d\n", mod_type);
		exit(1);
	}


}

#if 1
void		Mapper::map_tab_gen(double target_sig_energy)
{
	int i;
	switch (m_mod_type)
	{
	case MOD_QAM64: mapping_qam(m_2d_map_tab, target_sig_energy, m_N_2d_const, 0.0); break;
	case MOD_IQ:	mapping_IQqam(m_2d_map_tab, target_sig_energy, m_N_2d_const, 0.0); break;
	case MOD_QAM16:	mapping_qam(m_2d_map_tab, target_sig_energy, m_N_2d_const, 0.0); break;
	case MOD_PSK8:		mapping_psk(m_2d_map_tab, target_sig_energy, 8, m_dim); break;
	case MOD_QUAD_4PAM:		mapping_Quad_pam(m_2d_map_tab, target_sig_energy, 4, m_dim); break;
	case MOD_INP_4PAM:		mapping_InPhase_pam(m_2d_map_tab, target_sig_energy, 4, m_dim); break;
	case MOD_QPSK:		mapping_psk(m_2d_map_tab, target_sig_energy, 4, m_dim); break;
	case MOD_BPSK:		mapping_psk(m_2d_map_tab, target_sig_energy, 2, m_dim); break;
    case MOD_SUPER_16QAM: mapping_SUPER_16QAM(m_2d_map_tab, target_sig_energy, m_N_2d_const, 0.0); break;   //consider target signal energy as beta
    case MOD_SUPER_16QAM2: mapping_SUPER_16QAM2(m_2d_map_tab, target_sig_energy, m_N_2d_const, 0.0); break;   //consider target signal energy as beta

	default: printf("modulation type %d is notㅡ supported yet\n", m_mod_type); exit(1);
	}
	//m_avg_sig_energy = 0.0;	// initialze total signal energy
	//for (i = 0; i<m_N_2d_const; i++)
	//	m_avg_sig_energy += masq(m_2d_map_tab[i].re, m_2d_map_tab[i].im);
	//m_avg_sig_energy /= (double)m_N_2d_const;
	//// normalize the signal energy .....
	//for (i = 0; i<m_N_2d_const; i++)
	//m_2d_map_tab[i] = (m_2d_map_tab[i] / sqrt(m_avg_sig_energy)) * sqrt(target_sig_energy);


}
#endif

void mapping_SUPER_16QAM2(Complex<double> *signal, double a_s, int M, double shift){
	int i;
    double alpha = 1 - a_s, beta = a_s;
    
	if (M == 16)
	{

		i = 0;
		signal[i].re = alpha;     signal[i].im = alpha;
		i = 1;
		signal[i].re = alpha;     signal[i].im = alpha;
		i = 5;
		signal[i].re = alpha;      signal[i].im = - alpha;
		i = 4;
		signal[i].re = alpha;      signal[i].im = - alpha;

		i = 2;
		signal[i].re = alpha;     signal[i].im = alpha;
		i = 3;
		signal[i].re = alpha;     signal[i].im = alpha;
		i = 7;
		signal[i].re = alpha;      signal[i].im = - alpha;
		i = 6;
		signal[i].re = alpha;      signal[i].im = - alpha;

		i = 10;
		signal[i].re = - alpha;     signal[i].im = alpha;
		i = 11;
		signal[i].re = - alpha;     signal[i].im = alpha;
		i = 15;
		signal[i].re = - alpha;      signal[i].im = - alpha;
		i = 14;
		signal[i].re = - alpha;      signal[i].im = - alpha;

		i = 8;
		signal[i].re = - alpha;     signal[i].im = alpha;
		i = 9;
		signal[i].re = - alpha;     signal[i].im = alpha;
		i = 13;
		signal[i].re = - alpha;      signal[i].im = - alpha;
		i = 12;
		signal[i].re = - alpha;      signal[i].im = - alpha;
	}

	else
	{
		printf("constration number %d is not supported yet\n", M); exit(1);
	}

	for (i = 0; i<M; i++)
	{
		signal[i].re += shift;
		signal[i].im += shift;
	}

}

void mapping_PARTIAL_SM(Complex<double>* signal, double alpha, double beta, int M, double shift)
{
	int i;
	if (M == 64)
	{//=================수정해야함==============================
		i = 0;
		signal[i].re = alpha + 0.3 * beta;     signal[i].im = alpha + 0.3 * beta;
		i = 1;
		signal[i].re = alpha + 0.3 * beta;     signal[i].im = alpha + 0.1 * beta;
		i = 5;
		signal[i].re = alpha + 0.3 * beta;      signal[i].im = alpha - 0.1 * beta;
		i = 4;
		signal[i].re = alpha + 0.3 * beta;      signal[i].im = alpha - 0.3 * beta;

		i = 20;
		signal[i].re = - alpha + 0.3 * beta;     signal[i].im = alpha - 0.3 * beta;
		i = 21;
		signal[i].re = - alpha + 0.3 * beta;     signal[i].im = alpha - 0.1 * beta;
		i = 17;
		signal[i].re = - alpha + 0.3 * beta;      signal[i].im = alpha + 0.1 * beta;
		i = 16;
		signal[i].re = - alpha + 0.3 * beta;      signal[i].im = alpha + 0.3 * beta;

		i = 2;
		signal[i].re = alpha + 0.1 * beta;     signal[i].im = alpha + 0.3 * beta;
		i = 3;
		signal[i].re = alpha + 0.1 * beta;     signal[i].im = alpha + 0.1 * beta;
		i = 7;
		signal[i].re = alpha + 0.1 * beta;      signal[i].im = alpha - 0.1 * beta;
		i = 6;
		signal[i].re = alpha + 0.1 * beta;      signal[i].im = alpha - 0.3 * beta;

		i = 22;
		signal[i].re = - alpha + 0.1 * beta;     signal[i].im = alpha - 0.3 * beta;
		i = 23;
		signal[i].re = -alpha + 0.1 * beta;     signal[i].im = alpha - 0.1 * beta;
		i = 19;
		signal[i].re = -alpha + 0.1 * beta;      signal[i].im = -alpha + 0.1 * beta;
		i = 18;
		signal[i].re = -alpha + 0.1 * beta;      signal[i].im = -alpha + 0.3 * beta;

		i = 10;
		signal[i].re = alpha - 0.1 * beta;     signal[i].im = alpha + 0.3 * beta;
		i = 11;
		signal[i].re = alpha - 0.1 * beta;     signal[i].im = alpha + 0.1 * beta;
		i = 15;
		signal[i].re = alpha - 0.1 * beta;      signal[i].im = alpha - 0.1 * beta;
		i = 14;
		signal[i].re = alpha - 0.1 * beta;      signal[i].im = alpha - 0.3 * beta;

		i = 30;
		signal[i].re = - alpha - 0.1 * beta;     signal[i].im = alpha - 0.3 * beta;
		i = 31;
		signal[i].re = -alpha - 0.1 * beta;     signal[i].im = alpha - 0.1 * beta;
		i = 27;
		signal[i].re = -alpha - 0.1 * beta;      signal[i].im = alpha + 0.1 * beta;
		i = 26;
		signal[i].re = -alpha - 0.1 * beta;      signal[i].im = alpha + 0.3 * beta;

		i = 8;
		signal[i].re = alpha - 0.3 * beta;     signal[i].im = alpha + 0.3 * beta;
		i = 9;
		signal[i].re = alpha - 0.3 * beta;     signal[i].im = alpha + 0.1 * beta;
		i = 13;
		signal[i].re = alpha - 0.3 * beta;      signal[i].im = alpha - 0.1 * beta;
		i = 12;
		signal[i].re = alpha - 0.3 * beta;      signal[i].im = alpha - 0.3 * beta;

		i = 28;
		signal[i].re = - alpha - 0.3 * beta;     signal[i].im = alpha - 0.3 * beta;
		i = 29;
		signal[i].re = -alpha - 0.3 * beta;     signal[i].im = alpha - 0.1 * beta;
		i = 25;
		signal[i].re = -alpha - 0.3 * beta;      signal[i].im = alpha + 0.1 * beta;
		i = 24;
		signal[i].re = -alpha - 0.3 * beta;      signal[i].im = alpha + 0.3 * beta;

		i = 40;
		signal[i].re = alpha - 0.3 * beta;     signal[i].im = -alpha + 0.3 * beta;
		i = 41;
		signal[i].re = alpha - 0.3 * beta;     signal[i].im = -alpha + 0.1 * beta;
		i = 45;
		signal[i].re = alpha - 0.3 * beta;      signal[i].im = -alpha - 0.1 * beta;
		i = 44;
		signal[i].re = alpha - 0.3 * beta;      signal[i].im = -alpha - 0.3 * beta;

		i = 60;
		signal[i].re = -alpha - 0.3 * beta;     signal[i].im = -alpha - 0.3 * beta;
		i = 61;
		signal[i].re = -alpha - 0.3 * beta;     signal[i].im = -alpha - 0.1 * beta;
		i = 57;
		signal[i].re = -alpha - 0.3 * beta;      signal[i].im = -alpha + 0.1 * beta;
		i = 56;
		signal[i].re = -alpha - 0.3 * beta;      signal[i].im = -alpha + 0.3 * beta;

		i = 42;
		signal[i].re = alpha - 0.1 * beta;     signal[i].im = -alpha + 0.3 * beta;
		i = 43;
		signal[i].re = alpha - 0.1 * beta;     signal[i].im = -alpha + 0.1 * beta;
		i = 47;
		signal[i].re = alpha - 0.1 * beta;      signal[i].im = -alpha - 0.1 * beta;
		i = 46;
		signal[i].re = alpha - 0.1 * beta;      signal[i].im = -alpha - 0.3 * beta;

		i = 62;
		signal[i].re = -alpha + 0.1 * beta;     signal[i].im = -alpha - 0.3 * beta;
		i = 63;
		signal[i].re = -alpha + 0.1 * beta;     signal[i].im = -alpha - 0.1 * beta;
		i = 59;
		signal[i].re = -alpha + 0.1 * beta;      signal[i].im = -alpha + 0.1 * beta;
		i = 58;
		signal[i].re = -alpha + 0.1 * beta;      signal[i].im = -alpha + 0.3 * beta;

		i = 34;
		signal[i].re = alpha + 0.1 * beta;     signal[i].im = -alpha + 0.3 * beta;
		i = 35;
		signal[i].re = alpha + 0.1 * beta;     signal[i].im = -alpha + 0.1 * beta;
		i = 39;
		signal[i].re = alpha + 0.1 * beta;      signal[i].im = -alpha - 0.1 * beta;
		i = 38;
		signal[i].re = alpha + 0.1 * beta;      signal[i].im = -alpha - 0.3 * beta;

		i = 54;
		signal[i].re = -alpha + 0.1 * beta;     signal[i].im = -alpha - 0.3 * beta;
		i = 55;
		signal[i].re = -alpha + 0.1 * beta;     signal[i].im = -alpha - 0.1 * beta;
		i = 51;
		signal[i].re = -alpha + 0.1 * beta;      signal[i].im = -alpha + 0.1 * beta;
		i = 50;
		signal[i].re = -alpha + 0.1 * beta;      signal[i].im = -alpha + 0.3 * beta;

		i = 32;
		signal[i].re = alpha + 0.3 * beta;     signal[i].im = -alpha + 0.3 * beta;
		i = 33;
		signal[i].re = alpha + 0.3 * beta;     signal[i].im = -alpha + 0.1 * beta;
		i = 37;
		signal[i].re = alpha + 0.3 * beta;      signal[i].im = -alpha - 0.1 * beta;
		i = 36;
		signal[i].re = alpha + 0.3 * beta;      signal[i].im = -alpha - 0.3 * beta;

		i = 52;
		signal[i].re = -alpha + 0.3 * beta;     signal[i].im = -alpha - 0.3 * beta;
		i = 53;
		signal[i].re = -alpha + 0.3 * beta;     signal[i].im = -alpha - 0.1 * beta;
		i = 49;
		signal[i].re = -alpha + 0.3 * beta;      signal[i].im = -alpha + 0.1 * beta;
		i = 48;
		signal[i].re = -alpha + 0.3 * beta;      signal[i].im = -alpha + 0.3 * beta;
	}
	else
	{
		printf("constration number %d is not supported yet\n", M); exit(1);
	}
	for (i = 0; i<M; i++)
	{
		signal[i].re += shift;
		signal[i].im += shift;
	}
}

void mapping_SUPER_16QAM(Complex<double> *signal, double a_s, int M, double shift){
	int i;
    double alpha = 1 - a_s, beta = a_s;
    
	if (M == 16)
	{

		i = 0;
		signal[i].re = alpha + beta;     signal[i].im = alpha + beta;
		i = 1;
		signal[i].re = alpha + beta;     signal[i].im = alpha - beta;
		i = 5;
		signal[i].re = alpha + beta;      signal[i].im = - alpha - beta;
		i = 4;
		signal[i].re = alpha + beta;      signal[i].im = - alpha + beta;

		i = 2;
		signal[i].re = alpha - beta;     signal[i].im = alpha + beta;
		i = 3;
		signal[i].re = alpha - beta;     signal[i].im = alpha - beta;
		i = 7;
		signal[i].re = alpha - beta;      signal[i].im = - alpha - beta;
		i = 6;
		signal[i].re = alpha - beta;      signal[i].im = - alpha + beta;

		i = 10;
		signal[i].re = - alpha - beta;     signal[i].im = alpha + beta;
		i = 11;
		signal[i].re = - alpha - beta;     signal[i].im = alpha - beta;
		i = 15;
		signal[i].re = - alpha - beta;      signal[i].im = - alpha - beta;
		i = 14;
		signal[i].re = - alpha - beta;      signal[i].im = - alpha + beta;

		i = 8;
		signal[i].re = - alpha + beta;     signal[i].im = alpha + beta;
		i = 9;
		signal[i].re = - alpha + beta;     signal[i].im = alpha - beta;
		i = 13;
		signal[i].re = - alpha + beta;      signal[i].im = - alpha - beta;
		i = 12;
		signal[i].re = - alpha + beta;      signal[i].im = - alpha + beta;
	}

	else
	{
		printf("constration number %d is not supported yet\n", M); exit(1);
	}

	for (i = 0; i<M; i++)
	{
		signal[i].re += shift;
		signal[i].im += shift;
	}

}

void mapping_qam(Complex<double> *signal, double a_s, int M, double shift)
{

	int i;
	if (M == 16)
	{

		i = 0;
		signal[i].re = 3.0*a_s;     signal[i].im = 3.0*a_s;
		i = 1;
		signal[i].re = 3.0*a_s;     signal[i].im = 1.0*a_s;
		i = 5;
		signal[i].re = 3.0*a_s;      signal[i].im = -1.0*a_s;
		i = 4;
		signal[i].re = 3.0*a_s;      signal[i].im = -3.0*a_s;

		i = 2;
		signal[i].re = 1.0*a_s;     signal[i].im = 3.0*a_s;
		i = 3;
		signal[i].re = 1.0*a_s;     signal[i].im = 1.0*a_s;
		i = 7;
		signal[i].re = 1.0*a_s;      signal[i].im = -1.0*a_s;
		i = 6;
		signal[i].re = 1.0*a_s;      signal[i].im = -3.0*a_s;

		i = 10;
		signal[i].re = -1.0*a_s;     signal[i].im = 3.0*a_s;
		i = 11;
		signal[i].re = -1.0*a_s;     signal[i].im = 1.0*a_s;
		i = 15;
		signal[i].re = -1.0*a_s;      signal[i].im = -1.0*a_s;
		i = 14;
		signal[i].re = -1.0*a_s;      signal[i].im = -3.0*a_s;

		i = 8;
		signal[i].re = -3.0*a_s;     signal[i].im = 3.0*a_s;
		i = 9;
		signal[i].re = -3.0*a_s;     signal[i].im = 1.0*a_s;
		i = 13;
		signal[i].re = -3.0*a_s;      signal[i].im = -1.0*a_s;
		i = 12;
		signal[i].re = -3.0*a_s;      signal[i].im = -3.0*a_s;
	}
	else if (M == 64)
	{//=================수정해야함==============================
		i = 0;
		signal[i].re = 7.0*a_s;     signal[i].im = 7.0*a_s;
		i = 1;
		signal[i].re = 7.0*a_s;     signal[i].im = 5.0*a_s;
		i = 5;
		signal[i].re = 7.0*a_s;      signal[i].im = 3.0*a_s;
		i = 4;
		signal[i].re = 7.0*a_s;      signal[i].im = 1.0*a_s;

		i = 20;
		signal[i].re = 7.0*a_s;     signal[i].im = -1.0*a_s;
		i = 21;
		signal[i].re = 7.0*a_s;     signal[i].im = -3.0*a_s;
		i = 17;
		signal[i].re = 7.0*a_s;      signal[i].im = -5.0*a_s;
		i = 16;
		signal[i].re = 7.0*a_s;      signal[i].im = -7.0*a_s;

		i = 2;
		signal[i].re = 5.0*a_s;     signal[i].im = 7.0*a_s;
		i = 3;
		signal[i].re = 5.0*a_s;     signal[i].im = 5.0*a_s;
		i = 7;
		signal[i].re = 5.0*a_s;      signal[i].im = 3.0*a_s;
		i = 6;
		signal[i].re = 5.0*a_s;      signal[i].im = 1.0*a_s;

		i = 22;
		signal[i].re = 5.0*a_s;     signal[i].im = -1.0*a_s;
		i = 23;
		signal[i].re = 5.0*a_s;     signal[i].im = -3.0*a_s;
		i = 19;
		signal[i].re = 5.0*a_s;      signal[i].im = -5.0*a_s;
		i = 18;
		signal[i].re = 5.0*a_s;      signal[i].im = -7.0*a_s;

		i = 10;
		signal[i].re = 3.0*a_s;     signal[i].im = 7.0*a_s;
		i = 11;
		signal[i].re = 3.0*a_s;     signal[i].im = 5.0*a_s;
		i = 15;
		signal[i].re = 3.0*a_s;      signal[i].im = 3.0*a_s;
		i = 14;
		signal[i].re = 3.0*a_s;      signal[i].im = 1.0*a_s;

		i = 30;
		signal[i].re = 3.0*a_s;     signal[i].im = -1.0*a_s;
		i = 31;
		signal[i].re = 3.0*a_s;     signal[i].im = -3.0*a_s;
		i = 27;
		signal[i].re = 3.0*a_s;      signal[i].im = -5.0*a_s;
		i = 26;
		signal[i].re = 3.0*a_s;      signal[i].im = -7.0*a_s;

		i = 8;
		signal[i].re = 1.0*a_s;     signal[i].im = 7.0*a_s;
		i = 9;
		signal[i].re = 1.0*a_s;     signal[i].im = 5.0*a_s;
		i = 13;
		signal[i].re = 1.0*a_s;      signal[i].im = 3.0*a_s;
		i = 12;
		signal[i].re = 1.0*a_s;      signal[i].im = 1.0*a_s;

		i = 28;
		signal[i].re = 1.0*a_s;     signal[i].im = -1.0*a_s;
		i = 29;
		signal[i].re = 1.0*a_s;     signal[i].im = -3.0*a_s;
		i = 25;
		signal[i].re = 1.0*a_s;      signal[i].im = -5.0*a_s;
		i = 24;
		signal[i].re = 1.0*a_s;      signal[i].im = -7.0*a_s;

		i = 40;
		signal[i].re = -1.0*a_s;     signal[i].im = 7.0*a_s;
		i = 41;
		signal[i].re = -1.0*a_s;     signal[i].im = 5.0*a_s;
		i = 45;
		signal[i].re = -1.0*a_s;      signal[i].im = 3.0*a_s;
		i = 44;
		signal[i].re = -1.0*a_s;      signal[i].im = 1.0*a_s;

		i = 60;
		signal[i].re = -1.0*a_s;     signal[i].im = -1.0*a_s;
		i = 61;
		signal[i].re = -1.0*a_s;     signal[i].im = -3.0*a_s;
		i = 57;
		signal[i].re = -1.0*a_s;      signal[i].im = -5.0*a_s;
		i = 56;
		signal[i].re = -1.0*a_s;      signal[i].im = -7.0*a_s;

		i = 42;
		signal[i].re = -3.0*a_s;     signal[i].im = 7.0*a_s;
		i = 43;
		signal[i].re = -3.0*a_s;     signal[i].im = 5.0*a_s;
		i = 47;
		signal[i].re = -3.0*a_s;      signal[i].im = 3.0*a_s;
		i = 46;
		signal[i].re = -3.0*a_s;      signal[i].im = 1.0*a_s;

		i = 62;
		signal[i].re = -3.0*a_s;     signal[i].im = -1.0*a_s;
		i = 63;
		signal[i].re = -3.0*a_s;     signal[i].im = -3.0*a_s;
		i = 59;
		signal[i].re = -3.0*a_s;      signal[i].im = -5.0*a_s;
		i = 58;
		signal[i].re = -3.0*a_s;      signal[i].im = -7.0*a_s;

		i = 34;
		signal[i].re = -5.0*a_s;     signal[i].im = 7.0*a_s;
		i = 35;
		signal[i].re = -5.0*a_s;     signal[i].im = 5.0*a_s;
		i = 39;
		signal[i].re = -5.0*a_s;      signal[i].im = 3.0*a_s;
		i = 38;
		signal[i].re = -5.0*a_s;      signal[i].im = 1.0*a_s;

		i = 54;
		signal[i].re = -5.0*a_s;     signal[i].im = -1.0*a_s;
		i = 55;
		signal[i].re = -5.0*a_s;     signal[i].im = -3.0*a_s;
		i = 51;
		signal[i].re = -5.0*a_s;      signal[i].im = -5.0*a_s;
		i = 50;
		signal[i].re = -5.0*a_s;      signal[i].im = -7.0*a_s;

		i = 32;
		signal[i].re = -7.0*a_s;     signal[i].im = 7.0*a_s;
		i = 33;
		signal[i].re = -7.0*a_s;     signal[i].im = 5.0*a_s;
		i = 37;
		signal[i].re = -7.0*a_s;      signal[i].im = 3.0*a_s;
		i = 36;
		signal[i].re = -7.0*a_s;      signal[i].im = 1.0*a_s;

		i = 52;
		signal[i].re = -7.0*a_s;     signal[i].im = -1.0*a_s;
		i = 53;
		signal[i].re = -7.0*a_s;     signal[i].im = -3.0*a_s;
		i = 49;
		signal[i].re = -7.0*a_s;      signal[i].im = -5.0*a_s;
		i = 48;
		signal[i].re = -7.0*a_s;      signal[i].im = -7.0*a_s;
	}
	else
	{
		printf("constration number %d is not supported yet\n", M); exit(1);
	}
	for (i = 0; i<M; i++)
	{
		signal[i].re += shift;
		signal[i].im += shift;
	}
}

void mapping_IQqam(Complex<double>* signal, double a_s, int M, double shift)
{//쓰려면 손봐야함.
	int i;
	if (M == 16)
	{
		i = 0;
		signal[i].re = 3.0*a_s;     signal[i].im = 3.0*a_s;
		i = 1;
		signal[i].re = 3.0*a_s;     signal[i].im = 1.0*a_s;
		i = 5;
		signal[i].re = 3.0*a_s;      signal[i].im = -1.0*a_s;
		i = 4;
		signal[i].re = 3.0*a_s;      signal[i].im = -3.0*a_s;

		i = 2;
		signal[i].re = 1.0*a_s;     signal[i].im = 3.0*a_s;
		i = 3;
		signal[i].re = 1.0*a_s;     signal[i].im = 1.0*a_s;
		i = 7;
		signal[i].re = 1.0*a_s;      signal[i].im = -1.0*a_s;
		i = 6;
		signal[i].re = 1.0*a_s;      signal[i].im = -3.0*a_s;

		i = 10;
		signal[i].re = -1.0*a_s;     signal[i].im = 3.0*a_s;
		i = 11;
		signal[i].re = -1.0*a_s;     signal[i].im = 1.0*a_s;
		i = 15;
		signal[i].re = -1.0*a_s;      signal[i].im = -1.0*a_s;
		i = 14;
		signal[i].re = -1.0*a_s;      signal[i].im = -3.0*a_s;

		i = 8;
		signal[i].re = -3.0*a_s;     signal[i].im = 3.0*a_s;
		i = 9;
		signal[i].re = -3.0*a_s;     signal[i].im = 1.0*a_s;
		i = 13;
		signal[i].re = -3.0*a_s;      signal[i].im = -1.0*a_s;
		i = 12;
		signal[i].re = -3.0*a_s;      signal[i].im = -3.0*a_s;
	}
	else if (M == 64)
	{//=================수정해야함==============================
		i = 0;
		signal[i].re = 7.0*a_s;     signal[i].im = 7.0*a_s;
		i = 1;
		signal[i].re = 7.0*a_s;     signal[i].im = 5.0*a_s;
		i = 5;
		signal[i].re = 7.0*a_s;      signal[i].im = 3.0*a_s;
		i = 4;
		signal[i].re = 7.0*a_s;      signal[i].im = 1.0*a_s;

		i = 20;
		signal[i].re = 7.0*a_s;     signal[i].im = -1.0*a_s;
		i = 21;
		signal[i].re = 7.0*a_s;     signal[i].im = -3.0*a_s;
		i = 17;
		signal[i].re = 7.0*a_s;      signal[i].im = -5.0*a_s;
		i = 16;
		signal[i].re = 7.0*a_s;      signal[i].im = -7.0*a_s;

		i = 2;
		signal[i].re = 5.0*a_s;     signal[i].im = 7.0*a_s;
		i = 3;
		signal[i].re = 5.0*a_s;     signal[i].im = 5.0*a_s;
		i = 7;
		signal[i].re = 5.0*a_s;      signal[i].im = 3.0*a_s;
		i = 6;
		signal[i].re = 5.0*a_s;      signal[i].im = 1.0*a_s;

		i = 22;
		signal[i].re = 5.0*a_s;     signal[i].im = -1.0*a_s;
		i = 23;
		signal[i].re = 5.0*a_s;     signal[i].im = -3.0*a_s;
		i = 19;
		signal[i].re = 5.0*a_s;      signal[i].im = -5.0*a_s;
		i = 18;
		signal[i].re = 5.0*a_s;      signal[i].im = -7.0*a_s;

		i = 10;
		signal[i].re = 3.0*a_s;     signal[i].im = 7.0*a_s;
		i = 11;
		signal[i].re = 3.0*a_s;     signal[i].im = 5.0*a_s;
		i = 15;
		signal[i].re = 3.0*a_s;      signal[i].im = 3.0*a_s;
		i = 14;
		signal[i].re = 3.0*a_s;      signal[i].im = 1.0*a_s;

		i = 30;
		signal[i].re = 3.0*a_s;     signal[i].im = -1.0*a_s;
		i = 31;
		signal[i].re = 3.0*a_s;     signal[i].im = -3.0*a_s;
		i = 27;
		signal[i].re = 3.0*a_s;      signal[i].im = -5.0*a_s;
		i = 26;
		signal[i].re = 3.0*a_s;      signal[i].im = -7.0*a_s;

		i = 8;
		signal[i].re = 1.0*a_s;     signal[i].im = 7.0*a_s;
		i = 9;
		signal[i].re = 1.0*a_s;     signal[i].im = 5.0*a_s;
		i = 13;
		signal[i].re = 1.0*a_s;      signal[i].im = 3.0*a_s;
		i = 12;
		signal[i].re = 1.0*a_s;      signal[i].im = 1.0*a_s;

		i = 28;
		signal[i].re = 1.0*a_s;     signal[i].im = -1.0*a_s;
		i = 29;
		signal[i].re = 1.0*a_s;     signal[i].im = -3.0*a_s;
		i = 25;
		signal[i].re = 1.0*a_s;      signal[i].im = -5.0*a_s;
		i = 24;
		signal[i].re = 1.0*a_s;      signal[i].im = -7.0*a_s;

		i = 40;
		signal[i].re = -1.0*a_s;     signal[i].im = 7.0*a_s;
		i = 41;
		signal[i].re = -1.0*a_s;     signal[i].im = 5.0*a_s;
		i = 45;
		signal[i].re = -1.0*a_s;      signal[i].im = 3.0*a_s;
		i = 44;
		signal[i].re = -1.0*a_s;      signal[i].im = 1.0*a_s;

		i = 60;
		signal[i].re = -1.0*a_s;     signal[i].im = -1.0*a_s;
		i = 61;
		signal[i].re = -1.0*a_s;     signal[i].im = -3.0*a_s;
		i = 57;
		signal[i].re = -1.0*a_s;      signal[i].im = -5.0*a_s;
		i = 56;
		signal[i].re = -1.0*a_s;      signal[i].im = -7.0*a_s;

		i = 42;
		signal[i].re = -3.0*a_s;     signal[i].im = 7.0*a_s;
		i = 43;
		signal[i].re = -3.0*a_s;     signal[i].im = 5.0*a_s;
		i = 47;
		signal[i].re = -3.0*a_s;      signal[i].im = 3.0*a_s;
		i = 46;
		signal[i].re = -3.0*a_s;      signal[i].im = 1.0*a_s;

		i = 62;
		signal[i].re = -3.0*a_s;     signal[i].im = -1.0*a_s;
		i = 63;
		signal[i].re = -3.0*a_s;     signal[i].im = -3.0*a_s;
		i = 59;
		signal[i].re = -3.0*a_s;      signal[i].im = -5.0*a_s;
		i = 58;
		signal[i].re = -3.0*a_s;      signal[i].im = -7.0*a_s;

		i = 34;
		signal[i].re = -5.0*a_s;     signal[i].im = 7.0*a_s;
		i = 35;
		signal[i].re = -5.0*a_s;     signal[i].im = 5.0*a_s;
		i = 39;
		signal[i].re = -5.0*a_s;      signal[i].im = 3.0*a_s;
		i = 38;
		signal[i].re = -5.0*a_s;      signal[i].im = 1.0*a_s;

		i = 54;
		signal[i].re = -5.0*a_s;     signal[i].im = -1.0*a_s;
		i = 55;
		signal[i].re = -5.0*a_s;     signal[i].im = -3.0*a_s;
		i = 51;
		signal[i].re = -5.0*a_s;      signal[i].im = -5.0*a_s;
		i = 50;
		signal[i].re = -5.0*a_s;      signal[i].im = -7.0*a_s;

		i = 32;
		signal[i].re = -7.0*a_s;     signal[i].im = 7.0*a_s;
		i = 33;
		signal[i].re = -7.0*a_s;     signal[i].im = 5.0*a_s;
		i = 37;
		signal[i].re = -7.0*a_s;      signal[i].im = 3.0*a_s;
		i = 36;
		signal[i].re = -7.0*a_s;      signal[i].im = 1.0*a_s;

		i = 52;
		signal[i].re = -7.0*a_s;     signal[i].im = -1.0*a_s;
		i = 53;
		signal[i].re = -7.0*a_s;     signal[i].im = -3.0*a_s;
		i = 49;
		signal[i].re = -7.0*a_s;      signal[i].im = -5.0*a_s;
		i = 48;
		signal[i].re = -7.0*a_s;      signal[i].im = -7.0*a_s;
	}
	else
	{
		printf("constration number %d is not supported yet\n", M); exit(1);
	}
	for (i = 0; i<M; i++)
	{
		signal[i].re += shift;
		signal[i].im += shift;
	}
}


void mapping_psk(Complex<double> *signal, double a_s, int M, int d)
{
	int i;
	int j[16];	// max 16PSK is possible, this should be increased to adopt higher order modulation
	if (M == 2)
	{
		i = 0;
		signal[i].re = 1.0*a_s;     signal[i].im = 0.0*a_s;
		i = 1;
		signal[i].re = -1.0*a_s;     signal[i].im = 0.0*a_s;
	}
	else if (M == 4)
	{
		/*j[0] = 0; j[1] = 2; j[2] = 3; j[3] = 1;
		for (i = 0; i<4; i++) {
		signal[j[i]].re = cos((i)*PI / 2 + PI / 4.0)*a_s;
		signal[j[i]].im = sin((i)*PI / 2 + PI / 4.0)*a_s;
		}*/
		i = 0;
		signal[i].re = 1.0*a_s;     signal[i].im = 1.0*a_s;
		i = 1;
		signal[i].re = 1.0*a_s;     signal[i].im = -1.0*a_s;
		i = 2;
		signal[i].re = -1.0*a_s;     signal[i].im = 1.0*a_s;
		i = 3;
		signal[i].re = -1.0*a_s;     signal[i].im = -1.0*a_s;




	}
	else if (M == 8)
	{//수정해야함
		j[0] = 0; j[1] = 1; j[2] = 3; j[3] = 2;
		j[4] = 6; j[5] = 7; j[6] = 5; j[7] = 4;
		for (i = 0; i<8; i++) {
			signal[j[i]].re = cos(i*PI / 4 + PI / 8)*a_s;
			signal[j[i]].im = sin(i*PI / 4 + PI / 8)*a_s;
		}
	}
	else
	{
		printf("psk %d is not supported yet\n", M); exit(1);
	}
}

void mapping_InPhase_pam(Complex<double>* signal, double a_s, int M, int d)
{
	int i;
	int j[16];	// max 16PSK is possible, this should be increased to adopt higher order modulation

	double dist = a_s / 2;

	i = 0;
	signal[i].re = a_s + dist;     signal[i].im = 0;
	i = 1;
	signal[i].re = a_s - dist;     signal[i].im = 0;
	i = 2;
	signal[i].re = - a_s - dist;     signal[i].im = 0;
	i = 3;
	signal[i].re = - a_s + dist;     signal[i].im = 0;
}

void mapping_Quad_pam(Complex<double>* signal, double a_s, int M, int d)
{
	int i;
	int j[16];	// max 16PSK is possible, this should be increased to adopt higher order modulation

	double dist = a_s / 2;

	i = 0;
	signal[i].re = 0;     signal[i].im = a_s + dist;
	i = 1;
	signal[i].re = 0;     signal[i].im = a_s - dist;
	i = 2;
	signal[i].re = 0;     signal[i].im = - a_s - dist;
	i = 3;
	signal[i].re = 0;     signal[i].im = - a_s + dist;
}

//for Phase shift superposition scenario
#define NEITHER 1
#define RELAY 2
#define SOURCE 3

#define SCENARIO RELAY



void Mapper::PSK8_Mapping(vector<bool> source, vector<bool> relay, vector<Complex<double>>& result){

    int mod = source.size() % 2;
    //Mapper tmp;

	while (mod != 0) {
		source.push_back(0);
		relay.push_back(0);
		mod--;
	}

    int last = source.size()/2;    
    result.resize(last);
    
	PSK8LUT.resize(2);
    for(int i = 0; i < 2; i++){
        PSK8LUT[i].resize(2);
        for(int j = 0; j < 2; j++){
            PSK8LUT[i][j].resize(2);
            for(int k = 0; k < 2; k++){
                PSK8LUT[i][j][k].resize(2);
            }
        }
    }

#if SCENARIO == NEITHER
    PSK8LUT[0][0][0][0] = Complex<double>(1,1);
    PSK8LUT[0][0][0][1] = Complex<double>(SQRT2,0);    
    PSK8LUT[0][0][1][0] = Complex<double>(0,SQRT2);
    PSK8LUT[0][0][1][1] = Complex<double>(1,-1);
    PSK8LUT[0][1][0][0] = Complex<double>(SQRT2,0);
    PSK8LUT[0][1][0][1] = Complex<double>(1,-1);
    PSK8LUT[0][1][1][0] = Complex<double>(-1,-1);
    PSK8LUT[0][1][1][1] = Complex<double>(0,-SQRT2);
    PSK8LUT[1][0][0][0] = Complex<double>(0,SQRT2);
    PSK8LUT[1][0][0][1] = Complex<double>(1,1);
    PSK8LUT[1][0][1][0] = Complex<double>(-1,1);
    PSK8LUT[1][0][1][1] = Complex<double>(-SQRT2,0);
    PSK8LUT[1][1][0][0] = Complex<double>(-1,1);
    PSK8LUT[1][1][0][1] = Complex<double>(0,-SQRT2);
    PSK8LUT[1][1][1][0] = Complex<double>(-SQRT2,0);
    PSK8LUT[1][1][1][1] = Complex<double>(-1,-1);

    for(int i = 0; i < last; i++){
        result[i] = PSK8LUT[source[2*i]][source[2*i+1]][relay[2*i]][relay[2*i+1]];
    }
    
#elif SCENARIO == RELAY
    PSK8LUT[0][0][0][0] = Complex<double>(1,1);
    PSK8LUT[0][1][0][0] = Complex<double>(1,1);    
    PSK8LUT[1][0][0][0] = Complex<double>(0,SQRT2);
    PSK8LUT[1][1][0][0] = Complex<double>(0,SQRT2);
    PSK8LUT[0][0][0][1] = Complex<double>(SQRT2,0);
    PSK8LUT[0][1][0][1] = Complex<double>(1,-1);
    PSK8LUT[1][0][0][1] = Complex<double>(SQRT2,0);
    PSK8LUT[1][1][0][1] = Complex<double>(1,-1);
    PSK8LUT[0][0][1][0] = Complex<double>(-1,1);
    PSK8LUT[0][1][1][0] = Complex<double>(-SQRT2,0);
    PSK8LUT[1][0][1][0] = Complex<double>(-1,1);
    PSK8LUT[1][1][1][0] = Complex<double>(-SQRT2,0);
    PSK8LUT[0][0][1][1] = Complex<double>(0,-SQRT2);
    PSK8LUT[0][1][1][1] = Complex<double>(0,-SQRT2);
    PSK8LUT[1][0][1][1] = Complex<double>(-1,-1);
    PSK8LUT[1][1][1][1] = Complex<double>(-1,-1);

    for(int i = 0; i < last; i++){
        result[i] = PSK8LUT[relay[2*i]][relay[2*i+1]][source[2*i]][source[2*i+1]];
    }
    
#elif SCENARIO == SOURCE

#endif


    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 2; j++){
            for(int k = 0; k < 2; k++){
                for(int l = 0; l < 2; l++){
                    rotate(PSK8LUT[i][j][k][l], 22.5);
                }
            }
        }
    }

    for(int i = 0; i < last; i++){
        result[i] = PSK8LUT[source[2*i]][source[2*i+1]][relay[2*i]][relay[2*i+1]];
    }

    //vector<Complex<double>>(0).swap(PSK8LUT);
}


void Mapper::PSK8_Demapping(vector<bool> encoded, vector<Complex<double>> &result){
    int size = result.size();
    for(int i = 0; i < size; i ++){
        if(!encoded[2*i] && !encoded[2*i+1]){   //encoded equals to 00
            if(result[i].re > 0){   //symbol is in the quadrant1
                rotate(result[i],-67.5);
            }

            else{   //symbol is in the quadrant2
                rotate(result[i],67.5);
            }
        }
        else if(!encoded[2*i] && encoded[2*i+1]){   //encoded equals to 01
            if(result[i].im > 0){   //symbol is in the quadrant1
                rotate(result[i], 67.5);
            }

            else{   //symbol is in the quadrant4
                rotate(result[i], -67.5);
            }
        }
        else if(encoded[2*i] && !encoded[2*i+1]){   //encoded equals to 10
            if(result[i].im > 0){   //symbol is in the quadrant2
                rotate(result[i], -67.5);
            }

            else{   //symbol is in the quadrant3
                rotate(result[i], 67.5);
            }
        }
        else{   //encoded equals to 11
            if(result[i].re > 0){   //symbol is in the quadrant4
                rotate(result[i], 67.5);
            }

            else{   //symbol is in the quadrant3
                rotate(result[i], -67.5);
            }
        }
    }
}

void Mapper::PSK16_Mapping(vector<bool> source, vector<bool> relay, vector<Complex<double>>& result){

    int mod = source.size() % 2;
    //Mapper tmp;

	while (mod != 0) {
		source.push_back(0);
		relay.push_back(0);
		mod--;
	}

    int last = source.size()/2;    
    result.resize(last);
    
	PSK8LUT.resize(2);
    for(int i = 0; i < 2; i++){
        PSK8LUT[i].resize(2);
        for(int j = 0; j < 2; j++){
            PSK8LUT[i][j].resize(2);
            for(int k = 0; k < 2; k++){
                PSK8LUT[i][j][k].resize(2);
            }
        }
    }


    PSK8LUT[0][0][0][0] = Complex<double>(SQRT2,0);
    PSK8LUT[0][1][0][0] = Complex<double>(SQRT2,0);
    PSK8LUT[1][0][0][0] = Complex<double>(SQRT2,0);
    PSK8LUT[1][1][0][0] = Complex<double>(SQRT2,0);
    PSK8LUT[0][0][0][1] = Complex<double>(SQRT2,0);
    PSK8LUT[0][1][0][1] = Complex<double>(SQRT2,0);
    PSK8LUT[1][0][0][1] = Complex<double>(SQRT2,0);
    PSK8LUT[1][1][0][1] = Complex<double>(SQRT2,0);
    PSK8LUT[0][0][1][0] = Complex<double>(SQRT2,0);
    PSK8LUT[0][1][1][0] = Complex<double>(SQRT2,0);
    PSK8LUT[1][0][1][0] = Complex<double>(SQRT2,0);
    PSK8LUT[1][1][1][0] = Complex<double>(SQRT2,0);
    PSK8LUT[0][0][1][1] = Complex<double>(SQRT2,0);
    PSK8LUT[0][1][1][1] = Complex<double>(SQRT2,0);
    PSK8LUT[1][0][1][1] = Complex<double>(SQRT2,0);
    PSK8LUT[1][1][1][1] = Complex<double>(SQRT2,0);

#if 0 
    rotate(PSK8LUT[0][0][0][0],80);
    rotate(PSK8LUT[0][0][0][1],60);
    rotate(PSK8LUT[0][0][1][0],100);
    rotate(PSK8LUT[0][0][1][1],120);
    rotate(PSK8LUT[0][1][0][0],20);
    rotate(PSK8LUT[0][1][0][1],-20);
    rotate(PSK8LUT[0][1][1][0],40);
    rotate(PSK8LUT[0][1][1][1],-40);
    rotate(PSK8LUT[1][0][0][0],140);
    rotate(PSK8LUT[1][0][0][1],-140);
    rotate(PSK8LUT[1][0][1][0],160);
    rotate(PSK8LUT[1][0][1][1],-160);
    rotate(PSK8LUT[1][1][0][0],-60);
    rotate(PSK8LUT[1][1][0][1],-80);
    rotate(PSK8LUT[1][1][1][0],-120);
    rotate(PSK8LUT[1][1][1][1],-100);
#endif    

    double a = 9.0, b = 27.0;
    
#if 1
    rotate(PSK8LUT[0][0][0][0],90-a);
    rotate(PSK8LUT[0][0][0][1],90-b);
    rotate(PSK8LUT[0][0][1][0],90+a);
    rotate(PSK8LUT[0][0][1][1],90+b);
    rotate(PSK8LUT[0][1][0][0],a);
    rotate(PSK8LUT[0][1][0][1],-a);
    rotate(PSK8LUT[0][1][1][0],b);
    rotate(PSK8LUT[0][1][1][1],-b);
    rotate(PSK8LUT[1][0][0][0],180-b);
    rotate(PSK8LUT[1][0][0][1],-180+b);
    rotate(PSK8LUT[1][0][1][0],180-a);
    rotate(PSK8LUT[1][0][1][1],-180+a);
    rotate(PSK8LUT[1][1][0][0],-90+b);
    rotate(PSK8LUT[1][1][0][1],-90+a);
    rotate(PSK8LUT[1][1][1][0],-90-b);
    rotate(PSK8LUT[1][1][1][1],-90-a);
#endif
    
    for(int i = 0; i < last; i++){
        result[i] = PSK8LUT[relay[2*i]][relay[2*i+1]][source[2*i]][source[2*i+1]];
    }

    //vector<Complex<double>>(0).swap(PSK8LUT);
}

void Mapper::PSK16_Phase_Extension(vector<bool> encoded, vector<Complex<double>> &result){
    vector<double> theta;

    int last = result.size();

    theta.resize(last);
    
    for(int i = 0; i < last; i++){
		theta[i] = atan2(result[i].im, result[i].re) / PI * 180.0;
    }

    double scale = 4;
    
    for(int i = 0; i < last; i ++){
        if(!encoded[2*i] && !encoded[2*i+1]){   //encoded equals to 00
            if(result[i].re > 0){   //symbol is in the quadrant1
                rotate(result[i],-(90-theta[i])*scale);
            }

            else{   //symbol is in the quadrant2
                rotate(result[i],(theta[i]-90)*scale);
            }
        }
        else if(!encoded[2*i] && encoded[2*i+1]){   //encoded equals to 01
            if(result[i].im > 0){   //symbol is in the quadrant1
                rotate(result[i], theta[i]*scale);
            }

            else{   //symbol is in the quadrant4
                rotate(result[i], theta[i]*scale);
            }
        }
        else if(encoded[2*i] && !encoded[2*i+1]){   //encoded equals to 10
            if(result[i].im > 0){   //symbol is in the quadrant2
                rotate(result[i], -(180-theta[i])*scale);
            }

            else{   //symbol is in the quadrant3
                rotate(result[i], (180+theta[i])*scale);
            }
        }
        else{   //encoded equals to 11
            if(result[i].re > 0){   //symbol is in the quadrant4
                rotate(result[i], (90+theta[i])*scale);
            }

            else{   //symbol is in the quadrant3
                rotate(result[i], (90+theta[i])*scale);
            }
        }
    }
}

void Mapper::BPSK_Mapping(vector<bool> a, vector<Complex<double>>& result)
{
	result.resize(a.size());
	PSKLUT = (double*)malloc(2 * sizeof(double));

	PSKLUT[0] = -1;
	PSKLUT[1] = 1;
	for (int i = 0; i < a.size(); i++) {
		result[i].re = PSKLUT[a[i]];
		result[i].im = 0;
	}
	free(PSKLUT);

}

void Mapper::QPSK_Mapping(vector<bool> a, vector<Complex<double>>& result)
{
	int mod = a.size() % 2;


	while (mod != 0) {
		a.push_back(0);
		mod--;
	}
	int m2 = a.size() / 2;

	result.resize(m2);
	PSKLUT = (double*)malloc(2 * sizeof(double));

	PSKLUT[0] = 1;
	PSKLUT[1] = -1;

	for (int i = 0; i < m2; i++) {
		result[i].re = PSKLUT[a[2 * i]];
		result[i].im = PSKLUT[a[2 * i + 1]];
	}
	free(PSKLUT);

}

void Mapper::QPSK_Mapping(vector<bool> a, vector<Complex<double>> &result, double val)
{
	int mod = a.size() % 2;


	while (mod != 0) {
		a.push_back(0);
		mod--;
	}
	int m2 = a.size() / 2;

	result.resize(m2);
	PSKLUT = (double*)malloc(2 * sizeof(double));

	PSKLUT[0] = val;
	PSKLUT[1] = -val;

	for (int i = 0; i < m2; i++) {
		result[i].re = PSKLUT[a[2 * i]];
		result[i].im = PSKLUT[a[2 * i + 1]];
	}
	free(PSKLUT);
}

void Mapper::PAM_InPhase_Mapping(vector<bool> a, vector<Complex<double>>& result, double val)
{
	int mod = a.size() % 2;

	while (mod != 0) {
		a.push_back(0);
		mod--;
	}
	int m2 = a.size() / 2;

	result.resize(m2);

	PAMLUT.resize(2);
	for (int l = 0; l < 2; l++)
		PAMLUT[l].resize(2);

	double d = val / 2;


	PAMLUT[0][0] = val + d;
	PAMLUT[0][1] = val - d;
	PAMLUT[1][0] = -val - d;
	PAMLUT[1][1] = -val + d;

	for (int i = 0; i < m2; i++) {
		result[i].re = PAMLUT[a[2 * i]][a[2 * i + 1]];
		result[i].im = 0;
	}
	vector<vector<double>>(0).swap(PAMLUT);
}

void Mapper::PAM_Quad_Mapping(vector<bool> a, vector<Complex<double>> &result, double val)
{
	int mod = a.size() % 2;

	while (mod != 0) {
		a.push_back(0);
		mod--;
	}
	int m2 = a.size() / 2;

	result.resize(m2);

	PAMLUT.resize(2);
	for (int l = 0; l < 2; l++)
		PAMLUT[l].resize(2);

	double d = val / 2;
	

	PAMLUT[0][0] = val + d;
	PAMLUT[0][1] = val - d;
	PAMLUT[1][0] = -val - d;
	PAMLUT[1][1] = -val + d;

	for (int i = 0; i < m2; i++) {
		result[i].re = 0;
		result[i].im = PAMLUT[a[2 * i]][a[2 * i + 1]];
	}
	vector<vector<double>>(0).swap(PAMLUT);
}

void Mapper::QAM_Mapping(vector<bool> a, vector<Complex<double>>& result)
{
	int mod = a.size() % 4;

	while (mod != 0) {
		a.push_back(0);
		mod--;
	}

	int m2 = a.size() / 4;
	result.resize(m2);
	QAMLUT = (double**)malloc(2 * sizeof(double *));
	for (int l = 0; l < 2; l++)
		QAMLUT[l] = (double*)malloc(2 * sizeof(double));


	QAMLUT[0][0] = 3; //{ {3,1},{-3,-1} };
	QAMLUT[0][1] = 1;
	QAMLUT[1][0] = -3;
	QAMLUT[1][1] = -1;
	for (int i = 0; i < m2; i++) {
		result[i].re = QAMLUT[a[4 * i]][a[4 * i + 2]];
		result[i].im = QAMLUT[a[4 * i + 1]][a[4 * i + 3]];
	}
	free(QAMLUT);
}

void Mapper::QAM_Mapping(vector<bool> a, vector<Complex<double>> &result, double val)
{
	int mod = a.size() % 4;

	while (mod != 0) {
		a.push_back(0);
		mod--;
	}

	int m2 = a.size() / 4;
	result.resize(m2);
	QAMLUT = (double**)malloc(2 * sizeof(double*));
	for (int l = 0; l < 2; l++)
		QAMLUT[l] = (double*)malloc(2 * sizeof(double));


	QAMLUT[0][0] = 3 * val; //{ {3,1},{-3,-1} };
	QAMLUT[0][1] = 1 * val;
	QAMLUT[1][0] = (-3) * val;
	QAMLUT[1][1] = (-1) * val;

	for (int i = 0; i < m2; i++) {
		result[i].re = QAMLUT[a[4 * i]][a[4 * i + 2]];
		result[i].im = QAMLUT[a[4 * i + 1]][a[4 * i + 3]];
	}
	free(QAMLUT);

}

void Mapper::SUPER_QAM_Mapping(vector<bool> a, double alpha, double beta, vector<Complex<double>>& result)
{
	int mod = a.size() % 4;

	while (mod != 0) {
		a.push_back(0);
		mod--;
	}

	int m2 = a.size() / 4;
	result.resize(m2);
    
    SUPER_QAM_LUT.resize(2);
    for (int l = 0; l < 2; l++)
		SUPER_QAM_LUT[l].resize(2);
    
	SUPER_QAM_LUT[0][0] = alpha + beta;
	SUPER_QAM_LUT[0][1] = alpha - beta;
	SUPER_QAM_LUT[1][0] = - alpha + beta;
	SUPER_QAM_LUT[1][1] = - alpha - beta;
    
	for (int i = 0; i < m2; i++) {
		result[i].re = SUPER_QAM_LUT[a[4 * i]][a[4 * i + 2]];
		result[i].im = SUPER_QAM_LUT[a[4 * i + 1]][a[4 * i + 3]];
	}
    vector<vector<double>>(0).swap(SUPER_QAM_LUT);
}


void Mapper::IQ_Mapping(vector<bool> inPhase, double p_alpha, vector<bool> Quad, double p_beta, vector<Complex<double>>& result)
{
	int mod = inPhase.size() % 4, mod2 = Quad.size() % 4;

	while (mod != 0) {
		inPhase.push_back(0);
		mod--;
	}

	while (mod2 != 0) {
		Quad.push_back(0);
		mod--;
	}

	int m2 = inPhase.size() / 2;
	result.resize(m2);

	QAMLUT = (double**)malloc(2 * sizeof(double*));
	for (int l = 0; l < 2; l++)
		QAMLUT[l] = (double*)malloc(2 * sizeof(double));

	QAMLUT2 = (double**)malloc(2 * sizeof(double*));
	for (int l = 0; l < 2; l++)
		QAMLUT2[l] = (double*)malloc(2 * sizeof(double));

	double d_1 = p_alpha / 2, d_2 = p_beta / 2;
	QAMLUT[0][0] = p_alpha + d_1; //{ {3,1},{-3,-1} };
	QAMLUT[0][1] = p_alpha - d_1;
	QAMLUT[1][0] = -p_alpha - d_1;
	QAMLUT[1][1] = -p_alpha + d_1;

	QAMLUT2[0][0] = p_beta + d_2; //{ {3,1},{-3,-1} };
	QAMLUT2[0][1] = p_beta - d_2;
	QAMLUT2[1][0] = -p_beta - d_2;
	QAMLUT2[1][1] = -p_beta + d_2;

	for (int i = 0; i < m2; i += 2) {
		result[i].re = QAMLUT[inPhase[4 * i]][inPhase[4 * i + 1]];
		result[i + 1].re = QAMLUT[inPhase[4 * i + 2]][inPhase[4 * i + 3]];
		result[i].im = QAMLUT2[Quad[4 * i]][Quad[4 * i + 1]];
		result[i + 1].im = QAMLUT2[Quad[4 * i + 2]][Quad[4 * i + 3]];
	}

	free(QAMLUT);
	free(QAMLUT2);
}


void Mapper::QAM64_Mapping(vector<bool>a, vector<Complex<double>>& result)
{
	int mod = a.size() % 6;

	while (mod != 0) {
		a.push_back(0);
		mod--;
	}

	int m2 = a.size() / 6;
	result.resize(m2);
	/*
	QAM64LUT = (int ***)malloc(2);
	for (int l = 0;l < 2;l++)
	QAM64LUT[l] = (int **)malloc(2);

	for (int j = 0;j < 2;++j)
	{
	for (int h = 0;h < 2;++h)
	QAM64LUT[j][h] = (int*)malloc(2);

	}
	*/


	QAM64LUT[0][0][0] = 7; //{ {3,1},{-3,-1} };
	QAM64LUT[0][0][1] = 5;
	QAM64LUT[0][1][1] = 3;
	QAM64LUT[0][1][0] = 1;
	QAM64LUT[1][1][0] = -1; //{ {3,1},{-3,-1} };
	QAM64LUT[1][1][1] = -3;
	QAM64LUT[1][0][1] = -5;
	QAM64LUT[1][0][0] = -7;

	for (int i = 0; i < m2; i++) {
		result[i].re = QAM64LUT[a[6 * i]][a[6 * i + 2]][a[6 * i + 4]];
		result[i].im = QAM64LUT[a[6 * i + 1]][a[6 * i + 3]][a[6 * i + 5]];
	}
	free(QAM64LUT);


}
void Mapper::BPSK_Demapping(vector<Complex<double>> ndata, int m, vector<bool>& output)
{
	output.resize(m);
	for (int i = 0; i < m; i++) {
		ndata[i].re > 0 ? output[i] = 1 : output[i] = 0;
	}
}

void Mapper::QPSK_Demapping(vector<Complex<double>> ndata, int m, vector<bool>& output)
{
	int m2 = m * 2;
	output.resize(m2);
	for (int i = 0; i < m; i++) {
		ndata[i].re > 0 ? output[2 * i] = 0 : output[2 * i] = 1;
		ndata[i].im > 0 ? output[2 * i + 1] = 0 : output[2 * i + 1] = 1;
	}
}

void Mapper::QAM_Demapping(vector<Complex<double>> ndata, int m, vector<bool>& output)
{
	int m2 = m * 4;
	output.resize(m2);
	for (int i = 0; i < m; i++) {
		ndata[i].re > 0 ? output[4 * i] = 0 : output[4 * i] = 1;
		abs(ndata[i].re) < 2 ? output[4 * i + 2] = 1 : output[4 * i + 2] = 0;
		ndata[i].im > 0 ? output[4 * i + 1] = 0 : output[4 * i + 1] = 1;
		abs(ndata[i].im) < 2 ? output[4 * i + 3] = 1 : output[4 * i + 3] = 0;
	}
}

void Mapper::QAM64_Demapping(vector<Complex<double>> ndata, int m, vector<bool>& output)
{
	int m2 = m * 6;
	output.resize(m2);
	for (int i = 0; i < m; i++) {
		ndata[i].re > 0 ? output[6 * i] = 0 : output[6 * i] = 1;
		abs(ndata[i].re) < 4 ? output[6 * i + 2] = 1 : output[6 * i + 2] = 0;
		((abs(ndata[i].re) < 2) | (abs(ndata[i].re) > 6)) ? output[6 * i + 4] = 0 : output[6 * i + 4] = 1;


		ndata[i].im > 0 ? output[6 * i + 1] = 0 : output[6 * i + 1] = 1;
		abs(ndata[i].im) < 4 ? output[6 * i + 3] = 1 : output[6 * i + 3] = 0;
		((abs(ndata[i].im) < 2) | (abs(ndata[i].im) > 6)) ? output[6 * i + 5] = 0 : output[6 * i + 5] = 1;
	}
}




Mapper::~Mapper()
{
}

void Mapper::Super(vector<Complex<double>> a, vector<Complex<double>>& b)
{
	int size_row = a.size();
	for (int i = 0; i < size_row; i++) {
		b[i] += a[i];
	}
}

void Mapper::Super_Sub(vector<Complex<double>> a, vector<Complex<double>> &b)
{
    int size_row = a.size();
	for (int i = 0; i < size_row; i++) {
		b[i] -= a[i];
	}
}

void Mapper::Interference(vector<Complex<double>> a, vector<Complex<double>>& b)
{
	int size_row = a.size();
	for (int i = 0; i < size_row; i++) {
		b[i] += a[i];
	}
}

void Mapper::Partial_SM(vector<Complex<double>> a, vector<Complex<double>>& b)
{
	//define code rate or modulation order later
	//half interference at the moment
	int size_row = a.size();
	int offset = size_row;
	for (int i = 0; i < offset; i++) {
		b[i + offset] += a[i];
	}
}

void Mapper::Partial_SM_Super_Sub(vector<Complex<double>> a, vector<Complex<double>>& b)
{
	int size_row = a.size();
	for (int i = 0; i < size_row; i++) {
		b[i] -= a[i];
	}
}


void	Mapper::llr_generation(FADING FAD_MOD, vector<Complex<double>> rx_buf, vector<double> llr_wgt_buf, double* rx_llr0, double*rx_llr1, Mapper *M_MAP, int SP_NDCARperSYM)
{
	int	i, j, k;//l;
	int 	bit_flag, kk;// ll;
	double	max1, max0;//, bp_LLR[8][2];
	double	t_LLR[64];

	switch (FAD_MOD) {
	case Rayl:
		// 각 2 bit symbol에 대한 LLR값을 구한다.
		for (i = 0, k = 0; i < SP_NDCARperSYM; i++) {
			// soft metric calculation for 16 QAM points....
			for (j = 0; j < M_MAP->m_N_2d_const; j++) {
				t_LLR[j] = -masq((M_MAP->m_2d_map_tab[j] - rx_buf[i]).re, (M_MAP->m_2d_map_tab[j] - rx_buf[i]).im);
				t_LLR[j] *= (llr_wgt_buf[i]);
			}
			// log max 방법으로 bit plane별로의 LLR값을 구한다.
			bit_flag = 0x01 << (M_MAP->m_Ninfobit - 1);
			// bit plane 별로 max LLR값을 찾는다.
			for (kk = 0; kk < M_MAP->m_Ninfobit; kk++, bit_flag >>= 1) {
				max1 = max0 = (-100000.0);
				for (j = 0; j < M_MAP->m_N_2d_const; j++) {
					if ((j&bit_flag) == bit_flag) {
						if (t_LLR[j] > max1) max1 = t_LLR[j];
					}
					else {
						if (t_LLR[j] > max0) max0 = t_LLR[j];
					}
				}
				rx_llr0[k] = max0;
				rx_llr1[k] = max1;
				k++;
			}
		}
		break;
	case Rayl_Quasi:
		for (i = 0, k = 0; i < SP_NDCARperSYM; i++) {
			// soft metric calculation for 16 QAM points....
			for (j = 0; j < M_MAP->m_N_2d_const; j++) {
				t_LLR[j] = -masq((M_MAP->m_2d_map_tab[j] - rx_buf[i]).re, (M_MAP->m_2d_map_tab[j] - rx_buf[i]).im);
			}
			// log max 방법으로 bit plane별로의 LLR값을 구한다.
			bit_flag = 0x01 << (M_MAP->m_Ninfobit - 1);
			// bit plane 별로 max LLR값을 찾는다.
			for (kk = 0; kk < M_MAP->m_Ninfobit; kk++, bit_flag >>= 1) {
				max1 = max0 = (-100000.0);
				for (j = 0; j < M_MAP->m_N_2d_const; j++) {
					if ((j&bit_flag) == bit_flag) {
						if (t_LLR[j] > max1) max1 = t_LLR[j];
					}
					else {
						if (t_LLR[j] > max0) max0 = t_LLR[j];
					}
				}
				rx_llr0[k] = max0;
				rx_llr1[k] = max1;
				k++;
			}
		}
		break;
	}
}

void Mapper::llr_weighting(vector<double>&llr0, vector<double>&llr1, vector<double>llr_wgt, Mapper *M_Map) {
	for (unsigned int i = 0; i < llr_wgt.size(); ++i)
	{
		for (int k = 0; k < M_Map->m_Ninfobit; ++k)
		{
			llr0[i + k] *= llr_wgt[i] * llr_wgt[k];
			llr1[i + k] *= llr_wgt[i] * llr_wgt[k];

		}

	}

}

void Mapper::llr_Ca(double* llr1, double *llr2, double *llr3, int Bsize)
{
	for (int i = 0; i < Bsize; ++i)
		llr3[i] = llr1[i] - llr2[i];

}



void	Mapper::bit2symLLR_generation(double* rx_llr0_buf, double *rx_llr1_buf, double **LLR, int SP_NCODEBITperSYM, int SP_NCODEBIT, int SP_NCODE)
{
	int	i, j, k, l;
	double	t;
	//=========================================================
	//              Bit LLR to Symbol LLR
	//=========================================================
	for (i = 0, l = 0; i < SP_NCODEBITperSYM; i += SP_NCODEBIT, l++) {
		for (j = 0; j < SP_NCODE; j++) {	// 2bit.
											// LSB first, MSB last.......
			for (k = 0, t = 0; k < SP_NCODEBIT; k++) {	// 0,1,2,3..., depuncuring is not considered here... but could be included....
														// MSB first .....1, 0
														//t += ( ( (j&(0x01<<(SP_NCODEBIT-1-k)))==0 )? rx_llr_deint0_buf[i+k]: rx_llr_deint1_buf[i+k]);
				if ((j&(0x01 << (SP_NCODEBIT - 1 - k))) == 0)	t += rx_llr0_buf[i + k];
				else	t += rx_llr1_buf[i + k];
			}
			LLR[l][j] = t;
		}
	}

}

void Mapper::vec2array(vector<Complex<double>> in, Complex<double> *output, int size)
{
	for (int i = 0; i <size; ++i)
		output[i] = in[i];


}

void Mapper::array2vec(Complex<double>*input, vector<Complex<double>>&output, int size)
{
	output.resize(size);
	for (int i = 0; i < size; ++i)
		output[i] = input[i];


}

void Mapper::ssd_init(Mapper * M_map, double degree) {
	rotator(M_map->m_rot_map_tab, degree);
}

void Mapper::rotator(Complex<double> *map_ssd_tab, double degree)
{

	Complex<double> rotator = Complex<double>(cos(degree*PI / 180), sin(degree*PI / 180));

	for (int i = 0; i < m_N_2d_const; ++i)
	{
		map_ssd_tab[i] = m_2d_map_tab[i] * rotator;

	}



}

void Mapper::rot(vector<Complex<double>>&input, double degree)
{

	Complex<double> rotator = Complex<double>(cos(degree*PI / 180), sin(degree*PI / 180));
	for (unsigned int i = 0; i < input.size(); ++i)
	{
		input[i] = input[i] * rotator;

	}


}

void Mapper::rotate(Complex<double>& input, double degree)
{
	Complex<double> rotator = Complex<double>(cos(degree * PI / 180), sin(degree * PI / 180));
	input = input * rotator;
}



void Mapper::interleave(vector<Complex<double>>&input)
{
	double temp;
	for (unsigned int i = 0; i < input.size() / 2; ++i)
	{
		temp = input[i].im;
		input[i].im = (input[(input.size() / 2) + i]).im;
		(input[(input.size() / 2) + i]).im = temp;

	}

}



void Mapper::ssd_ML(vector<Complex<double>>rx, vector<bool>&decoded, Mapper *M_MAP, vector<double>wgt, int size) {
	decoded.resize(size);
	unsigned int i;
	int j, k;//l;
	int  kk, bit_flag;// ll;
	double	max1, max0;//, bp_LLR[8][2];
	double a1, a2;
	double	temp;
	int index;


	for (i = 0, k = 0; i < rx.size(); i++) {
		a1 = sqrt(wgt[i]);
		a2 = sqrt(wgt[(rx.size() / 2 + i) % rx.size()]);
		double max = (-100000.0);
		for (j = 0; j < m_N_2d_const; j++) {
			temp = -masq((rx[i].re - ((M_MAP->m_rot_map_tab[j].re)*a1)), (rx[i].im - ((M_MAP->m_rot_map_tab[j].im)*a2)));
			if (temp > max)
			{
				max = temp;
				index = j;
			}
		}
		bit_flag = 0x01 << (M_MAP->m_Ninfobit - 1);
		for (kk = 0; kk < M_MAP->m_Ninfobit; kk++, bit_flag >>= 1)
		{
			decoded[(M_MAP->m_Ninfobit*i) + kk] = bit_flag & index;

		}

	}

}

void	Mapper::SSD_llr_generation(vector<Complex<double>> rx_buf, vector<double> llr_wgt_buf, double* rx_llr0, double*rx_llr1, Mapper *M_MAP, int SP_NDCARperSYM)
{
	int	i, j, k;//l;
	int 	bit_flag, kk;// ll;
	double	max1, max0;//, bp_LLR[8][2];
	double	t_LLR[64];
	double a1, a2;
	// 각 2 bit symbol에 대한 LLR값을 구한다.
	for (i = 0, k = 0; i<SP_NDCARperSYM; i++) {
		// soft metric calculation for 16 QAM points....
		a1 = sqrt(llr_wgt_buf[i]);
		a2 = sqrt(llr_wgt_buf[(SP_NDCARperSYM / 2 + i) % SP_NDCARperSYM]);
		for (j = 0; j<M_MAP->m_N_2d_const; j++) {
			t_LLR[j] = -masq((rx_buf[i].re - ((M_MAP->m_rot_map_tab[j].re)*a1)), (rx_buf[i].im - ((M_MAP->m_rot_map_tab[j].im)*a2)));
			t_LLR[j] *= (llr_wgt_buf[i] + llr_wgt_buf[(SP_NDCARperSYM / 2 + i) % SP_NDCARperSYM]) / 2;
		}
		// log max 방법으로 bit plane별로의 LLR값을 구한다.
		bit_flag = 0x01 << (M_MAP->m_Ninfobit - 1);
		// bit plane 별로 max LLR값을 찾는다.
		for (kk = 0; kk<M_MAP->m_Ninfobit; kk++, bit_flag >>= 1) {
			max1 = max0 = (-100000.0);
			for (j = 0; j<M_MAP->m_N_2d_const; j++) {
				if ((j&bit_flag) == bit_flag) {
					if (t_LLR[j]>max1) max1 = t_LLR[j];
				}
				else {
					if (t_LLR[j]>max0) max0 = t_LLR[j];
				}
			}
			rx_llr0[k] = max0;
			rx_llr1[k] = max1;
			k++;
		}
	}
}
