#include "Simulator.h"
#if NOW == CONV_SC_POWER //Conventional Superposition
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
P_alpha1 = 0, P_alpha2 = 0, P_beta1 = 0, P_beta2 = 0;	//c는 QPSK를 위한 값
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
			file >> P_alpha1;
			P_alpha2 = 1 - P_alpha1;

			//power switch
			P_beta1 = P_alpha1;
			P_beta2 = P_alpha2;
		}
	}
}


void Sim() {

	//=============================Class Instances==========================================================================
	Generation Gen;
	Mapper Map, SOURCE1_Map, SOURCE2_Map, RELAY1_Map, RELAY2_Map;
	Rayleigh Ray;
	AWGN Awgn, AWGN2, AWGN3;
	Detector Detect;
	Comb Comb;
	Conv Conv;
	Turb turb;

	Map.init(Mod_int);
	Map.map_tab_gen(1.0);

	SOURCE1_Map.init(Mod_int);
	SOURCE1_Map.map_tab_gen(P_alpha1);

	SOURCE2_Map.init(Mod_int);
	SOURCE2_Map.map_tab_gen(P_alpha2);

	RELAY1_Map.init(Mod_int);
	RELAY1_Map.map_tab_gen(P_beta1);

	RELAY2_Map.init(Mod_int);
	RELAY2_Map.map_tab_gen(P_beta2);

	//==========================Simulation Parameter======================================================================


	int Size = (1 << m);

	int SP_NINFOBITperSYM = Size * Mod_int / rate;
	int SP_NCODEBITperSYM = Size * Mod_int;
	int SP_NDCARperSYM = Size;
	int NCODEBIT = 2;	//
	int SP_NCODE = pow(2, NCODEBIT);

	//======================Trellis diagram===============================================================================================

	int Convsize;
	double *SOURCE1_TX_llr0, *SOURCE1_TX_llr1, *SOURCE2_TX_llr0, *SOURCE2_TX_llr1,
		*RELAY1_TX_llr0, *RELAY1_TX_llr1, *RELAY2_TX_llr0, *RELAY2_TX_llr1,
		*llr0, *llr1, *SUPER_llr0, *SUPER_llr1;
	double **ConvLLR, **LLR1, **LLR2;

	int g[2] = { 0x0D,0x0B };//trellis polynomial for convolutional
	llr0 = NULL, llr1 = NULL, ConvLLR = NULL, LLR1 = NULL, LLR2 = NULL, SUPER_llr0 = NULL, SUPER_llr1 = NULL;
	SOURCE1_TX_llr0 = NULL, SOURCE1_TX_llr1 = NULL, SOURCE2_TX_llr0 = NULL, SOURCE2_TX_llr1 = NULL,
		RELAY1_TX_llr0 = NULL, RELAY1_TX_llr1 = NULL, RELAY2_TX_llr0 = NULL, RELAY2_TX_llr1 = NULL;

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

		SOURCE1_TX_llr0 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);
		SOURCE1_TX_llr1 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);

		SOURCE2_TX_llr0 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);
		SOURCE2_TX_llr1 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);

		RELAY1_TX_llr0 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);
		RELAY1_TX_llr1 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);

		RELAY2_TX_llr0 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);
		RELAY2_TX_llr1 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);


		llr0 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);
		llr1 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);

		SUPER_llr0 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);
		SUPER_llr1 = (double*)malloc(sizeof(double)*SP_NCODEBITperSYM);

		LLR1 = (double **)malloc(sizeof(double) *  (Size + turb.m_Nmemory));
		LLR2 = (double **)malloc(sizeof(double) *  (Size + turb.m_Nmemory));
		for (int i = 0; i < (Size + turb.m_Nmemory); i++) {
			LLR1[i] = (double *)malloc(SP_NCODE * sizeof(double));
			LLR2[i] = (double *)malloc(SP_NCODE * sizeof(double));
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
	int err, perr, count, soFar, earlyStopErr = Frame * Size * EARLYSTOPRATE, s1_fail_flag = true, s2_fail_flag = true;
	bool DEC_flag = false;
	SNR = SNRinit;
	BER_Record.resize((SNRMAX - SNR + Increment) / (Increment), 0);
	PER_Record.resize((SNRMAX - SNR + Increment) / (Increment), 0);
	BER_Record_idx = 0;


	//===================================================================================================================

	//==============================Buffer===============================================================================
	vector<vector<bool>> source1, source2;				// Block Buffer
	vector<bool> source1_int, source2_int,
		code_source1, code_source2,
		encoded_source1, demapped_source1, decoded_source1,
		encoded_source2, demapped_source2, decoded_source2;
	vector<Complex<double>> tx_source, rx_source, RE_RX, SR_RX, RD_TX, RD_RX,
		SOURCE1_TX, SOURCE2_TX, RELAY1_TX, RELAY2_TX,
		SOURCE_TX, RELAY_TX, SOURCE_RX, RELAY_RX,
		tmp_RD_TX,
		RE_RayFading, RayFading, SR_RayFading, RD_RayFading;
	vector<double> LLR_SR, llr_wgt_sd, LLR_RD, LLR_RE;		//channel gain
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

		//==============Source Generating==================================
		switch (Ch) {
		case CONV:
			Convsize = Size * Mod_int / rate - Conv.Nmemory;
			Gen.Generator(Frame, Convsize, source1);

			break;
		case TURBO:
			Convsize = Size;	//Convsize = Size * Mod_int / rate - turb.m_Nmemory;
			Gen.Generator(Frame, Convsize, source1);
			Gen.Generator(Frame, Convsize, source2);
			break;
		case UNCODED:
			Gen.Generator(Frame, SP_NINFOBITperSYM, source1);
			Convsize = Size * Mod_int / rate - Conv.Nmemory;
			break;
		}

		for (int h = 0; h < Frame; ++h)
		{
			code_source1 = source1[h];
			code_source2 = source2[h];
			//=================================================================

			s1_fail_flag = true, s2_fail_flag = true;

			//===============CHANNEL CODING====================================
			switch (Ch) {
			case CONV:
				for (int yy = 0; yy < Conv.Nmemory; ++yy)
					code_source1.push_back(0);


				Conv.encoder_init();
				encoded_source1 = Conv.Conv_encoder(code_source1, rate);

				Eb = 1.0 * rate;
				break;

			case TURBO:
				Eb = 1.0 * rate;
				RELAY_Eb = 1.0*rate;

				source1_int = turb.interleaver(code_source1);
				source2_int = turb.interleaver(code_source2);

				encoded_source1 = turb.encode(code_source1, source1_int);
				encoded_source2 = turb.encode(code_source2, source2_int);
				break;

			case UNCODED:
				encoded_source1 = code_source1;
				Eb = 1.0;
				break;
			}
			//=================================================================		

			//=============Modulation==========================================
			switch (Mod_int) {
			case(BPSK): {
				Eb = Eb * 1.0;
				Map.BPSK_Mapping(encoded_source1, tx_source);
				break;
			}
			case(QPSK): {
				Eb = Eb * 1.0;
				RELAY_Eb = RELAY_Eb * 1.0;								//energy of relay transmission


				SOURCE1_Map.QPSK_Mapping(encoded_source1, SOURCE1_TX, P_alpha1);
				SOURCE2_Map.QPSK_Mapping(encoded_source2, SOURCE2_TX, P_alpha2);

				RELAY1_Map.QPSK_Mapping(encoded_source1, RELAY1_TX, P_beta1);
				RELAY2_Map.QPSK_Mapping(encoded_source2, RELAY2_TX, P_beta2);

				SOURCE_TX = SOURCE1_TX;
				SOURCE2_Map.Super(SOURCE2_TX, SOURCE_TX);

				RELAY_TX = RELAY1_TX;
				RELAY2_Map.Super(RELAY2_TX, RELAY_TX);


				break;
			}
			case(QAM16): {
				Eb = Eb * 2.5;
				Map.QAM_Mapping(encoded_source1, tx_source);
				break;
			}
			case(QAM64): {
				Eb = Eb * 7;
				Map.QAM64_Mapping(encoded_source1, tx_source);

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
				Ray.slow_Rayleigh_Fading(SOURCE_TX, RayFading, SOURCE_RX, llr_wgt_sd);
				Ray.slow_Rayleigh_Fading(RELAY_TX, RD_RayFading, RELAY_RX, LLR_RD);
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
			Awgn.Awgn(SNR, Eb, SOURCE_RX);
			AWGN3.Awgn(SNR + RD_Gain, RELAY_Eb, RELAY_RX);
			//=================================================================

			//===========Coherent=============================================

			switch (Fad_Mod) {
			case(Rayl): {
				Ray.Coherent(RayFading, rx_source, llr_wgt_sd);
				break;
			}
			case(Rayl_Quasi): {
				Ray.Quasi_Coherent(RayFading, SOURCE_RX, llr_wgt_sd);
				Ray.Quasi_Coherent(RD_RayFading, RELAY_RX, LLR_RD);
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
				decoded_source1 = Conv.decoding(ConvLLR);
				break;

			case TURBO:
				LC = -1.0 / (2 * Awgn.sigma2);
				turb.turbo_llr_generation(Fad_Mod, SOURCE_RX, llr_wgt_sd, SOURCE1_TX_llr0, SOURCE1_TX_llr1, &SOURCE1_Map, SOURCE_RX.size(), LC);
				turb.turbo_bit2sym(SOURCE1_TX_llr0, SOURCE1_TX_llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
				decoded_source1 = turb.turbo_decoding(LLR1, LLR2, ITR);

				s1_fail_flag = Detect.Packet(code_source1, decoded_source1, SP_NINFOBITperSYM);

				if (!s1_fail_flag) {
					SOURCE1_Map.Super_Sub(SOURCE1_TX, SOURCE_RX);	//SUB S1 INFO
					turb.turbo_llr_generation(Fad_Mod, SOURCE_RX, llr_wgt_sd, SOURCE2_TX_llr0, SOURCE2_TX_llr1, &SOURCE2_Map, SOURCE_RX.size(), LC);
					turb.turbo_bit2sym(SOURCE2_TX_llr0, SOURCE2_TX_llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
					decoded_source2 = turb.turbo_decoding(LLR1, LLR2, ITR);

					s2_fail_flag = Detect.Packet(code_source2, decoded_source2, SP_NINFOBITperSYM);
				}
				break;

			case UNCODED:
				switch (Mod_int) {
				case BPSK:
					Map.BPSK_Demapping(rx_source, rx_source.size(), demapped_source1);
					break;
				case QPSK:
					Map.QPSK_Demapping(rx_source, rx_source.size(), demapped_source1);
					break;
				case QAM16:
					Map.QAM_Demapping(rx_source, rx_source.size(), demapped_source1);
					break;
				case QAM64:
					Map.QAM64_Demapping(rx_source, rx_source.size(), demapped_source1);
					break;
				}
				decoded_source1 = demapped_source1;

			}


			if (s1_fail_flag || s2_fail_flag) {		//CRC fail @ the gateway
													//Decoding @ Relay

				LC = -1.0 / (2 * AWGN3.sigma2);

				turb.turbo_llr_generation(Fad_Mod, RELAY_RX, LLR_RD, RELAY1_TX_llr0, RELAY1_TX_llr0, &RELAY1_Map, RELAY_RX.size(), LC);
				turb.turbo_bit2sym(RELAY1_TX_llr0, RELAY1_TX_llr0, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
				decoded_source1 = turb.turbo_decoding(LLR1, LLR2, ITR);

				s1_fail_flag = Detect.Packet(code_source1, decoded_source1, SP_NINFOBITperSYM);

				if (!s1_fail_flag) {
					RELAY1_Map.Super_Sub(RELAY1_TX, RELAY_RX);	//SUB S1 INFO
					turb.turbo_llr_generation(Fad_Mod, RELAY_RX, LLR_RD, RELAY2_TX_llr0, RELAY2_TX_llr1, &RELAY2_Map, RELAY_RX.size(), LC);
					turb.turbo_bit2sym(RELAY2_TX_llr0, RELAY2_TX_llr1, LLR1, LLR2, SP_NCODEBITperSYM, NCODEBIT, SP_NCODE);
					decoded_source2 = turb.turbo_decoding(LLR1, LLR2, ITR);

					s2_fail_flag = Detect.Packet(code_source2, decoded_source2, SP_NINFOBITperSYM);
				}

			}


			switch (Ch) {
			case CONV:
				Detect.Detection(code_source1, decoded_source1, err, Convsize);
				break;
			case TURBO:
#if (OUTPUT == SOURCE_ONLY) || (OUTPUT == SOURCE_RELAY_BOTH)
				Detect.Detection(code_source1, decoded_source1, err, Size);
#endif
#if (OUTPUT == RELAY_ONLY) || (OUTPUT == SOURCE_RELAY_BOTH)
				if(!s1_fail_flag) Detect.Detection(code_source2, decoded_source2, err, Size);
#endif
				break;
			case UNCODED:
				Detect.Detection(code_source1, decoded_source1, err, SP_NINFOBITperSYM);
				break;
			}

			//memory management
			vector<bool>(0).swap(code_source1), vector<bool>(0).swap(encoded_source1), vector<bool>(0).swap(decoded_source1),
				vector<bool>(0).swap(code_source2), vector<bool>(0).swap(encoded_source2), vector<bool>(0).swap(decoded_source2),
				vector<bool>(0).swap(demapped_source1), vector<Complex<double>>(0).swap(tx_source),
				vector<Complex<double>>(0).swap(SOURCE1_TX), vector<Complex<double>>(0).swap(SOURCE2_TX),
				vector<Complex<double>>(0).swap(RELAY2_TX), vector<Complex<double>>(0).swap(RELAY2_TX),
				vector<Complex<double>>(0).swap(SOURCE_TX), vector<Complex<double>>(0).swap(RELAY_TX),
				vector<Complex<double>>(0).swap(SOURCE_RX), vector<Complex<double>>(0).swap(RELAY_RX),
				vector<Complex<double>>(0).swap(rx_source), vector<double>(0).swap(llr_wgt_sd),
				vector<Complex<double>>(0).swap(RayFading), vector<bool>(0).swap(source1_int),
				vector<Complex<double>>(0).swap(RD_RX), vector<double>(0).swap(LLR_RD),
				vector<vector<double>>(0).swap(LLR_FIRST), vector<vector<double>>(0).swap(LLR_SECOND);
			if (err > earlyStopErr) {
#if (OUTPUT == SOURCE_ONLY) || (OUTPUT == RELAY_ONLY) 
				soFar = h;
#elif (OUTPUT == SOURCE_RELAY_BOTH)
				soFar = h << 1;
#endif

				soFar = h << 1;
				//then
				goto EARLYSTOP;
			}
		}
#if (OUTPUT == SOURCE_ONLY) || (OUTPUT == RELAY_ONLY) 
		soFar = (Frame);
#elif (OUTPUT == SOURCE_RELAY_BOTH)
		soFar = (Frame << 1);	//when EARLYSTOP NOT NEEDED
#endif

								//when EARLYSTOP NEEDED
	EARLYSTOP: Ch == TURBO ? BER = (double)err / ((soFar)* Size) : (double)err / (Frame * Convsize);
		PER = (double)perr / Frame;
		BER_Record[BER_Record_idx] = BER;
		PER_Record[BER_Record_idx++] = PER;
		std::cout << SNR << "\t" << BER << "\t" << PER << endl;

		//memory management
		for (int mm = 0; mm < Frame; mm++) {
			vector<bool>(0).swap(source1[mm]);
			vector<bool>(0).swap(source2[mm]);
		}

		vector<vector<bool>>(0).swap(source1);
		vector<vector<bool>>(0).swap(source2);

		SNR += Increment;
	} while (SNR <= (SNRMAX));
	std::cout << endl << "----------------------" << endl;
}
#endif
