#include "Conv.h"

void Conv::Trellis_clear() {
	FTrellis.clear();
	BTrellis.clear();
}

void Conv::encoder_init() {
	m_enc_prev_state = 0;
}


Conv::~Conv()
{
}

void Conv::trellis_gen(int *g, int m_Bsize, int m_Nmemory, int m_Ncodebit, int m_Ninfo)
{//Parameter
 //g= generator polynomial ,Bsize= Block size  ,Nstate=Number of state  ,Nmemory=Nuber of memory 
 //	Ncodebit=Coded bit size(ex rate 1/2 Ncodebit= 2) ,Ncode=Number of coded , Ninfo= information bit size , Ninfo= number of information 
 //
 //initialize parameter
	Bsize = m_Bsize;
	Nstate = pow(2, m_Nmemory);
	Nmemory = m_Nmemory;
	Ncode = (int)pow(2, m_Ncodebit);
	Ninfo = (int)pow(2, m_Ninfo);

	//Forward && Back Trellis setting
	FTrellis.resize(Nstate);//
	for (int h = 0; h < Nstate; ++h)
		FTrellis[h].resize(m_Ncodebit);

	BTrellis = FTrellis;


	//Polynomial generation
	int	i, j, k, l;
	//int	cur_state, t;
	int	g_and_s, xor_gns, bit_flag;
	int	t_code;
	//=============================================
	//               forward trellis generation
	//		for the time being, let's focus only on generator polynomial
	//		and only for not recursive polynomial 
	//	especially g[0]=5, g[1]=7
	// polynomial 은 좌측부터가 1,D,D2
	//  state 는 좌측부터가 MSB 이다.
	//=============================================
	for (i = 0; i < Nstate; i++) {
		for (j = 0; j <= 1; j++) {
			// 현재 메모리의 비트들을 우측으로 옮기고, 좌측에는 정보 비트를 입력한다. 좌측이 MSB이다.
			FTrellis[i][j].next_state = ((i >> 1) | (j << (m_Nmemory - 1)));
			FTrellis[i][j].info_bit = j;
			t_code = 0;
			for (k = 0; k < m_Ncodebit; k++) {
				// for each bit plane, multiply with generator polynomial
				// make bits including the input bits, input bits |
				//t=( (cur_state | (j<<(m_Nmem)) )&g[k]);
				// (input_bit, current_state) & generator polynomial
				g_and_s = (((j << (m_Nmemory)) | i) & g[k]);
				// xor every bits... (summation)
				for (l = 0, xor_gns = 0, bit_flag = 0x01; l <= m_Nmemory; l++)
					xor_gns ^= ((g_and_s &(bit_flag << l)) >> l);
				if ((xor_gns != 0) && (xor_gns != 1)) { printf("something terribly wrong\n"); exit(1); }
				t_code |= (xor_gns << k);// 수정 code[g[1]] code[g[0]] ---> code[g[0]] code [g[1]]
			}
			FTrellis[i][j].code_bit = t_code;
		}
	}

	//=============================================
	//               Backward trellis generation
	//=============================================
	for (i = 0; i < Nstate; i++) {
		l = 0;
		for (j = 0; j < Nstate; j++) {
			for (k = 0; k < Ninfo; k++) {
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


void Conv::weighting(vector<double>h_gain) {
	for (unsigned int j = 0; j < uc.size(); ++j)
	{
		for (unsigned char h = 0; h < Ncode; h++)
		{
			uc[j][h] = uc[j][h] * h_gain[j];
		}

	}

}




vector<bool> Conv::Conv_encoder(vector<bool>&input, double rate) {
	vector<bool>result(input.size() * rate);
	int	 info_bit;
	for (unsigned int i = 0; i <input.size(); i++) {
		info_bit = input[i];
		result[2 * i] = ((FTrellis[m_enc_prev_state][info_bit].code_bit & 0x02) >> 1);	// MSB first												
		result[2 * i + 1] = (FTrellis[m_enc_prev_state][info_bit].code_bit & 0x01);	// LSB last

		m_enc_prev_state = FTrellis[m_enc_prev_state][info_bit].next_state;
	}
	return result;
}



//
//void Conv::llr2code(vector<double>llr0, vector<double>llr1, int Ncode)
//{
//	for (unsigned int i = 0;i < llr0.size();i += 2)
//	{
//		llr
//
//
//	}
//}






vector<bool>Conv::decoding(double **LLR)
{
	int i, j, k;
	double	tmp[16];
	double	max; int max_index;
	int 		dec_bit, cur_state, cur_info;
	vector<bool> result;
	result.resize(Bsize);
	for (i = 0; i < Bsize; i++) {
		//==================================
		//		Add Compare Select (ACS) Logic
		//==================================
		for (j = 0; j < Nstate; j++) {
			// ADD
			for (k = 0; k < Ninfo; k++) {
				tmp[k] = m_log_alpa[i][BTrellis[j][k].prev_state] + LLR[i][BTrellis[j][k].code_bit];
			}
			// Compare & Select
			max = tmp[0]; max_index = 0;
			for (k = 1; k<Ninfo; k++)
				if (tmp[k]>max) {
					max = tmp[k];
					max_index = k;
				}
			m_log_alpa[i + 1][j] = max;
			m_traceback_mem[i + 1][j] = max_index;
			//=================================
			// Normalization might be needed
			// Traceback might have to be done per stage.....
			// In this case the whole sequence is determined once..
			//=================================
		}
	}
	//=====================================================
	//		Trace Back just once....
	//=====================================================
	max = m_log_alpa[Bsize][0]; max_index = 0;
	/*for (j = 1; j<Nstate; j++)
	{
	if (m_log_alpa[Bsize][j]<min) {
	min = m_log_alpa[Bsize][j];
	min_index = j;
	}
	}*/
	cur_state = max_index;
	for (i = Bsize - 1; i >= 0; i--) {
		cur_info = m_traceback_mem[i + 1][cur_state];
		dec_bit = BTrellis[cur_state][cur_info].info_bit;
		result[i] = dec_bit;
		cur_state = BTrellis[cur_state][cur_info].prev_state;
		// next time, only the enhanced mode should be considered.......

	}
	//printf("%d %d %d\n",i,tx_sym_seq

	return result;
}



void	Conv::init_dec_state()
{
	unsigned int i;
	m_traceback_mem.resize(Bsize + 1);
	for (i = 0; i < Bsize + 1; i++)
		m_traceback_mem[i].resize(Nstate);


	m_log_alpa.resize((Bsize + 1));
	for (i = 0; i < Bsize + 1; ++i)
		m_log_alpa[i].resize(Nstate);

	m_log_alpa[0][0] = 0;
	for (int j = 1; j<Nstate; ++j)
		m_log_alpa[0][j] = -1000.0;
}


