#include "Simulator.h"
#if NOW == CDRT_UL_8PSK
#include <iostream>
#include <fstream>	
#include <vector>
#include <string>
#include <math.h>
#include "AWGN.h"
#include "complex_new.h"
#include "Detector.h"
#include "Generation.h"
#include "Mapper.h"
#include "Rayleigh.h"
#include "Conv.h"
#include "Combining.h"
#include "Turbo2.h"


//#include "channel.h"	//for JTC
using namespace std;

//channel coding classfication
typedef enum channel { CONV, TURBO, UNCODED } CHANNEL;



ifstream file("parameter.txt");	//script file open


								//global parameters
int Frame = 0, m = 0, BER_Record_idx = 0, ITR = 0, ITR_RELAY = 0;  // SIZE=2^m으로 m을 결정해야함
double SNR = 0, SNRinit, Increment = 0, SNRMAX = 0, rate = 0, ER_Gain = 0, CR_Gain = 0,
RB_Gain = 0, CB_Gain = 0, P_alpha = 0, P_beta = 0,	//c는 QPSK를 위한 값
//필요없는 global variable
SR_Gain = 0, RD_Gain = 0, SB1_Gain = 0, SB2_Gain = 0, RB1_Gain = 0, RB2_Gain = 0;
string Mod = "", Title = "", Channel_code = "", Decoding = "", Fading = "";
vector<double> BER_Record, PER_Record, Thru_Record;
MOD Mod_int;
FADING Fad_Mod;
CHANNEL Ch;

//pre-sentence
void init();
void Write();
void Read();
void Sim();


int main() {

	while (!file.eof()) {
		Read();		//전역변수 설정
		Sim();	//Implement Simulation
		Write();
		init();
	}
	file.close();
	return 0;
}

void Constellation(vector<Complex<double>> obj) {
	ofstream Point("Constellation.m");
	Point << "Point = [" << endl;
	int m = obj.size();

	for (int i = 0; i < m; i++) {
		Point << obj[i].re << "\t";
		Point << obj[i].im << endl;
	}
	Point << "]" << endl
		<< "c1 = Point(:,1), c2 = Point(:,2)" << endl
		<< "figure" << endl
		<< "scatter(c1, c2, 'filled')" << endl
		<< "xlabel('I')" << endl
		<< "ylabel('Q')" << endl
		<< "grid on;" << endl;
}

void init() {
	Frame = 0, m = 0, BER_Record_idx = 0;  // SIZE=2^m으로 m을 결정해야함
	SNR = 0, SNRinit, Increment = 0, SNRMAX = 0, rate;	//c는 QPSK를 위한 값
	Mod = "", Title = "", Channel_code = "", Decoding = "", Fading = "";

}
//bool  Channelcoding_flag = false;


void Write() {
	string tmpper = Title, tmpber = Title, tmpthru = Title;

	tmpper.append("-PER.txt"), tmpber.append("-BER.txt"), tmpthru.append("-THRU.txt");
	ofstream per(tmpper), ber(tmpber), thru(tmpthru);
	int record_size = PER_Record.size();

	SNR = SNRinit;//maybe needed to be edited
	BER_Record_idx = 0;

	for (int i = 0; i < record_size; i++, SNR += Increment, BER_Record_idx++) {
		per << SNR << "\t" << PER_Record[BER_Record_idx] << endl;
	}

	SNR = SNRinit;//maybe needed to be edited
	BER_Record_idx = 0;

	for (int i = 0; i < record_size; i++, SNR += Increment, BER_Record_idx++) {
		ber << SNR << "\t" << BER_Record[BER_Record_idx] << endl;
	}

	SNR = SNRinit;//maybe needed to be edited
	BER_Record_idx = 0;

	for (int i = 0; i < record_size; i++, SNR += Increment, BER_Record_idx++) {
		thru << SNR << "\t" << Thru_Record[BER_Record_idx] << endl;
	}

	per.close();
	ber.close();
	thru.close();
}

