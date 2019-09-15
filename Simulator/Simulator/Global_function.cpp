#include "Simulator.h"

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
