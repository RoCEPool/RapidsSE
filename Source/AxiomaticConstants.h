/*============================================================================*/
/* AxiomaticConstants.h -- Constants on which other principles are built      */
/*============================================================================*/
#pragma once

static const i8 C_UNKNOWN = -1;

static const i8 C_DATUM_NULL           = LLONG_MIN + 1;
static const i8 C_DATUM_MINUS_INFINITY = LLONG_MIN;
static const i8 C_DATUM_PLUS_INFINITY  = LLONG_MAX;

static const i8 C_I8_MIN = LLONG_MIN + 2;
static const i8 C_I8_MAX = LLONG_MAX - 1;

static const d8 C_D8_MIN = DBL_MIN + DBL_EPSILON;
static const d8 C_D8_MAX = DBL_MAX;

static const u8 C_YEAR_BASE  = 1900;
static const u8 C_YEAR_RANGE = 255;
static const u8 C_YEAR_MAX   = C_YEAR_BASE + C_YEAR_RANGE;
static const u8 C_YEAR_MIN   = C_YEAR_BASE;


// move to UsefulConstants.h

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