void Read() {
	string str = "";

	while (str != "EofS") {
		file >> str;
		if (str == "Frame")
			file >> Frame;
		else if (str == "Size")
			file >> m;
		else if (str == "Increment")
			file >> Increment;
		else if (str == "Mod") {
			file >> Mod;
			if (Mod == "BPSK")
				Mod_int = BPSK;
			else if (Mod == "QPSK")
				Mod_int = QPSK;
			else if (Mod == "16QAM")
				Mod_int = QAM16;
			else if (Mod == "64QAM")
				Mod_int = QAM64;

		}
		else if (str == "Title")
			file >> Title;
		else if (str == "SNRMAX")
			file >> SNRMAX;
		else if (str == "SNRinit")
			file >> SNRinit;
		else if (str == "Channel_coding")
		{
			file >> Channel_code;
			if (Channel_code == "No")
				Ch = UNCODED;
			else if (Channel_code == "CONV")
				Ch = CONV;
			else if (Channel_code == "TURBO")
				Ch = TURBO;
		}
		else if (str == "ITR")
			file >> ITR;
		else if (str == "ITR_RELAY")
			file >> ITR_RELAY;
		else if (str == "Fading") {//Rayleigh or JTC
			file >> Fading;
			if (Fading == "No")
				Fad_Mod = No;
			else if (Fading == "Rayleigh")
				Fad_Mod = Rayl;
			else if (Fading == "Rayleigh_Quasi")
				Fad_Mod = Rayl_Quasi;
			else if (Fading == "STBC")
				Fad_Mod = STBC;
			else if (Fading == "STBC_Quasi")
				Fad_Mod = STBC_Quasi;
			else if (Fading == "CIOD")
				Fad_Mod = CIOD;
			else if (Fading == "CIOD_Quasi")
				Fad_Mod = CIOD_Quasi;
		}
		else if (str == "Code_rate")//coderate
			file >> rate;
		else if (str == "SR_Gain")
			file >> SR_Gain;
		else if (str == "RD_Gain")
			file >> RD_Gain;
		else if (str == "SB1_Gain")
			file >> SB1_Gain;
		else if (str == "SB2_Gain")
			file >> SB2_Gain;
		else if (str == "RB1_Gain")
			file >> RB1_Gain;
		else if (str == "RB2_Gain")
			file >> RB2_Gain;

		//======================
		else if (str == "ER_Gain")
			file >> ER_Gain;
		else if (str == "CR_Gain")
			file >> CR_Gain;
		else if (str == "RB_Gain")
			file >> RB_Gain;
		else if (str == "CB_Gain")
			file >> CB_Gain;
		else if (str == "P_alpha") {
			file >> P_alpha;
			P_beta = 1 - P_alpha;
		}
	}
}


