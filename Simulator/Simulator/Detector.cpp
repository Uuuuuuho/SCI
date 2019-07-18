#include "Detector.h"

Detector::Detector()
{

}

Detector::~Detector()
{
}

void Detector::demod(vector<double>input, vector<bool>&output, int size) {
	int k;
	output.resize(size);
	for (k = 0; k < size; ++k)
		output[k] = input[k] > 0;

}
void Detector::Detection(vector<bool> code, vector<bool> decoded, int &err, int size)
{

	for (unsigned int i = 0; i < size; i++) {
		if (code[i] != decoded[i]) {
			err++;
			//cout << i << endl;
		}
	}
}
void Detector::Half_Detection(vector<bool> code, vector<bool> decoded, int & err, int size)
{
	int only_source = size / 2;
	for (unsigned int i = 0; i < only_source; i++) {
		code[i] != decoded[i] ? err++ : err;
	}
}
int Detector::Packet(vector<bool>code, vector<bool>decoded, int size) {
	for (unsigned int i = 0; i < size; i++) {
		if (code[i] != decoded[i]) {
			return 1;
		}
	}

	return 0;
}

int Detector::BLOCKCHAIN_Packet(vector<bool> code, vector<bool> decoded, int size)
{
	for (unsigned int i = 0; i < size; i++) {
		if (code[i] != decoded[i]) {
			return 1;
		}
	}

	return 0;
}

bool Detector::CRC_CHECK(vector<bool> code, vector<bool> decoded)
{
	for (unsigned int i = 0; i < code.size(); ++i)
	{
		if (code[i] != decoded[i])
			return 1;//err occurs

	}

	return 0;

}


