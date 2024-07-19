#include "softfloat.h"

bool sf_isINF32UI(uint_fast32_t sf){
    return (sf&0x7FC00000 ==0x7F800000) & (sf&0x003FFFFF);
}

uint_fast32_t propagateNaN(uint_fast32_t sf1, uint_fast32_t sf2, bool sign) {
    // 如果有一个是INF(无穷大)
    if (sf_isINF32UI( sf1 ) || sf_isINF32UI( sf2 ) ) {
        return (defaultINF32UI | ((uint32_t)sign << 31));
    }
    return defaultNaNF32UI;
}

/* 构造一个32位浮点数 */
float32_t sf32_pack(bool sign, int_fast16_t exp, uint_fast32_t sig)
{
    union ui32_f32 uC;
    uC.ui = packToF32UI(sign, exp, sig);
    return uC.f;
}

float32_t sf32_from_float(float f) {
    union
    {
        float f;
        float32_t u;
    } usf;
    usf.f = f;
    return usf.u;
}

float sf32_to_float(float32_t sf) {
    union
    {
        float f;
        float32_t u;
    } usf;
    usf.u = sf;
    return usf.f;
}

static float32_t impl_sf32_sub(uint_fast32_t sf1, uint_fast32_t sf2) {}

/**加法操作流程: 
 * 1. 获取符号位，指数位，尾数位
 * 2. 判断溢出，规格化/非规格化
 * 3. 对阶
 * 4. 对尾数做加法，进行舍入
 * 5. 判断上溢
 * 6. pack符号位，指数位，尾数位**/
static float32_t impl_sf32_add(uint_fast32_t sf1, uint_fast32_t sf2) {
    bool sign;
    int_fast16_t exp1, exp2, expC;
    uint_fast32_t sig1, sig2, sigC;
    union ui32_f32 uC;

    exp1 = expF32UI(sf1);
    exp2 = expF32UI(sf2);
    sig1 = fracF32UI(sf1);
    sig2 = fracF32UI(sf2);
    sign = signF32UI(sf1);
    
    /*阶差*/
    int_fast16_t expdiff = exp1 - exp2;
    if(!expdiff) { // 指数位相同
        // 非规格化: 直接加尾数
        if(!exp1) {
            uC.ui = sf1 + sig2; 
            return uC.f;
        }
        // 指数全为1，nan or inf
        if(exp1 == 0xFF) {
            // 尾数位不全为0
            if ( sig1 | sig2 ) {
                uC.ui = propagateNaN(sf1, sf2, sign);
                return uC.f;
            }
            uC.ui = sf1;
            return uC.f;
        }
        // 规格化数相加
        expC = exp1;
        // 尾数相加，因为规格化的尾数前面隐藏了1（也就是第24位隐藏了1）。所以需要加上0x01000000
        // 为啥是0x01000000：因为尾数23位，补上第24位1（两个1相加需要向第25位进位，因此是0x01000000）
        sigC = 0x01000000 + sig1 + sig2;
        
        // 如果尾数是偶数（最低有效位为 0），并且指数还没有达到最大值（0xFE）
        if(!(sigC &1) && (expC < 0xFE)) {
            // 尾数右移的意义是为了规格化，但如果尾数相加溢出，似乎这里没有做特别的处理
            uC.ui = packToF32UI(sign, expC, sigC>>1);
            return uC.f;
        }
        sigC <<= 6;
    }

}

float32_t sf32_add(float32_t sf1, float32_t sf2) {
    union ui32_f32 uA;
    union ui32_f32 uB;
    uint_fast32_t uiA;
    uint_fast32_t uiB;

    uA.f = sf1;
    uiA = uA.ui;
    uB.f = sf2;
    uiB = uB.ui;

    /// (正 + 负) or (负 + 正) 两者符号位相同
    if(signF32UI(uiA ^ uiB)) {
        return impl_sf32_sub(uiA, uiB);
    }else {  /// (正 + 正) or (负 + 负) 两者符号位相同
        return impl_sf32_add(uiA, uiB);
    }

}