/*============================================================================*/
/* AxiomaticConstants.h -- Constants on which other principles are built      */
/*============================================================================*/
#pragma once

static const i8 C_ALL          = -1;
static const i8 C_VARGS_BEGIN  = -1;
static const i8 C_VARGS_END    = -1;
static const i8 C_NEVER_THROWN = -1;
static const i8 C_NIL          = -1;
static const i8 C_UNKNOWN      = -1;
static const i8 C_IGNORED      = -1;

static const i8 C_DATUM_NULL           = LLONG_MIN + 1;
static const i8 C_DATUM_MINUS_INFINITY = LLONG_MIN;
static const i8 C_DATUM_PLUS_INFINITY  = LLONG_MAX;

static const i8 C_I8_MIN = LLONG_MIN + 2;
static const i8 C_I8_MAX = LLONG_MAX - 1;
static const i8 C_I8_MID = C_I8_MAX / 2;

static const d8 C_D8_MIN = DBL_MIN + DBL_EPSILON;
static const d8 C_D8_MAX = DBL_MAX;
static const d8 C_D8_MID = DBL_MAX / 2.0;

static const u8 C_YEAR_BASE  = 1900;
static const u8 C_YEAR_RANGE = 255;
static const u8 C_YEAR_MAX   = C_YEAR_BASE + C_YEAR_RANGE;
static const u8 C_YEAR_MIN   = C_YEAR_BASE;

static const u8 C_IVL_SEC_MAX  = 999999999;
static const u8 C_IVL_USEC_MAX = 999999;

static const bool B_CALL_AGAIN      = true;
static const bool B_DONT_CALL_AGAIN = false;

static const b8 C_NONFOCAL          = true;

static const i8 C_SET_OFFSETS       = 128;
static const i8 C_LOAD_READCHUNK    = C_1M; // its 1M not 1MB (i.e. < 1MB)
static const i8 C_CHUNKBLOCK        = 2048;

static const i8 C_NULL_1B = 1;// <NULL> '\0' 1B

static const i8 C_C_1B = 1;   // <C> 1B number of compressed bytes
static const i8 C_L_1B = 1;   // <L> 1B string length in bytes
static const i8 C_M_1B = 1;   // <M> 1B overflowed metacode
static const i8 C_T_8B = 8;   // <T> 8B transaction perspective
static const i8 C_U_1B = 1;   // <U> 1B over/tail line size in units
static const i8 C_W_1B = 1;   // <W> 1B head line size in words

static const i8 C_L_2B = 2;   // <L> 2B string length in bytes

static const i8 C_FT_2B = 2;  // |FT| 2B absolute from-tail i_Line
static const i8 C_IT_2B = 2;  // [IT] 2B relative into-tail i_Line
static const i8 C_OT_2B = 2;  // [OT] 2B relative over-tail i_Line

static const i8 C_ERA_0 =  0; // 1900
static const i8 C_ERA_1 = 92; // 1992
static const i8 C_ERA_2 = 99; // 1999

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
