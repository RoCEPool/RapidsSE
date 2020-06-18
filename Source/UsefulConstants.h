/*============================================================================*/
/* UsefulConstants.h -- Some Useful Constants                                 */
/*============================================================================*/
#pragma once

using namespace Base;

static const i8 C_0B = 0;
static const i8 C_1B = 1;
static const i8 C_2B = 2;
static const i8 C_3B = 3;
static const i8 C_4B = 4;
static const i8 C_5B = 5;
static const i8 C_6B = 6;
static const i8 C_7B = 7;
static const i8 C_8B = 8;

static const i8 C_01_JAN = 1;
static const i8 C_02_FEB = 2;
static const i8 C_03_MAR = 3;
static const i8 C_04_APR = 4;
static const i8 C_05_MAY = 5;
static const i8 C_06_JUN = 6;
static const i8 C_07_JUL = 7;
static const i8 C_08_AUG = 8;
static const i8 C_09_SEP = 9;
static const i8 C_10_OCT = 10;
static const i8 C_11_NOV = 11;
static const i8 C_12_DEC = 12;

static const i8 C_TSP_LOCAL = 1;
static const i8 C_TSP_UTC   = 0;

static const i8 C_SECS_PER_MIN = 60;
static const i8 C_MINS_PER_HR   = 60;
static const i8 C_HRS_PER_DAY   = 24;

static const i8 C_SECS_PER_HR  = C_SECS_PER_MIN * C_MINS_PER_HR;
static const i8 C_SECS_PER_DAY = C_SECS_PER_HR  * C_HRS_PER_DAY;

// the 1024^n MagLib
static const i8 C_1KB = 1024LL;          // 1 kilobyte    = 1,024
static const i8 C_1MB = C_1KB * C_1KB;   // 1 megabyte    = 1,048,576
static const i8 C_1GB = C_1MB * C_1KB;   // 1 gigabyte    = 1,073,741,824
static const i8 C_1TB = C_1GB * C_1KB;   // 1 terabyte    = 1,099,511,627,776
static const i8 C_1PB = C_1TB * C_1KB;   // 1 petabyte    = 1,125,899,906,842,624
static const i8 C_1EB = C_1PB * C_1KB;   // 1 exabyte     = 1,152,921,504,606,846,976

static const i8 C_2KB    = C_1KB * 2;    // 2 kilobytes   = 2,048
static const i8 C_4KB    = C_1KB * 4;    // 4 kilobytes   = 4,096
static const i8 C_8KB    = C_1KB * 8;    // 8 kilobytes   = 8,192
static const i8 C_10KB   = C_1KB * 10;   // 10 kilobytes  = 10,240
static const i8 C_12KB   = C_1KB * 12;   // 10 kilobytes  = 12,288
static const i8 C_16KB   = C_1KB * 16;   // 16 kilobytes  = 16,384
static const i8 C_24KB   = C_1KB * 24;   // 24 kilobytes  = 24,576
static const i8 C_32KB   = C_1KB * 32;   // 32 kilobytes  = 32,768
static const i8 C_48KB   = C_1KB * 48;   // 48 kilobytes  = 49,152
static const i8 C_64KB   = C_1KB * 64;   // 64 kilobytes  = 65,536
static const i8 C_80KB   = C_1KB * 80;   // 80 kilobytes  = 81,920
static const i8 C_96KB   = C_1KB * 96;   // 96 kilobytes  = 98,304
static const i8 C_128KB  = C_1KB * 128;  // 128 kilobytes = 131,072
static const i8 C_256KB  = C_1KB * 256;  // 256 kilobytes = 262,144
static const i8 C_512KB  = C_1KB * 512;  // 512 kilobytes = 524,288

static const i8 C_2MB    = C_1MB * 2;    // 2 megabytes   =
static const i8 C_4MB    = C_1MB * 4;    // 4 megabytes   =
static const i8 C_8MB    = C_1MB * 8;    // 8 megabytes   =
static const i8 C_10MB   = C_1MB * 10;   // 10 megabytes  =
static const i8 C_16MB   = C_1MB * 16;   // 16 megabytes  =
static const i8 C_32MB   = C_1MB * 32;   // 32 megabytes  =
static const i8 C_48MB   = C_1MB * 48;   // 48 megabytes  =
static const i8 C_64MB   = C_1MB * 64;   // 64 megabytes  =
static const i8 C_96MB   = C_1MB * 96;   // 96 megabytes  =
static const i8 C_128MB  = C_1MB * 128;  // 128 megabytes =
static const i8 C_256MB  = C_1MB * 256;  // 256 megabytes =
static const i8 C_512MB  = C_1MB * 512;  // 512 megabytes =

