#pragma once


#ifndef DETECTOR_H
#define DETECTOR_H

#include "complex_new.h"
#include <vector>
#include <iostream>
using namespace std;

class Detector
{
public:
	Detector();
	~Detector();
	void Detection(vector<bool> code, vector<bool> decoded, int &err,int size);
	void Half_Detection(vector<bool> code, vector<bool> decoded, int &err, int size);
	int Packet(vector<bool>code, vector<bool>decoded, int size);
	int BLOCKCHAIN_Packet(vector<bool>code, vector<bool>decoded, int size);
	bool CRC_CHECK(vector<bool>, vector<bool>);
	void demod(vector<double>input, vector<bool>&output,int size);

	
	int nOfTx;			//각 노드별로 Tx # counting
	double Throughput;	//각 노드별 throughput 계산
	int sOfDec;
};



#endif