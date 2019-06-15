/*============================================================================*/
/* AxiomaticConstants.h -- Constants on which other principles are built      */
/*============================================================================*/
#pragma once

static const i8 C_DATUM_NULL           = LLONG_MIN + 1;
static const i8 C_DATUM_MINUS_INFINITY = LLONG_MIN;
static const i8 C_DATUM_PLUS_INFINITY  = LLONG_MAX;

static const i8 C_I8_MIN = LLONG_MIN + 2;
static const i8 C_I8_MAX = LLONG_MAX - 1;

static const d8 C_D8_MIN = DBL_MIN + DBL_EPSILON;
static const d8 C_D8_MAX = DBL_MAX;

static const u8 C_BASE_YEAR  = 1900;
static const u8 C_YEAR_RANGE = 255;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
