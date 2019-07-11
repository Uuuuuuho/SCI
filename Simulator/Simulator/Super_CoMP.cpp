#include "Simulator.h"
#if NOW == SUPER_COMP
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
int Frame = 0, m = 0, BER_Record_idx = 0, ITR = 0, ITR_RELAY = 0;  // SIZE=2^m���� m�� �����ؾ���
double SNR = 0, SNRinit, Increment = 0, SNRMAX = 0, rate = 0, SB1_Gain = 0, SB2_Gain = 0, SR_Gain = 0, RB1_Gain = 0, RB2_Gain = 0, RD_Gain = 0;	//c�� QPSK�� ���� ��
string Mod = "", Title = "", Channel_code = "", Decoding = "", Fading = "";
vector<double> BER_Record, PER_Record;
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
		Read();		//�������� ����
		Sim();	//Implement Simulation
		Write();
		init();
	}
	file.close();
	return 0;
}



void init() {
	Frame = 0, m = 0, BER_Record_idx = 0;  // SIZE=2^m���� m�� �����ؾ���
	SNR = 0, SNRinit, Increment = 0, SNRMAX = 0, rate;	//c�� QPSK�� ���� ��
	Mod = "", Title = "", Channel_code = "", Decoding = "", Fading = "";

}
//bool  Channelcoding_flag = false;


void Write() {
	string tmpper = Title, tmpber = Title;

	tmpper.append("-PER.txt"), tmpber.append("-BER.txt");
	ofstream per(tmpper), ber(tmpber);
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

	per.close();
	ber.close();
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
	}
}


