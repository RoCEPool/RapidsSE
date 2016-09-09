/*============================================================================*/
/* Magnitudes.h -- Various Useful Magnitude Values                            */
/*============================================================================*/
#pragma once

#include "Magic.h"

namespace Magic { }

using namespace Magic;

// the KB, MB,... 1024^n magnitudes
const i8 C_1KB = 1024LL;          // 1 kilobyte = 1,024
const i8 C_1MB = C_1KB * C_1KB;   // 1 megabyte = 1,048,576
const i8 C_1GB = C_1MB * C_1KB;   // 1 gigabyte = 1,073,741,824
const i8 C_1TB = C_1GB * C_1KB;   // 1 terabyte = 1,099,511,627,776
const i8 C_1PB = C_1TB * C_1KB;   // 1 petabyte = 1,125,899,906,842,624
const i8 C_1EB = C_1PB * C_1KB;   // 1 exabyte  = 1,152,921,504,606,846,976

const i8 C_2KB    = C_1KB * 2;    // 2 kilobytes
const i8 C_4KB    = C_1KB * 4;    // 4 kilobytes
const i8 C_8KB    = C_1KB * 8;    // 8 kilobytes
const i8 C_16KB   = C_1KB * 16;   // 16 kilobytes
const i8 C_32KB   = C_1KB * 32;   // 32 kilobytes
const i8 C_64KB   = C_1KB * 64;   // 64 kilobytes
const i8 C_128KB  = C_1KB * 128;  // 128 kilobytes
const i8 C_256KB  = C_1KB * 256;  // 256 kilobytes
const i8 C_512KB  = C_1KB * 512;  // 512 kilobytes
const i8 C_1008KB = C_1KB * 1008; // 1008 kilobytes (1MB - 16KB)

// the 1000^n magnitudes
const i8 C_1K  = 1000LL;          // 1 thousand
const i8 C_1M  = C_1K * C_1K;     // 1 million
const i8 C_1Bn = C_1M * C_1K;     // 1 billion

class Magnitudes {
public:
  static void vMagnitude(i8 iValue, char* psBuf);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/