void Sim() {

	//=============================Class Instances==========================================================================
	Generation Gen;
	Mapper CEU_Map, CCU_Map, Relay_Map;
	Rayleigh Ray;
	AWGN AWGN_ER, AWGN_CR, AWGN_RB, AWGN_CB;
	Detector Detect;
	Comb Comb;
	Conv Conv;
	Turb turb;


	CEU_Map.init(Mod_int);
	CEU_Map.map_tab_gen(P_alpha);

	Relay_Map.init(Mod_int);
	Relay_Map.map_tab_gen(P_alpha);

	CCU_Map.init(Mod_int);
	CCU_Map.map_tab_gen(P_beta);
	//==========================Simulation Parameter======================================================================


	int Size = (1 << m);

	int SP_NINFOBITperSYM = Size * Mod_int / rate;
	int SP_NCODEBITperSYM = Size * Mod_int;
	int SP_NDCARperSYM = Size;
	int NCODEBIT = 2;	//
	int SP_NCODE = pow(2, NCODEBIT);

	//======================Trellis diagram===============================================================================================

	int Convsize;
	double* llr0, *llr1, *SUPER_llr0, *SUPER_llr1, *RELAY_llr0, *RELAY_llr1;
	double** ConvLLR, **LLR1, **LLR2, **SUPER_LLR1, **SUPER_LLR2, **RELAY_LLR1, **RELAY_LLR2;

	int g[2] = { 0x0D,0x0B };//trellis polynomial for convolutional
	llr0 = NULL, llr1 = NULL, ConvLLR = NULL, LLR1 = NULL, LLR2 = NULL, RELAY_llr0 = NULL, RELAY_llr1 = NULL, RELAY_LLR1 = NULL, RELAY_LLR2 = NULL,
		SUPER_llr0 = NULL, SUPER_llr1 = NULL, SUPER_LLR1 = NULL, SUPER_LLR2 = NULL;

	switch (Ch) {
	case CONV:
		//==============================Convolution============================================================================
		Conv.trellis_gen(g, SP_NINFOBITperSYM, 2, NCODEBIT, 1);

		llr0 = (double*)malloc(sizeof(double) * SP_NCODEBITperSYM),
			llr1 = (double*)malloc(sizeof(double) * SP_NCODEBITperSYM);

		ConvLLR = (double**)malloc(sizeof(int) * 2 * SP_NCODEBITperSYM);
		for (int i = 0; i < SP_NCODEBITperSYM; i++) {
			ConvLLR[i] = (double*)malloc(SP_NCODE * sizeof(int) * 2);
		}

		break;

	case TURBO:
		//==================================Turbo================================================================================

		turb.turbo_trellis_gen(g, Size, 3, 2, 1);
		turb.int_init();

		SP_NCODEBITperSYM = (3 * Size) + (4 * turb.m_Nmemory);
		SP_NDCARperSYM = SP_NCODEBITperSYM / Mod_int;

		llr0 = (double*)malloc(sizeof(double) * SP_NCODEBITperSYM);
		llr1 = (double*)malloc(sizeof(double) * SP_NCODEBITperSYM);

		RELAY_llr0 = (double*)malloc(sizeof(double) * SP_NCODEBITperSYM);
		RELAY_llr1 = (double*)malloc(sizeof(double) * SP_NCODEBITperSYM);

		SUPER_llr0 = (double*)malloc(sizeof(double) * SP_NCODEBITperSYM);
		SUPER_llr1 = (double*)malloc(sizeof(double) * SP_NCODEBITperSYM);

		LLR1 = (double**)malloc(sizeof(double) * (Size + turb.m_Nmemory));
		LLR2 = (double**)malloc(sizeof(double) * (Size + turb.m_Nmemory));

		RELAY_LLR1 = (double**)malloc(sizeof(double) * (Size + turb.m_Nmemory));
		RELAY_LLR2 = (double**)malloc(sizeof(double) * (Size + turb.m_Nmemory));

		SUPER_LLR1 = (double**)malloc(sizeof(double) * (Size + turb.m_Nmemory));
		SUPER_LLR2 = (double**)malloc(sizeof(double) * (Size + turb.m_Nmemory));

		for (int i = 0; i < (Size + turb.m_Nmemory); i++) {
			LLR1[i] = (double*)malloc(SP_NCODE * sizeof(double));
			LLR2[i] = (double*)malloc(SP_NCODE * sizeof(double));

			RELAY_LLR1[i] = (double*)malloc(SP_NCODE * sizeof(double));
			RELAY_LLR2[i] = (double*)malloc(SP_NCODE * sizeof(double));

			SUPER_LLR1[i] = (double*)malloc(SP_NCODE * sizeof(double));
			SUPER_LLR2[i] = (double*)malloc(SP_NCODE * sizeof(double));
		}

		turb.log_sum_exp_lut_generation();
		break;

	case UNCODED:

		break;

	default:

		break;
	}


	//=============================initiallization=============================================================================
	double Eb = 0, RELAY_Eb = 0, BER, PER, SNR_SR, SNR_RD, LC;
	int err, perr, count;
	bool DEC_flag = true;
	SNR = SNRinit;
	BER_Record.resize((SNRMAX - SNR + Increment) / (Increment), 0);
	PER_Record.resize((SNRMAX - SNR + Increment) / (Increment), 0);
	Thru_Record.resize((SNRMAX - SNR + Increment) / (Increment), 0);
	BER_Record_idx = 0;


	//===================================================================================================================

	//==============================Buffer===============================================================================
	vector<vector<bool>> source, relay_source; // Block Buffer
	vector<bool> source_int, relay_int, code_source, code_relay, encoded_source, demapped_source, decoded_source,
		encoded_relay, decoded_relay, decoded_CCU, decoded_CEU, decoded_CB, decoded_BS;//Packet buffer
	vector<Complex<double>> tx_CCU, tx_CEU, tx_Relay, ER_RX, CR_RX, RB_RX, CB_RX,
		RayFading_ER, RayFading_CR, RayFading_RB, RayFading_CB;
	vector<double> LLR_ER, LLR_CR, LLR_RB, LLR_CB;	//channel gain
	vector<vector<double>> LLR_FIRST, LLR_SECOND;
	//====================================================================================================================

	std::cout << "Simulation : " << Title << endl;
	std::cout << "SNR\tBER\t\tPER\tThroughput" << endl;



	//------------------------------------------------------------------------------------------
	//-----------------------------Main loop----------------------------------------------------
	//1.Source generate
	//2.Channel coding or Not
	//3.Constellation Mapping 
	//4.Constellation Rotate and interleaving or not (SSD OR CIOD)
	//5.Fading or Not
	//6.AWGN
	//7.Channel 보상
	//8.Decoding and Detection
	//9.ERROR CHECK
	//10.(RELAY) IF ERROR OCCUR 
	//------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------




	do {



		count = 0;
		BER = 0;
		err = 0;
		PER = 0;
		perr = 0;
		Detect.nOfTx = 0;
		Detect.sOfDec = 0;
		//==============Source Generating==================================
		switch (Ch) {
		case CONV:
			Convsize = Size * Mod_int / rate - Conv.Nmemory;
			Gen.Generator(Frame, Convsize, source);
			break;
		case TURBO:
			Convsize = Size;	//Convsize = Size * Mod_int / rate - turb.m_Nmemory;
			Gen.Generator(Frame, Convsize, source);
			Gen.Generator(Frame, Convsize, relay_source);
			break;
		case UNCODED:
			Gen.Generator(Frame, SP_NINFOBITperSYM, source);
			Convsize = Size * Mod_int / rate - Conv.Nmemory;
			break;
		}

		for (int h = 0; h < Frame; ++h)
		{
			code_source = source[h];
			code_relay = relay_source[h];

			//=================================================================

			//===============CHANNEL CODING====================================
			switch (Ch) {
			case CONV:
				for (int yy = 0; yy < Conv.Nmemory; ++yy)
					code_source.push_back(0);


				Conv.encoder_init();
				encoded_source = Conv.Conv_encoder(code_source, rate);

				Eb = 1.0 * rate;
				break;

			case TURBO:
				Eb = 1.0 * rate;
				RELAY_Eb = 1.0 * rate;

				source_int = turb.interleaver(code_source);
				relay_int = turb.interleaver(code_relay);

				encoded_source = turb.encode(code_source, source_int);
				encoded_relay = turb.encode(code_relay, relay_int);
				break;

			case UNCODED:
				encoded_source = code_source;
				Eb = 1.0;
				break;
			}
			//=================================================================		

			//=============Modulation==========================================
			switch (Mod_int) {
			case(BPSK): {
				Eb = Eb * 1.0;
				CCU_Map.BPSK_Mapping(encoded_source, tx_CCU);
				break;
			}
			case(QPSK): {
				Eb = Eb * P_beta;
				RELAY_Eb = RELAY_Eb * P_alpha;

				CCU_Map.QPSK_Mapping(encoded_source, tx_CCU, P_beta);
				Relay_Map.QPSK_Mapping(encoded_relay, tx_Relay, P_alpha);
				Relay_Map.rot(tx_Relay, 45);
				Relay_Map.rotator(Relay_Map.m_2d_map_tab, 45);

				CEU_Map.QPSK_Mapping(encoded_relay, tx_CEU, P_alpha);
				CEU_Map.rot(tx_CEU, 45);
				CEU_Map.rotator(CEU_Map.m_2d_map_tab, 45);

				break;
			}
			case(QAM16): {
				Eb = Eb * 2.5;
				CCU_Map.QAM_Mapping(encoded_source, tx_CCU);
				break;
			}
			case(QAM64): {
				Eb = Eb * 7;
				CCU_Map.QAM64_Mapping(encoded_source, tx_CCU);

				break;
			}
			}

			//============Fading Channel=======================================
			//Fading
			switch (Fad_Mod) {
			case(Rayl): {
				Ray.Rayleigh_Fading(tx_CCU, RayFading_CB, CB_RX, LLR_CB);
				break;
			}
			case(Rayl_Quasi): {
				Ray.slow_Rayleigh_Fading(tx_CCU, RayFading_CB, CB_RX, LLR_CB);
				Ray.slow_Rayleigh_Fading(tx_CCU, RayFading_CR, CR_RX, LLR_CR);
				Ray.slow_Rayleigh_Fading(tx_CEU, RayFading_ER, ER_RX, LLR_ER);
				Ray.slow_Rayleigh_Fading(tx_Relay, RayFading_RB, RB_RX, LLR_RB);
				break;
			}
			default: {
				//rx_source = tx_source;
				//LLR_SB1.resize(tx_source.size());
				//for (unsigned int z = 0; z < tx_source.size(); ++z)
				//	LLR_SB1[z] = 1;
			}
			}
			//=================================================================

			//===========AWGN channel==========================================
			//AWGN_CB.Awgn(SNR + CB_Gain, Eb, CB_RX);
			//AWGN_CR.Awgn(SNR + CR_Gain, Eb, CR_RX);
			//AWGN_ER.Awgn(SNR + ER_Gain, RELAY_Eb, ER_RX);
			//AWGN_RB.Awgn(SNR + RB_Gain, RELAY_Eb, RB_RX);
			//=================================================================

			//===========Coherent=============================================

			switch (Fad_Mod) {
			case(Rayl): {
				Ray.Coherent(RayFading_CB, CB_RX, LLR_CB);
				break;
			}
			case(Rayl_Quasi): {
				Ray.Quasi_Coherent(RayFading_CB, CB_RX, LLR_CB);
				Ray.Quasi_Coherent(RayFading_CR, CR_RX, LLR_CR);
				Ray.Quasi_Coherent(RayFading_ER, ER_RX, LLR_ER);
				Ray.Quasi_Coherent(RayFading_RB, RB_RX, LLR_RB);

				break;
			}
			default: {
				break;
			}
			}

			//===========Decoding==============================================


			switch (Ch) {
			case CONV:
				//Conv.init_dec_state();
				//Map.llr_generation(Fad_Mod, rx_source, llr_wgt_sd, llr0, llr1, &Map, SP_NDCARperSYM);
				//Map.bit2symLLR_generation(llr0, llr1, ConvLLR, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
				//decoded_source = Conv.decoding(ConvLLR);
				break;

			case TURBO:
#if COMB == MRComb		Comb.COOP_MRC(Fad_Mod, SNR + SB1_Gain, SB1_RX, LLR_SB1, SNR + SB2_Gain, SB2_RX, LLR_SB2);   //SLC
#elif COMB == EGComb	Comb.COOP_EGC(Fad_Mod, SNR + SB1_Gain, SB1_RX, LLR_SB1, SNR + SB2_Gain, SB2_RX, LLR_SB2);   //SLC
#endif

				//CB decoding
				Detect.nOfTx++;
				LC = -1.0 / (2 * AWGN_CB.sigma2);
				turb.turbo_llr_generation(Fad_Mod, CB_RX, LLR_CB, llr0, llr1, &CCU_Map, CB_RX.size(), LC);
				turb.turbo_bit2sym(llr0, llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
				decoded_CCU = turb.turbo_decoding(LLR1, LLR2, ITR);




				//ER decoding
				CCU_Map.Interference(ER_RX, CR_RX);
				Constellation(CR_RX);

				Detect.nOfTx++;
				if (P_beta > P_alpha) {	//CR channel 상태가 더 좋으면
					LC = -1.0 / (2 * AWGN_CR.sigma2);
					turb.turbo_llr_generation(Fad_Mod, CR_RX, LLR_CR, llr0, llr1, &CCU_Map, CR_RX.size(), LC);
					turb.turbo_bit2sym(llr0, llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
					decoded_relay = turb.turbo_decoding(LLR1, LLR2, ITR);

					if (!Detect.Packet(code_source, decoded_relay, SP_NINFOBITperSYM)) {	//CR decoding에 성공하면, SIC후 decoding CEU
						LC = -1.0 / (2 * AWGN_ER.sigma2);
						turb.turbo_llr_generation(Fad_Mod, ER_RX, LLR_ER, llr0, llr1, &CEU_Map, ER_RX.size(), LC);
						turb.turbo_bit2sym(llr0, llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
						decoded_CEU = turb.turbo_decoding(LLR1, LLR2, ITR);
					}
					else;	//CR decoding에 실패하면, CEU Tx fail
				}
				if (P_beta <= P_alpha) {	//ER channel 상태가 더 좋으면
					LC = -1.0 / (2 * AWGN_ER.sigma2);
					turb.turbo_llr_generation(Fad_Mod, ER_RX, LLR_ER, llr0, llr1, &Relay_Map, ER_RX.size(), LC);
					turb.turbo_bit2sym(llr0, llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
					decoded_CEU = turb.turbo_decoding(LLR1, LLR2, ITR);
				}


				if (!decoded_CEU.empty() && !Detect.Packet(code_relay, decoded_CEU, SP_NINFOBITperSYM)) {	//CEU를 decoding 했다면
					Detect.nOfTx++;
					CCU_Map.Interference(RB_RX, CB_RX);

					if (P_beta > P_alpha) {
						LC = -1.0 / (2 * AWGN_CB.sigma2);
						turb.turbo_llr_generation(Fad_Mod, CB_RX, LLR_CB, llr0, llr1, &CCU_Map, CB_RX.size(), LC);
						turb.turbo_bit2sym(llr0, llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
						decoded_CB = turb.turbo_decoding(LLR1, LLR2, ITR);

						if (!Detect.Packet(code_source, decoded_CB, SP_NINFOBITperSYM)) {	//CB decoding에 성공하면, SIC후 decoding CEU
							LC = -1.0 / (2 * AWGN_RB.sigma2);
							turb.turbo_llr_generation(Fad_Mod, RB_RX, LLR_RB, llr0, llr1, &Relay_Map, RB_RX.size(), LC);
							turb.turbo_bit2sym(llr0, llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
							decoded_BS = turb.turbo_decoding(LLR1, LLR2, ITR);
						}
					}

					if (P_beta <= P_alpha) {
						LC = -1.0 / (2 * AWGN_RB.sigma2);
						turb.turbo_llr_generation(Fad_Mod, RB_RX, LLR_RB, llr0, llr1, &Relay_Map, RB_RX.size(), LC);
						turb.turbo_bit2sym(llr0, llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
						decoded_BS = turb.turbo_decoding(LLR1, LLR2, ITR);
					}


				}

				break;

			case UNCODED:
				//switch (Mod_int) {
				//case BPSK:
				//	Map.BPSK_Demapping(rx_source, rx_source.size(), demapped_source);
				//	break;
				//case QPSK:
				//	Map.QPSK_Demapping(rx_source, rx_source.size(), demapped_source);
				//	break;
				//case QAM16:
				//	Map.QAM_Demapping(rx_source, rx_source.size(), demapped_source);
				//	break;
				//case QAM64:
				//	Map.QAM64_Demapping(rx_source, rx_source.size(), demapped_source);
				//	break;
				//}
				//decoded_source = demapped_source;
				break;
			}



			//			if (Detect.Packet(code_source, decoded_CCU, SP_NINFOBITperSYM)) {    //CRC fail @ the gateway
			//						//RE_TX @ Source & Combining
			//#if COMB == MRComb		Comb.MRC(Fad_Mod, SB1_RX, LLR_SB1, RE_RX, LLR_RE);
			//#else COMB == EGComb	Comb.EGC(Fad_Mod, SB1_RX, LLR_SB1, RE_RX, LLR_RE);
			//#endif
			//				LC = -1.0 / (2 * AWGN_SB1.sigma2);
			//				turb.turbo_llr_generation(Fad_Mod, SB1_RX, LLR_RE, llr0, llr1, &Map, SB1_RX.size(), LC);
			//				turb.turbo_bit2sym(llr0, llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
			//				decoded_source = turb.turbo_decoding(LLR1, LLR2, ITR);
			//				count++;
			//				perr += DEC_flag;
			//			}
			//
			//			else {//source decoding이 한 번에 성공
			//#if		COMB == MRComb Comb.COOP_MRC(Fad_Mod, SNR + RB1_Gain, SB1_RX, LLR_RB1, SNR + RB2_Gain, RB2_RX, LLR_RB2);   //SLC
			//#elif	COMB == EGComb Comb.COOP_EGC(Fad_Mod, SNR + RB1_Gain, SB1_RX, LLR_RB1, SNR + RB2_Gain, RB2_RX, LLR_RB2);   //SLC
			//#endif
			//				LC = -1.0 / (2 * AWGN_RB1.sigma2);
			//				turb.turbo_llr_generation(Fad_Mod, RB1_RX, LLR_RB1, llr0, llr1, &RELAY_Map, RB1_RX.size(), LC);
			//				turb.turbo_bit2sym(llr0, llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
			//				decoded_relay = turb.turbo_decoding(LLR1, LLR2, ITR);
			//				//Detect.Detection(code_relay, decoded_relay, err, Size);
			//			};

			switch (Ch) {
			case CONV:
				Detect.Detection(code_source, decoded_source, err, Convsize);
				break;
			case TURBO:


				Detect.Detection(code_source, decoded_CCU, err, Size);
				if (!Detect.Packet(code_source, decoded_CCU, Size))
					Detect.sOfDec++;

				if (!decoded_BS.empty()) {
					Detect.Detection(code_relay, decoded_BS, err, Size);
					if (!Detect.Packet(code_relay, decoded_BS, Size))
						Detect.sOfDec++;
				}
				break;
			case UNCODED:
				Detect.Detection(code_source, decoded_source, err, SP_NINFOBITperSYM);
				break;
			}

			//memory management
			vector<Complex<double>> tx_source, RE_RX, SB1_RX, SB2_RX, SR_RX, tx_Relay, tx_SourceAtRelay, Tmp_tx_Relay, RB1_RX, RB2_RX,
				RE_RayFading, RayFading_SB1, RayFading_SB2, RayFading_SR, RayFading_RB1, RayFading_RB2;
			vector<double> LLR_SB1, LLR_SB2, LLR_SR, LLR_RB1, LLR_RB2, LLR_RE;	//channel gain

			vector<bool>(0).swap(code_source), vector<bool>(0).swap(encoded_source), vector<bool>(0).swap(decoded_source),
				vector<bool>(0).swap(demapped_source), vector<Complex<double>>(0).swap(tx_source),

				vector<Complex<double>>(0).swap(RE_RX), vector<double>(0).swap(LLR_SB1),
				vector<Complex<double>>(0).swap(SB1_RX), vector<double>(0).swap(LLR_SB2),
				vector<Complex<double>>(0).swap(SB2_RX), vector<double>(0).swap(LLR_SR),
				vector<Complex<double>>(0).swap(SR_RX), vector<double>(0).swap(LLR_RB1),
				vector<Complex<double>>(0).swap(tx_Relay), vector<double>(0).swap(LLR_RB2),
				vector<Complex<double>>(0).swap(tx_SourceAtRelay), vector<double>(0).swap(LLR_RE),
				vector<Complex<double>>(0).swap(Tmp_tx_Relay),
				vector<Complex<double>>(0).swap(RB1_RX),
				vector<Complex<double>>(0).swap(RB2_RX);

			//vector<bool>(0).swap(decoded_CEU),

		}


		Ch == TURBO ? BER = (double)err / ((double)Frame * (double)Size * 2) : (double)err / ((double)Frame * (double)Convsize);
		Detect.Throughput = 13.3 * 2 * (1.0 - BER);

		PER = (double)perr / Frame;
		BER_Record[BER_Record_idx] = BER;
		PER_Record[BER_Record_idx] = PER;
		Thru_Record[BER_Record_idx++] = Detect.Throughput;

		std::cout << SNR << "\t" << BER << "\t" << PER << "\t" << Detect.Throughput << endl;

		//memory management
		for (int mm = 0; mm < Frame; mm++)
			vector<bool>(0).swap(source[mm]);

		vector<vector<bool>>(0).swap(source);

		SNR += Increment;
	} while (SNR <= (SNRMAX));
	std::cout << endl << "----------------------" << endl;
}
#endif