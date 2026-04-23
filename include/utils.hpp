#pragma once

#include <vector>
#include <string>
#include <utility>

/**
 * @brief Load (km, price) pairs from a CSV file.
 *        The CSV must have a header row: km,price
 *
 * @param filename  Path to the CSV file.
 * @return          Vector of (km, price) pairs.
 */
std::vector<std::pair<double, double>> loadCSV(const std::string &filename);

/**
 * @brief Save theta0 and theta1 to a file.
 *
 * @param filename  Destination file path.
 * @param theta0    Intercept.
 * @param theta1    Slope.
 */
void saveThetas(const std::string &filename, double theta0, double theta1);

/**
 * @brief Load theta0 and theta1 from a file.
 *        If the file does not exist, both values default to 0.
 *
 * @param filename  Source file path.
 * @param theta0    Output intercept.
 * @param theta1    Output slope.
 */
void loadThetas(const std::string &filename, double &theta0, double &theta1);
