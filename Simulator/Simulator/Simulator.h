#pragma once
//types of simulator
#define CONV_ANALOG_COMB 1	//conventional analog MRC combining
#define CONV_LOG_COMB 2
#define SIC 3
#define LARGER_RELAY_SIC 4
#define COOP_16QAM 5
#define LLR_HARQ 6          //Conventional Superposition
#define ANALOG_HARQ 7       
#define LLR_HARQ2 8         
#define LLR_HARQ3 9         //Pam Superposition
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
#define LLR_HARQ4 20        //Phase Overlap
#define PHASE_EXTENSION 21    //Phase Extension
#define SUPER_16QAM 22

#define NOW SUPER_16QAM

#define SOURCE_ONLY 1
#define RELAY_ONLY 2
#define SOURCE_RELAY_BOTH 3

#define OUTPUT SOURCE_RELAY_BOTH

//Combining
#define EGComb 1
#define MRComb 2
#define NONE 3

#define COMB NONE

//BER or PER
#define BERFILE 1
#define PERFILE 2
#define BERnPER 3

#define OUTFILE BERFILE





#if NOW == SIC
//dfdf
#endif





#if COMB == MRComb
//dfdf
#elif COMB == EGComb
//ffff
#endif

