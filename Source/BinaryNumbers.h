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
const i8 C_10KB   = C_1KB * 10;   // 10 kilobytes  = 10,240
const i8 C_16KB   = C_1KB * 16;   // 16 kilobytes  = 16,384
const i8 C_17KB   = C_1KB * 17;   // 17 kilobytes  = 17,408
const i8 C_32KB   = C_1KB * 32;   // 32 kilobytes  = 32,768
const i8 C_48KB   = C_1KB * 48;   // 48 kilobytes  = 49,152
const i8 C_64KB   = C_1KB * 64;   // 64 kilobytes  = 65,536
const i8 C_96KB   = C_1KB * 96;   // 96 kilobytes  = 98,304
const i8 C_128KB  = C_1KB * 128;  // 128 kilobytes = 131,072
const i8 C_256KB  = C_1KB * 256;  // 256 kilobytes = 262,144
const i8 C_512KB  = C_1KB * 512;  // 512 kilobytes = 524,288

const i8 C_2MB    = C_1MB * 2;    // 2 megabytes   =
const i8 C_4MB    = C_1MB * 4;    // 4 megabytes   =
const i8 C_8MB    = C_1MB * 8;    // 8 megabytes   =
const i8 C_10MB   = C_1MB * 10;   // 10 megabytes  =
const i8 C_16MB   = C_1MB * 16;   // 16 megabytes  =
const i8 C_32MB   = C_1MB * 32;   // 32 megabytes  =
const i8 C_48MB   = C_1MB * 48;   // 48 megabytes  =
const i8 C_64MB   = C_1MB * 64;   // 64 megabytes  =
const i8 C_96MB   = C_1MB * 96;   // 96 megabytes  =
const i8 C_128MB  = C_1MB * 128;  // 128 megabytes =
const i8 C_256MB  = C_1MB * 256;  // 256 megabytes =
const i8 C_512MB  = C_1MB * 512;  // 512 megabytes =

const i8 C_2GB    = C_1GB * 2;    // 2 gigabytes   =
const i8 C_4GB    = C_1GB * 4;    // 4 gigabytes   =
const i8 C_8GB    = C_1GB * 8;    // 8 gigabytes   =
const i8 C_10GB   = C_1GB * 10;   // 10 gigabytes  =
const i8 C_16GB   = C_1GB * 16;   // 16 gigabytes  =
const i8 C_32GB   = C_1GB * 32;   // 32 gigabytes  =
const i8 C_48GB   = C_1GB * 48;   // 48 gigabytes  =
const i8 C_64GB   = C_1GB * 64;   // 64 gigabytes  =
const i8 C_96GB   = C_1GB * 96;   // 96 gigabytes  =
const i8 C_128GB  = C_1GB * 128;  // 128 gigabytes =
const i8 C_256GB  = C_1GB * 256;  // 256 gigabytes =
const i8 C_512GB  = C_1GB * 512;  // 512 gigabytes =

const i8 C_2TB    = C_1TB * 2;    // 2 terabytes   =
const i8 C_4TB    = C_1TB * 4;    // 4 terabytes   =
const i8 C_8TB    = C_1TB * 8;    // 8 terabytes   =
const i8 C_10TB   = C_1TB * 10;   // 10 terabytes  =
const i8 C_16TB   = C_1TB * 16;   // 16 terabytes  =
const i8 C_32TB   = C_1TB * 32;   // 32 terabytes  =
const i8 C_48TB   = C_1TB * 48;   // 48 terabytes  =
const i8 C_64TB   = C_1TB * 64;   // 64 terabytes  =
const i8 C_96TB   = C_1TB * 96;   // 96 terabytes  =
const i8 C_128TB  = C_1TB * 128;  // 128 terabytes =
const i8 C_256TB  = C_1TB * 256;  // 256 terabytes =
const i8 C_512TB  = C_1TB * 512;  // 512 terabytes =

const i8 C_2PB    = C_1PB * 2;    // 2 petabytes   =
const i8 C_4PB    = C_1PB * 4;    // 4 petabytes   =
const i8 C_8PB    = C_1PB * 8;    // 8 petabytes   =
const i8 C_10PB   = C_1PB * 10;   // 10 petabytes  =
const i8 C_16PB   = C_1PB * 16;   // 16 petabytes  =
const i8 C_32PB   = C_1PB * 32;   // 32 petabytes  =
const i8 C_48PB   = C_1PB * 48;   // 48 petabytes  =
const i8 C_64PB   = C_1PB * 64;   // 64 petabytes  =
const i8 C_96PB   = C_1PB * 96;   // 96 petabytes  =
const i8 C_128PB  = C_1PB * 128;  // 128 petabytes =
const i8 C_256PB  = C_1PB * 256;  // 256 petabytes =
const i8 C_512PB  = C_1PB * 512;  // 512 petabytes =

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
