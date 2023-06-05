

/*====================================================================*/
/* Flattened code executed by the dynamicOP                           */
/*====================================================================*/

  #include <stdio.h>
  #include <string.h>
  #include <stdbool.h>

  #include "../src/primitives/BaseTypes_tcc.h"
  #include "../src/base/CodeSynthCommon.h"
  #include "../src/base/ClangStuff.h"
  #include "../src/base/ClangDynamic.h"

  void vNeverReachCode(i8 iLine) {
    printf("NEVER_REACH_CODE point in synthesised code was reached at line %%lld !!!\n", iLine);
    //exit(0);
  }

  #define NEVER_REACH vNeverReachCode(__LINE__)

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
  static const i8 C_BCD_MIN = -36028797018963967;
  static const i8 C_BCD_MAX = 36028797018963967;
  static const i8 C_CRW_HEADBYTES = 32;
  static const i8 C_CRW_FIRSTHEAD = 640;
  static const tET C_ENT_END_TICKET = 0xFFFF;
  static const d8 C_FLT_MAX = 1.797693134862315708145274237317043567981e+308;
  static const d8 C_FLT_MIN = -1.797693134862315708145274237317043567981e+308;
  static const u8 C_YEAR_BASE  = 1900;
  static const i8 TMP_TBLCOL_SCALE  = 2;

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
  #include "../src/base/TccMacros.h"

  /* time.h */
  typedef long                    __darwin_time_t;
  typedef __darwin_time_t         time_t;

  struct tm {
  int tm_sec; /* seconds after the minute [0-60] */
  int tm_min; /* minutes after the hour [0-59] */
  int tm_hour; /* hours since midnight [0-23] */
  int tm_mday; /* day of the month [1-31] */
  int tm_mon; /* months since January [0-11] */
  int tm_year; /* years since 1900 */
  int tm_wday; /* days since Sunday [0-6] */
  int tm_yday; /* days since January 1 [0-365] */
  int tm_isdst; /* Daylight Savings Time flag */
  long tm_gmtoff; /* offset from UTC in seconds */
  char *tm_zone; /* timezone abbreviation */
  };

  time_t time(time_t *time);
  struct tm *localtime(const time_t *time);

