/*============================================================================*/
/* vFLAT_Header.h -- Flattened function header                                */
/*============================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "../src/primitives/BaseTypes_general.h"
#include "../src/base/CodeSynthCommon.h"
#include "../src/base/ClangStuff.h"
#include "../src/base/ClangDynamic.h"

#define NEVER_REACH  { \
  printf("NEVER_REACH CODE point in synthesised code was reached at line %d !!!\n", __LINE__); \
  aDynamicT->m_iErrorCode = (i8)EC_INTERNAL_ERROR;                                             \
  return 0;                                                                                    \
}

typedef char i1;
typedef unsigned short u2;
typedef long long i8;
typedef unsigned char byte;
typedef double d8;
typedef unsigned long long m8;
typedef unsigned long long u8;
typedef char Stz;

typedef unsigned char eEO;
typedef unsigned short tET;

static const i8 C_1MB = 1048576;

static const void* C_ADR_REGION = (void*)0x204800000000;
static const i8 C_DATUM_NULL = -9223372036854775807;
static const i8 C_I8_MIN = -9223372036854775806;
static const i8 C_I8_MAX = 9223372036854775806;

// Need to include the constants from CommonTypes.h rather than duplicating here.
static const i8 C_BCD_MIN = -99999999999999999;
static const i8 C_BCD_MAX = 99999999999999999;
static const i8 C_MAX_BCD_PRECISION = 17;
static const i8 C_MIN_BCD_SCALE = 0;
static const i8 C_MAX_BCD_SCALE = 17; //C_MAX_BCD_PRECISION
static const i8 C_BCD_DEFAULT_SCALE = 0;
static const i8 C_MAX_I8_PRECISION = 19;
static const i8 C_MAX_I16_PRECISION = 38;

static const i8 C_CRW_HEADBYTES = 32;
static const i8 C_CRW_FIRSTHEAD = 640;
static const tET C_ENT_END_TICKET = 0xFFFF;
static const d8 C_FLT_MAX =  1.797693134862315708145274237317043567981e+308;
static const d8 C_FLT_MIN = -1.797693134862315708145274237317043567981e+308;
static const u8 C_YEAR_BASE  = 1900;
static const u8 C_YEAR_RANGE = 255;
static const u8 C_YEAR_MAX   = 2155;

static i8 iDaysPerMonth[13]     = { -1,31,28,31,30,31,30,31,31,30,31,30,31};
static i8 iDaysPerMonthLeap[13] = { -1,31,29,31,30,31,30,31,31,30,31,30,31};
static const i8 C_SECS_PER_DAY = 86400;

#define C_BITS_PER_WORD 64
#define C_MAX_LVL_LEVELS 6
#define C_LOGIC_LOCALS 32
#define C_UNKNOWN -1

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Get precision of an unsigned 64 bit integer                                */
/*----------------------------------------------------------------------------*/
i8 iPrecision(u8 uArg) {

  // Refer to IntLib::iPrecision() for details and comments on this.

  if (uArg == 0) {
    return 1;
  }

  // Powers of 10 for a 64 bit unsigned integer range
  static const u8 uaTenToThe[] = {
      1ull,
      10ull,
      100ull,
      1000ull,
      10000ull,
      100000ull,
      1000000ull,
      10000000ull,
      100000000ull,
      1000000000ull,
      10000000000ull,
      100000000000ull,
      1000000000000ull,
      10000000000000ull,
      100000000000000ull,
      1000000000000000ull,
      10000000000000000ull,
      100000000000000000ull,
      1000000000000000000ull,
      10000000000000000000ull,
  };

  // The lookup table of (guessed) precision given the number of base 2 digits
  // (as the index in to the array).
  static const i8 iaGuess[65] = {
      0 ,0 , 0 ,  0 ,  1 ,1 ,1 ,   2 ,2 ,2 ,
      3 ,3 ,3 , 3 , 4 ,4 ,4 , 5 ,5 ,5 ,
      6 ,6 ,6 ,6 , 7 ,7 ,7 , 8 ,8 ,8 ,
      9 ,9 ,9 ,9 , 10,10,10, 11,11,11,
      12,12,12,12, 13,13,13, 14,14,14,
      15,15,15,15, 16,16,16, 17,17,17,
      18,18,18,18, 19
  };
  
  i8 iBase2Digits = 64 - __builtin_clzll(uArg);
  i8 iDigits = iaGuess[iBase2Digits];
  i8 iPrec = iDigits + (uArg >= uaTenToThe[iDigits]);

  return iPrec;
}

