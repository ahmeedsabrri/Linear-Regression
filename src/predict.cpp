#include "utils.hpp"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <algorithm>

static const std::string DEFAULT_MODEL_FILE = "models/thetas.csv";

int main(int argc, char **argv)
{
    std::string modelFile = DEFAULT_MODEL_FILE;
    if (argc >= 2) modelFile = argv[1];

    double theta0 = 0.0;
    double theta1 = 0.0;

    try
    {
        loadThetas(modelFile, theta0, theta1);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Warning: could not load model (" << e.what()
                  << "). Defaulting to θ₀=0, θ₁=0.\n";
    }

    // ── Prompt user ────────────────────────────────────────────────────────
    double mileage = 0.0;

    while (true)
    {
        std::cout << "Enter mileage (km): ";

        std::string input;
        if (!std::getline(std::cin, input))
        {
            std::cerr << "Error: failed to read input.\n";
            return 1;
        }

        // Trim leading/trailing whitespace
        input.erase(0, input.find_first_not_of(" \t\r\n"));
        input.erase(input.find_last_not_of(" \t\r\n") + 1);

        if (input.empty())
        {
            std::cerr << "Input cannot be empty. Please enter a numeric value.\n";
            continue;
        }

        bool valid = true;
        std::size_t idx = 0;

        try
        {
            mileage = std::stod(input, &idx);
        }
        catch (...)
        {
            valid = false;
        }

        // Reject input if it wasn't fully consumed (e.g. "100abc")
        if (!valid || idx != input.size())
        {
            std::cerr << "Invalid input: \"" << input
                      << "\". Please enter a non-negative number.\n";
            continue;
        }

        if (mileage < 0.0)
        {
            std::cerr << "Mileage cannot be negative. Please try again.\n";
            continue;
        }

        break; // valid input received
    }

    // ── Predict ────────────────────────────────────────────────────────────
    // estimatePrice(mileage) = θ₀ + θ₁ * mileage  (as per subject spec)
    double estimatedPrice = theta0 + theta1 * mileage;

    std::cout << "Estimated price for " << mileage
              << " km: " << estimatedPrice << "\n";

    return 0;
}
