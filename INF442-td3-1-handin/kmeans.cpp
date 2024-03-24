#include <iostream>
#include <cassert>
#include <cmath>	// for sqrt, fabs
#include <cfloat>	// for DBL_MAX
#include <cstdlib>	// for rand, srand
#include <ctime>	// for rand seed
#include <fstream>
#include <cstdio>	// for EOF
#include <string>
#include <algorithm>	// for count
#include <vector>

using std::rand;
using std::srand;
using std::time;

class point
{
    public:

        static int d;
        double *coords;
        int label;

		point() {
			coords = new double[d];
			for (int i = 0; i < d; i++){
				coords[i] = 0.0;
			}
			label = 0;
		}

		~point() {
			delete[] coords;
		}

		// point(point& p){
		// 	this->d = p.d;
		// 	this->label = p.label;
		// 	this->coords = new double[d];

		// 	for (int i = 0; i < d; i++){
		// 		this->coords[i] = p.coords[i];
		// 	}
		// }

		void print() const {
			for(int i = 0; i < d; i++){
				std::cout << coords[i];
				if (i =! d-1){
					std::cout << "\t";
				}
			}
			std::cout << std::endl;
		}

		double squared_dist(const point &q) const {
			double dist = 0.0;
			for(int i = 0; i < d; i++){
				dist += pow(coords[i] - q.coords[i], 2);
			}
			return dist;
		}
};

int point::d;

class cloud
{
	private:

	int d;
	int n;
	int k;

	// maximum possible number of points
	int nmax;

	point *points;
	point *centers;


	public:

	cloud(int _d, int _nmax, int _k)
	{
		d = _d;
		point::d = _d;
		n = 0;
		k = _k;

		nmax = _nmax;

		points = new point[nmax];
		centers = new point[k];

		srand(time(0));
	}

	~cloud()
	{
		delete[] centers;
		delete[] points;
	}

	void add_point(const point &p, int label)
	{
		for(int m = 0; m < d; m++)
		{
			points[n].coords[m] = p.coords[m];
		}

		points[n].label = label;

		n++;
	}

	int get_d() const
	{
		return d;
	}

	int get_n() const
	{
		return n;
	}

	int get_k() const
	{
		return k;
	}

	point &get_point(int i)
	{
		return points[i];
	}

	point &get_center(int j)
	{
		return centers[j];
	}

	void set_center(const point &p, int j)
	{
		for(int m = 0; m < d; m++)
			centers[j].coords[m] = p.coords[m];
	}

	double intracluster_variance() const
	{
		double var = 0.0;
		for(int i = 0; i < n; i++){
			var += points[i].squared_dist(centers[points[i].label]);
		}
		return var / n;
	}

	int set_voronoi_labels()
	{
		int counter = 0;
		for(int i = 0; i < n; i++){
			int curr_label = points[i].label;
			for(int j = 0; j < k; j++){
				if(points[i].squared_dist(centers[j]) < points[i].squared_dist(centers[curr_label])){
					curr_label = j;
				}
			}
			if(curr_label != points[i].label){
				counter++;
			}
			points[i].label = curr_label;
		}
		return counter;
	}

	void copyArray(point* source, point* destination, int size) {
		for (int i = 0; i < size; ++i) {
			for (int cord = 0; cord < d; cord++)
				destination[i].coords[cord] = source[i].coords[cord];
		}
	}

	void set_centroid_centers()
	{
		int *counter = new int[k];
		point *prev_centers = new point[k];

		copyArray(centers, prev_centers, k);

		for(int j = 0; j < k; j++){
			counter[j] = 0;
		}
		for(int i = 0; i < n; i++){
			int c_p = points[i].label;
			counter[c_p]++;
			for(int c_coord = 0; c_coord < d; c_coord++){
				// std::cout << "before assignement "<<  centers[c_p].coords[c_coord] << ' ' << prev_centers[c_p].coords[c_coord]<< std::endl;
				centers[c_p].coords[c_coord] += points[i].coords[c_coord];
				// std::cout << "after assignement " << centers[c_p].coords[c_coord] << ' ' << prev_centers[c_p].coords[c_coord]<< std::endl;
			}
		}
		for(int j = 0; j < k; j++){
			for(int c_coord = 0; c_coord < d; c_coord++){
				if(counter[j] != 0){
					centers[j].coords[c_coord] = (centers[j].coords[c_coord] - prev_centers[j].coords[c_coord]) / (double) counter[j];
				}
			}
		}

		delete [] counter;
		delete[] prev_centers;
	}

	void init_random_partition()
	{
		for (int i = 0; i < n; i++) {
			points[i].label = rand() % k;
		}
		set_centroid_centers();
	}

	void lloyd()
	{
		double var = INFINITY;
		double newvar = intracluster_variance();
		while(abs(newvar - var) > 1e-6){
			var = newvar;
			set_voronoi_labels();
			set_centroid_centers();
			newvar = intracluster_variance();
		}
	}

	void init_forgy()
	{
	}

	void init_plusplus()
	{
	}
};
