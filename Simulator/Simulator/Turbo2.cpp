#include "Turbo2.h"


Turb::~Turb()
{
}

void Turb::turbo_trellis_gen(int *g, int Bsize, int Nmemory, int Ncodebit, int Ninfobit)
{ //Parameter


  //g= generator polynomial ,Bsize= Block size(Packet size)  ,Nstate=Number of state  ,Nmemory=Number of memory 
  //	Ncodebit=Coded bit size(ex rate 1/2 Ncodebit= 2) ,Ncode=Number of coded , Ninfo= information bit size , Ninfo= number of information 
  //
  //initialize parameter
	m_Bsize = Bsize;
	m_Nstate = pow(2, Nmemory);
	m_Nmemory = Nmemory;
	m_Ncode = (int)pow(2, Ncodebit);
	m_Ninfo = (int)pow(2, Ninfobit);

	//interleaver pattern memory alloc
	inter_pattern = (int*)malloc(sizeof(int)*m_Bsize);
	deinter_pattern = (int*)malloc(sizeof(int)*m_Bsize);
	//현재는 rate 1/2 짜리 trellis 구조만 고려함 rate를 늘리려면 이부분 확인할것
	m_g0 = *g > *(g + 1) ? *g : *(g + 1);
	m_g1 = *g > *(g + 1) ? *(g + 1) : *g;


	//Forward && Back Trellis setting
	FTrellis.resize(m_Nstate);//
	for (int h = 0; h < m_Nstate; ++h)
		FTrellis[h].resize(Ncodebit);

	BTrellis = FTrellis;
	tb.resize(Nmemory);


	//Memory allocation
	int i, j, k, l, alloc = Bsize + m_Nmemory;


	PU_I1 = (double **)malloc(sizeof(double)*alloc);
	for (i = 0; i < alloc; i++)
		PU_I1[i] = (double *)malloc(sizeof(double)*m_Ninfo);
	PU_O1 = (double **)malloc(sizeof(double)* alloc);
	for (i = 0; i < alloc; i++)
		PU_O1[i] = (double *)malloc(sizeof(double)*m_Ninfo);


	PU_I2 = (double **)malloc(sizeof(double)*alloc);
	for (i = 0; i < alloc; i++)
		PU_I2[i] = (double *)malloc(sizeof(double)*m_Ninfo);
	PU_O2 = (double **)malloc(sizeof(double)*alloc);
	for (i = 0; i < alloc; i++)
		PU_O2[i] = (double *)malloc(sizeof(double)*m_Ninfo);

	L1 = (double **)malloc(sizeof(double)*m_Bsize);
	for (i = 0; i < m_Bsize; i++)
		L1[i] = (double *)malloc(sizeof(double)*m_Ninfo);

	L2 = (double **)malloc(sizeof(double)*alloc);
	for (i = 0; i < alloc; i++)
		L2[i] = (double *)malloc(sizeof(double)*m_Ninfo);

	//=====================================================
	//				Memory Allocation 
	//=====================================================

	log_alpa = (double **)malloc(sizeof(double)*(alloc + 1));
	for (i = 0; i < alloc + 1; i++)
		log_alpa[i] = (double *)malloc(sizeof(double)*m_Nstate);

	log_beta = (double **)malloc(sizeof(double)*(alloc + 1));
	for (i = 0; i < alloc + 1; i++)
		log_beta[i] = (double *)malloc(sizeof(double)*m_Nstate);

	tot_log_a = (double *)malloc(sizeof(double)*alloc);

	//dist_rec_m_ref = (double **)malloc(sizeof(int)*m_Bsize) ;
	//for ( int i = 0 ; i < m_Bsize ; i++ )
	//	dist_rec_m_ref[i] = (double *)malloc(sizeof(double)*m_Ncode) ;

	tmp1 = (double *)malloc(sizeof(double)*m_Ninfo);
	tmp2 = (double *)malloc(sizeof(double)*m_Nstate);
	tmp3 = (double **)malloc(sizeof(double)*m_Ninfo);
	for (i = 0; i < m_Ninfo; i++)
		tmp3[i] = (double *)malloc(sizeof(double)*m_Nstate);
	tmp4 = (double *)malloc(sizeof(double)*m_Ninfo);
	tmp5 = (int *)malloc(sizeof(int)*m_Nstate);
	tmp33 = (double **)malloc(sizeof(double)*m_Ncode);
	for (int i = 0; i < m_Ncode; i++)
		tmp33[i] = (double *)malloc(sizeof(double)*m_Nstate);
	tmp44 = (double *)malloc(sizeof(double)*m_Ncode);


	//=====================================================
	//				Memory Allocation for decoding
	//=====================================================
	info0 = (double*)malloc(sizeof(double)*(alloc));
	info1 = (double*)malloc(sizeof(double)*(alloc));
	info0_int = (double*)malloc(sizeof(double)*(alloc));
	info1_int = (double*)malloc(sizeof(double)*(alloc));
	parity1_0 = (double*)malloc(sizeof(double)*(2 * (alloc + m_Nmemory)));
	parity1_1 = (double*)malloc(sizeof(double)* (2 * (alloc + m_Nmemory)));
	parity2_0 = (double*)malloc(sizeof(double)*(2 * (alloc + m_Nmemory)));
	parity2_1 = (double*)malloc(sizeof(double)*(2 * (alloc + m_Nmemory)));

	info0_v.resize(alloc);
	info1_v.resize(alloc);
	info0_int_v.resize(alloc);
	info1_int_v.resize(alloc);
	parity1_0_v.resize(2 * (alloc + m_Nmemory));
	parity1_1_v.resize(2 * (alloc + m_Nmemory));
	parity2_0_v.resize(2 * (alloc + m_Nmemory));
	parity2_1_v.resize(2 * (alloc + m_Nmemory));


	// setting up the encoding table
	//state는 메모리의 우측이 MSB이다. ex)스테이트가 2이면 0 1이다.

	int out, nst, temp;
	for (i = 0; i < m_Nstate; ++i)
	{
		for (j = 0; j < m_Ninfo; ++j)
		{
			out = 0;
			nst = (i << 1) ^ j; // input bit    ex) if(state=2 and input bit =0)    ==>   This nst= 0 0 1  


			temp = nst & m_g0; for (k = 1; k <= m_Nmemory; ++k) nst ^= (temp >> k) & 1; //poly nomial 연산및 xor 연산(LSB만 연산을 진행함) 
			temp = nst & m_g1; for (k = 0; k <= m_Nmemory; ++k) out ^= (temp >> k) & 1; // output calculation

			nst &= (1 << m_Nmemory) - 1; //shift register ex) 0 0 1 & 1 1 0  ==> 0 0 


			FTrellis[i][j].next_state = nst;
			FTrellis[i][j].info_bit = j;
			FTrellis[i][j].code_bit = (j << 1) | (out == 1);


		}

	}

	//=============================================
	//               Backward trellis generation
	//=============================================
	for (i = 0; i < m_Nstate; i++) {
		l = 0;
		for (j = 0; j < m_Nstate; j++) {
			for (k = 0; k < m_Ninfo; k++) {
				if (FTrellis[j][k].next_state == i) {
					BTrellis[i][l].prev_state = j;
					BTrellis[i][l].info_bit = FTrellis[j][k].info_bit;
					BTrellis[i][l].code_bit = FTrellis[j][k].code_bit;
					l++;


				}
			}
		}
	}
}


