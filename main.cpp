#include "data_lab.hpp"
#include <random>

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
}
