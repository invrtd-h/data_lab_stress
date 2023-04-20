#include "data_lab.hpp"
#include <array>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <random>

int bitAndTest(int x, int y) {
    return x & y;
}

int getByteTest(int x, int n) {
    return int(*((unsigned char*)(&x)+n));
}

int bitCountTest(int x) {
    unsigned y = *(unsigned*)(&x);
    int r = 0;
    for (; y; y /= 2) {
        r += int(y % 2U);
    }
    return r;
}

int bangTest(int x) {
    return !x;
}

int fitsBitsTest(int x, int n) {
    if (n == 0) {
        return 0;
    }
    if (n == 32) {
        return 1;
    }
    return -(1L << n) <= x && x < (1L << n);
}

int divpwr2Test(int x, int n) {
    return x / (1<<n);
}

int negateTest(int x) {
    return -x;
}

int isPositiveTest(int x) {
    return x > 0;
}

int isLessOrEqualTest(int x, int y) {
    return x <= y;
}

int logicalShiftTest(int x, int n) {
    unsigned y = *(unsigned*)(&x);
    y>>=n;
    return *(int*)(&y);
}

int ilog2Test(int x) {
    if (x <= 0) {
        return -1;
    }
    int n = 0;
    for (; x; x /= 2) {
        ++n;
    }
    return n - 1;
}

unsigned float_neg_test(unsigned uf) {
    float f = *(float*)(&uf);
    if (std::isnan(f)) {
        return uf;
    }
    f = -f;
    return *(unsigned*)(&f);
}

unsigned float_i2f_test(int x) {
    auto f = (float)x;
    return *(unsigned*)(&f);
}

unsigned float_twice_test(unsigned uf) {
    float f = *(float*)(&uf);
    if (std::isnan(f)) {
        return uf;
    }
    f = 2 * f;
    return *(unsigned*)(&f);
}

struct FuncHolder {
    enum {II, III, UU, UI} tag;
    void *func, *test;
    int ubd;
    
    using ii = int (*)(int);
    using iii = int (*)(int, int);
    using uu = unsigned (*)(unsigned);
    using ui = unsigned (*)(int);
    
    static FuncHolder from(ii f, ii g, int ubd = 0) {
        return FuncHolder{II, (void*)f, (void*)g, ubd};
    }
    static FuncHolder from(iii f, iii g, int ubd = 0) {
        return FuncHolder{III, (void*)f, (void*)g, ubd};
    }
    static FuncHolder from(uu f, uu g, int ubd = 0) {
        return FuncHolder{UU, (void*)f, (void*)g, ubd};
    }
    static FuncHolder from(ui f, ui g, int ubd = 0) {
        return FuncHolder{UI, (void*)f, (void*)g, ubd};
    }
    
    bool operator()(int t, int u = 0) const {
        if (ubd != 0) {
            u = u < 0 ? ~u : u;
            u %= ubd;
        }
        if (tag == II) {
            auto func_ret = ii(func)(t);
            auto test_ret = ii(test)(t);
            if (func_ret != test_ret) {
                printf("failed %d\n", t);
                printf("got %d, expected %d\n", func_ret, test_ret);
                std::cout << std::bitset<32>(*(unsigned*)(&t)).to_string() << std::endl;
                return false;
            }
        } else if (tag == III) {
            auto func_ret = iii(func)(t, u);
            auto test_ret = iii(test)(t, u);
            if (func_ret != test_ret) {
                printf("failed %d %d\n", t, u);
                printf("got %d, expected %d\n", func_ret, test_ret);
                std::cout << std::bitset<32>(*(unsigned*)(&t)).to_string() << '\n'
                          << std::bitset<32>(*(unsigned*)(&u)).to_string() << std::endl;
                return false;
            }
        } else if (tag == UU) {
            auto func_ret = uu(func)(*(unsigned*)(&t));
            auto test_ret = uu(test)(*(unsigned*)(&t));
            if (func_ret != test_ret) {
                printf("failed %u\n", t);
                printf("got %u, expected %u\n", func_ret, test_ret);
                std::cout << "t:\t\t" << std::bitset<32>(*(unsigned*)(&t)).to_string() << std::endl;
                std::cout << "got:\t" << std::bitset<32>(*(unsigned*)(&func_ret)).to_string() << std::endl;
                std::cout << "expect:\t" << std::bitset<32>(*(unsigned*)(&test_ret)).to_string() << std::endl;
                return false;
            }
        } else if (tag == UI) {
            auto func_ret = ui(func)(t);
            auto test_ret = ui(test)(t);
            if (func_ret != test_ret) {
                printf("failed %d\n", t);
                printf("got %u, expected %u\n", func_ret, test_ret);
                std::cout << "t:\t\t" << std::bitset<32>(*(unsigned*)(&t)).to_string() << std::endl;
                std::cout << "got:\t" << std::bitset<32>(*(unsigned*)(&func_ret)).to_string() << std::endl;
                std::cout << "expect:\t" << std::bitset<32>(*(unsigned*)(&test_ret)).to_string() << std::endl;
                return false;
            }
        }
        return true;
    }
};

unsigned gen() {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static constexpr unsigned pretest[] = {0, 1, 7, 0xFF, 1<<22, 1<<23, 0x40000000, 1u<<31, ~(1u<<31), INT32_MAX, UINT32_MAX, UINT32_MAX-1, 1<<30, (1<<30)-1};
    static constexpr std::size_t n = std::extent<decltype(pretest)>::value;
    
    unsigned ret = gen();
    if (ret % 127 == 0) {
        return pretest[ret % n];
    }
    return ret;
}

int main() {
    
    FuncHolder test_objs[] = {
            FuncHolder::from(bitAnd, bitAndTest),
            FuncHolder::from(getByte, getByteTest, 4),
            FuncHolder::from(logicalShift, logicalShiftTest, 32),
            FuncHolder::from(bitCount, bitCountTest),
            FuncHolder::from(bang, bangTest),
            FuncHolder::from(fitsBits, fitsBitsTest, 33),
            FuncHolder::from(divpwr2, divpwr2Test, 31),
            FuncHolder::from(negate, negateTest),
            FuncHolder::from(isPositive, isPositiveTest),
            FuncHolder::from(isLessOrEqual, isLessOrEqualTest),
            FuncHolder::from(ilog2, ilog2Test),
            FuncHolder::from(float_neg, float_neg_test),
            FuncHolder::from(float_i2f, float_i2f_test),
            FuncHolder::from(float_twice, float_twice_test),
    };
    
    auto N = std::extent<decltype(test_objs)>::value;
    
    
    for (int i = 0; i < N; ++i) {
        const auto& test_obj = test_objs[i];
        bool succeed = true;
        int fail_num = 0;
        for (int j = 0; j < 10000000; ++j) {
            auto x = gen();
            auto y = gen();
            if (!test_obj(*(int*)(&x), *(int*)(&y))) {
                printf("%d-th function, %d-th test failed\n", i, j);
                printf("=================================\n");
                succeed = false;
                ++fail_num;
                if (fail_num >= 10) {
                    break;
                } else {
                    continue;
                }
            }
        }
        if (succeed) {
            printf("%d-th function succeed\n", i);
            printf("======================\n");
            std::cout.flush();
        }
    }
    printf("\ntest end\n");
}
