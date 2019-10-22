#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "complex_new.h"
using namespace std;

//==========types of simulator
#define CONV_ANALOG_COMB 1				//conventional analog MRC combining
#define CONV_LOG_COMB 2
#define SIC 3
#define LARGER_RELAY_SIC 4
#define COOP_16QAM 5
#define LLR_HARQ 6						//Conventional Superposition
#define ANALOG_HARQ 7       
#define LLR_HARQ2 8         
#define LLR_HARQ3 9						//Pam Superposition
#define CONV_LOG_COMB2 10
#define LARGER_RELAY_LLR_COMB 11
#define LARGER_RELAY_ANALOG_COMB 12
#define SUPER_COMP 13
#define COMP 14
#define CDRT_UL 15
#define CDRT_UL_8PSK 16
#define CONV_SUPER_BLC 17
#define PAM_SUPER_BLC 18
#define INCRE_ITER_SUPER_BLC 19
#define LLR_HARQ4 20					//Phase Overlap
#define PHASE_EXTENSION 21				//Phase Extension
#define SUPER_16QAM 22					//new decoding scheme
#define SUPER_16QAM2 23					//new decoding scheme
#define LLR_HARQ_SMALL_ALPHA 24			//new decoding scheme
#define PARTIAL_SM 25					//Partially interferenced SM Tx. Considering LARGE-ALPHA case only
#define PARTIAL_SM_PRIOR_SOURCE 26		//PARTIAL-SM PRIORITY ON SOURCE
#define CONV_SM 27						//MRC BLC before iteration for turbo code
#define NEW_SM 28						//MRC BLC before iteration for turbo with new decoding scheme
#define PARTIAL_SM_B4_ITER 29
#define PARTIAL_SM_PRIOR_SOURCE_B4_ITER 30
#define PARTIAL_NEW_SM 31				//Not done yet

#define POWER_SWITCH 32					//SWITCH POWER AT THE 2ND PHASE
#define POWER_NEW_SWITCHING 33			//SWITCH SIGN OF POWER RATIO AT THE 2ND PHASE
#define POWER_CONV 34					//EQUAL POWER TX IN BOTH PHASES
#define POWER_ITER_EQUAL 35				//
#define POWER_ITER_SWITCH 36

#define NOW POWER_NEW_SWITCHING


//=========BER cases
#define SOURCE_ONLY 1
#define RELAY_ONLY 2
#define SOURCE_RELAY_BOTH 3

#define OUTPUT SOURCE_ONLY 

//========Combining type
#define EGComb 1
#define MRComb 2
#define NONE 3

#define COMB MRComb

#define EARLYSTOPRATE 0.001

//========Output file : BER or PER
#define BERFILE 1
#define PERFILE 2
#define BERnPER 3

#define OUTFILE BERFILE




//=======pre-compiler test
#if NOW == SIC
//dfdf
#endif





#if COMB == MRComb
//dfdf
#elif COMB == EGComb
//ffff
#endif

void Constellation(vector<Complex<double>> obj);
