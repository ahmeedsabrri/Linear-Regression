#include "utils.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

// ---------------------------------------------------------------------------
// loadCSV
// ---------------------------------------------------------------------------
std::vector<std::pair<double, double>> loadCSV(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filename);

    std::vector<std::pair<double, double>> data;
    std::string line;

    // Skip header line (km,price)
    if (!std::getline(file, line))
        throw std::runtime_error("CSV file is empty: " + filename);

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::istringstream ss(line);
        std::string kmStr, priceStr;

        if (!std::getline(ss, kmStr, ',') || !std::getline(ss, priceStr, ','))
            throw std::runtime_error("Malformed CSV row: " + line);

        try
        {
            double km    = std::stod(kmStr);
            double price = std::stod(priceStr);
            data.emplace_back(km, price);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Invalid numeric value in CSV row: " + line);
        }
    }

    if (data.empty())
        throw std::runtime_error("No data found in CSV file: " + filename);

    return data;
}

// ---------------------------------------------------------------------------
// saveThetas
// ---------------------------------------------------------------------------
void saveThetas(const std::string &filename, double theta0, double theta1)
{
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot write to file: " + filename);

    // Store with full precision so predictions remain accurate
    file.precision(15);
    file << theta0 << "," << theta1 << "\n";
}

// ---------------------------------------------------------------------------
// loadThetas
// ---------------------------------------------------------------------------
void loadThetas(const std::string &filename, double &theta0, double &theta1)
{
    // Safe default: untrained model returns 0
    theta0 = 0.0;
    theta1 = 0.0;

    std::ifstream file(filename);
    if (!file.is_open())
    {
        // Model file doesn't exist yet — this is expected before first training
        return;
    }

    std::string line;
    if (!std::getline(file, line) || line.empty())
        return; // Empty file → keep defaults

    std::istringstream ss(line);
    std::string t0Str, t1Str;

    if (!std::getline(ss, t0Str, ',') || !std::getline(ss, t1Str, ','))
        throw std::runtime_error("Malformed thetas file: " + filename);

    try
    {
        theta0 = std::stod(t0Str);
        theta1 = std::stod(t1Str);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Invalid values in thetas file: " + filename);
    }
}
