#include<iostream>
#include<cassert>
#include "LinearRegression.hpp"
#include "Dataset.hpp"
#include "Regression.hpp"

LinearRegression::LinearRegression(Dataset* dataset, int col_regr) 
: Regression(dataset, col_regr) {
	m_beta = NULL;
	set_coefficients();
}

LinearRegression::~LinearRegression() {
	if (m_beta != NULL) {
		m_beta->resize(0);
		delete m_beta;
	}
}

Eigen::MatrixXd LinearRegression::construct_matrix() {
	// TODO Exercise 1

	int n_samples = m_dataset->get_nbr_samples();
	int dim = m_dataset->get_dim();
	Eigen::MatrixXd X(n_samples, dim);
	for (int i = 0; i < n_samples; i++){
		X(i,0) = 1.0;
		std::vector<double> row_i = m_dataset->get_instance(i);
		for (int j = 0; j < dim; j++){
			if (j < m_col_regr) X(i, j+1) = row_i[j];
			else if (j > m_col_regr) X(i, j) = row_i[j];
		}
	}
	return X;
}

Eigen::VectorXd LinearRegression::construct_y() {
	// TODO Exercise 1

	int n_samples = m_dataset->get_nbr_samples();
	Eigen::VectorXd y(n_samples);
	for (int i = 0; i < n_samples; i++){
		std::vector<double> row_i = m_dataset->get_instance(i);
		y(i) = row_i[m_col_regr];
	}
	return y;
}

void LinearRegression::set_coefficients() {
	// TODO Exercise 2

	Eigen::MatrixXd X = construct_matrix();
	Eigen::VectorXd y = construct_y();

	// linear system Ax = b
	Eigen::MatrixXd A = X.transpose() * X;
	Eigen::VectorXd b = X.transpose() * y;

	m_beta = new Eigen::VectorXd(A.colPivHouseholderQr().solve(b));
	// m_beta = new Eigen::VectorXd(beta.rows());
	// *m_beta = beta;
}

const Eigen::VectorXd* LinearRegression::get_coefficients() const {
	if (!m_beta) {
		std::cout <<"Coefficients have not been allocated." <<std::endl;
		return NULL;
	}
	return m_beta;
}

void LinearRegression::show_coefficients() const {
	if (!m_beta) {
		std::cout << "Coefficients have not been allocated." <<std::endl;
		return;
	}
	
	if (m_beta->size() != m_dataset->get_dim()) {  // ( beta_0 beta_1 ... beta_{d} )
		std::cout << "Warning, unexpected size of coefficients vector: " << m_beta->size() << std::endl;
	}
	
	std::cout<< "beta = (";
	for (int i=0; i<m_beta->size(); i++) {
		std::cout << " " << (*m_beta)[i];
	}
	std::cout << " )" <<std::endl;
}

void LinearRegression::print_raw_coefficients() const {
	std::cout<< "{ ";
	for (int i = 0; i < m_beta->size() - 1; i++) {
		std::cout << (*m_beta)[i] << ", ";
	}
	std::cout << (*m_beta)[m_beta->size() - 1];
	std::cout << " }" << std::endl;
}

void LinearRegression::sum_of_squares(Dataset* dataset, double& ess, double& rss, double& tss) const {
	assert(dataset->get_dim()==m_dataset->get_dim());
	// TODO Exercise 4

	int n_samples = dataset->get_nbr_samples();
	int dim = dataset->get_dim();

	LinearRegression linReg = LinearRegression(dataset, m_col_regr);

	Eigen::MatrixXd X = linReg.construct_matrix();
	Eigen::VectorXd y = linReg.construct_y();
	Eigen::VectorXd y_pred(n_samples);

	double y_mean = 0;
	for (int i = 0; i < n_samples; i++) y_mean += y[i];
	y_mean /= n_samples;

	rss = 0;
	ess = 0;
	tss = 0;
	for (int i = 0; i < n_samples; i++){
		Eigen::VectorXd X_i = X.row(i);
		y_pred[i] = m_beta->dot(X_i);

		rss += (y_pred[i] - y[i])*(y_pred[i] - y[i]);
		ess += (y_pred[i] - y_mean)*(y_pred[i] - y_mean);
		tss += (y[i] - y_mean)*(y[i] - y_mean);
	}
}

double LinearRegression::estimate(const Eigen::VectorXd & x) const {
	// TODO Exercise 3

	int dim = m_dataset->get_dim();
	Eigen::VectorXd betas = *m_beta;
	double res = betas[0];
	for (int i = 0; i < dim-1; i++){
		res += x[i] * betas[i+1];
	}
	return res;
}
