#ifndef GENRATION_H
#define GENRATION_H


#include <vector>
#include <random>
using namespace std;


class Generation
{
public:
	Generation();
	vector<int> test;
	void Generator(int, int, vector<vector<bool>>&);
	~Generation();
};

Generation::Generation()
{
}

void Generation::Generator(int frame, int size, vector<vector<bool>>& result)
{
	result.resize(frame);

	for (int i = 0; i < frame; i++) {
		result[i].resize(size);
		for (int j = 0; j < size; j++) {
			result[i][j] = ((double)rand() / RAND_MAX) > 0.5;
		}
	}
}

Generation::~Generation()
{
}

#endif // !GENRATION_H