vector <bool> Turb::encode(vector <bool> source, vector<bool> source_int)
{
	vector<bool> result((source.size()) * 3 + (m_Nmemory * 4));        //결과를 아래와 같이 뽑을 것임  ()안은 크기를 의미하며 tail_in은 termination하기 위한 입력비트이고 out은 그에따른 출력비트를 의미함
																	   //       info1(B_size)
																	   //       parity1(B_size)  tail_in1(m_Nmemory) tail_out1(m_Nmemory)  
																	   //       parity2(B_size   tail_in2(m_Nmemory) tail_out2(m_Nmemory)
	Bsize = source.size() + m_Nmemory;
	int code_size = source.size() + (2 * m_Nmemory);
	int i, state = 0;
	vector <bool> coded(code_size);
	vector<bool> coded_int(code_size);
	/*vector<bool> tail_infobit(m_Nmemory * 2);*/




	// encoder #1=========================================================================
	for (i = 0; i < source.size(); ++i)
	{
		coded[i] = (FTrellis[state][source[i]].code_bit) & 1;
		state = FTrellis[state][source[i]].next_state;

	}

	// tail termination
	int temp, k, g_tail;
	g_tail = m_g0 >> 1;

	bool tail_bit;
	for (i = source.size(); i < Bsize; i += 2)
	{

		temp = state & g_tail;
		tail_bit = temp & 1;
		for (k = 1; k < m_Nmemory; ++k)
			tail_bit ^= (temp >> k) & 1;


		coded[i] = tail_bit;
		coded[i + 1] = (FTrellis[state][tail_bit].code_bit) & 1;
		state = FTrellis[state][tail_bit].next_state;
	}

	//====================================================================================




	// encoder#2=========================================================================
	state = 0;//state initialization

	for (i = 0; i < source.size(); ++i)
	{
		coded_int[i] = (FTrellis[state][source_int[i]].code_bit) & 1;
		state = FTrellis[state][source_int[i]].next_state;

	}

	// tail termination
	for (i = source.size(); i < Bsize; i += 2)
	{

		temp = state & g_tail;
		tail_bit = temp & 1;
		for (k = 1; k < m_Nmemory; ++k)
			tail_bit ^= (temp >> k) & 1;


		coded_int[i] = tail_bit;
		coded_int[i + 1] = (FTrellis[state][tail_bit].code_bit) & 1;
		state = FTrellis[state][tail_bit].next_state;
	}
	//====================================================================================


	//parallel to series==================================================================
	//BLOCK의 크기를 k라 할때 코드의 배치는  
	//info(1) p1(1) p2(1) ...... info(k) p1(k) p2(k)   tail_in1 tail_out1 tail_in2 tail_out2 의 순서로 배치했음 


	int z, l, q;
	for (q = 0; q < m_Bsize; q++)
	{

		result[3 * q] = source[q];
		result[3 * q + 1] = coded[q];
		result[3 * q + 2] = coded_int[q];

	}

	for (z = m_Bsize, l = m_Bsize; z < Bsize; z += 2, l += 4)
	{
		result[l] = coded[z];
		result[l + 1] = coded[z + 1];
		result[l + 2] = coded_int[z];
		result[l + 3] = coded_int[z + 1];
	}

	//=====================================================================================



	return (result);
}

void Turb::int_init() {
	//TTA 표준 인터리버

	if (m_Bsize == 80)
	{
		f1 = 11, f2 = 20;
	}
	else if (m_Bsize == 88)
	{
		f1 = 5, f2 = 22;
	}
	else if (m_Bsize == 96)
	{
		f1 = 11, f2 = 24;
	}
	else if (m_Bsize == 104)
	{
		f1 = 7, f2 = 26;

	}
	else if (m_Bsize == 112)
	{
		f1 = 41, f2 = 84;

	}
	else if (m_Bsize == 120)
	{
		f1 = 103, f2 = 90;

	}
	else if (m_Bsize == 128)
	{
		f1 = 15, f2 = 32;

	}
	else if (m_Bsize == 136)
	{
		f1 = 144, f2 = 152;

	}
	else if (m_Bsize == 144)
	{
		f1 = 17, f2 = 108;

	}
	else if (m_Bsize == 152)
	{
		f1 = 9, f2 = 38;

	}
	else if (m_Bsize == 160)
	{
		f1 = 21, f2 = 120;
	}
	else if (m_Bsize == 168)
	{
		f1 = 101, f2 = 84;
	}
	else if (m_Bsize == 176)
	{
		f1 = 21, f2 = 44;
	}
	else if (m_Bsize == 184)
	{
		f1 = 57, f2 = 46;
	}
	else if (m_Bsize == 192)
	{
		f1 = 23, f2 = 48;
	}
	else if (m_Bsize == 200)
	{
		f1 = 13, f2 = 50;
	}
	else if (m_Bsize == 208)
	{
		f1 = 27, f2 = 52;
	}
	else if (m_Bsize == 216)
	{
		f1 = 11, f2 = 36;
	}
	else if (m_Bsize == 224)
	{
		f1 = 27, f2 = 56;
	}
	else if (m_Bsize == 232)
	{
		f1 = 85, f2 = 58;
	}
	else if (m_Bsize == 240)
	{
		f1 = 29, f2 = 60;
	}
	else if (m_Bsize == 248)
	{
		f1 = 33, f2 = 62;
	}
	else if (m_Bsize == 256)
	{
		f1 = 15, f2 = 32;
	}
	else if (m_Bsize == 264)
	{
		f1 = 17, f2 = 198;
	}
	else if (m_Bsize == 272)
	{
		f1 = 33, f2 = 68;
	}
	else if (m_Bsize == 280)
	{
		f1 = 103, f2 = 210;
	}
	else if (m_Bsize == 288)
	{
		f1 = 19, f2 = 36;
	}
	else if (m_Bsize == 296)
	{
		f1 = 19, f2 = 74;
	}
	else if (m_Bsize == 304)
	{
		f1 = 37, f2 = 76;
	}
	else if (m_Bsize == 312)
	{
		f1 = 19, f2 = 78;
	}
	else if (m_Bsize == 320)
	{
		f1 = 21, f2 = 120;
	}
	else if (m_Bsize == 328)
	{
		f1 = 21, f2 = 82;
	}
	else if (m_Bsize == 336)
	{
		f1 = 115, f2 = 84;
	}
	else if (m_Bsize == 344)
	{
		f1 = 193, f2 = 86;
	}
	else if (m_Bsize == 352)
	{
		f1 = 21, f2 = 44;
	}
	else if (m_Bsize == 360)
	{
		f1 = 133, f2 = 90;
	}
	else if (m_Bsize == 368)
	{
		f1 = 81, f2 = 46;
	}
	else if (m_Bsize == 376)
	{
		f1 = 45, f2 = 94;
	}
	else if (m_Bsize == 384)
	{
		f1 = 23, f2 = 48;
	}
	else if (m_Bsize == 392)
	{
		f1 = 243, f2 = 98;
	}
	else if (m_Bsize == 400)
	{
		f1 = 151, f2 = 40;
	}
	else if (m_Bsize == 408)
	{
		f1 = 155, f2 = 102;
	}
	else if (m_Bsize == 416)
	{
		f1 = 25, f2 = 52;
	}
	else if (m_Bsize == 424)
	{
		f1 = 51, f2 = 106;
	}
	else if (m_Bsize == 432)
	{
		f1 = 47, f2 = 72;
	}
	else if (m_Bsize == 440)
	{
		f1 = 91, f2 = 110;
	}
	else if (m_Bsize == 448)
	{
		f1 = 29, f2 = 168;
	}
	else if (m_Bsize == 456)
	{
		f1 = 29, f2 = 114;
	}
	else if (m_Bsize == 464)
	{
		f1 = 247, f2 = 58;
	}
	else if (m_Bsize == 472)
	{
		f1 = 29, f2 = 118;
	}
	else if (m_Bsize == 480)
	{
		f1 = 89, f2 = 180;
	}
	else if (m_Bsize == 488)
	{
		f1 = 91, f2 = 122;
	}
	else if (m_Bsize == 496)
	{
		f1 = 157, f2 = 62;
	}
	else if (m_Bsize == 504)
	{
		f1 = 55, f2 = 84;
	}
	else if (m_Bsize == 512)
	{
		f1 = 31, f2 = 64;
	}
	else cout << "잘못된 사이즈입니다." << endl;

}

