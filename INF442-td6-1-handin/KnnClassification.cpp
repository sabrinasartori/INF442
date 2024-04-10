
#include "KnnClassification.hpp"
#include <iostream>
#include <ANN/ANN.h>


KnnClassification::KnnClassification(int k, Dataset *dataset, int col_class)
: Classification(dataset, col_class) {
    // TODO Exercise 1
    m_k = k;
    int d = dataset->get_dim();
    int n = dataset->get_n_samples();
    m_data_pts = new ANNpoint[n];
    for(int i = 0; i < n; i++){
        std::vector<double> instance = dataset->get_instance(i);
        m_data_pts[i] = new double[d-1];
        int m_coord = 0;
        for(int j = 0; j < d; j++){
            if(j == col_class) {
                continue;
            };
            m_data_pts[i][m_coord] = instance[j];
            m_coord++;
        }
    }

    m_kd_tree = new ANNkd_tree(m_data_pts, n, d-1);

}

KnnClassification::~KnnClassification() {
    // TODO Exercise 1
    delete[] m_data_pts;
    delete m_kd_tree;
}

int KnnClassification::estimate(const ANNpoint &x, double threshold) const {
    // TODO Exercise 2
    ANNidxArray nn_idx = new ANNidx[m_k];
    ANNdistArray dists = new ANNdist[m_k];
    m_kd_tree->annkSearch(x, m_k, nn_idx, dists, 1e-4);
    
    double count = 0;
    for(int i = 0; i < m_k; i++)
        count += m_dataset->get_instance(nn_idx[i])[m_col_class];
    double proba = count / (double) m_k;

    return proba > threshold;
}

int KnnClassification::get_k() const {
    return m_k;
}

ANNkd_tree *KnnClassification::get_kd_tree() {
    return m_kd_tree;
}

const ANNpointArray KnnClassification::get_points() const {
    return m_data_pts;
} 
