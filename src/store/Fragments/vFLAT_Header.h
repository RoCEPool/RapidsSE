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

typedef signed char i1;
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
static const i8 C_BCD_MIN = -36028797018963967;
static const i8 C_BCD_MAX = 36028797018963967;
static const i8 C_CRW_HEADBYTES = 32;
static const i8 C_CRW_FIRSTHEAD = 640;
static const tET C_ENT_END_TICKET = 0xFFFF;
static const d8 C_FLT_MAX = 1.797693134862315708145274237317043567981e+308;
static const d8 C_FLT_MIN = -1.797693134862315708145274237317043567981e+308;
static const u8 C_YEAR_BASE  = 1900;
static const u8 C_YEAR_RANGE = 255;
static const u8 C_YEAR_MAX   = 2155;
static const i8 TMP_TBLCOL_SCALE  = 2;

static i8 iDaysPerMonth[13]     = { -1,31,28,31,30,31,30,31,31,30,31,30,31};
static i8 iDaysPerMonthLeap[13] = { -1,31,29,31,30,31,30,31,31,30,31,30,31};

#define C_BITS_PER_WORD 64
#define C_MAX_LVL_LEVELS 6
#define C_LOGIC_LOCALS 32
#define C_UNKNOWN -1

#include "../src/store/Ent_int.h"
#include "../src/base/eEC.h"
#include "../src/base/CastMacros.h"
#include "../src/base/LogicalMacros.h"
#include "../src/base/RelationalMacros.h"
#include "../src/base/MathMacros.h"
#include "../src/base/FunctionMacros.h"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/