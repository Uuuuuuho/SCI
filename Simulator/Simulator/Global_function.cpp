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

template <typename T> void info_out(vector<T> obj, string name)
{	//only for vector data structure at the moment
	string title = name + ".txt";
	ofstream Info(title);
	Info << name << " = [" << endl;
	int m = obj.size();

	for (int i = 0; i < m; i++) {
		Info << obj[i] << endl;
	}

	Info << "]" << endl;
}