void Sim() {

	//=============================Class Instances==========================================================================
	Generation Gen;
	Mapper Map, SOURCE_Map, RELAY_Map, InCase_Map;
	Rayleigh Ray;
	AWGN AWGN_SB1, AWGN_SB2, AWGN_SR, AWGN_RB1, AWGN_RB2, AWGN_RE, AWGN_InCase1, AWGN_InCase2;
	Detector Detect;
	Comb Comb;
	Conv Conv;
	Turb turb;

	Map.init(Mod_int);
	Map.map_tab_gen(1.0);

	SOURCE_Map.init(Mod_int);
	SOURCE_Map.map_tab_gen(0.3);

	RELAY_Map.init(Mod_int);
	RELAY_Map.map_tab_gen(0.7);

	InCase_Map.init(Mod_int);
	InCase_Map.map_tab_gen(1.0);
	//==========================Simulation Parameter======================================================================


	int Size = (1 << m);

	int SP_NINFOBITperSYM = Size * Mod_int / rate;
	int SP_NCODEBITperSYM = Size * Mod_int;
	int SP_NDCARperSYM = Size;
	int NCODEBIT = 2;	//
	int SP_NCODE = pow(2, NCODEBIT);

	//======================Trellis diagram===============================================================================================

	int Convsize;
	double *llr0, *llr1, *SUPER_llr0, *SUPER_llr1, *RELAY_llr0, *RELAY_llr1;
	double **ConvLLR, **LLR1, **LLR2, **SUPER_LLR1, **SUPER_LLR2, **RELAY_LLR1, **RELAY_LLR2;

	int g[2] = { 0x0D,0x0B };//trellis polynomial for convolutional
	llr0 = NULL, llr1 = NULL, ConvLLR = NULL, LLR1 = NULL, LLR2 = NULL, RELAY_llr0 = NULL, RELAY_llr1 = NULL, RELAY_LLR1 = NULL, RELAY_LLR2 = NULL,
		SUPER_llr0 = NULL, SUPER_llr1 = NULL, SUPER_LLR1 = NULL, SUPER_LLR2 = NULL;

	switch (Ch) {
	case CONV:
		//==============================Convolution============================================================================
		Conv.trellis_gen(g, SP_NINFOBITperSYM, 2, NCODEBIT, 1);

		llr0 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM),
			llr1 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);

		ConvLLR = (double **)malloc(sizeof(int) * 2 * SP_NCODEBITperSYM);
		for (int i = 0; i < SP_NCODEBITperSYM; i++) {
			ConvLLR[i] = (double *)malloc(SP_NCODE * sizeof(int) * 2);
		}

		break;

	case TURBO:
		//==================================Turbo================================================================================

		turb.turbo_trellis_gen(g, Size, 3, 2, 1);
		turb.int_init();

		SP_NCODEBITperSYM = (3 * Size) + (4 * turb.m_Nmemory);
		SP_NDCARperSYM = SP_NCODEBITperSYM / Mod_int;

		llr0 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);
		llr1 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);

		RELAY_llr0 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);
		RELAY_llr1 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);

		SUPER_llr0 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);
		SUPER_llr1 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);

		LLR1 = (double **)malloc(sizeof(double) *  (Size + turb.m_Nmemory));
		LLR2 = (double **)malloc(sizeof(double) *  (Size + turb.m_Nmemory));

		RELAY_LLR1 = (double **)malloc(sizeof(double) *  (Size + turb.m_Nmemory));
		RELAY_LLR2 = (double **)malloc(sizeof(double) *  (Size + turb.m_Nmemory));

		SUPER_LLR1 = (double **)malloc(sizeof(double) *  (Size + turb.m_Nmemory));
		SUPER_LLR2 = (double **)malloc(sizeof(double) *  (Size + turb.m_Nmemory));

		for (int i = 0; i < (Size + turb.m_Nmemory); i++) {
			LLR1[i] = (double *)malloc(SP_NCODE * sizeof(double));
			LLR2[i] = (double *)malloc(SP_NCODE * sizeof(double));

			RELAY_LLR1[i] = (double *)malloc(SP_NCODE * sizeof(double));
			RELAY_LLR2[i] = (double *)malloc(SP_NCODE * sizeof(double));

			SUPER_LLR1[i] = (double *)malloc(SP_NCODE * sizeof(double));
			SUPER_LLR2[i] = (double *)malloc(SP_NCODE * sizeof(double));
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
	int err, perr, count = 0;
	bool DEC_flag = true;
	SNR = SNRinit;
	BER_Record.resize((SNRMAX - SNR + Increment) / (Increment), 0);
	PER_Record.resize((SNRMAX - SNR + Increment) / (Increment), 0);
	BER_Record_idx = 0;


	//===================================================================================================================

	//==============================Buffer===============================================================================
	vector<vector<bool>> source, relay_source; // Block Buffer
	vector<bool> source_int, relay_int, code_source, code_relay, encoded_source, demapped_source, decoded_source,
		encoded_relay, decoded_relay;//Packet buffer
	vector<Complex<double>> tx_source, RE_RX, SB1_RX, SB2_RX, SR_RX, tx_Relay, tx_SourceAtRelay, Tmp_tx_Relay, InCase_Relay, RB1_RX, RB2_RX, RB_InCase_Relay1, RB_InCase_Relay2,
		RE_RayFading, RayFading_SB1, RayFading_SB2, RayFading_SR, RayFading_RB1, RayFading_RB2, RayFading_InCase1, RayFading_InCase2;
	vector<double> LLR_SB1, LLR_SB2, LLR_SR, LLR_RB1, LLR_RB2, LLR_RE, LLR_InCase1, LLR_InCase2;	//channel gain
	vector<vector<double>> LLR_FIRST, LLR_SECOND;
	//====================================================================================================================

	std::cout << "Simulation : " << Title << endl;
	std::cout << "SNR\tBER\tPER" << endl;



	//------------------------------------------------------------------------------------------
	//-----------------------------Main loop----------------------------------------------------
	//1.Source generate
	//2.Channel coding or Not
	//3.Constellation Mapping 
	//4.Constellation Rotate and interleaving or not (SSD OR CIOD)
	//5.Fading or Not
	//6.AWGN
	//7.Channel ����
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
				RELAY_Eb = 1.0*rate;

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
				Map.BPSK_Mapping(encoded_source, tx_source);
				break;
			}
			case(QPSK): {
				Eb = Eb * 1.0;
				RELAY_Eb = RELAY_Eb * 1.0;  //energy of relay transmission
				Map.QPSK_Mapping(encoded_source, tx_source);
				SOURCE_Map.QPSK_Mapping(encoded_source, tx_SourceAtRelay , 0.3);
				RELAY_Map.QPSK_Mapping(encoded_relay, tx_Relay, 0.7);	//Relay�� codeword�� small energy modulation�ؼ� ������.
				Tmp_tx_Relay = tx_Relay;									//we need to add RD_TX & tx_source here!
				RELAY_Map.Super(tx_SourceAtRelay, tx_Relay);

				InCase_Map.QPSK_Mapping(encoded_source, InCase_Relay);
				
				break;
			}
			case(QAM16): {
				Eb = Eb * 2.5;
				Map.QAM_Mapping(encoded_source, tx_source);
				break;
			}
			case(QAM64): {
				Eb = Eb * 7;
				Map.QAM64_Mapping(encoded_source, tx_source);

				break;
			}
			}

			//============Fading Channel=======================================
			//Fading
			switch (Fad_Mod) {
			case(Rayl): {
				Ray.Rayleigh_Fading(tx_source, RayFading_SB1, SB1_RX, LLR_SB1);
				break;
			}
			case(Rayl_Quasi): {
				Ray.slow_Rayleigh_Fading(tx_source, RayFading_SB1, SB1_RX, LLR_SB1);
				Ray.slow_Rayleigh_Fading(tx_source, RayFading_SB2, SB2_RX, LLR_SB2);
				Ray.slow_Rayleigh_Fading(tx_source, RayFading_SR, SR_RX, LLR_SR);
				Ray.slow_Rayleigh_Fading(tx_Relay, RayFading_RB1, RB1_RX, LLR_RB1);
				Ray.slow_Rayleigh_Fading(tx_Relay, RayFading_RB2, RB2_RX, LLR_RB2);
				Ray.slow_Rayleigh_Fading(tx_source, RE_RayFading, RE_RX, LLR_RE);
				Ray.slow_Rayleigh_Fading(InCase_Relay, RayFading_InCase1, RB_InCase_Relay1, LLR_InCase1);
				Ray.slow_Rayleigh_Fading(InCase_Relay, RayFading_InCase2, RB_InCase_Relay2, LLR_InCase2);
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
			AWGN_SB1.Awgn(SNR + SB1_Gain, Eb, SB1_RX);
			AWGN_SB2.Awgn(SNR + SB2_Gain, Eb, SB2_RX);
			AWGN_RE.Awgn(SNR + SB1_Gain, Eb, RE_RX);

			AWGN_SR.Awgn(SNR + SR_Gain, Eb, SR_RX);

			AWGN_RB1.Awgn(SNR + RB1_Gain, RELAY_Eb, RB1_RX);
			AWGN_RB2.Awgn(SNR + RB2_Gain, RELAY_Eb, RB2_RX);

			AWGN_InCase1.Awgn(SNR + RB1_Gain, RELAY_Eb, RB_InCase_Relay1);
			AWGN_InCase2.Awgn(SNR + RB2_Gain, RELAY_Eb, RB_InCase_Relay2);
			//=================================================================

			//===========Coherent=============================================

			switch (Fad_Mod) {
			case(Rayl): {
				Ray.Coherent(RayFading_SB1, SB1_RX, LLR_SB1);
				break;
			}
			case(Rayl_Quasi): {
				Ray.Quasi_Coherent(RayFading_SB1, SB1_RX, LLR_SB1);
				Ray.Quasi_Coherent(RayFading_SB2, SB2_RX, LLR_SB2);
				Ray.Quasi_Coherent(RayFading_SR, SR_RX, LLR_SR);
				Ray.Quasi_Coherent(RayFading_RB1, RB1_RX, LLR_RB1);
				Ray.Quasi_Coherent(RayFading_RB2, RB2_RX, LLR_RB2);
				Ray.Quasi_Coherent(RE_RayFading, RE_RX, LLR_RE);
				Ray.Quasi_Coherent(RayFading_InCase1, RB_InCase_Relay1, LLR_InCase1);
				Ray.Quasi_Coherent(RayFading_InCase2, RB_InCase_Relay2, LLR_InCase2);
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
				LC = -1.0 / (2 * AWGN_SB1.sigma2);
				turb.turbo_llr_generation(Fad_Mod, SB1_RX, LLR_SB1, llr0, llr1, &Map, SB1_RX.size(), LC);
				turb.turbo_bit2sym(llr0, llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
				decoded_source = turb.turbo_decoding(LLR1, LLR2, ITR);
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

			//only consider PER for HARQ

			if (Detect.Packet(code_source, decoded_source, SP_NINFOBITperSYM)) {    //CRC fail @ the gateway
																					//Decoding @ Relay
				LC = -1.0 / (2 * AWGN_SR.sigma2);
				turb.turbo_llr_generation(Fad_Mod, SR_RX, LLR_SR, llr0, llr1, &Map, SR_RX.size(), LC);
				turb.turbo_bit2sym(llr0, llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
				decoded_source = turb.turbo_decoding(LLR1, LLR2, ITR_RELAY);

				if (!Detect.Packet(code_source, decoded_source, SP_NINFOBITperSYM)) {	//decoding succeeded @ Relay
																						//RE_TX @ Relay & Combining
#if COMB == MRComb		Comb.COOP_MRC(Fad_Mod, SNR + RB1_Gain, RB1_RX, LLR_RB1, SNR + RB2_Gain, RB2_RX, LLR_RB2);   //SLC
#elif COMB == EGComb	Comb.COOP_EGC(Fad_Mod, SNR + RB1_Gain, RB1_RX, LLR_RB1, SNR + RB2_Gain, RB2_RX, LLR_RB2);   //SLC
#endif
					LC = -1.0 / (2 * AWGN_RB2.sigma2);
					turb.turbo_llr_generation(Fad_Mod, RB1_RX, LLR_RB1, llr0, llr1, &RELAY_Map, RB1_RX.size(), LC);
					turb.turbo_bit2sym(llr0, llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
					decoded_relay = turb.turbo_decoding(LLR1, LLR2, ITR);
					
					//relay BER�� �ʿ����
					//Detect.Detection(code_relay, decoded_relay, err, Size);
					
					DEC_flag = Detect.Packet(code_relay, decoded_relay, SP_NINFOBITperSYM);
				}

				else {	//decoding faild @ Relay
						//RE_TX @ Source & Combining
#if COMB == MRComb		Comb.MRC(Fad_Mod, SB1_RX, LLR_SB1, RE_RX, LLR_RE);
#else COMB == EGComb	Comb.EGC(Fad_Mod, SB1_RX, LLR_SB1, RE_RX, LLR_RE);
#endif
					LC = -1.0 / (2 * AWGN_SB1.sigma2);
					turb.turbo_llr_generation(Fad_Mod, SB1_RX, LLR_RE, llr0, llr1, &Map, SB1_RX.size(), LC);
					turb.turbo_bit2sym(llr0, llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
					decoded_source = turb.turbo_decoding(LLR1, LLR2, ITR);
				}


				if (!DEC_flag)  //combining relay's packet & decoding succeeded
				{   //SIC
					RELAY_Map.Super_Sub(Tmp_tx_Relay, RB1_RX);

					LC = -1.0 / (2 * AWGN_RB2.sigma2);
#if COMB == MRComb		Comb.SUPER_MRC(Fad_Mod, SNR, SB1_RX, LLR_SB1, SNR, RB1_RX, LLR_RB1);
#elif COMB == EGComb	Comb.SUPER_EGC(Fad_Mod, SNR, SB1_RX, LLR_SB1, SNR, RB1_RX, LLR_RB1);
#endif
					turb.turbo_llr_generation(Fad_Mod, SB1_RX, LLR_SB1, SUPER_llr0, SUPER_llr1, &Map, SB1_RX.size(), LC);
					turb.turbo_bit2sym(SUPER_llr0, SUPER_llr1, SUPER_LLR1, SUPER_LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);

					decoded_source = turb.turbo_decoding(SUPER_LLR1, SUPER_LLR2, ITR);

				}
				else;
				perr += DEC_flag;
			}

			else {//source decoding�� �� ���� ����
#if		COMB == MRComb Comb.COOP_MRC(Fad_Mod, SNR + RB1_Gain, RB_InCase_Relay1, LLR_InCase1, SNR + RB2_Gain, RB_InCase_Relay2, LLR_InCase2);   //SLC
#elif	COMB == EGComb Comb.COOP_EGC(Fad_Mod, SNR + RB1_Gain, RB_InCase_Relay1, LLR_InCase1, SNR + RB2_Gain, RB_InCase_Relay2, LLR_InCase2);   //SLC
#endif
				LC = -1.0 / (2 * AWGN_InCase1.sigma2);
				turb.turbo_llr_generation(Fad_Mod, RB_InCase_Relay1, LLR_RB1, llr0, llr1, &InCase_Map, RB_InCase_Relay1.size(), LC);
				turb.turbo_bit2sym(llr0, llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
				decoded_relay = turb.turbo_decoding(LLR1, LLR2, ITR);
				//Detect.Detection(code_relay, decoded_relay, err, Size);
			};

			switch (Ch) {
			case CONV:
				Detect.Detection(code_source, decoded_source, err, Convsize);
				break;
			case TURBO:
				Detect.Detection(code_source, decoded_source, err, Size);
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

		}


		Ch == TURBO ? BER = (double)err / (Frame*Size + count * Size) : (double)err / (Frame*Convsize);

		PER = (double)perr / Frame;
		BER_Record[BER_Record_idx] = BER;
		PER_Record[BER_Record_idx++] = PER;
		std::cout << SNR << "\t" << BER << "\t" << PER << endl;

		//memory management
		for (int mm = 0; mm < Frame; mm++)
			vector<bool>(0).swap(source[mm]);

		vector<vector<bool>>(0).swap(source);

		SNR += Increment;
	} while (SNR <= (SNRMAX));
	cout << endl << "----------------------" << endl;
}
#endif