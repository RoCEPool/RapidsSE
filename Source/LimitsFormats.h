/*============================================================================*/
/* LimitsFormats.h -- Library Limits and Standard Formats                     */
/*============================================================================*/
#pragma once

static const i8 C_MAX_BUCKETS       = C_64KB;
static const i8 C_MAX_COLUMNS       = 150;    // max columns per table
static const i8 C_MAX_COLUMNVECS    = 32;     // max column vectors per streamDef
static const i8 C_MAX_DEFCOLUMNS    = 256;    // max columns in a stream definition
static const i8 C_MAX_ENTRY_SIZE    = C_1KB;  // max index entry size
static const i8 C_MAX_EXPR          = 256;    // max expressions in expr vec
static const i8 C_MAX_FROMVEC       = 16;
static const i8 C_MAX_IDXCOLVEC     = 16;
static const i8 C_MAX_INDEXES       = 16;     // max indexes per table
static const i8 C_MAX_INDEX_COLUMNS = 16;
static const i8 C_MAX_KEY           = 256;    // max length of index entry key
static const i8 C_MAX_LOG_ENTRIES   = C_256K * 1;
static const i8 C_MAX_LOG_NOTE      = C_1K;
static const i8 C_MAX_LVL_LEVELS    = 6;
static const i8 C_MAX_LVL_RATIO     = 6;
static const i8 C_MAX_NESTED_WORDS  = 16;
static const i8 C_MAX_NODES         = 128;    // max nodes in a grid
static const i8 C_MAX_OPS           = 256;
static const i8 C_MAX_PATH          = 256;    // max length of file path
static const i8 C_MAX_PAY           = C_1KB;  // max length of index entry payload
static const i8 C_MAX_PREDICATEVEC  = 16;
static const i8 C_MAX_PTNS          = C_64KB;
static const i8 C_MAX_SEEKOFFSETS   = 16;
static const i8 C_MAX_SESSIONS      = 64;     // max number of sessions
static const i8 C_MAX_SHORTCUTS     = 1024;
static const i8 C_MAX_STAGES        = 256;
static const i8 C_MAX_STRANDS       = 256;
static const i8 C_MAX_STREAMDEFS    = 16;     // max stream definitions
static const i8 C_MAX_TABLEPTNS     = 128;    // max tablePtn's per table
static const i8 C_MAX_THREADS       = 4096;
static const i8 C_MAX_TOKENS        = C_2KB;

// max partitions in a grid         GET RID OF THIS.  USE  C_MAX_BUCKETS INSTEAD
static const i8 C_MAX_PARTITIONS    = C_MAX_TABLEPTNS * C_MAX_NODES;

static const i8 C_MIN_LVL_LEVELS    = 2;
static const i8 C_MIN_LVL_RATIO     = 2;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static const i8 C_MAX_BCD_PRECISION = 19;
static const i8 C_MAX_FLT_PRECISION = 19;
static const i8 C_MAX_INT_PRECISION = 19;
static const i8 C_MAX_VCH_PRECISION = C_64KB; // max defined length of a vch col

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static const Stz* F_BCD       = "%s%lld"_Stz;
static const Stz* F_BCD_frac  = "%s%lld.%.*lld"_Stz;
static const Stz* F_DTE       = "%04lld-%02lld-%02lld"_Stz;
static const Stz* F_FLT       = "%g"_Stz;
static const Stz* F_INT       = "%lld"_Stz;
static const Stz* F_IVL       = "%lld.%06lld"_Stz;
static const Stz* F_IVL_frac  = "%lld.%.*lld"_Stz;
static const Stz* F_TME       = "%02lld:%02lld:%02lld"_Stz;
static const Stz* F_TME_frac  = "%02lld:%02lld:%02lld.%s"_Stz;
static const Stz* F_TSP       = "%02lld-%02lld-%02lld %02lld:%02lld:%02lld"_Stz;
static const Stz* F_TSP_frac  = "%02lld-%02lld-%02lld %02lld:%02lld:%02lld.%s"_Stz;

static const Stz* F_VCH_enclose = "'"_Stz;

static const Stz* F_NOW       = "%02lld-%02lld-%02lld %02lld:%02lld:%02lld"_Stz;
static const Stz* F_NOW_path  = "%02lld-%02lld-%02lld-%02lld-%02lld-%02lld"_Stz;


