#include <cmath> // for pow, should you need it

#include <point.hpp>
#include <cloud.hpp>
#include <radial.hpp>

// TODO 2.1: density and radial constructor
// Use profile and volume... although it will only be implemented in the "sisters" classes
// Use kernel's constructor

radial::radial(cloud* data_, double bandwidth_) : kernel::kernel(data_){
	bandwidth = bandwidth_;
	data = data_;
}


double radial::density(const point& p) const
{
	double c = 1/volume();
	int n = data->get_n();
	int d = data->get_point(0).get_dim();

	double f = 0.0;
	double cte = c/(n * pow(bandwidth, d));

	for(int i = 0; i < n; i++){
		f += cte * profile(pow(p.dist(data->get_point(i)), 2) / pow(bandwidth, 2));
	}
	return f;
} 
