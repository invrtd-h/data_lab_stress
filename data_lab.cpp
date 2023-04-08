#include "data_lab.hpp"

int isLessOrEqual(int x, int y) {
    return ((y+(~x)+1)>>31)&1;
}