vector<bool> Turb::interleaver(vector<bool>source) {
	vector<bool>result(m_Bsize);
	int pi;
	for (int i = 0; i < m_Bsize; ++i)
	{
		pi = ((f1*i) + (f2*i*i)) % m_Bsize;
		inter_pattern[i] = pi;
		result[i] = source[pi];

	}

	for (int j = 0; j < m_Bsize; ++j)
		deinter_pattern[inter_pattern[j]] = j;

	return result;
}

void Turb::log_sum_exp_lut_generation()
{
	int i;
	for (i = 0; i < 256; i++)
		m_log_sum_exp_correction_lut[i] = log(1.0 + exp(-(double)i / 50));
}

double Turb::log_sum_exp(double *din, int num)
{
	int i, q_diff;
	double	delta, diff;

	delta = din[0];
	for (i = 1, q_diff; i < num; i++) {
		diff = (din[i] - delta) * 50;
		if (diff > 0) {
			if (diff > 255) q_diff = 255;
			else				q_diff = (int)diff;
			delta = din[i] + m_log_sum_exp_correction_lut[q_diff];
		}
		else {
			diff = -diff;
			if (diff > 255) q_diff = 255;
			else				q_diff = (int)diff;
			delta = delta + m_log_sum_exp_correction_lut[q_diff];
		}
	}
	return(delta);
}

double	Turb::log_sum_exp(double din1, double din2)
{
	double	delta, diff;
	int		q_diff;

	diff = (din1 - din2) * 50;
	if (diff > 0) {
		if (diff > 255)	q_diff = 255;
		else				q_diff = (int)diff;
		delta = din1 + m_log_sum_exp_correction_lut[q_diff];
	}
	else {
		diff = -diff;
		if (diff > 255)		q_diff = 255;
		else				q_diff = (int)diff;
		delta = din2 + m_log_sum_exp_correction_lut[q_diff];
	}
	return(delta);
}

