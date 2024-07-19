#include <stdio.h>
#include "softfloat.h"

#define INF (1.f / 0.f)
#define NAN (0.f / 0.f)

static void test_add(float f1, float f2)
{
    float f = f1 + f2;
    float32_t sf1 = sf32_from_float(f1);
    float32_t sf2 = sf32_from_float(f2);
    float32_t sf = sf32_add(sf1, sf2);
    printf("%.10e + %.10e = [hard]%.10e, [soft]%.10e\n", f1, f2, f, sf32_to_float(sf));
}

int main() {
    test_add(1.1, 2.2);
    test_add(3e38, 2e38);
    test_add(1.1, 0);
    test_add(2e20, 2);
    test_add(2, INF);
    test_add(INF, 100);
    test_add(NAN, NAN);
    test_add(NAN, INF);
    test_add(INF, NAN);
    test_add(NAN, 2);
    test_add(100, NAN);
    printf("\n");
    return 0;
}