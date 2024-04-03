#include <cmath> // for pow, atan, should you need them

#include <point.hpp>
#include <flat.hpp>

// TODO 2.1.1: implement volume and profile
// HINT: pi = std::atan(1) * 4.0
double flat::volume() const {
	int d = data->get_point(0).get_dim();
	double pi = std::atan(1) * 4.0;
	double gamma = std::tgamma((double)d/2 + 1);
	return pow(pi, (double)d/2) / gamma;
}

double flat::profile(double t) const {
	if (t <= 1){
		return 1.0;
	}
	return 0.0;
}
