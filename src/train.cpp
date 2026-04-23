#include "LinearRegression.hpp"
#include "utils.hpp"

#include <iostream>
#include <stdexcept>

// Default paths — can be overridden via command-line arguments
static const std::string DEFAULT_DATA_FILE   = "data/data.csv";
static const std::string DEFAULT_MODEL_FILE  = "models/thetas.csv";

int main(int argc, char **argv)
{
    std::string dataFile  = DEFAULT_DATA_FILE;
    std::string modelFile = DEFAULT_MODEL_FILE;

    // Optional positional arguments: ./train [data_file] [model_file]
    if (argc >= 2) dataFile  = argv[1];
    if (argc >= 3) modelFile = argv[2];

    try
    {
        // ── 1. Load dataset ────────────────────────────────────────────────
        std::cout << "Loading data from: " << dataFile << "\n";
        auto data = loadCSV(dataFile);
        std::cout << "Loaded " << data.size() << " samples.\n\n";

        // ── 2. Train model ─────────────────────────────────────────────────
        //   Learning rate 0.01 on z-score normalized data is well-calibrated.
        //   10 000 iterations is more than enough for this small dataset.
        LinearRegression model(0.01, 10000);
        model.train(data);

        // ── 3. Persist thetas ──────────────────────────────────────────────
        saveThetas(modelFile, model.getTheta0(), model.getTheta1());
        std::cout << "\nModel saved to: " << modelFile << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
