/*============================================================================*/
/* LimitsFormats.h -- System Limits and Standard Formats                      */
/*============================================================================*/
#pragma once

//! review these with Alka

static const i8 C_SLOTS_PER_NODE = 128;
static const i8 C_MAX_NODES      = 128;
static const i8 C_MAX_PARTITIONS = C_SLOTS_PER_NODE * C_MAX_NODES;

static const i8 C_MAX_RECORD     = C_16KB;
static const i8 C_MAX_COLUMNS    = 150;
static const i8 C_PATH_MAX       = 256;
static const i8 C_MAX_VCH        = C_4KB;
static const i8 C_MAX_SESSIONS   = 1024;

static const i8 C_MAX_PRECISION_BCD = 19;
static const i8 C_MAX_PRECISION_FLT = 19;
static const i8 C_MAX_PRECISION_INT = 19;

// allow for maximum 1025 indexes per table
static const i8 C_INDEXES_PER_TABLE = 1025;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static const Stz* F_BCD       = "%s%lld"_Stz;
static const Stz* F_BCD_frac  = "%s%lld.%.*lld"_Stz;
static const Stz* F_DTE       = "%04lld-%02lld-%02lld"_Stz;
static const Stz* F_FLT       = "%g"_Stz;
static const Stz* F_INT       = "%lld"_Stz;
static const Stz* F_IVL       = "%lld"_Stz;
static const Stz* F_TME       = "%02lld:%02lld:%02lld"_Stz;
static const Stz* F_TME_frac  = "%02lld:%02lld:%02lld.%s"_Stz;
static const Stz* F_TSP       = "%02lld-%02lld-%02lld %02lld:%02lld:%02lld"_Stz;
static const Stz* F_TSP_frac  = "%02lld-%02lld-%02lld %02lld:%02lld:%02lld.%s"_Stz;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