/*----------------------------------------------------------------------------*/
/* Get precision of a signed 64 bit integer                                   */
/*----------------------------------------------------------------------------*/
i8 iPrecision_signed(i8 iArg)
{
  if (iArg < 0) {
    iArg = -iArg;
  }

  i8 iPrec = iPrecision((u8) iArg);
  return iPrec;
}

/*----------------------------------------------------------------------------*/
/* Get precision of an unsigned 128 bit integer                               */
/*----------------------------------------------------------------------------*/
i8 iPrecision128(u16 uArg, struct Dynamic_T* pDynamic) {

  // Refer to IntLib::iPrecision() for details and comments on this.

  if (uArg == 0) {
    return 1;
  }


  // The lookup table of (guessed) precision given the number of base 2 digits
  // (as the index in to the array).
  static const i8 iaGuess[129] = {
      0 ,0 ,0 ,0 , 1 ,1 ,1 , 2 ,2 ,2 ,
      3 ,3 ,3 ,3 , 4 ,4 ,4 , 5 ,5 ,5 ,
      6 ,6 ,6 ,6 , 7 ,7 ,7 , 8 ,8 ,8 ,
      9 ,9 ,9 ,9 , 10,10,10, 11,11,11,
      12,12,12,12, 13,13,13, 14,14,14,
      15,15,15,15, 16,16,16, 17,17,17,
      18,18,18,18, 19,19,19, 20,20,20,
      21,21,21,21, 22,22,22, 23,23,23,
      24,24,24,24, 25,25,25, 26,26,26,
      27,27,27,27, 28,28,28, 29,29,29,
      30,30,30,30, 31,31,31, 32,32,32,
      33,33,33,33, 34,34,34, 35,35,35,
      36,36,36,36, 37,37,37, 38,38,
  };

  u8 uHi = uArg >> 64;
  u8 uLo = (u8) uArg;
  // Note: result is undefined if 0 is fed to __builtin_clzll()
  u8 uLeadingZeros = uHi == 0 ? 64 : __builtin_clzll(uHi);

  i8 iBase2Digits;
  if (uLeadingZeros == 64) {
    iBase2Digits = 128 - uLeadingZeros - __builtin_clzll(uLo); // uLo cannot be zero due to the zero check above

  } else {
    iBase2Digits = 128 - uLeadingZeros;
  }

  i8 iDigits = iaGuess[iBase2Digits];
  i8 iPrec = iDigits + (uArg >= (u16) pDynamic->m_iaPow10[iDigits]);

  return iPrec;
}


/*----------------------------------------------------------------------------*/
/* Get precision of a signed 128 bit integer                                  */
/*----------------------------------------------------------------------------*/
i8 iPrecision128_signed(i16 iArg, struct Dynamic_T* pDynamic)
{
  if (iArg < 0) {
    iArg = -iArg;
  }

  i8 iPrec = iPrecision128((u16) iArg, pDynamic);
  return iPrec;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


#include "../src/store/Ent_int.h"
#include "../src/base/eEC.h"
#include "../src/base/RelationalMacros.h"
#include "../src/base/CastMacros.h"
#include "../src/base/LogicalMacros.h"
#include "../src/base/MathMacros.h"
#include "../src/base/FunctionMacros.h"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/