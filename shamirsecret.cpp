#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <bigint.hpp>

BigInteger decode(const std::string &value, int base) {
    return BigInteger(value, base);
}

BigInteger lagrangeInterpolation(const std::vector<int>& x, const std::vector<BigInteger>& y, int k) {
    BigInteger result = BigInteger(0);

    for (int i = 0; i < k; i++) {
        BigInteger term = y[i];

        for (int j = 0; j < k; j++) {
            if (i != j) {
                BigInteger numerator = BigInteger(-x[j]);
                BigInteger denominator = BigInteger(x[i] - x[j]);
                term = term * numerator / denominator;
            }
        }

        result = result + term;
    }

    return result;
}

int main() {
    try {
        std::ifstream file("input.json");
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string json = buffer.str();

        size_t pos_n = json.find("\"n\":");
        size_t pos_k = json.find("\"k\":");

        int n = std::stoi(json.substr(pos_n + 4, json.find(",", pos_n) - pos_n - 4));
        int k = std::stoi(json.substr(pos_k + 4, json.find("}", pos_k) - pos_k - 4));

        std::cout << "Parsed n: " << n << ", k: " << k << std::endl;

        if (k > n) {
            std::cout << "Error: k cannot be greater than n." << std::endl;
            return 0;
        }

        std::vector<int> x;
        std::vector<BigInteger> y;

        for (int i = 1; i <= n; i++) {
            std::string index = "\"" + std::to_string(i) + "\":";
            size_t startIndex = json.find(index) + index.length();
            size_t endIndex = json.find("}", startIndex);

            std::string point = json.substr(startIndex, endIndex - startIndex + 1);

            size_t base_pos = point.find("\"base\":") + 7;
            size_t base_end = point.find(",", base_pos);
            int base = std::stoi(point.substr(base_pos, base_end - base_pos));

            size_t value_pos = point.find("\"value\":") + 8;
            size_t value_end = point.find("\"", value_pos + 1);
            std::string valueStr = point.substr(value_pos, value_end - value_pos);

            x.push_back(i);
            y.push_back(decode(valueStr, base));
        }

        if (x.size() < k || y.size() < k) {
            std::cout << "Error: Insufficient data for interpolation." << std::endl;
            return 0;
        }

        BigInteger constantTerm = lagrangeInterpolation({x.begin(), x.begin() + k}, {y.begin(), y.begin() + k}, k);

        std::cout << "The constant term (f(0)) is: " << constantTerm << std::endl;

    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

// OUTPUT:- 
// test case 1: 3 
// test case 2: 79836264049764
