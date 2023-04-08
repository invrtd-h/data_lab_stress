#include "data_lab.hpp"
#include <random>
#include <cstdio>

int bitAndTest(int x, int y) {
    return x & y;
}

int getByteTest(int x, int n) {
    return int(*((unsigned char*)(&x)+n));
}

int isLessOrEqualTest(int x, int y) {
    return x <= y;
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
            u = u < 0 ? -u : u;
            u %= ubd;
        }
        if (tag == II) {
            auto func_ret = ii(func)(t);
            auto test_ret = ii(test)(t);
            if (func_ret != test_ret) {
                printf("failed %d\n", t);
                printf("got %d, expected %d\n", func_ret, test_ret);
                return false;
            }
        } else if (tag == III) {
            auto func_ret = iii(func)(t, u);
            auto test_ret = iii(test)(t, u);
            if (func_ret != test_ret) {
                printf("failed %d %d\n", t, u);
                printf("got %d, expected %d\n", func_ret, test_ret);
                return false;
            }
        } else if (tag == UU) {
            auto func_ret = uu(func)(*(unsigned*)(&t));
            auto test_ret = uu(test)(*(unsigned*)(&t));
            if (func_ret != test_ret) {
                printf("failed %u\n", t);
                printf("got %d, expected %d\n", func_ret, test_ret);
                return false;
            }
        } else if (tag == UI) {
            auto func_ret = ui(func)(t);
            auto test_ret = ui(test)(t);
            if (func_ret != test_ret) {
                printf("failed %d\n", t);
                printf("got %d, expected %d\n", func_ret, test_ret);
                return false;
            }
        }
        return true;
    }
};

int main() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    
    FuncHolder test_objs[] = {
            FuncHolder::from(bitAnd, bitAndTest),
            FuncHolder::from(getByte, getByteTest, 4),
            FuncHolder::from(isLessOrEqual, isLessOrEqualTest),
    };
    
    auto N = std::extent<decltype(test_objs)>::value;
    for (int i = 0; i < N; ++i) {
        const auto& test_obj = test_objs[i];
        bool succeed = true;
        for (int j = 0; j < 10000000; ++j) {
            auto x = gen();
            auto y = gen();
            if (!test_obj(*(int*)(&x), *(int*)(&y))) {
                printf("%d-th function, %d-th test failed\n", i, j);
                succeed = false;
                break;
            }
        }
        if (succeed) {
            printf("%d-th function succeed\n", i);
        }
    }
    printf("\ntest end\n");
}