static Stz* C_DATE_FORMAT          = "date_format"_Stz;
static Stz* C_ENCLOSED_BY_LEADING  = "enclosed_by_leading"_Stz;
static Stz* C_ENCLOSED_BY_TRAILING = "enclosed_by_trailing"_Stz;
static Stz* C_ESCAPE_CHAR          = "escape_char"_Stz;
static Stz* C_FIELD_DELIMITER      = "field_delimiter"_Stz;
static Stz* C_MAX_ERRORS           = "max_errors"_Stz;
static Stz* C_RECORD_ENDS          = "record_ends"_Stz;
static Stz* C_RECORD_TERMINATOR    = "record_terminator"_Stz;
static Stz* C_SAME_MACHINE         = "same_machine"_Stz;

static const char D_ENCLOSED_BY_LEADING  = '\0';
static const char D_ENCLOSED_BY_TRAILING = '\0';
static const char D_ESCAPE_CHAR          = '\\';
static const char D_FIELD_DELIMITER      = ',';
static const char D_RECORD_TERMINATOR    = '\0';

static Stz* C_NO_ARG  = "-- NO_ARG --"_Stz;
static Stz* C_NO_NAME = "-- NO_NAME --"_Stz;
static Stz* C_NO_NOTE = "-- no note --"_Stz;
static Stz* C_NO_WORD = "-- NO_WORD --"_Stz;
static Stz* C_NULL    = "null"_Stz;

static Stz* C_PROCEDURE_CATG = "\\procedure"_Stz;

static Stz* C_SE_PATH = "/tenants/boray/catalogs/rapidsse/schemas/public/tables"_Stz;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


static Stz* C_TDEF_ANALYZEINDEX     = "__analyzeIndex"_Stz;
static Stz* C_TDEF_ANALYZETABLE     = "__analyzeTable"_Stz;
static Stz* C_TDEF_CALLPIPELINE     = "__callPipeline"_Stz;
static Stz* C_TDEF_COUNT            = "__count"_Stz;
static Stz* C_TDEF_COUNTINFO        = "__countInfo"_Stz;
static Stz* C_TDEF_GRIDSANITY       = "__gridSanity"_Stz;
static Stz* C_TDEF_LISTCOLUMNS      = "__listColumns"_Stz;
static Stz* C_TDEF_LISTCONTENTS     = "__listContents"_Stz;
static Stz* C_TDEF_LISTFOLDER       = "__listFolder"_Stz;
static Stz* C_TDEF_LISTINDEX        = "__listIndex"_Stz;
static Stz* C_TDEF_LISTPARTITIONS   = "__listPartitions"_Stz;
static Stz* C_TDEF_LOCKSTATS        = "__lockStats"_Stz;
static Stz* C_TDEF_NODEINFO         = "__nodeInfo"_Stz;
static Stz* C_TDEF_PATHINFO         = "__pathInfo"_Stz;
static Stz* C_TDEF_PINGINFO         = "__pingInfo"_Stz;
static Stz* C_TDEF_PROGRESSUNITS    = "__progressUnits"_Stz;
static Stz* C_TDEF_PROMPTOUT        = "__promptOut"_Stz;
static Stz* C_TDEF_PWF              = "__pwf"_Stz;
static Stz* C_TDEF_RETURNPIPELINE   = "__returnPipeline"_Stz;
static Stz* C_TDEF_SEEKOFFSETS      = "__seekOffsets"_Stz;
static Stz* C_TDEF_SHOWCLUSTER      = "__showCluster"_Stz;
static Stz* C_TDEF_SIZEINDEX        = "__sizeIndex"_Stz;
static Stz* C_TDEF_SIZETABLE        = "__sizeTable"_Stz;
static Stz* C_TDEF_SPACE            = "__space"_Stz;
static Stz* C_TDEF_STATUS           = "__status"_Stz;
static Stz* C_TDEF_TABLESIZES       = "__tableSizes"_Stz;
static Stz* C_TDEF_TOTALUNITS       = "__totalUnits"_Stz;
static Stz* C_TDEF_VERIFYINDEX      = "__verifyIndex"_Stz;
static Stz* C_TDEF_VERIFYTABLE      = "__verifyTable"_Stz;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
