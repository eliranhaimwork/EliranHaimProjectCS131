// CS 131 Repository.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

using namespace std;

// Function to read CSV data
vector<vector<double>> readCSV(const string& filename) {
    vector<vector<double>> data;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return data;
    }

    string line;
    while (getline(file, line)) {
        vector<double> row;
        stringstream ss(line);
        string value;
        while (getline(ss, value, ',')) {
            row.push_back(stod(value));
        }
        data.push_back(row);
    }
    file.close();
    return data;
}

// Linear Regression: Train Model
pair<vector<double>, double> trainLinearRegression(const vector<vector<double>>& data) {
    int n = data.size();
    int features = data[0].size() - 1;  // Exclude the target column
    vector<double> weights(features, 0);
    double bias = 0;

    // Training Parameters
    double learning_rate = 0.01;
    int epochs = 1000;

    for (int epoch = 0; epoch < epochs; ++epoch) {
        vector<double> gradients(features, 0);
        double bias_gradient = 0;

        for (const auto& row : data) {
            double prediction = bias;
            for (int j = 0; j < features; ++j) {
                prediction += weights[j] * row[j];
            }
            double error = prediction - row[features];
            bias_gradient += error;

            for (int j = 0; j < features; ++j) {
                gradients[j] += error * row[j];
            }
        }

        // Update weights and bias
        for (int j = 0; j < features; ++j) {
            weights[j] -= learning_rate * gradients[j] / n;
        }
        bias -= learning_rate * bias_gradient / n;
    }

    return { weights, bias };
}

// Predict Temperature
double predictTemperature(const vector<double>& features, const vector<double>& weights, double bias) {
    double prediction = bias;
    for (size_t i = 0; i < features.size(); ++i) {
        prediction += weights[i] * features[i];
    }
    return prediction;
}

int main() {
    string filename = "weather_data.csv";  // Ensure you have a CSV file with your data
    auto data = readCSV(filename);

    if (data.empty()) {
        cerr << "No data found!" << endl;
        return 1;
    }

    // Split data into training and testing sets (80-20 split)
    int train_size = static_cast<int>(0.8 * data.size());
    vector<vector<double>> train_data(data.begin(), data.begin() + train_size);
    vector<vector<double>> test_data(data.begin() + train_size, data.end());

    // Train Linear Regression Model
    auto result = trainLinearRegression(train_data);  // Get weights and bias as a pair
    vector<double> weights = result.first;           // Extract weights
    double bias = result.second;                     // Extract bias

    // Test Model
    cout << "Testing Model..." << endl;
    for (const auto& row : test_data) {
        vector<double> features(row.begin(), row.end() - 1);
        double actual = row.back();
        double predicted = predictTemperature(features, weights, bias);
        cout << "Actual: " << actual << ", Predicted: " << predicted << endl;
    }

    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
