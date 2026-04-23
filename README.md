# Linear-Regression

An introduction to machine learning through first-principles engineering.

This project implements simple linear regression from scratch in C++17, without ML frameworks.

If you want to start your career as a machine learning engineer, this is one of the best first projects you can do. It teaches the core mindset you will need later for deep learning, MLOps, and production systems:

- understand the math behind the model
- implement training yourself
- debug the full pipeline (data, training, persistence, inference)
- reason about convergence and numerical stability

You are not only training a model here. You are training your engineering thinking.

---

## Why This Project Matters

Most beginners start by calling high-level libraries. That is useful later, but dangerous if you skip fundamentals.

This project makes you own every important part of the ML loop:

1. define the hypothesis
2. compute gradients
3. update parameters correctly (simultaneously)
4. monitor training quality
5. save and reuse a model for inference

Once you can do that confidently, advanced tools become much easier to learn and use correctly.

---

## Project Goal

Predict car price from mileage using one-feature linear regression:

estimatePrice(km) = theta0 + theta1 * km

You build two programs:

- training program: reads dataset, learns theta0/theta1, saves model
- prediction program: loads model and estimates a price from user mileage input

---

## ML Concepts You Practice

- supervised learning
- regression
- cost minimization
- batch gradient descent
- feature scaling and normalization
- model persistence
- input validation and failure handling

---

## Mathematical Core

### 1) Hypothesis

h(x) = theta0 + theta1 * x

Where:

- x is mileage
- h(x) is predicted price

### 2) Error and Cost

For each sample i:

error_i = h(x_i) - y_i

Global cost:

J(theta0, theta1) = (1 / (2m)) * sum(error_i^2)

### 3) Gradients

tmpTheta0 = alpha * (1 / m) * sum(error_i)

tmpTheta1 = alpha * (1 / m) * sum(error_i * x_i)

Update step (simultaneous):

theta0 = theta0 - tmpTheta0

theta1 = theta1 - tmpTheta1

This is exactly what the project subject requires.

### 4) Normalization

The project normalizes km and price with z-score before training:

x_norm = (x - mean_x) / std_x

y_norm = (y - mean_y) / std_y

Then it denormalizes learned parameters so saved thetas work directly on raw mileage inputs.

Why this matters:

- faster convergence
- more stable training
- easier learning-rate tuning

---

## Implementation Overview

### Data Pipeline

- Load CSV from data/data.csv
- Parse each row as (km, price)
- Fail fast on malformed/invalid rows

### Training Pipeline

- Initialize theta0 = 0 and theta1 = 0
- Normalize feature and label vectors
- Run batch gradient descent for configured iterations
- Track cost every 1000 iterations
- Denormalize parameters
- Save model to models/thetas.csv

### Inference Pipeline

- Load theta0/theta1 from models/thetas.csv
- If model file is missing, default to theta0 = 0 and theta1 = 0
- Prompt user for mileage
- Validate non-empty numeric non-negative input
- Predict using estimatePrice(km) = theta0 + theta1 * km

---

## Project Structure

```
.
├── data/
│   └── data.csv               # training dataset (km, price)
├── include/
│   ├── LinearRegression.hpp   # model class interface
│   └── utils.hpp              # CSV/model file helper declarations
├── models/
│   └── thetas.csv             # saved parameters after training
├── src/
│   ├── LinearRegression.cpp   # training + normalization logic
│   ├── utils.cpp              # CSV parsing and theta save/load
│   ├── train.cpp              # training executable entrypoint
│   └── predict.cpp            # prediction executable entrypoint
├── objects/                   # generated object files
├── bin/                       # generated executables
└── Makefile
```

---

## Build

```bash
make          # build train and psumredict
make re       # full rebuild
make clean    # remove objects/
make fclean   # remove objects/ and bin/
```

Compiler flags:

-Wall -Wextra -Werror -std=c++17

---

## Usage

### 1) Train

```bash
./bin/train
```

Optional custom paths:

```bash
./bin/train path/to/data.csv path/to/output_thetas.csv
```

Expected flow:

- load dataset
- run 10,000 iterations at lr = 0.01
- print periodic cost
- print final theta0/theta1
- save model file

### 2) Predict

```bash
./bin/predict
```

Optional custom model path:

```bash
./bin/predict path/to/thetas.csv
```

Example:

```text
Enter mileage (km): 100000
Estimated price for 100000 km: 6354.7
```

Input behavior:

- empty input is rejected
- non-numeric input is rejected
- negative mileage is rejected

---

## Typical Result (Current Dataset)

After training on data/data.csv (24 samples), a typical model is:

- theta0 (intercept): 8499.60
- theta1 (slope): -0.021449

Example predictions:

- 0 km -> 8499.6
- 100000 km -> 6354.7
- 240000 km -> 3351.9

---

## Why You Should Do This Before Your ML Career

If you can explain and implement this project deeply, you are already ahead of many entry-level candidates.

You prove that you can:

- move from equations to reliable code
- build an end-to-end ML workflow
- understand what libraries automate under the hood
- debug model behavior instead of guessing

This is the foundation you need before bigger topics like neural networks, feature engineering at scale, model serving, and MLOps.

Start here. Master this. Then scale up.
