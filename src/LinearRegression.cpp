#include "LinearRegression.hpp"

#include <cmath>
#include <stdexcept>
#include <numeric>
#include <iostream>
#include <iomanip>

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
LinearRegression::LinearRegression(double learningRate, int iterations)
    : theta0_(0.0)
    , theta1_(0.0)
    , learningRate_(learningRate)
    , iterations_(iterations)
    , meanKm_(0.0)
    , stdKm_(1.0)
    , meanPrice_(0.0)
    , stdPrice_(1.0)
{
    if (learningRate_ <= 0.0)
        throw std::invalid_argument("Learning rate must be positive.");
    if (iterations_ <= 0)
        throw std::invalid_argument("Number of iterations must be positive.");
}

// ---------------------------------------------------------------------------
// Public interface
// ---------------------------------------------------------------------------
double LinearRegression::estimatePrice(double km) const
{
    return theta0_ + theta1_ * km;
}

double LinearRegression::getTheta0() const { return theta0_; }
double LinearRegression::getTheta1() const { return theta1_; }

// ---------------------------------------------------------------------------
// Training
// ---------------------------------------------------------------------------
void LinearRegression::train(const std::vector<std::pair<double, double>> &data)
{
    if (data.empty())
        throw std::invalid_argument("Training data must not be empty.");

    const int m = static_cast<int>(data.size());

    // Separate km and price into individual vectors for normalization
    std::vector<double> km(m), price(m);
    for (int i = 0; i < m; ++i)
    {
        km[i]    = data[i].first;
        price[i] = data[i].second;
    }

    // Z-score normalize both features and labels
    normalize(km,    meanKm_,    stdKm_);
    normalize(price, meanPrice_, stdPrice_);

    // Reset thetas to 0 for normalized training
    theta0_ = 0.0;
    theta1_ = 0.0;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Training on " << m << " samples, "
              << iterations_ << " iterations, "
              << "lr=" << learningRate_ << "\n";

    // -----------------------------------------------------------------------
    // Batch gradient descent
    //
    // Cost:   J(θ) = (1/2m) * Σ (estimatePrice(km[i]) - price[i])²
    //
    // Gradients:
    //   ∂J/∂θ₀ = (1/m) * Σ (estimatePrice(km[i]) - price[i])
    //   ∂J/∂θ₁ = (1/m) * Σ (estimatePrice(km[i]) - price[i]) * km[i]
    //
    // Update rule (simultaneously):
    //   θ₀ -= α * ∂J/∂θ₀
    //   θ₁ -= α * ∂J/∂θ₁
    // -----------------------------------------------------------------------
    for (int iter = 0; iter < iterations_; ++iter)
    {
        double sumGrad0 = 0.0;
        double sumGrad1 = 0.0;

        for (int i = 0; i < m; ++i)
        {
            double error = (theta0_ + theta1_ * km[i]) - price[i];
            sumGrad0 += error;
            sumGrad1 += error * km[i];
        }

        // Simultaneous update using temporary variables
        double tmpTheta0 = learningRate_ * (sumGrad0 / m);
        double tmpTheta1 = learningRate_ * (sumGrad1 / m);

        theta0_ -= tmpTheta0;
        theta1_ -= tmpTheta1;

        // Print cost every 1000 iterations for monitoring convergence
        if ((iter + 1) % 1000 == 0)
        {
            double cost = 0.0;
            for (int i = 0; i < m; ++i)
            {
                double e = (theta0_ + theta1_ * km[i]) - price[i];
                cost += e * e;
            }
            cost /= (2.0 * m);
            std::cout << "  Iteration " << std::setw(6) << (iter + 1)
                      << "  |  Cost: " << cost << "\n";
        }
    }

    // Convert thetas from normalized space back to real-world scale
    denormalize();

    std::cout << "\nTraining complete.\n";
    std::cout << "  θ₀ (intercept) = " << theta0_ << "\n";
    std::cout << "  θ₁ (slope)     = " << theta1_ << "\n";
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------
double LinearRegression::mean(const std::vector<double> &v) const
{
    double sum = 0.0;
    for (double x : v)
        sum += x;
    return sum / static_cast<double>(v.size());
}

double LinearRegression::stddev(const std::vector<double> &v, double m) const
{
    double variance = 0.0;
    for (double x : v)
        variance += (x - m) * (x - m);
    variance /= static_cast<double>(v.size()); // population std dev
    return std::sqrt(variance);
}

void LinearRegression::normalize(std::vector<double> &v,
                                  double &outMean, double &outStd) const
{
    outMean = mean(v);
    outStd  = stddev(v, outMean);

    if (outStd == 0.0)
        throw std::runtime_error("Standard deviation is zero — all feature values are identical.");

    for (double &x : v)
        x = (x - outMean) / outStd;
}

void LinearRegression::denormalize()
{
    // Derivation:
    //   price_norm = θ₀_norm + θ₁_norm * km_norm
    //   (price - μ_p) / σ_p = θ₀_norm + θ₁_norm * (km - μ_k) / σ_k
    //   price = μ_p + σ_p * θ₀_norm + σ_p * θ₁_norm / σ_k * km
    //                               - σ_p * θ₁_norm * μ_k / σ_k
    //   ⟹  θ₁ = θ₁_norm * (σ_p / σ_k)
    //   ⟹  θ₀ = μ_p + σ_p * (θ₀_norm - θ₁_norm * μ_k / σ_k)

    theta1_ = theta1_ * (stdPrice_ / stdKm_);
    theta0_ = meanPrice_ + stdPrice_ * (theta0_ - (theta1_ * meanKm_ / stdPrice_));
}
