#if defined(NUM)
    #define ITERATIONS NUM
#else
    #define ITERATIONS 1000
#endif
#include <iostream>
#include <stdio.h>

using namespace std;

struct DV { int64_t a; int64_t b; };

int64_t dsum(int64_t c)
{
    DV** arr = (DV**)malloc(ITERATIONS * sizeof(DV*));
    for(int64_t i = 0; i < c; ++i) {
        arr[i] = (DV*)malloc(sizeof(DV));
        arr[i]->a = i;
        arr[i]->b = i;
    }

    int64_t res = 0;
    for(int64_t i = 0; i < c; ++i) {
        res += arr[i]->a + arr[i]->b;
    }

    for(int64_t i = 0; i < c; ++i) {
        free(arr[i]);
    }
    free(arr);

    return res;
}

int main() {
    cout << dsum(ITERATIONS) << endl;
}
