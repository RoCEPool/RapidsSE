/*============================================================================*/
/* Constants.h - constants                                                    */
/*============================================================================*/
#pragma once

#include "Types.h"

namespace Types { }

using namespace Types;

// the KB, MB,... constants
const i8 C_1KB = 1024LL;         // 1 kilobyte = 1,024
const i8 C_1MB = C_1KB * C_1KB;  // 1 megabyte = 1,048,576
const i8 C_1GB = C_1MB * C_1KB;  // 1 gigabyte = 1,073,741,824
const i8 C_1TB = C_1GB * C_1KB;  // 1 terabyte = 1,099,511,627,776
const i8 C_1PB = C_1TB * C_1KB;  // 1 petabyte = 1,125,899,906,842,624
const i8 C_1EB = C_1PB * C_1KB;  // 1 exabyte  = 1,152,921,504,606,846,976

static const int C_NO_CASE    = 0;
static const int C_CASE       = 1;

const i8 C_1K  = 1000LL;         // 1 thousand
const i8 C_1M  = C_1K * C_1K;    // 1 million
const i8 C_1Bn = C_1M * C_1K;    // 1 billion

const i8 C_16KB = C_1KB * 16;    // 16 kilobytes

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/