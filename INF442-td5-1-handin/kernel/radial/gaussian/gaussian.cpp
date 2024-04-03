#include <cmath> // for pow, atan, should you need them
#include <iostream> // for cerr

#include <point.hpp>
#include <cloud.hpp>
#include <gaussian.hpp>

// TODO 2.1.2: implement volume, profile and guess_bandwidth
// HINTS: pi = std::atan(1) * 4.0, e^x is std::exp(x)
double gaussian::volume() const {
	int d = data->get_point(0).get_dim();
	double pi = std::atan(1) * 4.0;
	return pow(2*pi, (double)d/2);
}

double gaussian::profile(double t) const {
	return std::exp((double)-t/2);
}

void gaussian::guess_bandwidth() {
	int n = data->get_n();
	double sigma = data->standard_deviation();
	bandwidth = 1.06 * (sigma / pow(n, (double)1/5));
}
