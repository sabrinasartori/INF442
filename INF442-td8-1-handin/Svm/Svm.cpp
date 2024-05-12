#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <numeric>

#include "Kernel.hpp"
#include "ConfusionMatrix.hpp"
#include "Svm.hpp"

SVM::SVM(Dataset* dataset, int col_class, Kernel K):
    col_class(col_class), kernel(K) {
    train_labels = std::vector<int>(dataset->get_nbr_samples());
    train_features = std::vector<std::vector<double>>(dataset->get_nbr_samples(), std::vector<double>(dataset->get_dim() - 1));
    // Exercise 2: put the correct columns of dataset in train_labels and _features AGAIN!
    // BEWARE: transform 0/1 labels to -1/1
    for (int i = 0; i < dataset->get_nbr_samples(); i++){
        std::vector<double> row = dataset->get_instance(i);
        for (int j = 0; j < dataset->get_dim(); j++){
            if (j == col_class) train_labels[i] = 2*row[j] - 1;
            else if (j < col_class) train_features[i][j] = row[j];
            else train_features[i][j-1] = row[j];
        }
    }
    compute_kernel();
}

SVM::~SVM() {
}

void SVM::compute_kernel() {
    const int n = train_features.size();
    alpha = std::vector<double>(n);
    computed_kernel = std::vector<std::vector<double>>(n, std::vector<double>(n));

    // Exercise 2: put y_i y_j k(x_i, x_j) in the (i, j)th coordinate of computed_kernel
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            computed_kernel[i][j] = train_labels[i] * train_labels[j] * kernel.k(train_features[i], train_features[j]);
        }
    }
}

void SVM::compute_beta_0(double C) {
    // count keeps track of the number of support vectors (denoted by n_s)
    int count = 0;
    beta_0 = 0.0;
    // Exercise 3
    // Use clipping_epsilon < alpha < C - clipping_epsilon instead of 0 < alpha < C
    // This performs 1/n_s
    const int n = train_features.size();
    for (int s = 0; s < n; s++){ 
        if(clipping_epsilon < alpha[s] && alpha[s] < (C-clipping_epsilon)){
            count += 1;
            for (int i = 0; i < n; i++){
                beta_0 += alpha[i] * train_labels[i] * kernel.k(train_features[i], train_features[s]);
            }
            beta_0 -= train_labels[s];
        }
    }
    beta_0 /= count;
}

void SVM::train(const double C, const double lr) {
    // Exercise 4
    // Perform projected gradient ascent
    // While some alpha is not clipped AND its gradient is above stopping_criterion
    // (1) Set stop = false
    // (2) While not stop do
    // (2.1) Set stop = true
    // (2.2) For i = 1 to n do
    // (2.2.1) Compute the gradient - HINT: make good use of computed_kernel
    // (2.2.2) Move alpha in the direction of the gradient - eta corresponds to lr (for learning rate)
    // (2.2.3) Project alpha in the constraint box by clipping it
    // (2.2.4) Adjust stop if necessary
    // (3) Compute beta_0

    const int n = train_features.size();
    bool stop = false;
    while(!stop) {
        stop = true;
        for (int i = 0; i < n; i++) {
            double gradient = 1.0;
            for (int j = 0; j < n; j++){
                gradient -= alpha[j] * computed_kernel[i][j];
            }
            alpha[i] += lr * gradient;
            if (0.0 < alpha[i] && alpha[i] < C && std::abs(gradient) > stopping_criterion) stop = false;
            alpha[i] = std::max(0.0, std::min(alpha[i], C));
        }
    }
    // Update beta_0
    compute_beta_0(C);
}

int SVM::f_hat(const std::vector<double> x) {
    // Exercise 5
    int n = train_features.size();
    double y_pred = 0;
    for (int i = 0; i < n; i++){
        y_pred += alpha[i] * train_labels[i] * kernel.k(train_features[i], x);
    }
    y_pred -= beta_0;
    return 2*(y_pred > 0) - 1;
}

ConfusionMatrix SVM::test(const Dataset* test) {
    // Collect test_features and test_labels and compute confusion matrix
    std::vector<double> test_labels (test->get_nbr_samples());
    std::vector<std::vector<double>> test_features (test->get_nbr_samples(), std::vector<double>(test->get_dim() - 1));
    ConfusionMatrix cm;

    // Exercise 6
    // Put test dataset in features and labels
    // Use f_hat to predict and put into the confusion matrix
    // BEWARE: transform -1/1 prediction to 0/1 label

    int n = test->get_nbr_samples();
    int dim = test->get_dim();
    for (int i = 0; i < n; i++){
        std::vector<double> row = test->get_instance(i);
        for (int j = 0; j < dim; j++){
            if (j == col_class) test_labels[i] = 2*row[j]-1;
            test_features[i][j - (j>col_class)] = row[j];
        }
    }
    for(int i = 0; i < n; i++){
        int y = (test_labels[i]+1)/2;
        int y_pred = (f_hat(test_features[i])+1)/2;
        cm.add_prediction(y, y_pred);
    }
    return cm;
}

int SVM::get_col_class() const {
    return col_class;
}

Kernel SVM::get_kernel() const {
    return kernel;
}

std::vector<int> SVM::get_train_labels() const {
    return train_labels;
}

std::vector<std::vector<double>> SVM::get_train_features() const {
    return train_features;
}

std::vector<std::vector<double>> SVM::get_computed_kernel() const {
    return computed_kernel;
}

std::vector<double> SVM::get_alphas() const {
    return alpha;
}

double SVM::get_beta_0() const {
    return beta_0;
}

void SVM::set_alphas(std::vector<double> alpha) {
    this->alpha = alpha;
}
