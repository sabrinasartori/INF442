#include <iostream>
#include <ANN/ANN.h>
#include "KnnRegression.hpp"

KnnRegression::KnnRegression(int k, Dataset* dataset, int col_regr)
: Regression(dataset, col_regr) {
	m_k = k;
 	// TODO Exercise 5
	m_dataset = dataset;
	m_col_regr = col_regr;
    int n_samples = dataset->get_nbr_samples();
    int dim = dataset->get_dim();
    m_dataPts = annAllocPts(n_samples, dim);

	int n_points = 0;
	while(n_points < n_samples){
		double* curr_point = new double[dim-1];
		for(int i = 0; i < dim; i++){
			if (i < col_regr) curr_point[i] = dataset->get_instance(n_points)[i];
			else if (i > col_regr) curr_point[i-1] = dataset->get_instance(n_points)[i];
		}
		n_points++;
	}

	m_kdTree = new ANNkd_tree(m_dataPts,n_samples,dim);

}

KnnRegression::~KnnRegression() {
	// TODO Exercise 5
	delete [] m_dataPts;
    delete m_kdTree;
}

double KnnRegression::estimate(const Eigen::VectorXd & x) const {
	assert(x.size()==m_dataset->get_dim()-1);
	// TODO Exercise 6
	int k = get_k();

	ANNpoint x_ann = annAllocPt(x.size());
    ANNidxArray nn_idx = new ANNidx[k];
    ANNdistArray dists = new ANNdist[k];

	for(int i = 0; i < x.size(); i++) x_ann[i] = x[i];

	m_kdTree->annkSearch(x_ann, k, nn_idx, dists, 0.0001);
    
    double y_est = 0;
    for(int i = 0; i < k; i++){
		y_est += m_dataset->get_instance(nn_idx[i])[m_col_regr];
	}
    y_est = y_est/((double) k);

	delete [] nn_idx;
    delete [] dists;

    return y_est;
}


int KnnRegression::get_k() const {
	return m_k;
}

ANNkd_tree* KnnRegression::get_kdTree() const {
	return m_kdTree;
}
