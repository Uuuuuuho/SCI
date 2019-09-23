#include "Simulator.h"
#if NOW == NEW_SM //Conventional Superposition
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
double SNR = 0, SNRinit, Increment = 0, SNRMAX = 0, rate = 0, SR_Gain = 0, RD_Gain = 0,
P_alpha = 0, P_beta = 0;	//c는 QPSK를 위한 값
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
		Read();		//전역변수 설정
		Sim();	//Implement Simulation
		Write();
		init();
	}
	file.close();
	return 0;
}



void init() {
	Frame = 0, m = 0, BER_Record_idx = 0;  // SIZE=2^m으로 m을 결정해야함
	SNR = 0, SNRinit, Increment = 0, SNRMAX = 0, rate;	//c는 QPSK를 위한 값
	Mod = "", Title = "", Channel_code = "", Decoding = "", Fading = "";

}
//bool  Channelcoding_flag = false;


void Write() {
	string tmpper = Title, tmpber = Title;

	tmpper.append("-PER.txt"), tmpber.append("-BER.txt");

#if (OUTFILE == PERFILE)
	ofstream per(tmpper);
#elif (OUTFILE == BERFILE)
	ofstream ber(tmpber);
#elif (OUTFILE == BERnPER)
	ofstream per(tmpper), ber(tmpber);

#endif

	int record_size = PER_Record.size();

#if (OUTFILE == PERFILE)

	SNR = SNRinit;//maybe needed to be edited
	BER_Record_idx = 0;

	for (int i = 0; i < record_size; i++, SNR += Increment, BER_Record_idx++) {
		per << SNR << "\t" << PER_Record[BER_Record_idx] << endl;
	}
	per.close();

#elif (OUTFILE == BERFILE)

	SNR = SNRinit;//maybe needed to be edited
	BER_Record_idx = 0;

	for (int i = 0; i < record_size; i++, SNR += Increment, BER_Record_idx++) {
		ber << SNR << "\t" << BER_Record[BER_Record_idx] << endl;
	}
	ber.close();

#elif (OUTFILE == BERnPER)
	SNR = SNRinit;//maybe needed to be edited
	BER_Record_idx = 0;

	for (int i = 0; i < record_size; i++, SNR += Increment, BER_Record_idx++) {
		per << SNR << "\t" << PER_Record[BER_Record_idx] << endl;
	}
	per.close();
	SNR = SNRinit;//maybe needed to be edited
	BER_Record_idx = 0;

	for (int i = 0; i < record_size; i++, SNR += Increment, BER_Record_idx++) {
		ber << SNR << "\t" << BER_Record[BER_Record_idx] << endl;
	}
	ber.close();
#endif

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

		else if (str == "P_alpha") {
			file >> P_alpha;
			P_beta = 1 - P_alpha;
		}
	}
}