void	Turb::turbo_llr_generation(FADING FAD_MOD, vector<Complex<double>> rx_buf, vector<double> llr_wgt_buf, double* rx_llr0, double*rx_llr1, Mapper *M_MAP, int SP_NDCARperSYM, double LC)
{
	int	i, j, k;//l;
	int 	bit_flag, kk;// ll;
	double	max1, max0;//, bp_LLR[8][2];
	bool max_flag0 = 0, max_flag1 = 0;
	double	t_LLR[64];

	switch (FAD_MOD) {
	case Rayl:
		// 각 2 bit symbol에 대한 LLR값을 구한다.
		for (i = 0, k = 0; i < SP_NDCARperSYM; i++) {
			// soft metric calculation for 16 QAM points....
			for (j = 0; j < M_MAP->m_N_2d_const; j++) {
				t_LLR[j] = masq((M_MAP->m_2d_map_tab[j] - rx_buf[i]).re, (M_MAP->m_2d_map_tab[j] - rx_buf[i]).im)*LC;
				t_LLR[j] *= (llr_wgt_buf[i]);	//for fast fading
			}
			// log map 방법으로 bit plane별로의 LLR값을 구한다.
			bit_flag = 0x01 << (M_MAP->m_Ninfobit - 1);
			// bit plane 별로  LOG map LLR값을 찾는다.
			for (kk = 0; kk < M_MAP->m_Ninfobit; kk++, bit_flag >>= 1) {
				max1 = max0 = (-100000.0);
				for (j = 0; j < M_MAP->m_N_2d_const; j++) {
					if ((j&bit_flag) == bit_flag) {
						if ((t_LLR[j] > max1) && (max_flag1 == 0)) {
							max1 = t_LLR[j];
							max_flag1 = 1;
						}
						else max1 = log_sum_exp(max1, t_LLR[j]);


					}
					else {
						if ((t_LLR[j] > max0) && (max_flag0 == 0)) {
							max0 = t_LLR[j];
							max_flag0 = 1;
						}
						else max0 = log_sum_exp(max0, t_LLR[j]);


					}
				}
				rx_llr0[k] = max0;
				rx_llr1[k] = max1;
				k++;
			}
		}
		break;

	case Rayl_Quasi:
		// 각 2 bit symbol에 대한 LLR값을 구한다.
		for (i = 0, k = 0; i < SP_NDCARperSYM; i++) {
			// soft metric calculation for 16 QAM points....
			for (j = 0; j < M_MAP->m_N_2d_const; j++) {
				t_LLR[j] = masq((M_MAP->m_2d_map_tab[j] - rx_buf[i]).re, (M_MAP->m_2d_map_tab[j] - rx_buf[i]).im) *LC;
			}
			// log map 방법으로 bit plane별로의 LLR값을 구한다.
			bit_flag = 0x01 << (M_MAP->m_Ninfobit - 1);
			// bit plane 별로  LOG map LLR값을 찾는다.
			for (kk = 0; kk < M_MAP->m_Ninfobit; kk++, bit_flag >>= 1) {
				max1 = max0 = (-1000000000000000000.0);
				for (j = 0; j < M_MAP->m_N_2d_const; j++) {
					if ((j&bit_flag) == bit_flag) {
						if ((t_LLR[j] > max1) && (max_flag1 == 0)) {
							max1 = t_LLR[j];
							max_flag1 = 1;
						}
						else max1 = log_sum_exp(max1, t_LLR[j]);


					}
					else {
						if ((t_LLR[j] > max0) && (max_flag0 == 0)) {
							max0 = t_LLR[j];
							max_flag0 = 1;
						}
						else max0 = log_sum_exp(max0, t_LLR[j]);


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

void	Turb::turbo_llr_generation(FADING FAD_MOD, vector<Complex<double>> rx_buf, vector<double> llr_wgt_buf, vector<double> &rx_llr0, vector<double> &rx_llr1, Mapper *M_MAP, int SP_NDCARperSYM, double LC)
{
	int	i, j, k;//l;
	int 	bit_flag, kk;// ll;
	double	max1, max0;//, bp_LLR[8][2];
	bool max_flag0 = 0, max_flag1 = 0;
	double	t_LLR[64];

	switch (FAD_MOD) {
	case Rayl:

		break;

	case Rayl_Quasi:
		// 각 2 bit symbol에 대한 LLR값을 구한다.
		for (i = 0, k = 0; i < SP_NDCARperSYM; i++) {
			// soft metric calculation for 16 QAM points....
			for (j = 0; j < M_MAP->m_N_2d_const; j++) {
				t_LLR[j] = masq((M_MAP->m_2d_map_tab[j] - rx_buf[i]).re, (M_MAP->m_2d_map_tab[j] - rx_buf[i]).im) *LC;
			}
			// log map 방법으로 bit plane별로의 LLR값을 구한다.
			bit_flag = 0x01 << (M_MAP->m_Ninfobit - 1);
			// bit plane 별로  LOG map LLR값을 찾는다.
			for (kk = 0; kk < M_MAP->m_Ninfobit; kk++, bit_flag >>= 1) {
				max1 = max0 = (-1000000000000000000.0);
				for (j = 0; j < M_MAP->m_N_2d_const; j++) {
					if ((j&bit_flag) == bit_flag) {
						if ((t_LLR[j] > max1) && (max_flag1 == 0)) {
							max1 = t_LLR[j];
							max_flag1 = 1;
						}
						else max1 = log_sum_exp(max1, t_LLR[j]);


					}
					else {
						if ((t_LLR[j] > max0) && (max_flag0 == 0)) {
							max0 = t_LLR[j];
							max_flag0 = 1;
						}
						else max0 = log_sum_exp(max0, t_LLR[j]);


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


void Turb::llr_segment(double *llr0, double *llr1, double *source_llr0, double *source_llr1,  double *relay_llr0, double *relay_llr1, int size){
    for(int i = 0; i < size/2; i++){
        relay_llr0[2 * i] = llr0[4 * i];
        relay_llr0[2 * i + 1] = llr0[4 * i + 1];
        source_llr0[2 * i] = llr0[4 * i + 2];
        source_llr0[2 * i + 1] = llr0[4 * i + 3];

        relay_llr1[2 * i] = llr1[4 * i];        
        relay_llr1[2 * i + 1] = llr1[4 * i + 1];
        source_llr1[2 * i] = llr1[4 * i + 2];
        source_llr1[2 * i + 1] = llr1[4 * i + 3];
    }
}

void Turb::llr_segment(vector<double> llr0, vector<double> llr1, vector<double> &source_llr0, vector<double> &source_llr1, vector<double> &relay_llr0, vector<double> &relay_llr1, int size) {
	for (int i = 0; i < size / 2; i++) {
		relay_llr0[2 * i] = llr0[4 * i];
		relay_llr0[2 * i + 1] = llr0[4 * i + 1];
		source_llr0[2 * i] = llr0[4 * i + 2];
		source_llr0[2 * i + 1] = llr0[4 * i + 3];

		relay_llr1[2 * i] = llr1[4 * i];
		relay_llr1[2 * i + 1] = llr1[4 * i + 1];
		source_llr1[2 * i] = llr1[4 * i + 2];
		source_llr1[2 * i + 1] = llr1[4 * i + 3];
	}
}


void Turb::turbo_llr_generation_InPhase(FADING FAD_MOD, vector<Complex<double>> rx_buf, vector<double> llr_wgt_buf, double* rx_llr0, double* rx_llr1, Mapper* M_MAP, int SP_NDCARperSYM, double LC)
{
	int	i, j, k;//l;
	int 	bit_flag, kk;// ll;
	double	max1, max0;//, bp_LLR[8][2];
	bool max_flag0 = 0, max_flag1 = 0;
	double	t_LLR[64];

	switch (FAD_MOD) {
	case Rayl:
		// 각 2 bit symbol에 대한 LLR값을 구한다.
		for (i = 0, k = 0; i < SP_NDCARperSYM; i++) {
			// soft metric calculation for 16 QAM points....
			for (j = 0; j < M_MAP->m_N_2d_const; j++) {
				t_LLR[j] = masq((M_MAP->m_2d_map_tab[j] - rx_buf[i]).re, (M_MAP->m_2d_map_tab[j] - rx_buf[i]).im) * LC;
				t_LLR[j] *= (llr_wgt_buf[i]);	//for fast fading
			}
			// log map 방법으로 bit plane별로의 LLR값을 구한다.
			bit_flag = 0x01 << (M_MAP->m_Ninfobit - 1);
			// bit plane 별로  LOG map LLR값을 찾는다.
			for (kk = 0; kk < M_MAP->m_Ninfobit; kk++, bit_flag >>= 1) {
				max1 = max0 = (-100000.0);
				for (j = 0; j < M_MAP->m_N_2d_const; j++) {
					if ((j & bit_flag) == bit_flag) {
						if ((t_LLR[j] > max1) && (max_flag1 == 0)) {
							max1 = t_LLR[j];
							max_flag1 = 1;
						}
						else max1 = log_sum_exp(max1, t_LLR[j]);


					}
					else {
						if ((t_LLR[j] > max0) && (max_flag0 == 0)) {
							max0 = t_LLR[j];
							max_flag0 = 1;
						}
						else max0 = log_sum_exp(max0, t_LLR[j]);


					}
				}
				rx_llr0[k] = max0;
				rx_llr1[k] = max1;
				k++;
			}
		}
		break;

	case Rayl_Quasi:
		// 각 2 bit symbol에 대한 LLR값을 구한다.
		for (i = 0, k = 0; i < SP_NDCARperSYM; i++) {
			// soft metric calculation for 16 QAM points....
			for (j = 0; j < M_MAP->m_N_2d_const; j++) {
				t_LLR[j] = masq((M_MAP->m_2d_map_tab[j] - rx_buf[i]).re, 0) * LC;
			}
			// log map 방법으로 bit plane별로의 LLR값을 구한다.
			bit_flag = 0x01 << (M_MAP->m_Ninfobit - 1);
			// bit plane 별로  LOG map LLR값을 찾는다.
			for (kk = 0; kk < M_MAP->m_Ninfobit; kk++, bit_flag >>= 1) {
				max1 = max0 = (-100000.0);
				for (j = 0; j < M_MAP->m_N_2d_const; j++) {
					if ((j & bit_flag) == bit_flag) {
						if ((t_LLR[j] > max1) && (max_flag1 == 0)) {
							max1 = t_LLR[j];
							max_flag1 = 1;
						}
						else max1 = log_sum_exp(max1, t_LLR[j]);


					}
					else {
						if ((t_LLR[j] > max0) && (max_flag0 == 0)) {
							max0 = t_LLR[j];
							max_flag0 = 1;
						}
						else max0 = log_sum_exp(max0, t_LLR[j]);


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

void Turb::turbo_llr_generation_Quad(FADING FAD_MOD, vector<Complex<double>> rx_buf, vector<double> llr_wgt_buf, double* rx_llr0, double* rx_llr1, Mapper* M_MAP, int SP_NDCARperSYM, double LC)
{
	int	i, j, k;//l;
	int 	bit_flag, kk;// ll;
	double	max1, max0;//, bp_LLR[8][2];
	bool max_flag0 = 0, max_flag1 = 0;
	double	t_LLR[64];

	switch (FAD_MOD) {
	case Rayl:
		// 각 2 bit symbol에 대한 LLR값을 구한다.
		for (i = 0, k = 0; i < SP_NDCARperSYM; i++) {
			// soft metric calculation for 16 QAM points....
			for (j = 0; j < M_MAP->m_N_2d_const; j++) {
				t_LLR[j] = masq((M_MAP->m_2d_map_tab[j] - rx_buf[i]).re, (M_MAP->m_2d_map_tab[j] - rx_buf[i]).im) * LC;
				t_LLR[j] *= (llr_wgt_buf[i]);	//for fast fading
			}
			// log map 방법으로 bit plane별로의 LLR값을 구한다.
			bit_flag = 0x01 << (M_MAP->m_Ninfobit - 1);
			// bit plane 별로  LOG map LLR값을 찾는다.
			for (kk = 0; kk < M_MAP->m_Ninfobit; kk++, bit_flag >>= 1) {
				max1 = max0 = (-100000.0);
				for (j = 0; j < M_MAP->m_N_2d_const; j++) {
					if ((j & bit_flag) == bit_flag) {
						if ((t_LLR[j] > max1) && (max_flag1 == 0)) {
							max1 = t_LLR[j];
							max_flag1 = 1;
						}
						else max1 = log_sum_exp(max1, t_LLR[j]);


					}
					else {
						if ((t_LLR[j] > max0) && (max_flag0 == 0)) {
							max0 = t_LLR[j];
							max_flag0 = 1;
						}
						else max0 = log_sum_exp(max0, t_LLR[j]);


					}
				}
				rx_llr0[k] = max0;
				rx_llr1[k] = max1;
				k++;
			}
		}
		break;

	case Rayl_Quasi:
		// 각 2 bit symbol에 대한 LLR값을 구한다.
		for (i = 0, k = 0; i < SP_NDCARperSYM; i++) {
			// soft metric calculation for 16 QAM points....
			for (j = 0; j < M_MAP->m_N_2d_const; j++) {
				t_LLR[j] = masq(0, (M_MAP->m_2d_map_tab[j] - rx_buf[i]).im) * LC;
			}
			// log map 방법으로 bit plane별로의 LLR값을 구한다.
			bit_flag = 0x01 << (M_MAP->m_Ninfobit - 1);
			// bit plane 별로  LOG map LLR값을 찾는다.
			for (kk = 0; kk < M_MAP->m_Ninfobit; kk++, bit_flag >>= 1) {
				max1 = max0 = (-100000.0);
				for (j = 0; j < M_MAP->m_N_2d_const; j++) {
					if ((j & bit_flag) == bit_flag) {
						if ((t_LLR[j] > max1) && (max_flag1 == 0)) {
							max1 = t_LLR[j];
							max_flag1 = 1;
						}
						else max1 = log_sum_exp(max1, t_LLR[j]);


					}
					else {
						if ((t_LLR[j] > max0) && (max_flag0 == 0)) {
							max0 = t_LLR[j];
							max_flag0 = 1;
						}
						else max0 = log_sum_exp(max0, t_LLR[j]);


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



vector<bool>  Turb::turbo_decoding(double** LLR1, double **LLR2, int iter_num) {
	int i, j, k;
	vector<bool>result(m_Bsize);

	//First staged decoding apriori initialization
	for (i = 0; i < Bsize; i++)
		for (j = 0; j < m_Ninfo; j++)PU_I1[i][j] = -log(2);


	for (k = 0; k < iter_num; k++) {

		// First Stage Decoding	
		decoding(LLR1, PU_I1, PU_O1);

		//--------------------- Extrinsic Information Extraction ---------------------------
		for (i = 0; i < m_Bsize; i++)for (j = 0; j < m_Ninfo; j++)
		{
			PU_O1[i][j] = PU_O1[i][j] - PU_I1[i][j];
		}
		//======================================================
		//			Interleaving
		//======================================================
		for (i = 0; i < Bsize; i++)
			for (j = 0; j < m_Ninfo; j++)
			{
				if (i < m_Bsize) PU_I2[i][j] = PU_O1[inter_pattern[i]][j];
				else PU_I2[i][j] = -log(2);
			}
		//======================================================
		//			Second Stage Decoding............
		//======================================================


		decoding(LLR2, PU_I2, PU_O2);
		//--------------------- Extrinsic Information Extraction ---------------------------
		for (i = 0; i < m_Bsize; i++)for (j = 0; j < m_Ninfo; j++)
		{
			L2[i][j] = PU_O2[i][j];
			PU_O2[i][j] = PU_O2[i][j] - PU_I2[i][j];
		}

		//======================================================
		//			DeInterleaving
		//======================================================
		for (i = 0; i < Bsize; i++)
			for (j = 0; j < m_Ninfo; j++)
			{
				if (i < m_Bsize)
				{
					PU_I1[i][j] = PU_O2[deinter_pattern[i]][j];
					L1[i][j] = L2[deinter_pattern[i]][j];
				}
				else PU_I1[i][j] = -log(2);
			}

	}


	for (int a = 0; a < m_Bsize; ++a)
		result[a] = (L1[a][0] > L1[a][1]) ? 0 : 1;


	return result;
}

void Turb::Decision(vector<vector<double>> LLR, vector<bool> &result){
    int size = LLR.size();
    result.resize(size);
    for(int i = 0; i < size; i++)
        result[i] = LLR[i][0]>LLR[i][1]?0:1;
}

vector<vector<double>>  Turb::ExportLLR_turbo_decoding(double** LLR1, double **LLR2, int iter_num) {
	int i, j, k;
	vector<vector<double>> result(m_Bsize);

	//First staged decoding apriori initialization
	for (i = 0; i < Bsize; i++)
		for (j = 0; j < m_Ninfo; j++)PU_I1[i][j] = -log(2);


	for (k = 0; k < iter_num; k++) {

		// First Stage Decoding	
		decoding(LLR1, PU_I1, PU_O1);

		//--------------------- Extrinsic Information Extraction ---------------------------
		for (i = 0; i < m_Bsize; i++)for (j = 0; j < m_Ninfo; j++)
		{
			PU_O1[i][j] = PU_O1[i][j] - PU_I1[i][j];
		}
		//======================================================
		//			Interleaving
		//======================================================
		for (i = 0; i < Bsize; i++)
			for (j = 0; j < m_Ninfo; j++)
			{
				if (i < m_Bsize) PU_I2[i][j] = PU_O1[inter_pattern[i]][j];
				else PU_I2[i][j] = -log(2);
			}
		//======================================================
		//			Second Stage Decoding............
		//======================================================


		decoding(LLR2, PU_I2, PU_O2);
		//--------------------- Extrinsic Information Extraction ---------------------------
		for (i = 0; i < m_Bsize; i++)for (j = 0; j < m_Ninfo; j++)
		{
			L2[i][j] = PU_O2[i][j];
			PU_O2[i][j] = PU_O2[i][j] - PU_I2[i][j];
		}

		//======================================================
		//			DeInterleaving
		//======================================================
		for (i = 0; i < Bsize; i++)
			for (j = 0; j < m_Ninfo; j++)
			{
				if (i < m_Bsize)
				{
					PU_I1[i][j] = PU_O2[deinter_pattern[i]][j];
					L1[i][j] = L2[deinter_pattern[i]][j];
				}
				else PU_I1[i][j] = -log(2);
			}

	}


	for (int a = 0; a < m_Bsize; ++a){
		result[a].resize(2);
		result[a][0] = L1[a][0];
        result[a][1] = L1[a][1];
       }

	return result;
}

vector<vector<double>>  Turb::ExportLLR_turbo_decoding(vector<vector<double>> LLR1, vector<vector<double>> LLR2, int iter_num) {
	int i, j, k;
	vector<vector<double>> result(m_Bsize);

	//First staged decoding apriori initialization
	for (i = 0; i < Bsize; i++)
		for (j = 0; j < m_Ninfo; j++)PU_I1[i][j] = -log(2);


	for (k = 0; k < iter_num; k++) {

		// First Stage Decoding	
		decoding(LLR1, PU_I1, PU_O1);

		//--------------------- Extrinsic Information Extraction ---------------------------
		for (i = 0; i < m_Bsize; i++)for (j = 0; j < m_Ninfo; j++)
		{
			PU_O1[i][j] = PU_O1[i][j] - PU_I1[i][j];
		}
		//======================================================
		//			Interleaving
		//======================================================
		for (i = 0; i < Bsize; i++)
			for (j = 0; j < m_Ninfo; j++)
			{
				if (i < m_Bsize) PU_I2[i][j] = PU_O1[inter_pattern[i]][j];
				else PU_I2[i][j] = -log(2);
			}
		//======================================================
		//			Second Stage Decoding............
		//======================================================


		decoding(LLR2, PU_I2, PU_O2);
		//--------------------- Extrinsic Information Extraction ---------------------------
		for (i = 0; i < m_Bsize; i++)for (j = 0; j < m_Ninfo; j++)
		{
			L2[i][j] = PU_O2[i][j];
			PU_O2[i][j] = PU_O2[i][j] - PU_I2[i][j];
		}

		//======================================================
		//			DeInterleaving
		//======================================================
		for (i = 0; i < Bsize; i++)
			for (j = 0; j < m_Ninfo; j++)
			{
				if (i < m_Bsize)
				{
					PU_I1[i][j] = PU_O2[deinter_pattern[i]][j];
					L1[i][j] = L2[deinter_pattern[i]][j];
				}
				else PU_I1[i][j] = -log(2);
			}

	}


	for (int a = 0; a < m_Bsize; ++a){
		result[a].resize(2);
		result[a][0] = L1[a][0];
        result[a][1] = L1[a][1];
       }

	return result;
}

vector<vector<double>>  Turb::ExportLLR_turbo_decoding_excluding(double** LLR1, double **LLR2, int iter_num) {
	int i, j, k;
	vector<vector<double>> result(m_Bsize);

	//First staged decoding apriori initialization
	for (i = 0; i < Bsize; i++)
		for (j = 0; j < m_Ninfo; j++)PU_I1[i][j] = -log(2);


	for (k = 0; k < iter_num; k++) {

		// First Stage Decoding	
		decoding(LLR1, PU_I1, PU_O1);

		//--------------------- Extrinsic Information Extraction ---------------------------
		for (i = 0; i < m_Bsize; i++)for (j = 0; j < m_Ninfo; j++)
		{
			PU_O1[i][j] = PU_O1[i][j] - PU_I1[i][j];
		}
		//======================================================
		//			Interleaving
		//======================================================
		for (i = 0; i < Bsize; i++)
			for (j = 0; j < m_Ninfo; j++)
			{
				if (i < m_Bsize) PU_I2[i][j] = PU_O1[inter_pattern[i]][j];
				else PU_I2[i][j] = -log(2);
			}
		//======================================================
		//			Second Stage Decoding............
		//======================================================


		decoding(LLR2, PU_I2, PU_O2);
		//--------------------- Extrinsic Information Extraction ---------------------------
		for (i = 0; i < m_Bsize; i++)for (j = 0; j < m_Ninfo; j++)
		{
			L2[i][j] = PU_O2[i][j];
			PU_O2[i][j] = PU_O2[i][j] - PU_I2[i][j];
		}

		//======================================================
		//			DeInterleaving
		//======================================================
		for (i = 0; i < Bsize; i++)
			for (j = 0; j < m_Ninfo; j++)
			{
				if (i < m_Bsize)
				{
					PU_I1[i][j] = PU_O2[deinter_pattern[i]][j];
					L1[i][j] = L2[deinter_pattern[i]][j];
				}
				else PU_I1[i][j] = -log(2);
			}

	}


	for (int a = 1; a < m_Bsize * 2; a+=2){
    	result[a].resize(2);
    	result[a][0] = L1[a][0];
        result[a][1] = L1[a][1];
   }

	return result;
}

void Turb::decoding(double **PC_I, double	**PU_I, double	**PU_O) {


	int		i, j, k;
	//=====================================================
	//				Initialization of feedforward/back recursion
	//=====================================================

	log_alpa[0][0] = log_beta[Bsize][0] = 0;
	for (i = 1; i < m_Nstate; i++)	log_alpa[0][i] = mInfty;
	for (i = 1; i < m_Nstate; i++)	log_beta[Bsize][i] = mInfty;

	//----------------   Iteration to update Alpha  ----------------------------

	for (i = 0; i < Bsize; i++) {
		for (j = 0; j < m_Nstate; j++) {
			for (k = 0; k < m_Ninfo; k++) {
				tmp1[k] = PU_I[i][BTrellis[j][k].info_bit]
					+ PC_I[i][BTrellis[j][k].code_bit]
					+ log_alpa[i][BTrellis[j][k].prev_state];
				//tmp1[k] =0;
			}
			tmp2[j] = log_sum_exp(tmp1, m_Ninfo);
		}
		tot_log_a[i] = log_sum_exp(tmp2, m_Nstate);
		//============================================================
		// normalization to make log_alpha[s]=0
		//============================================================
		for (j = 0; j < m_Nstate; j++)
			log_alpa[i + 1][j] = tmp2[j] - tot_log_a[i];
	}
	/*
	for ( i = 0 ; i < m_Bsize ; i++ ) {
	printf("%d : ",EXT_TB->m_state1_seq[i]);
	for ( j = 0 ; j < m_Nstate ; j++ )	{
	printf("%lf ", log_alpa[i][j] );
	}
	printf("\n");
	getchar();
	}
	*/
	//----------------   Iteration to update Beta  ----------------------------

	for (i = Bsize - 1; i >= 0; i--) {
		for (j = 0; j < m_Nstate; j++) {
			for (k = 0; k < m_Ninfo; k++) {
				tmp1[k] = PU_I[i][FTrellis[j][k].info_bit]
					+ PC_I[i][FTrellis[j][k].code_bit]
					+ log_beta[i + 1][FTrellis[j][k].next_state];
				//tmp1[k]=0;
			}
			tmp2[j] = log_sum_exp(tmp1, m_Ninfo);
		}
		for (j = 0; j < m_Nstate; j++)
			log_beta[i][j] = tmp2[j] - tot_log_a[i];
	}


	//----------------   APP Calculation ---------------------------------------
	for (i = 0; i < Bsize; i++) {
		for (j = 0; j < m_Nstate; j++) tmp5[j] = 0;
		for (j = 0; j < m_Nstate; j++)
			for (k = 0; k < m_Ninfo; k++) {
				tmp3[FTrellis[j][k].info_bit][(tmp5[FTrellis[j][k].info_bit]++)]
					= PU_I[i][FTrellis[j][k].info_bit]
					+ PC_I[i][FTrellis[j][k].code_bit]
					//+ log_alpa[i][m_FTrellis.B[j][k].code_bit]
					+ log_alpa[i][j]
					+ log_beta[i + 1][FTrellis[j][k].next_state];
			}
		// if Max-Log-MAP is used, maximum of the above value is the LLR.....
		// Actually, all the logmap maxlogmap blablabla is determined how to calculate the log-sum-exponential values...
		for (j = 0; j < m_Ninfo; j++) {
			tmp4[j] = log_sum_exp(tmp3[j], m_Nstate);
		}
		double 	tot_app;
		tot_app = log_sum_exp(tmp4, m_Ninfo);
		for (j = 0; j < m_Ninfo; j++)
			PU_O[i][j] = tmp4[j] - tot_app;
	}
}

void Turb::decoding(vector<vector<double>>& PC_I, double	**PU_I, double	**PU_O) {


	int		i, j, k;
	//=====================================================
	//				Initialization of feedforward/back recursion
	//=====================================================

	log_alpa[0][0] = log_beta[Bsize][0] = 0;
	for (i = 1; i < m_Nstate; i++)	log_alpa[0][i] = mInfty;
	for (i = 1; i < m_Nstate; i++)	log_beta[Bsize][i] = mInfty;

	//----------------   Iteration to update Alpha  ----------------------------

	for (i = 0; i < Bsize; i++) {
		for (j = 0; j < m_Nstate; j++) {
			for (k = 0; k < m_Ninfo; k++) {
				tmp1[k] = PU_I[i][BTrellis[j][k].info_bit]
					+ PC_I[i][BTrellis[j][k].code_bit]
					+ log_alpa[i][BTrellis[j][k].prev_state];
				//tmp1[k] =0;
			}
			tmp2[j] = log_sum_exp(tmp1, m_Ninfo);
		}
		tot_log_a[i] = log_sum_exp(tmp2, m_Nstate);
		//============================================================
		// normalization to make log_alpha[s]=0
		//============================================================
		for (j = 0; j < m_Nstate; j++)
			log_alpa[i + 1][j] = tmp2[j] - tot_log_a[i];
	}
	/*
	for ( i = 0 ; i < m_Bsize ; i++ ) {
	printf("%d : ",EXT_TB->m_state1_seq[i]);
	for ( j = 0 ; j < m_Nstate ; j++ )	{
	printf("%lf ", log_alpa[i][j] );
	}
	printf("\n");
	getchar();
	}
	*/
	//----------------   Iteration to update Beta  ----------------------------

	for (i = Bsize - 1; i >= 0; i--) {
		for (j = 0; j < m_Nstate; j++) {
			for (k = 0; k < m_Ninfo; k++) {
				tmp1[k] = PU_I[i][FTrellis[j][k].info_bit]
					+ PC_I[i][FTrellis[j][k].code_bit]
					+ log_beta[i + 1][FTrellis[j][k].next_state];
				//tmp1[k]=0;
			}
			tmp2[j] = log_sum_exp(tmp1, m_Ninfo);
		}
		for (j = 0; j < m_Nstate; j++)
			log_beta[i][j] = tmp2[j] - tot_log_a[i];
	}


	//----------------   APP Calculation ---------------------------------------
	for (i = 0; i < Bsize; i++) {
		for (j = 0; j < m_Nstate; j++) tmp5[j] = 0;
		for (j = 0; j < m_Nstate; j++)
			for (k = 0; k < m_Ninfo; k++) {
				tmp3[FTrellis[j][k].info_bit][(tmp5[FTrellis[j][k].info_bit]++)]
					= PU_I[i][FTrellis[j][k].info_bit]
					+ PC_I[i][FTrellis[j][k].code_bit]
					//+ log_alpa[i][m_FTrellis.B[j][k].code_bit]
					+ log_alpa[i][j]
					+ log_beta[i + 1][FTrellis[j][k].next_state];
			}
		// if Max-Log-MAP is used, maximum of the above value is the LLR.....
		// Actually, all the logmap maxlogmap blablabla is determined how to calculate the log-sum-exponential values...
		for (j = 0; j < m_Ninfo; j++) {
			tmp4[j] = log_sum_exp(tmp3[j], m_Nstate);
		}
		double 	tot_app;
		tot_app = log_sum_exp(tmp4, m_Ninfo);
		for (j = 0; j < m_Ninfo; j++)
			PU_O[i][j] = tmp4[j] - tot_app;
	}
}

vector<bool> Turb::Attach(vector<bool> a, vector<bool> b)
{
	int size = a.size();
	vector<bool> result(2*size);

	for (int i = 0; i < size; i++) {
		result[i] = a[i];
		result[i + size] = b[i];
	}
	return result;
}

void Turb::turbo_bit2sym(double* rx_llr0_buf, double *rx_llr1_buf, double **LLR1, double **LLR2, int SP_NCODEBITperSYM, int SP_NCODEBIT, int SP_NCODE) {
	//series to parallel
	int	i, j, k, l, h;
	double	t;

	for (i = 0, j = 0, k = 1; i < SP_NCODEBITperSYM - (4 * m_Nmemory); i++)
	{
		if (i % 3 == 0)
		{
			info0[i / 3] = rx_llr0_buf[i];
			info1[i / 3] = rx_llr1_buf[i];
			parity1_0[i - j] = rx_llr0_buf[i];
			parity1_1[i - j] = rx_llr0_buf[i];

		}
		if (i % 3 == 1)
		{
			parity1_0[i - j] = rx_llr0_buf[i];
			parity1_1[i - j] = rx_llr1_buf[i];
		}
		if (i % 3 == 2)
		{
			parity2_0[i / 3 + k] = rx_llr0_buf[i];
			parity2_1[i / 3 + k] = rx_llr1_buf[i];
			j++;
			k++;
		}
	}

	//interleaving
	for (j = 0; j < m_Bsize; ++j)
	{
		info0_int[j] = info0[inter_pattern[j]];
		info1_int[j] = info1[inter_pattern[j]];
	}

	for (i = 0; i < 2 * m_Bsize; i += 2)
	{
		parity2_0[i] = info0_int[i / 2];
		parity2_1[i] = info1_int[i / 2];

	}

	//tail bit paralell to series
	for (i = (SP_NCODEBITperSYM - (4 * m_Nmemory)), j = 0, h = (2 * m_Bsize), k = (2 * m_Bsize); i < SP_NCODEBITperSYM; i += 2, j++)
	{//tail1 in tail1 out
		if (j % 2 == 0) {
			parity1_0[h] = rx_llr0_buf[i];
			parity1_1[h] = rx_llr1_buf[i];
			parity1_0[h + 1] = rx_llr0_buf[i + 1];
			parity1_1[h + 1] = rx_llr1_buf[i + 1];
			h += 2;
		}

		//tail2 in tail2 out
		else
		{
			parity2_0[k] = rx_llr0_buf[i];
			parity2_1[k] = rx_llr1_buf[i];
			parity2_0[k + 1] = rx_llr0_buf[i + 1];
			parity2_1[k + 1] = rx_llr1_buf[i + 1];
			k += 2;
		}

	}


	//=========================================================
	//              Bit LLR to Symbol LLR #1
	//=========================================================
	for (i = 0, l = 0; i < 2 * (m_Bsize + m_Nmemory); i += SP_NCODEBIT, l++) {
		for (j = 0; j < SP_NCODE; j++) {	// 2bit.
											// LSB first, MSB last.......
			for (k = 0, t = 0; k < SP_NCODEBIT; k++) {	// 0,1,2,3..., depuncuring is not considered here... but could be included....
														// MSB first .....1, 0
														//t += ( ( (j&(0x01<<(SP_NCODEBIT-1-k)))==0 )? rx_llr_deint0_buf[i+k]: rx_llr_deint1_buf[i+k]);
				if ((j&(0x01 << (SP_NCODEBIT - 1 - k))) == 0)	t += parity1_0[i + k];
				else	t += parity1_1[i + k];
			}
			LLR1[l][j] = t;
		}
	}

	//=========================================================
	//              Bit LLR to Symbol LLR #2
	//=========================================================

	for (i = 0, l = 0; i < 2 * (m_Bsize + m_Nmemory); i += SP_NCODEBIT, l++) {
		for (j = 0; j < SP_NCODE; j++) {	// 2bit.
											// LSB first, MSB last.......
			for (k = 0, t = 0; k < SP_NCODEBIT; k++) {	// 0,1,2,3..., depuncuring is not considered here... but could be included....
														// MSB first .....1, 0
														//t += ( ( (j&(0x01<<(SP_NCODEBIT-1-k)))==0 )? rx_llr_deint0_buf[i+k]: rx_llr_deint1_buf[i+k]);
				if ((j&(0x01 << (SP_NCODEBIT - 1 - k))) == 0)	t += parity2_0[i + k];
				else	t += parity2_1[i + k];
			}
			LLR2[l][j] = t;
		}
	}

}


void Turb::turbo_bit2sym(vector<double> rx_llr0_buf, vector<double> rx_llr1_buf, vector<vector<double>>& LLR1, vector<vector<double>>& LLR2, int SP_NCODEBITperSYM, int SP_NCODEBIT, int SP_NCODE) {
	//series to parallel
	int	i, j, k, l, h;
	double	t;

	for (i = 0, j = 0, k = 1; i < SP_NCODEBITperSYM - (4 * m_Nmemory); i++)
	{
		if (i % 3 == 0)
		{
			info0_v[i / 3] = rx_llr0_buf[i];
			info1_v[i / 3] = rx_llr1_buf[i];
			parity1_0_v[i - j] = rx_llr0_buf[i];
			parity1_1_v[i - j] = rx_llr0_buf[i];

		}
		if (i % 3 == 1)
		{
			parity1_0_v[i - j] = rx_llr0_buf[i];
			parity1_1_v[i - j] = rx_llr1_buf[i];
		}
		if (i % 3 == 2)
		{
			parity2_0_v[i / 3 + k] = rx_llr0_buf[i];
			parity2_1_v[i / 3 + k] = rx_llr1_buf[i];
			j++;
			k++;
		}
	}

	//interleaving
	for (j = 0; j < m_Bsize; ++j)
	{
		info0_int_v[j] = info0_v[inter_pattern[j]];
		info1_int_v[j] = info1_v[inter_pattern[j]];
	}

	for (i = 0; i < 2 * m_Bsize; i += 2)
	{
		parity2_0_v[i] = info0_int_v[i / 2];
		parity2_1_v[i] = info1_int_v[i / 2];

	}

	//tail bit paralell to series
	for (i = (SP_NCODEBITperSYM - (4 * m_Nmemory)), j = 0, h = (2 * m_Bsize), k = (2 * m_Bsize); i < SP_NCODEBITperSYM; i += 2, j++)
	{//tail1 in tail1 out
		if (j % 2 == 0) {
			parity1_0_v[h] = rx_llr0_buf[i];
			parity1_1_v[h] = rx_llr1_buf[i];
			parity1_0_v[h + 1] = rx_llr0_buf[i + 1];
			parity1_1_v[h + 1] = rx_llr1_buf[i + 1];
			h += 2;
		}

		//tail2 in tail2 out
		else
		{
			parity2_0_v[k] = rx_llr0_buf[i];
			parity2_1_v[k] = rx_llr1_buf[i];
			parity2_0_v[k + 1] = rx_llr0_buf[i + 1];
			parity2_1_v[k + 1] = rx_llr1_buf[i + 1];
			k += 2;
		}

	}


	//=========================================================
	//              Bit LLR to Symbol LLR #1
	//=========================================================
	for (i = 0, l = 0; i < 2 * (m_Bsize + m_Nmemory); i += SP_NCODEBIT, l++) {
		for (j = 0; j < SP_NCODE; j++) {	// 2bit.
											// LSB first, MSB last.......
			for (k = 0, t = 0; k < SP_NCODEBIT; k++) {	// 0,1,2,3..., depuncuring is not considered here... but could be included....
														// MSB first .....1, 0
														//t += ( ( (j&(0x01<<(SP_NCODEBIT-1-k)))==0 )? rx_llr_deint0_buf[i+k]: rx_llr_deint1_buf[i+k]);
				if ((j&(0x01 << (SP_NCODEBIT - 1 - k))) == 0)	t += parity1_0_v[i + k];
				else	t += parity1_1_v[i + k];
			}
			LLR1[l][j] = t;
		}
	}

	//=========================================================
	//              Bit LLR to Symbol LLR #2
	//=========================================================

	for (i = 0, l = 0; i < 2 * (m_Bsize + m_Nmemory); i += SP_NCODEBIT, l++) {
		for (j = 0; j < SP_NCODE; j++) {	// 2bit.
											// LSB first, MSB last.......
			for (k = 0, t = 0; k < SP_NCODEBIT; k++) {	// 0,1,2,3..., depuncuring is not considered here... but could be included....
														// MSB first .....1, 0
														//t += ( ( (j&(0x01<<(SP_NCODEBIT-1-k)))==0 )? rx_llr_deint0_buf[i+k]: rx_llr_deint1_buf[i+k]);
				if ((j&(0x01 << (SP_NCODEBIT - 1 - k))) == 0)	t += parity2_0_v[i + k];
				else	t += parity2_1_v[i + k];
			}
			LLR2[l][j] = t;
		}
	}

}