static const i8 C_2GB    = C_1GB * 2;    // 2 gigabytes   =
static const i8 C_4GB    = C_1GB * 4;    // 4 gigabytes   =
static const i8 C_8GB    = C_1GB * 8;    // 8 gigabytes   =
static const i8 C_10GB   = C_1GB * 10;   // 10 gigabytes  =
static const i8 C_16GB   = C_1GB * 16;   // 16 gigabytes  =
static const i8 C_32GB   = C_1GB * 32;   // 32 gigabytes  =
static const i8 C_48GB   = C_1GB * 48;   // 48 gigabytes  =
static const i8 C_64GB   = C_1GB * 64;   // 64 gigabytes  =
static const i8 C_96GB   = C_1GB * 96;   // 96 gigabytes  =
static const i8 C_128GB  = C_1GB * 128;  // 128 gigabytes =
static const i8 C_256GB  = C_1GB * 256;  // 256 gigabytes =
static const i8 C_512GB  = C_1GB * 512;  // 512 gigabytes =

static const i8 C_2TB    = C_1TB * 2;    // 2 terabytes   =
static const i8 C_4TB    = C_1TB * 4;    // 4 terabytes   =
static const i8 C_8TB    = C_1TB * 8;    // 8 terabytes   =
static const i8 C_10TB   = C_1TB * 10;   // 10 terabytes  =
static const i8 C_16TB   = C_1TB * 16;   // 16 terabytes  =
static const i8 C_32TB   = C_1TB * 32;   // 32 terabytes  =
static const i8 C_48TB   = C_1TB * 48;   // 48 terabytes  =
static const i8 C_64TB   = C_1TB * 64;   // 64 terabytes  =
static const i8 C_96TB   = C_1TB * 96;   // 96 terabytes  =
static const i8 C_128TB  = C_1TB * 128;  // 128 terabytes =
static const i8 C_256TB  = C_1TB * 256;  // 256 terabytes =
static const i8 C_512TB  = C_1TB * 512;  // 512 terabytes =

static const i8 C_2PB    = C_1PB * 2;    // 2 petabytes   =
static const i8 C_4PB    = C_1PB * 4;    // 4 petabytes   =
static const i8 C_8PB    = C_1PB * 8;    // 8 petabytes   =
static const i8 C_10PB   = C_1PB * 10;   // 10 petabytes  =
static const i8 C_16PB   = C_1PB * 16;   // 16 petabytes  =
static const i8 C_32PB   = C_1PB * 32;   // 32 petabytes  =
static const i8 C_48PB   = C_1PB * 48;   // 48 petabytes  =
static const i8 C_64PB   = C_1PB * 64;   // 64 petabytes  =
static const i8 C_96PB   = C_1PB * 96;   // 96 petabytes  =
static const i8 C_128PB  = C_1PB * 128;  // 128 petabytes =
static const i8 C_256PB  = C_1PB * 256;  // 256 petabytes =
static const i8 C_512PB  = C_1PB * 512;  // 512 petabytes =

// the 10^n MagLib
static const i8 C_1K   = 1000LL;         // 1 thousand    = 1,000
static const i8 C_10K  = C_1K   * 10;    // 10 thousand   = 10,000
static const i8 C_100K = C_10K  * 10;    // 100 thousand  = 100,000
static const i8 C_1M   = C_100K * 10;    // 1 million     = 1,000,000
static const i8 C_10M  = C_1M   * 10;    // 10 million    = 10,000,000
static const i8 C_100M = C_10M  * 10;    // 100 million   = 100,000,000
static const i8 C_1G   = C_100M * 10;    // 1 billion     = 1,000,000,000
static const i8 C_10G  = C_1G   * 10;    // 10 billion    = 10,000,000,000
static const i8 C_100G = C_10G  * 10;    // 100 billion   = 100,000,000,000
static const i8 C_1T   = C_100G * 10;    // 1 trillion    = 1,000,000,000,000
static const i8 C_10T  = C_1T   * 10;    // 10 trillion   = 10,000,000,000,000
static const i8 C_100T = C_10T  * 10;    // 100 trillion  = 100,000,000,000,000
static const i8 C_1P   = C_100T * 10;    // 1 quadrillion = 1,000,000,000,000,000
static const i8 C_10P  = C_1P   * 10;    // 10 quadrillion = 10,000,000,000,000,000
static const i8 C_100P = C_10P  * 10;    // 100 quadrillion = 100,000,000,000,000,000
static const i8 C_1E   = C_100P * 10;    // 1 quintillion   = 1,000,000,000,000,000,000

static const i8 C_2K   = C_1K * 2;       // 2 thousand    = 2,000
static const i8 C_4K   = C_1K * 4;       // 4 thousand    = 4,000
static const i8 C_8K   = C_1K * 8;       // 8 thousand    = 8,000
static const i8 C_16K  = C_1K * 16;      // 16 thousand   = 16,000
static const i8 C_32K  = C_1K * 32;      // 32 thousand   = 32,000
static const i8 C_48K  = C_1K * 48;      // 48 thousand   = 48,000
static const i8 C_64K  = C_1K * 64;      // 64 thousand   = 64,000
static const i8 C_96K  = C_1K * 96;      // 64 thousand   = 64,000
static const i8 C_128K = C_1K * 128;     // 128 thousand  = 128,000
static const i8 C_256K = C_1K * 256;     // 256 thousand  = 256,000
static const i8 C_512K = C_1K * 512;     // 512 thousand  = 512,000

