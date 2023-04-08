#ifndef DATA_LAB_STRESS_DATA_LAB_HPP
#define DATA_LAB_STRESS_DATA_LAB_HPP

#include <bitset>
#include <iostream>

template<typename T>
void bit_print(T t) {
    std::cout << std::bitset<32>(*(unsigned*)(&t)).to_string() << std::endl;
}

int bitAnd(int, int);
int getByte(int, int);
int logicalShift(int, int);
int bitCount(int);
int bang(int);
int fitsBits(int x, int n);
int isLessOrEqual(int, int);

#endif
