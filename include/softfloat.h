
#pragma once

#include<stdint.h>
#include<stdbool.h>

#include "softfloat_type.h"

///////////////// common function ////////////////
uint_fast32_t propagateNaN(uint_fast32_t sf1, uint_fast32_t sf2, bool sign);
bool sf_isSigNaNF32UI(uint_fast32_t sf);

uint32_t sf_shiftRightJam32( uint32_t a, uint_fast16_t dist );

float32_t sf_roundPackToF32( bool sign, int_fast16_t exp, uint_fast32_t sig );

///////////////// float 32 //////////////////////

// 将浮点数转换到软浮点数
float32_t sf32_from_float(float f);
// 将软浮点数转换到浮点数
float sf32_to_float(float32_t sf);
//软浮点数加法
float32_t sf32_add(float32_t sf1, float32_t sf2);
//构造软浮点数
float32_t sf32_pack(bool sign, int_fast16_t exp, uint_fast32_t sig);