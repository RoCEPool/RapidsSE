/*============================================================================*/
/* BinaryNumbers.h -- useful binary numbers                                   */
/*============================================================================*/

#pragma once

// the 1024^n magnitudes
const i8 C_1KB = 1024LL;          // 1 kilobyte    = 1,024
const i8 C_1MB = C_1KB * C_1KB;   // 1 megabyte    = 1,048,576
const i8 C_1GB = C_1MB * C_1KB;   // 1 gigabyte    = 1,073,741,824
const i8 C_1TB = C_1GB * C_1KB;   // 1 terabyte    = 1,099,511,627,776
const i8 C_1PB = C_1TB * C_1KB;   // 1 petabyte    = 1,125,899,906,842,624
const i8 C_1EB = C_1PB * C_1KB;   // 1 exabyte     = 1,152,921,504,606,846,976

const i8 C_2KB    = C_1KB * 2;    // 2 kilobytes   = 2,048
const i8 C_4KB    = C_1KB * 4;    // 4 kilobytes   = 4,096
const i8 C_8KB    = C_1KB * 8;    // 8 kilobytes   = 8,192
const i8 C_16KB   = C_1KB * 16;   // 16 kilobytes  = 16,384
const i8 C_17KB   = C_1KB * 17;   // 17 kilobytes  = 17,408
const i8 C_32KB   = C_1KB * 32;   // 32 kilobytes  = 32,768
const i8 C_48KB   = C_1KB * 48;   // 48 kilobytes  = 49,152
const i8 C_64KB   = C_1KB * 64;   // 64 kilobytes  = 65,536
const i8 C_96KB   = C_1KB * 96;   // 96 kilobytes  = 98,304
const i8 C_128KB  = C_1KB * 128;  // 128 kilobytes = 131,072
const i8 C_256KB  = C_1KB * 256;  // 256 kilobytes = 262,144
const i8 C_512KB  = C_1KB * 512;  // 512 kilobytes = 524,288

// the 1000^n magnitudes
const i8 C_1K   = 1000LL;         // 1 thousand    = 1,000
const i8 C_1M   = C_1K * C_1K;    // 1 million     = 1,000,000
const i8 C_1G   = C_1M * C_1K;    // 1 billion     = 1,000,000,000
const i8 C_1T   = C_1G * C_1K;    // 1 trillion    = 1,000,000,000,000
const i8 C_1P   = C_1T * C_1K;    // 1 quadrillion = 1,000,000,000,000,000

const i8 C_2K   = C_1K * 2;       // 2 thousand    = 2,000
const i8 C_4K   = C_1K * 4;       // 4 thousand    = 4,000
const i8 C_8K   = C_1K * 8;       // 8 thousand    = 8,000
const i8 C_16K  = C_1K * 16;      // 16 thousand   = 16,000
const i8 C_32K  = C_1K * 32;      // 32 thousand   = 32,000
const i8 C_48K  = C_1K * 48;      // 48 thousand   = 48,000
const i8 C_64K  = C_1K * 64;      // 64 thousand   = 64,000
const i8 C_128K = C_1K * 128;     // 128 thousand  = 128,000
const i8 C_256K = C_1K * 256;     // 256 thousand  = 256,000
const i8 C_512K = C_1K * 512;     // 512 thousand  = 512,000

const u8 C_ALL_BITS_ON = 0XFFFFFFFFFFFFFFFFLL;
const u8 C_BITS_PER_WORD = 64;

const u1 C_1_BIT  = 1;
const u1 C_2_BITS = 3;
const u1 C_3_BITS = 7;
const u1 C_4_BITS = 15;
const u1 C_5_BITS = 31;
const u1 C_6_BITS = 63;
const u1 C_7_BITS = 127;
const u1 C_8_BITS = 255;


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
