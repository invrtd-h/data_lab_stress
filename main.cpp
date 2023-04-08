#include "data_lab.hpp"
#include <random>
#include <cstdio>

int isLessOrEqualTest(int x, int y) {
    return x <= y;
}

struct FuncHolder {
    enum {II, III, UU, UI} tag;
    void *func, *test;
    
    using ii = int (*)(int);
    using iii = int (*)(int, int);
    using uu = unsigned (*)(unsigned);
    using ui = unsigned (*)(int);
    
    static FuncHolder from(ii f, ii g) {
        return FuncHolder{II, (void*)f, (void*)g};
    }
    static FuncHolder from(iii f, iii g) {
        return FuncHolder{III, (void*)f, (void*)g};
    }
    static FuncHolder from(uu f, uu g) {
        return FuncHolder{UU, (void*)f, (void*)g};
    }
    static FuncHolder from(ui f, ui g) {
        return FuncHolder{UI, (void*)f, (void*)g};
    }
    
    bool operator()(int t, int u = 0) const {
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
    
    auto test_obj = FuncHolder::from(isLessOrEqual, isLessOrEqualTest);
    
    for (int i = 0; i < 100000000; ++i) {
        auto x = gen();
        auto y = gen();
        if (!test_obj(*(int*)(&x), *(int*)(&y))) {
            printf("%d-th test failed\n", i);
            return 0;
        }
    }
    printf("test may succeed\n");
}
