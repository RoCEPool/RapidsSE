/*============================================================================*/
/* LimitsFormats.h -- System Limits and Standard Formats                      */
/*============================================================================*/
#pragma once

static const i8 C_MAX_COLUMNS    = 150;    // max columns per table
static const i8 C_MAX_INDEXES    = 1025;   // max indexes per table
static const i8 C_MAX_KEY        = 256;    // max length of index entry key
static const i8 C_MAX_NODES      = 128;    // max nodes in a grid
static const i8 C_MAX_PATH       = 256;    // max length of file path
static const i8 C_MAX_RECORD     = C_16KB; // max length of record
static const i8 C_MAX_SESSIONS   = 1024;   // max number of sessions
static const i8 C_MAX_SLOTS      = 128;    // max slots per node
static const i8 C_MAX_VCH        = C_4KB;  // max practical length of vch value

// max partitions in a grid
static const i8 C_MAX_PARTITIONS = C_MAX_SLOTS * C_MAX_NODES;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static const i8 C_MAX_PRECISION_BCD = 19;
static const i8 C_MAX_PRECISION_FLT = 19;
static const i8 C_MAX_PRECISION_INT = 19;
static const i8 C_MAX_PRECISION_VCH = C_64KB; // max defined length of a vch col

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static const Stz* F_BCD       = "%s%lld"_Stz;
static const Stz* F_BCD_frac  = "%s%lld.%.*lld"_Stz;
static const Stz* F_DTE       = "%04lld-%02lld-%02lld"_Stz;
static const Stz* F_FLT       = "%g"_Stz;
static const Stz* F_INT       = "%lld"_Stz;
static const Stz* F_IVL       = "%lld"_Stz;
static const Stz* F_IVL_frac  = "%lld.%.*lld"_Stz;
static const Stz* F_TME       = "%02lld:%02lld:%02lld"_Stz;
static const Stz* F_TME_frac  = "%02lld:%02lld:%02lld.%s"_Stz;
static const Stz* F_TSP       = "TIMESTAMP '%02lld-%02lld-%02lld %02lld:%02lld:%02lld'"_Stz;
static const Stz* F_TSP_frac  = "TIMESTAMP '%02lld-%02lld-%02lld %02lld:%02lld:%02lld.%s'"_Stz;

static const Stz* F_VCH_enclose = "'"_Stz;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