void Sim() {

	//=============================Class Instances==========================================================================
	Generation Gen;
	Mapper Map, SOURCE_Map, RELAY_Map, SMALL_Map;
	Rayleigh Ray;
	AWGN Awgn, AWGN2, AWGN3;
	Detector Detect;
	Comb Comb;
	Conv Conv;
	Turb turb, turb2, turbo2;

	Map.init(Mod_int);
	Map.map_tab_gen(1.0);

	SOURCE_Map.init(MOD_SUPER_16QAM);
	SOURCE_Map.map_tab_gen(P_beta);

	SMALL_Map.init(Mod_int);
	SMALL_Map.map_tab_gen(P_beta);

	RELAY_Map.init(Mod_int);
	RELAY_Map.map_tab_gen(P_alpha);

	//RELAY_Map.init(MOD_SUPER_16QAM2);
	//RELAY_Map.map_tab_gen(P_beta);

	//==========================Simulation Parameter======================================================================


	int Size = (1 << m);

	int SP_NINFOBITperSYM = Size * Mod_int / rate;
	int SP_NCODEBITperSYM = Size * Mod_int;
	int SP_NCODEBITperSYM2 = Size * Mod_int * 2;
	int SP_NDCARperSYM = Size;
	int NCODEBIT = 2;	//
	int SP_NCODE = pow(2, NCODEBIT);

	//======================Trellis diagram===============================================================================================

	int Convsize;
	double* llr0, *llr1, *SUPER_llr0, *SUPER_llr1, *source_llr0, *source_llr1, *relay_llr0, *relay_llr1;
	double** ConvLLR, **LLR1, **LLR2, **SUPER_LLR1, **SUPER_LLR2;

	int g[2] = { 0x0D,0x0B };//trellis polynomial for convolutional
	llr0 = NULL, llr1 = NULL, ConvLLR = NULL, LLR1 = NULL, LLR2 = NULL, SUPER_llr0 = NULL, SUPER_llr1 = NULL,
		SUPER_LLR1 = NULL, SUPER_LLR2 = NULL, source_llr0 = NULL, source_llr1 = NULL, relay_llr0 = NULL, relay_llr1 = NULL;

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

		turb2.turbo_trellis_gen(g, Size, 3, 2, 1);
		turb2.int_init();


		SP_NCODEBITperSYM = (3 * Size) + (4 * turb.m_Nmemory);
		SP_NDCARperSYM = SP_NCODEBITperSYM / Mod_int;

		llr0 = (double*)malloc(sizeof(double) * SP_NCODEBITperSYM);
		llr1 = (double*)malloc(sizeof(double) * SP_NCODEBITperSYM);

		source_llr0 = (double*)malloc(sizeof(double) * SP_NCODEBITperSYM);
		source_llr1 = (double*)malloc(sizeof(double) * SP_NCODEBITperSYM);
		relay_llr0 = (double*)malloc(sizeof(double) * SP_NCODEBITperSYM);
		relay_llr1 = (double*)malloc(sizeof(double) * SP_NCODEBITperSYM);

		LLR1 = (double**)malloc(sizeof(double) * (Size + turb.m_Nmemory));
		LLR2 = (double**)malloc(sizeof(double) * (Size + turb.m_Nmemory));
		for (int i = 0; i < (Size + turb.m_Nmemory); i++) {
			LLR1[i] = (double*)malloc(SP_NCODE * sizeof(double));
			LLR2[i] = (double*)malloc(SP_NCODE * sizeof(double));
		}

		//dynamic allocation makes noise at the moment.. let's have a look at these later
		SUPER_llr0 = (double*)malloc(sizeof(double) * SP_NCODEBITperSYM2);
		SUPER_llr1 = (double*)malloc(sizeof(double) * SP_NCODEBITperSYM2);

		SUPER_LLR1 = (double**)malloc(sizeof(double) * (Size * 2 + turb.m_Nmemory));
		SUPER_LLR2 = (double**)malloc(sizeof(double) * (Size * 2 + turb.m_Nmemory));
		for (int i = 0; i < (Size * 2 + turb.m_Nmemory); i++) {
			SUPER_LLR1[i] = (double*)malloc(SP_NCODE * sizeof(double));
			SUPER_LLR2[i] = (double*)malloc(SP_NCODE * sizeof(double));
		}

		turb.log_sum_exp_lut_generation();
		turb2.log_sum_exp_lut_generation();

		/////////for QAM
		turbo2.turbo_trellis_gen(g, 2 * Size, 3, 2, 1);  //LLR을 2배크기로 받아서 source information과 관련한 부분만 결정짓기로함.
		turbo2.int_init();

		SP_NCODEBITperSYM2 = (3 * Size * 2) + (4 * turb.m_Nmemory);
		SP_NDCARperSYM = SP_NCODEBITperSYM2 / Mod_int;


		turbo2.log_sum_exp_lut_generation();

		break;

	case UNCODED:

		break;

	default:

		break;
	}


	//=============================initiallization=============================================================================
	double Eb = 0, RELAY_Eb = 0, BER, PER, SNR_SR, SNR_RD, LC;
	int err, perr, count, CONV_cnt, NEW_cnt, soFar;
	bool DEC_flag = false;
	SNR = SNRinit;
	BER_Record.resize((SNRMAX - SNR + Increment) / (Increment), 0);
	PER_Record.resize((SNRMAX - SNR + Increment) / (Increment), 0);
	BER_Record_idx = 0;

	//==========================llr vectors======================================================================
	vector<double> SOURCE_llr0(SP_NCODEBITperSYM), SOURCE_llr1(SP_NCODEBITperSYM), RELAY_llr0(SP_NCODEBITperSYM), RELAY_llr1(SP_NCODEBITperSYM),
		MERGED_llr0(SP_NCODEBITperSYM * 2), MERGED_llr1(SP_NCODEBITperSYM * 2);

	vector<vector<double>> LOGLR1, LOGLR2;

	LOGLR1.resize(Size + turb.m_Nmemory);
	LOGLR2.resize(Size + turb.m_Nmemory);

	for (int i = 0; i < (Size + turb.m_Nmemory); i++) {
		LOGLR1[i].resize(SP_NCODE);
		LOGLR2[i].resize(SP_NCODE);
	}
	//===================================================================================================================

	//==============================Buffer===============================================================================
	vector<vector<bool>> source, relay_source;				// Block Buffer
	vector<bool> source_int, relay_int, code_source, code_relay, encoded_source, demapped_source, decoded_source,
		encoded_relay, decoded_relay, tmp_code, tmp_int, tmp4size;						//Packet buffer
	vector<Complex<double>> tx_source, rx_source, RE_RX, SR_RX, RD_TX, RD_RX, SOURCE_TX, tmp_RD_TX, tmp_SOURCE_TX, SECOND_RD_RX,
		RE_RayFading, RayFading, SR_RayFading, RD_RayFading;
	vector<double> LLR_SR, llr_wgt_sd, LLR_RD, LLR_RE;		//channel gain
	vector<vector<double>> LLR_FIRST, LLR_SECOND, LLR_THIRD, LLR_FOURTH;
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
	//7.Channel 보상
	//8.Decoding and Detection
	//9.ERROR CHECK
	//10.(RELAY) IF ERROR OCCUR 
	//------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------




	do {


		CONV_cnt = 0;
		NEW_cnt = 0;
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
				RELAY_Eb = 1.0 * rate;

				source_int = turb.interleaver(code_source);
				relay_int = turb2.interleaver(code_relay);

				encoded_source = turb.encode(code_source, source_int);
				encoded_relay = turb2.encode(code_relay, relay_int);

				//				encoded_relay = Comb.Comnining_PAIRS(encoded_source, encoded_relay);


							   ///encoding done already
								//tmp_code.insert(tmp_code.end(), code_relay.begin(), code_relay.end());    //vector append
								//tmp_code.insert(tmp_code.end(), code_source.begin(), code_source.end());    //vector append

								//tmp_int.insert(tmp_int.end(), relay_int.begin(), relay_int.end());    //vector append
								//tmp_int.insert(tmp_int.end(), source_int.begin(), source_int.end());    //vector append

					//            encoded_relay = turbo2.encode(tmp_code, tmp_int);   //for size

					//            vector<bool>(0).swap(tmp_code), vector<bool>(0).swap(tmp_int);

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
				RELAY_Eb = RELAY_Eb * 1.0;								//energy of relay transmission
				Map.QPSK_Mapping(encoded_source, tx_source);
				SOURCE_Map.QPSK_Mapping(encoded_source, SOURCE_TX, P_beta);
				tmp_SOURCE_TX = SOURCE_TX;
				RELAY_Map.QPSK_Mapping(encoded_relay, RD_TX, P_alpha);	//Relay의 codeword를 small energy modulation해서 더하자.
				tmp_RD_TX = RD_TX;

				RELAY_Map.Super(SOURCE_TX, RD_TX);
				//				Constellation(RD_TX);
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
				Ray.Rayleigh_Fading(tx_source, RayFading, rx_source, llr_wgt_sd);
				break;
			}
			case(Rayl_Quasi): {
				Ray.slow_Rayleigh_Fading(tx_source, RayFading, rx_source, llr_wgt_sd);
				//Ray.slow_Rayleigh_Fading(tx_source, RE_RayFading, RE_RX, LLR_RE);//why do we need this?
				//Ray.slow_Rayleigh_Fading(tx_source, SR_RayFading, SR_RX, LLR_SR);
				Ray.slow_Rayleigh_Fading(RD_TX, RD_RayFading, RD_RX, LLR_RD);
				break;
			}
			default: {
				rx_source = tx_source;
				llr_wgt_sd.resize(tx_source.size());
				for (unsigned int z = 0; z < tx_source.size(); ++z)
					llr_wgt_sd[z] = 1;
			}
			}
			//=================================================================

			//===========AWGN channel==========================================
			Awgn.Awgn(SNR, Eb, rx_source);
			//Awgn.Awgn(SNR, Eb, RE_RX);
			//AWGN2.Awgn(SNR + SR_Gain, Eb, SR_RX);
			AWGN3.Awgn(SNR + RD_Gain, RELAY_Eb, RD_RX);
			//=================================================================

			//===========Coherent=============================================

			switch (Fad_Mod) {
			case(Rayl): {
				Ray.Coherent(RayFading, rx_source, llr_wgt_sd);
				break;
			}
			case(Rayl_Quasi): {
				Ray.Quasi_Coherent(RayFading, rx_source, llr_wgt_sd);
				//Ray.Quasi_Coherent(RE_RayFading, RE_RX, LLR_RE);
				//Ray.Quasi_Coherent(SR_RayFading, SR_RX, LLR_SR);
				Ray.Quasi_Coherent(RD_RayFading, RD_RX, LLR_RD);
				break;
			}
			default: {
				break;
			}
			}

			//===========Decoding==============================================


			switch (Ch) {
			case CONV:
				Conv.init_dec_state();
				Map.llr_generation(Fad_Mod, rx_source, llr_wgt_sd, llr0, llr1, &Map, SP_NDCARperSYM);
				Map.bit2symLLR_generation(llr0, llr1, ConvLLR, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
				decoded_source = Conv.decoding(ConvLLR);
				break;

			case TURBO:
				LC = -1.0 / (2 * Awgn.sigma2);
				turb.turbo_llr_generation(Fad_Mod, rx_source, llr_wgt_sd, llr0, llr1, &Map, rx_source.size(), LC);
				turb.turbo_bit2sym(llr0, llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
				//LLR_FIRST에 iteration이 끝난 정보를 저장함
				LLR_FIRST = turb.ExportLLR_turbo_decoding(LLR1, LLR2, ITR);
				turb.Decision(LLR_FIRST, decoded_source);
				break;

			case UNCODED:
				switch (Mod_int) {
				case BPSK:
					Map.BPSK_Demapping(rx_source, rx_source.size(), demapped_source);
					break;
				case QPSK:
					Map.QPSK_Demapping(rx_source, rx_source.size(), demapped_source);
					break;
				case QAM16:
					Map.QAM_Demapping(rx_source, rx_source.size(), demapped_source);
					break;
				case QAM64:
					Map.QAM64_Demapping(rx_source, rx_source.size(), demapped_source);
					break;
				}
				decoded_source = demapped_source;

			}


			if (Detect.Packet(code_source, decoded_source, SP_NINFOBITperSYM)) {    //CRC fail @ the gateway
																					//Decoding @ Relay
				SECOND_RD_RX = RD_RX;

				LC = -1.0 / (2 * AWGN3.sigma2);
				turb.turbo_llr_generation(Fad_Mod, RD_RX, LLR_RD, MERGED_llr0, MERGED_llr1, &SOURCE_Map, RD_RX.size(), LC);
				//16QAM에서 BIT LLR을 출력해서 INDEXING해줌
				turb.llr_segment(MERGED_llr0, MERGED_llr1, SOURCE_llr0, SOURCE_llr1, RELAY_llr0, RELAY_llr1, SP_NCODEBITperSYM);
				
				//LLR COMBINING IN THIS STEP. ACTUALLY THIS IS THE FIRST LLR COMBINING. THERE WOULD BE ANOTHER BLC LATER POSSIBLY WHEN SUCCEED DECODING RELAY INFORMATION
				Comb.LLR_MRC(Fad_Mod, SNR, llr0, llr1, llr_wgt_sd, SNR + RD_Gain, SOURCE_llr0, SOURCE_llr1, LLR_RD, SP_NCODEBITperSYM);
				turb.turbo_bit2sym(llr0, llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
				decoded_source = turb.turbo_decoding(LLR1, LLR2, ITR);

				if (!Detect.Packet(code_source, decoded_source, SP_NINFOBITperSYM)) {
					//after eliminate source info and try decoding again
					RELAY_Map.Super_Sub(tmp_SOURCE_TX, SECOND_RD_RX);

					//relay re-decoding
					//THIS IS ALL ABOUT RELAY INFORMATION
					LC = -1.0 / (2 * AWGN3.sigma2);
					turb.turbo_llr_generation(Fad_Mod, SECOND_RD_RX, LLR_RD, relay_llr0, relay_llr1, &RELAY_Map, RD_RX.size(), LC);
					turb.turbo_bit2sym(relay_llr0, relay_llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
					decoded_relay = turb.turbo_decoding(LLR1, LLR2, ITR);

				}

				else {
					LC = -1.0 / (2 * AWGN3.sigma2);
					turb.turbo_llr_generation(Fad_Mod, RD_RX, LLR_RD, relay_llr0, relay_llr1, &RELAY_Map, RD_RX.size(), LC);
					turb.turbo_bit2sym(relay_llr0, relay_llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
					decoded_relay = turb.turbo_decoding(LLR1, LLR2, ITR);
				}




#if (OUTPUT == SOURCE_ONLY) || (OUTPUT == SOURCE_RELAY_BOTH)
				if (!Detect.Packet(code_relay, decoded_relay, SP_NINFOBITperSYM)) {
					RELAY_Map.Super_Sub(tmp_RD_TX, RD_RX);

					LC = -1.0 / (2 * AWGN3.sigma2);
					turb.turbo_llr_generation(Fad_Mod, RD_RX, LLR_RD, SUPER_llr0, SUPER_llr1, &SMALL_Map, RD_RX.size(), LC);
					Comb.LLR_MRC(Fad_Mod, SNR, llr0, llr1, llr_wgt_sd, SNR + RD_Gain, SUPER_llr0, SUPER_llr1, LLR_RD, SP_NCODEBITperSYM);
					turb.turbo_bit2sym(llr0, llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
					decoded_source = turb.turbo_decoding(LLR1, LLR2, ITR);
				}

#if (OUTPUT == RELAY_ONLY) || (OUTPUT == SOURCE_RELAY_BOTH)

				Detect.Detection(code_relay, decoded_relay, err, Size);
				count++;
#endif

#endif


			}
			switch (Ch) {
			case CONV:
				Detect.Detection(code_source, decoded_source, err, Convsize);
				break;
			case TURBO:
#if (OUTPUT == SOURCE_ONLY) || (OUTPUT == SOURCE_RELAY_BOTH)
				Detect.Detection(code_source, decoded_source, err, Size);
#endif

				break;
			case UNCODED:
				Detect.Detection(code_source, decoded_source, err, SP_NINFOBITperSYM);
				break;
			}

			//memory management
			vector<bool>(0).swap(code_source), vector<bool>(0).swap(encoded_source), vector<bool>(0).swap(decoded_source),
				vector<bool>(0).swap(demapped_source), vector<Complex<double>>(0).swap(tx_source),
				vector<Complex<double>>(0).swap(rx_source), vector<double>(0).swap(llr_wgt_sd),
				vector<Complex<double>>(0).swap(RayFading), vector<bool>(0).swap(source_int),
				vector<Complex<double>>(0).swap(RE_RX), vector<double>(0).swap(LLR_RE),
				vector<Complex<double>>(0).swap(SR_RX), vector<double>(0).swap(LLR_SR),
				vector<Complex<double>>(0).swap(RD_RX), vector<double>(0).swap(LLR_RD),
				vector<vector<double>>(0).swap(LLR_FIRST), vector<vector<double>>(0).swap(LLR_SECOND);
			if (err > (Frame * Size * EARLYSTOPRATE)) {
#if (OUTPUT == SOURCE_ONLY) || (OUTPUT == SOURCE_RELAY_BOTH)
				soFar = h + count;
#elif (OUTPUT == RELAY_ONLY)
				soFar = count;
#endif
				//then
				goto EARLYSTOP;
			}
		}

#if (OUTPUT == SOURCE_ONLY) || (OUTPUT == SOURCE_RELAY_BOTH)
		Ch == TURBO ? BER = (double)err / ((Frame + count) * Size) : (double)err / (Frame * Convsize);
		soFar = (Frame + count);	//when EARLYSTOP NOT NEEDED
		//when EARLYSTOP NEEDED
	EARLYSTOP:Ch == TURBO ? BER = (double)err / ((soFar)* Size) : (double)err / (Frame * Convsize);
#elif (OUTPUT == RELAY_ONLY)
		Ch == TURBO ? BER = (double)err / ((count)* Size) : (double)err / (Frame * Convsize);
		soFar = count;	//when EARLYSTOP NOT NEEDED
		//when EARLYSTOP NEEDED
	EARLYSTOP:Ch == TURBO ? BER = (double)err / ((soFar)* Size) : (double)err / (Frame * Convsize);
#endif
		PER = (double)perr / Frame;
		BER_Record[BER_Record_idx] = BER;
		PER_Record[BER_Record_idx++] = PER;
		std::cout << SNR << "\t" << BER << "\t" << PER << "\t" << CONV_cnt << "\t" << NEW_cnt << endl;

		//memory management
		for (int mm = 0; mm < Frame; mm++)
			vector<bool>(0).swap(source[mm]);

		vector<vector<bool>>(0).swap(source);

		SNR += Increment;
	} while (SNR <= (SNRMAX));
	cout << endl << "----------------------" << endl;
}
#endif