// refer: https://en.wikipedia.org/wiki/List_of_prime_numbers#The_first_1000_prime_numbers
static const i8 C_PRIME_1  = 1;
static const i8 C_PRIME_2  = 2;
static const i8 C_PRIME_3  = 3;
static const i8 C_PRIME_4  = 5;
static const i8 C_PRIME_5  = 7;
static const i8 C_PRIME_6  = 11;
static const i8 C_PRIME_7  = 13;
static const i8 C_PRIME_8  = 17;
static const i8 C_PRIME_9  = 19;
static const i8 C_PRIME_10 = 23;

static const i8 C_PRIME_11 = 29;
static const i8 C_PRIME_12 = 31;
static const i8 C_PRIME_13 = 37;
static const i8 C_PRIME_14 = 41;
static const i8 C_PRIME_15 = 43;
static const i8 C_PRIME_16 = 47;
static const i8 C_PRIME_17 = 53;
static const i8 C_PRIME_18 = 59;
static const i8 C_PRIME_19 = 61;
static const i8 C_PRIME_20 = 67;

static const i8 C_PRIME_21 = 71;
static const i8 C_PRIME_22 = 73;
static const i8 C_PRIME_23 = 79;
static const i8 C_PRIME_24 = 83;
static const i8 C_PRIME_25 = 89;
static const i8 C_PRIME_26 = 97;
static const i8 C_PRIME_27 = 101;
static const i8 C_PRIME_28 = 103;
static const i8 C_PRIME_29 = 107;
static const i8 C_PRIME_30 = 109;

static const i8 C_PRIME_31 = 113;
static const i8 C_PRIME_32 = 127;
static const i8 C_PRIME_33 = 131;
static const i8 C_PRIME_34 = 137;
static const i8 C_PRIME_35 = 139;
static const i8 C_PRIME_36 = 149;
static const i8 C_PRIME_37 = 151;
static const i8 C_PRIME_38 = 157;
static const i8 C_PRIME_39 = 163;
static const i8 C_PRIME_40 = 167;

static const i8 C_PRIME_41 = 173;
static const i8 C_PRIME_42 = 179;
static const i8 C_PRIME_43 = 181;
static const i8 C_PRIME_44 = 191;
static const i8 C_PRIME_45 = 193;
static const i8 C_PRIME_46 = 197;
static const i8 C_PRIME_47 = 199;
static const i8 C_PRIME_48 = 211;
static const i8 C_PRIME_49 = 223;
static const i8 C_PRIME_50 = 227;

static const i8 C_PRIME_51 = 229;
static const i8 C_PRIME_52 = 233;
static const i8 C_PRIME_53 = 239;
static const i8 C_PRIME_54 = 241;
static const i8 C_PRIME_55 = 251;
static const i8 C_PRIME_56 = 257;
static const i8 C_PRIME_57 = 263;
static const i8 C_PRIME_58 = 269;
static const i8 C_PRIME_59 = 271;
static const i8 C_PRIME_60 = 277;

static const i8 C_PRIME_61 = 281;
static const i8 C_PRIME_62 = 283;
static const i8 C_PRIME_63 = 293;
static const i8 C_PRIME_64 = 307;
static const i8 C_PRIME_65 = 311;
static const i8 C_PRIME_66 = 313;
static const i8 C_PRIME_67 = 317;
static const i8 C_PRIME_68 = 331;
static const i8 C_PRIME_69 = 337;
static const i8 C_PRIME_70 = 347;

static const i8 C_PRIME_71 = 349;
static const i8 C_PRIME_72 = 353;
static const i8 C_PRIME_73 = 359;
static const i8 C_PRIME_74 = 367;
static const i8 C_PRIME_75 = 373;
static const i8 C_PRIME_76 = 379;
static const i8 C_PRIME_77 = 383;
static const i8 C_PRIME_78 = 389;
static const i8 C_PRIME_79 = 397;
static const i8 C_PRIME_80 = 401;

static const i8 C_PRIME_81 = 409;
static const i8 C_PRIME_82 = 419;
static const i8 C_PRIME_83 = 421;
static const i8 C_PRIME_84 = 431;
static const i8 C_PRIME_85 = 433;
static const i8 C_PRIME_86 = 439;
static const i8 C_PRIME_87 = 443;
static const i8 C_PRIME_88 = 449;
static const i8 C_PRIME_89 = 457;
static const i8 C_PRIME_90 = 461;

static const i8 C_PRIME_91 = 463;
static const i8 C_PRIME_92 = 467;
static const i8 C_PRIME_93 = 479;
static const i8 C_PRIME_94 = 487;
static const i8 C_PRIME_95 = 491;
static const i8 C_PRIME_96 = 499;
static const i8 C_PRIME_97 = 503;
static const i8 C_PRIME_98 = 509;
static const i8 C_PRIME_99 = 521;
static const i8 C_PRIME_100 = 523;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
