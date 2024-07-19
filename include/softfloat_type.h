
#ifndef softfloat_types_h
#define softfloat_types_h 1

#include <stdint.h>

typedef struct { uint16_t v; } float16_t;
typedef struct { uint32_t v; } float32_t;
typedef struct { uint64_t v; } float64_t;
typedef struct { uint64_t v[2]; } float128_t;

union ui16_f16 { uint16_t ui; float16_t f; };
union ui32_f32 { uint32_t ui; float32_t f; };
union ui64_f64 { uint64_t ui; float64_t f; };

#define signF32UI(a) ((bool)(uint32_t)(a)>>31)
#define expF32UI( a ) ((int_fast16_t) ((a)>>23) & 0xFF)
#define fracF32UI( a ) ((a) & 0x007FFFFF)
#define packToF32UI( sign, exp, sig ) (((uint32_t) (sign)<<31) + ((uint32_t) (exp)<<23) + (sig))

// The bit pattern for a default generated 32-bit floating-point NaN.
// 8086
#define defaultNaNF32UI 0xFFC00000
#define defaultINF32UI  0x7FC00000
// arm？
// #define defaultNaNF32UI 0x7FC00000

#endif

