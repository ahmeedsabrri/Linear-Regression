#pragma once

#include <vector>
#include <utility>

/**
 * @brief Simple linear regression trained with batch gradient descent.
 *
 *  Model:  estimatePrice(km) = theta0 + theta1 * km
 *
 *  Training uses z-score normalization on both features and labels to ensure
 *  stable and fast convergence, then denormalizes the parameters so that
 *  the saved thetas operate directly on raw (un-normalized) km values.
 */
class LinearRegression
{
public:
    /**
     * @param learningRate  Step size for gradient descent (default 0.01).
     * @param iterations    Number of full-dataset passes (default 10000).
     */
    LinearRegression(double learningRate = 0.01, int iterations = 10000);

    /**
     * @brief Run batch gradient descent on the supplied data.
     *
     * Internally normalizes km and price, runs gradient descent, then
     * denormalizes theta0/theta1 back to the original scale.
     *
     * @param data  Vector of (km, price) pairs.
     */
    void train(const std::vector<std::pair<double, double>> &data);

    /**
     * @brief Predict the price for a given mileage using the trained model.
     *
     * @param km  Mileage (raw, un-normalized).
     * @return    Estimated price.
     */
    double estimatePrice(double km) const;

    double getTheta0() const;
    double getTheta1() const;

private:
    double theta0_;
    double theta1_;
    double learningRate_;
    int    iterations_;

    // Normalization statistics (computed during training)
    double meanKm_;
    double stdKm_;
    double meanPrice_;
    double stdPrice_;

    /** Compute mean of a vector. */
    double mean(const std::vector<double> &v) const;

    /** Compute population standard deviation of a vector. */
    double stddev(const std::vector<double> &v, double mean) const;

    /** Z-score normalize a vector in-place; returns mean and std. */
    void normalize(std::vector<double> &v, double &outMean, double &outStd) const;

    /**
     * @brief Convert normalized theta0/theta1 back to real-world scale.
     *
     * If we trained on normalized variables:
     *   price_norm = theta0_norm + theta1_norm * km_norm
     * where:
     *   km_norm    = (km    - meanKm)    / stdKm
     *   price_norm = (price - meanPrice) / stdPrice
     *
     * Then in real scale:
     *   theta1 = theta1_norm * (stdPrice / stdKm)
     *   theta0 = meanPrice   - theta1 * meanKm
     */
    void denormalize();
};
