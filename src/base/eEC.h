/*============================================================================*/
/* eEC.h -- Error Codes                                                       */
/*============================================================================*/
#pragma once

#ifdef __TINYC__

// when being included by the tcc compiler
typedef enum eEC // error codes

#else

// when not being included by the tcc compiler
enum eEC : u2 // error codes

#endif

{
  EC_NONE = 0,

  EC_DONE,

  // - - - - - - - - - - - - - - - - - - -
  /* _A_ */
  EC_AGG_EXPR_IN_WHERE_CLAUSE,      // SQL Compiler Subsystem
  EC_AMBIGUOUS_ALIAS,               // SQL Compiler Subsystem
  EC_AMBIGUOUS_COLUMN,              // SQL Compiler Subsystem
  EC_AMBIGUOUS_TABLE,               // SQL Compiler Subsystem
  EC_AMBIGUOUS_USING_COLUMN,        // SQL Compiler Subsystem
  EC_AMBIGUOUS_WITH_QUERY,          // SQL Compiler Subsystem
  EC_ARITHMETIC_OVERFLOW,           // Arithmetic operation Overflow
  EC_ARITHMETIC_UNDERFLOW,          // Arithmetic operation Underflow
  EC_ASSERT,

  /* _B_ */
  EC_BLANKLINE,

  /* _C_ */
  EC_CATALOG_EXISTS,
  EC_CLUSTER_NOT_CONNECTED,
  EC_CLUSTER_PARTITIONS_MISMATCH,
  EC_CLUSTER_SIZE_MISMATCH,
  EC_COLUMN_COUNT_MISMATCH,         // SQL Compiler Subsystem
  EC_COLUMN_TYPE_MISMATCH,          // SQL Compiler Subsystem
  EC_CORRBY_COLUMN_NOT_FOUND,       // SQL Compiler Subsystem
  EC_CORRBY_COLUMN_TYPE_MISMATCH,   // SQL Compiler Subsystem
  EC_CORRBY_NO_COMMON_COLUMNS,      // SQL Compiler Subsystem
  EC_CSV_BOOLEAN_INVALID,
  EC_CSV_BOOLEAN_TOO_LARGE,
  EC_CSV_DATE_INVALID,
  EC_CSV_DATE_SEP_MISSING,
  EC_CSV_DAY_INVALID,
  EC_CSV_DAY_NOT_LEAPYEAR,
  EC_CSV_DAY_TOO_LARGE,
  EC_CSV_DECIMAL_INVALID,
  EC_CSV_DECIMAL_TOO_LARGE,
  EC_CSV_DECIMAL_TOO_SMALL,
  EC_CSV_EXPONENT_MISSING,
  EC_CSV_FLOAT_INVALID,
  EC_CSV_HOUR_INVALID,
  EC_CSV_HOUR_TOO_LARGE,
  EC_CSV_INTEGER_INVALID,
  EC_CSV_INTEGER_TOO_LARGE,
  EC_CSV_INTERVAL_INVALID,
  EC_CSV_INTERVAL_SECS_TOO_LARGE,
  EC_CSV_INTERVAL_USECS_TOO_LARGE,
  EC_CSV_MINUTE_INVALID,
  EC_CSV_MINUTE_TOO_LARGE,
  EC_CSV_MONTH_INVALID,
  EC_CSV_MONTH_TOO_LARGE,
  EC_CSV_SECOND_INVALID,
  EC_CSV_SECOND_TOO_LARGE,
  EC_CSV_TIME_INVALID,
  EC_CSV_TIME_SEP_MISSING,
  EC_CSV_TIMESTAMP_INVALID,
  EC_CSV_VARCHAR_INVALID,
  EC_CSV_YEAR_INVALID,
  EC_CSV_YEAR_TOO_SMALL,
  EC_CSV_YEAR_TOO_LARGE,

  /* _D_ */
  EC_DECIMAL_NOT_SUPPORTED,
  EC_DIVIDE_BY_ZERO,
  EC_DUPLICATE_COLUMN,
  EC_DUPLICATE_KEY,

  /* _E_ */
  EC_ESCAPE_SINGLE_CHAR,            // SQL Compiler Subsystem
  EC_EXPLAIN_INVALID_FORMAT,        // SQL Compiler Subsystem
  EC_EXPR_AGG_MIX_NO_GROUPBY_LIST,  // SQL Compiler Subsystem
  EC_EXPR_INVALID_COMBINATION,      // SQL Compiler Subsystem
  EC_EXPR_IS_AGG_IN_GROUPBY_LIST,   // SQL Compiler Subsystem
  EC_EXPR_IS_LIT_IN_GROUPBY_LIST,   // SQL Compiler Subsystem
  EC_EXPR_MUST_BE_BOOLEAN,          // SQL Compiler Subsystem
  EC_EXPR_MUST_BE_EQUIJOIN,         // SQL Compiler Subsystem
  EC_EXPR_NOT_IN_GROUPBY_LIST,      // SQL Compiler Subsystem
  EC_EXPR_NOT_PROJECTED,            // SQL Compiler Subsystem
  EC_EXPR_TYPE_INCOMPATIBLE,        // SQL Compiler Subsystem

  /* _F_ */
  EC_FOLDER_IN_USE,
  EC_FOLDER_NOT_EMPTY,

  /* _G_ */

  /* _H_ */
  EC_HAVING_EXPR_NOT_AGG_OR_IN_GROUPBY, // SQL Compiler Subsystem
  EC_HINT_VALUE_UNRECOGNISED,           // SQL Compiler Subsystem
  EC_HINT_VALUE_MISSING,                // SQL Compiler Subsystem

  /* _I_ */
  EC_IDXVFY_ADJACENT_ENTRIES,
  EC_IDXVFY_BUFFER_REFS,
  EC_IDXVFY_FIRST_KEYS,
  EC_IDXVFY_ITEMS,
  EC_IDXVFY_KEY_BUFFER,
  EC_IDXVFY_KEY_ORDER,
  EC_IDXVFY_NEXT_PREV,
  EC_IDXVFY_ROW_REFS,
  EC_IDXVFY_SKIPLIST_ENTRY,
  EC_IDXVFY_SKIPLIST_LEVELS,
  EC_INDEX_EXISTS,
  EC_INSUFFICIENT_EXPRS,            // SQL Compiler Subsystem
  EC_INTERNAL_ERROR,
  EC_INTERVAL_NOT_SUPPORTED,
  EC_INVALID_CLASS,
  EC_INVALID_COLUMN,
  EC_INVALID_COLUMNS_UNIQUE_INDEX,
  EC_INVALID_COMMAND,
  EC_INVALID_DATE_FORMAT,           // SQL Compiler Subsystem
  EC_INVALID_FILE,
  EC_INVALID_FLOAT_FORMAT,          // SQL Compiler Subsystem
  EC_INVALID_INTEGER_FORMAT,        // SQL Compiler Subsystem
  EC_INVALID_INTERFACE,
  EC_INVALID_LIMIT_VALUE,           // SQL Compiler Subsystem
  EC_INVALID_OFFSET_VALUE,          // SQL Compiler Subsystem
  EC_INVALID_PARAMETER,
  EC_INVALID_PARAMETERS,
  EC_INVALID_PARTITION,
  EC_INVALID_PARTITION_KEY,
  EC_INVALID_PIPELINE,
  EC_INVALID_INDEX_COLUMN,
  EC_INVALID_INSTANCE,
  EC_INVALID_SIZE,
  EC_INVALID_SYNTAX,
  EC_INVALID_TIME_FORMAT,           // SQL Compiler Subsystem
  EC_INVALID_TIMESTAMP_FORMAT,      // SQL Compiler Subsystem
  EC_INVALID_TYPES,
  EC_INVALID_VALUE,

  /* _J_ */

  /* _K_ */

  /* _L_ */
  EC_LOW_MEMORY,

  /* _M_ */
  EC_MAX_INDEXES,
  EC_MAX_COLUMNS,
  EC_MAX_ROW_LENGTH,

  /* _N_ */
  EC_NO_COLUMNS_DIC,
  EC_NO_SUCH_CATALOG,
  EC_NO_SUCH_COLUMN,                // SQL Compiler Subsystem
  EC_NO_SUCH_DIRECTORY,
  EC_NO_SUCH_FILE,
  EC_NO_SUCH_FOLDER,
  EC_NO_SUCH_FUNCTION,
  EC_NO_SUCH_INDEX,
  EC_NO_SUCH_JOB,
  EC_NO_SUCH_NODE,
  EC_NO_SUCH_SCHEMA,
  EC_NO_SUCH_TABLE,
  EC_NO_SUCH_TABLE2,                // SQL Compiler Subsystem
  EC_NO_SUCH_TENANT,
  EC_NO_SUCH_WORD,
  EC_NODE_EXISTS,
  EC_NOT_A_CATALOG,
  EC_NOT_A_FOLDER,
  EC_NOT_AN_INDEX,
  EC_NOT_A_SCHEMA,
  EC_NOT_A_TABLE,
  EC_NOT_A_TENANT,
  EC_NOT_IMPLEMENTED,               // SQL Compiler Subsystem
  EC_NOT_IN_CATALOGS,
  EC_NOT_IN_COLUMNS,
  EC_NOT_IN_INDEXES,
  EC_NOT_IN_PARTITIONS,
  EC_NOT_IN_SCHEMAS,
  EC_NOT_IN_TABLE,
  EC_NOT_IN_TABLES,
  EC_NOT_IN_TENANTS,

  EC_NULL_INTERFACE,
  EC_NULLPTR,

  /* _O_ */
  EC_OBJECT_ADDRESS_INVALID,
  EC_OBJECT_CLASSNO_INVALID,
  EC_OBJECT_FOOTER_INVALID,
  EC_OBJECT_MAGIC_INVALID,
  EC_ORDERBY_NOT_SUPPORTED,

  /* _P_ */
  EC_PIPELINE_PROTOCOL_ERROR,

  /* _Q_ */

  /* _R_ */
  EC_RESPONSE_PENDING,

  /* _S_ */
  EC_SCALE_OUT_OF_BOUNDS,
  EC_SCHEMA_EXISTS,
  EC_SE_NOT_RUNNING,
  EC_SE_UNKNOWN_ERROR,
  EC_SEMANTIC_ERROR,                // SQL Compiler Subsystem
  EC_SEND_FAILURE,
  EC_SUBQUERY_SELF_REFERENCE,       // SQL Compiler Subsystem
  EC_SUBQUERY_PROJ_TOO_MANY_COLS,   // SQL Compiler Subsystem
  EC_SYNTAX_ERROR,                  // SQL Compiler Subsystem
  EC_SYSTEM_CLOSED,

  /* _T_ */
  EC_TABLE_ALREADY_LOADED,
  EC_TABLE_EXISTS,
  EC_TABLE_NOT_EMPTY,
  EC_TABLE_IN_USE,
  EC_TABLE_SUBQUERY_OUT_OF_SCOPE,   // SQL Compiler Subsystem
  EC_TENANT_EXISTS,
  EC_TINYC_ERROR,
  EC_TOO_MANY_NAMED_COLUMNS,
  EC_TX_BEGIN_FAILED,
  EC_TX_COMMIT_FAILED,
  EC_TX_END_FAILED,
  EC_TX_ROLLBACK_FAILED,

  /* _U_ */
  EC_UNRESOLVED_COLUMN_WILDCARD,    // SQL Compiler Subsystem
  EC_USING_COLUMN_NOT_FOUND,        // SQL Compiler Subsystem

  /* _V_ */
  EC_VALIDATION_FAILURE,
  EC_VALUE_OUT_OF_RANGE,            // SQL Compiler Subsystem
  EC_VERSION_MISMATCH,

  /* _W_ */

  /* _X_ */

  /* _Y_ */

  /* _Z_ */
  EC_ZZZ_OTHER,

  EC_ENUMS
#ifdef __TINYC__
} eEC;
#else
};
#endif

#ifndef __TINYC__

// This should contain the text string representation of the EC enum.
// Not the error message.
static Stz* aEC[] =
{
  /* */ (Stz*)"none",
  
  /* */ (Stz*)"done",

  // - - - - - - - - - - - - - - - - - - -

  /* _A_ */
  /* EC_AGG_EXPR_IN_WHERE_CLAUSE      */ (Stz*)"EC_AGG_EXPR_IN_WHERE_CLAUSE", // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_ALIAS               */ (Stz*)"EC_AMBIGUOUS_ALIAS",          // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_COLUMN              */ (Stz*)"EC_AMBIGUOUS_COLUMN",         // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_TABLE               */ (Stz*)"EC_AMBIGUOUS_TABLE",          // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_USING_COLUMN        */ (Stz*)"EC_AMBIGUOUS_USING_COLUMN",   // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_WITH_QUERY          */ (Stz*)"EC_AMBIGUOUS_WITH_QUERY",     // SQL Compiler Subsystem
  /* EC_ARITHMETIC_OVERFLOW,          */ (Stz*)"EC_ARITHMETIC_OVERFLOW",      // Arithmetic operation Overflow
  /* EC_ARITHMETIC_UNDERFLOW,         */ (Stz*)"EC_ARITHMETIC_UNDERFLOW",     // Arithmetic operation Underflow
  /* EC_ASSERT                        */ (Stz*)"assert",

  /* _B_ */
  /* EC_BLANKLINE                     */ (Stz*)"blank line",

  /* _C_ */
  /* EC_CATALOG_EXISTS                */ (Stz*)"catalog exists",
  /* EC_CLUSTER_NOT_CONNECTED         */ (Stz*)"cluster not connected (use connectCluster command)",
  /* EC_CLUSTER_PARTITIONS_MISMATCH   */ (Stz*)"cluster partitions per node mismatch",
  /* EC_CLUSTER_SIZE_MISMATCH         */ (Stz*)"cluster node size mismatch",
  /* EC_COLUMN_COUNT_MISMATCH         */ (Stz*)"EC_COLUMN_COUNT_MISMATCH",        // SQL Compiler Subsystem
  /* EC_COLUMN_TYPE_MISMATCH          */ (Stz*)"EC_COLUMN_TYPE_MISMATCH",         // SQL Compiler Subsystem
  /* EC_CORRBY_COLUMN_NOT_FOUND       */ (Stz*)"EC_CORRBY_COLUMN_NOT_FOUND",      // SQL Compiler Subsystem
  /* EC_CORRBY_COLUMN_TYPE_MISMATCH   */ (Stz*)"EC_CORRBY_COLUMN_TYPE_MISMATCH",  // SQL Compiler Subsystem
  /* EC_CORRBY_NO_COMMON_COLUMNS      */ (Stz*)"EC_CORRBY_NO_COMMON_COLUMNS",     // SQL Compiler Subsystem
  /* EC_CSV_BOOLEAN_INVALID           */ (Stz*)"EC_CSV_BOOLEAN_INVALID",
  /* EC_CSV_BOOLEAN_TOO_LARGE         */ (Stz*)"EC_CSV_BOOLEAN_TOO_LARGE",
  /* EC_CSV_DATE_INVALID              */ (Stz*)"EC_CSV_DATE_INVALID",
  /* EC_CSV_DATE_SEP_MISSING          */ (Stz*)"EC_CSV_DATE_SEP_MISSING",
  /* EC_CSV_DAY_INVALID               */ (Stz*)"EC_CSV_DAY_INVALID",
  /* EC_CSV_DAY_NOT_LEAPYEAR          */ (Stz*)"EC_CSV_DAY_NOT_LEAPYEAR",
  /* EC_CSV_DAY_TOO_LARGE             */ (Stz*)"EC_CSV_DAY_TOO_LARGE",
  /* EC_CSV_DECIMAL_INVALID           */ (Stz*)"EC_CSV_DECIMAL_INVALID",
  /* EC_CSV_DECIMAL_TOO_LARGE         */ (Stz*)"EC_CSV_DECIMAL_TOO_LARGE",
  /* EC_CSV_DECIMAL_TOO_SMALL         */ (Stz*)"EC_CSV_DECIMAL_TOO_SMALL",
  /* EC_CSV_EXPONENT_MISSING          */ (Stz*)"EC_CSV_EXPONENT_MISSING",
  /* EC_CSV_FLOAT_INVALID             */ (Stz*)"EC_CSV_FLOAT_INVALID",
  /* EC_CSV_HOUR_INVALID              */ (Stz*)"EC_CSV_HOUR_INVALID",
  /* EC_CSV_HOUR_TOO_LARGE            */ (Stz*)"EC_CSV_HOUR_TOO_LARGE",
  /* EC_CSV_INTEGER_INVALID           */ (Stz*)"EC_CSV_INTEGER_INVALID",
  /* EC_CSV_INTEGER_TOO_LARGE         */ (Stz*)"EC_CSV_INTEGER_TOO_LARGE",
  /* EC_CSV_INTERVAL_INVALID          */ (Stz*)"EC_CSV_INTERVAL_INVALID",
  /* EC_CSV_INTERVAL_SECS_TOO_LARGE   */ (Stz*)"EC_CSV_INTERVAL_SECS_TOO_LARGE",
  /* EC_CSV_INTERVAL_USECS_TOO_LARGE  */ (Stz*)"EC_CSV_INTERVAL_USECS_TOO_LARGE",
  /* EC_CSV_MINUTE_INVALID            */ (Stz*)"EC_CSV_MINUTE_INVALID",
  /* EC_CSV_MINUTE_TOO_LARGE          */ (Stz*)"EC_CSV_MINUTE_TOO_LARGE",
  /* EC_CSV_MONTH_INVALID             */ (Stz*)"EC_CSV_MONTH_INVALID",
  /* EC_CSV_MONTH_TOO_LARGE           */ (Stz*)"EC_CSV_MONTH_TOO_LARGE",
  /* EC_CSV_SECOND_INVALID            */ (Stz*)"EC_CSV_SECOND_INVALID",
  /* EC_CSV_SECOND_TOO_LARGE          */ (Stz*)"EC_CSV_SECOND_TOO_LARGE",
  /* EC_CSV_TIME_INVALID              */ (Stz*)"EC_CSV_TIME_INVALID",
  /* EC_CSV_TIME_SEP_MISSING          */ (Stz*)"EC_CSV_TIME_SEP_MISSING",
  /* EC_CSV_TIMESTAMP_INVALID         */ (Stz*)"EC_CSV_TIMESTAMP_INVALID",
  /* EC_CSV_VARCHAR_INVALID           */ (Stz*)"EC_CSV_VARCHAR_INVALID",
  /* EC_CSV_YEAR_INVALID              */ (Stz*)"EC_CSV_YEAR_INVALID",
  /* EC_CSV_YEAR_TOO_SMALL            */ (Stz*)"EC_CSV_YEAR_TOO_SMALL",
  /* EC_CSV_YEAR_TOO_LARGE            */ (Stz*)"EC_CSV_YEAR_TOO_LARGE",

  /* _D_ */
  /* EC_DECIMAL_NOT_SUPPORTED         */ (Stz*)"decimal only with scale 2 currently supported",
  /* EC_DIVIDE_BY_ZERO                */ (Stz*)"divide by zero",
  /* EC_DUPLICATE_COLUMN              */ (Stz*)"duplicate column",
  /* EC_DUPLICATE_KEY                 */ (Stz*)"duplicate key",

  /* _E_ */
  /* EC_ESCAPE_SINGLE_CHAR            */ (Stz*)"EC_ESCAPE_SINGLE_CHAR",           // SQL Compiler Subsystem
  /* EC_EXPLAIN_INVALID_FORMAT        */ (Stz*)"EC_EXPLAIN_INVALID_FORMAT",       // SQL Compiler Subsystem
  /* EC_EXPR_AGG_MIX_NO_GROUPBY_LIST  */ (Stz*)"EC_EXPR_AGG_MIX_NO_GROUPBY_LIST", // SQL Compiler Subsystem
  /* EC_EXPR_INVALID_COMBINATION      */ (Stz*)"EC_EXPR_INVALID_COMBINATION",     // SQL Compiler Subsystem
  /* EC_EXPR_IS_AGG_IN_GROUPBY_LIST   */ (Stz*)"EC_EXPR_IS_AGG_IN_GROUPBY_LIST",  // SQL Compiler Subsystem
  /* EC_EXPR_IS_LIT_IN_GROUPBY_LIST   */ (Stz*)"EC_EXPR_IS_LIT_IN_GROUPBY_LIST",  // SQL Compiler Subsystem
  /* EC_EXPR_MUST_BE_BOOLEAN          */ (Stz*)"EC_EXPR_MUST_BE_BOOLEAN",         // SQL Compiler Subsystem
  /* EC_EXPR_MUST_BE_EQUIJOIN         */ (Stz*)"EC_EXPR_MUST_BE_EQUIJOIN",        // SQL Compiler Subsystem
  /* EC_EXPR_NOT_IN_GROUPBY_LIST      */ (Stz*)"EC_EXPR_NOT_IN_GROUPBY_LIST",     // SQL Compiler Subsystem
  /* EC_EXPR_NOT_PROJECTED            */ (Stz*)"EC_EXPR_NOT_PROJECTED",           // SQL Compiler Subsystem
  /* EC_EXPR_TYPE_INCOMPATIBLE        */ (Stz*)"EC_EXPR_TYPE_INCOMPATIBLE",           // SQL Compiler Subsystem

  /* _F_ */
  /* EC_FOLDER_IN_USE                 */ (Stz*)"folder in use",
  /* EC_FOLDER_NOT_EMPTY              */ (Stz*)"folder not empty",

  /* _G_ */

  /* _H_ */
  /* EC_HAVING_EXPR_NOT_AGG_OR_IN_GROUPBY */ (Stz*)"EC_HAVING_EXPR_NOT_AGG_OR_IN_GROUPBY", // SQL Compiler Subsystem
  /* EC_HINT_VALUE_UNRECOGNISED           */ (Stz*)"EC_HINT_VALUE_UNRECOGNISED",           // SQL Compiler Subsystem
  /* EC_HINT_VALUE_MISSING                */ (Stz*)"EC_HINT_VALUE_MISSING",                // SQL Compiler Subsystem

  /* _I_ */
  /* EC_IDXVFY_ADJACENT_ENTRIES       */ (Stz*)"EC_IDXVFY_ADJACENT_ENTRIES",
  /* EC_IDXVFY_BUFFER_REFS            */ (Stz*)"EC_IDXVFY_BUFFER_REFS",
  /* EC_IDXVFY_FIRST_KEYS             */ (Stz*)"EC_IDXVFY_FIRST_KEYS",
  /* EC_IDXVFY_ITEMS                  */ (Stz*)"EC_IDXVFY_ITEMS",
  /* EC_IDXVFY_KEY_BUFFER             */ (Stz*)"EC_IDXVFY_KEY_BUFFER",
  /* EC_IDXVFY_KEY_ORDER              */ (Stz*)"EC_IDXVFY_KEY_ORDER",
  /* EC_IDXVFY_NEXT_PREV              */ (Stz*)"EC_IDXVFY_NEXT_PREV",
  /* EC_IDXVFY_ROW_REFS               */ (Stz*)"EC_IDXVFY_ROW_REFS",
  /* EC_IDXVFY_SKIPLIST_ENTRY         */ (Stz*)"EC_IDXVFY_SKIPLIST_ENTRY",
  /* EC_IDXVFY_SKIPLIST_LEVELS        */ (Stz*)"EC_IDXVFY_SKIPLIST_LEVELS",
  /* EC_INDEX_EXISTS                  */ (Stz*)"index exists",
  /* EC_INSUFFICIENT_EXPRS            */ (Stz*)"EC_INSUFFICIENT_EXPRS",         // SQL Compiler Subsystem
  /* EC_INTERNAL_ERROR                */ (Stz*)"EC_INTERNAL_ERROR",
  /* EC_INTERVAL_NOT_SUPPORTED        */ (Stz*)"interval currently not supported",
  /* EC_INVALID_CLASS                 */ (Stz*)"invalid class",
  /* EC_INVALID_COLUMN                */ (Stz*)"invalid column",
  /* EC_INVALID_COLUMNS_UNIQUE_INDEX  */ (Stz*)"invalid columns for unique index (include all partitioning columns)",
  /* EC_INVALID_COMMAND               */ (Stz*)"invalid command",
  /* EC_INVALID_DATE_FORMAT           */ (Stz*)"EC_INVALID_DATE_FORMAT",        // SQL Compiler Subsystem
  /* EC_INVALID_FILE                  */ (Stz*)"invalid file",
  /* EC_INVALID_FLOAT_FORMAT          */ (Stz*)"EC_INVALID_FLOAT_FORMAT",       // SQL Compiler Subsystem
  /* EC_INVALID_INTEGER_FORMAT        */ (Stz*)"EC_INVALID_INTEGER_FORMAT",     // SQL Compiler Subsystem
  /* EC_INVALID_INTERFACE             */ (Stz*)"invalid interface",
  /* EC_INVALID_LIMIT_VALUE           */ (Stz*)"EC_INVALID_LIMIT_VALUE",        // SQL Compiler Subsystem
  /* EC_INVALID_OFFSET_VALUE          */ (Stz*)"EC_INVALID_OFFSET_VALUE",       // SQL Compiler Subsystem
  /* EC_INVALID_PARAMETER             */ (Stz*)"invalid parameter",
  /* EC_INVALID_PARAMETERS            */ (Stz*)"invalid parameters",
  /* EC_INVALID_PARTITION             */ (Stz*)"invalid partition",
  /* EC_INVALID_PARTITION_KEY         */ (Stz*)"invalid partition key",
  /* EC_INVALID_PIPELINE              */ (Stz*)"invalid pipeline",
  /* EC_INVALID_INDEX_COLUMN          */ (Stz*)"invalid index column",
  /* EC_INVALID_INSTANCE              */ (Stz*)"invalid instance",
  /* EC_INVALID_SIZE                  */ (Stz*)"invalid size",
  /* EC_INVALID_SYNTAX                */ (Stz*)"invalid syntax",
  /* EC_INVALID_TIME_FORMAT           */ (Stz*)"EC_INVALID_TIME_FORMAT",        // SQL Compiler Subsystem
  /* EC_INVALID_TIMESTAMP_FORMAT      */ (Stz*)"EC_INVALID_TIMESTAMP_FORMAT",   // SQL Compiler Subsystem
  /* EC_INVALID_TYPES                 */ (Stz*)"EC_INVALID_TYPES",
  /* EC_INVALID_VALUE                 */ (Stz*)"invalid value",

  /* _J_ */

  /* _K_ */

  /* _L_ */
  /* EC_LOW_MEMORY                    */ (Stz*)"low memory",

  /* _M_ */
  /* EC_MAX_INDEXES                   */ (Stz*)"max allowed indexes already created",
  /* EC_MAX_COLUMNS                   */ (Stz*)"max 150 columns supported in a table",
  /* EC_MAX_ROW_LENGTH                */ (Stz*)"max row length allowed is 16k bytes",

  /* _N_ */
  /* EC_NO_COLUMNS_DIC                */ (Stz*)"no columns dictionary",
  /* EC_NO_SUCH_CATALOG               */ (Stz*)"no such catalog",
  /* EC_NO_SUCH_COLUMN                */ (Stz*)"EC_NO_SUCH_COLUMN",     // SQL Compiler Subsystem
  /* EC_NO_SUCH_DIRECTORY             */ (Stz*)"no such directory",
  /* EC_NO_SUCH_FILE                  */ (Stz*)"no such file",
  /* EC_NO_SUCH_FOLDER                */ (Stz*)"no such folder",
  /* EC_NO_SUCH_FUNCTION              */ (Stz*)"no such function",
  /* EC_NO_SUCH_INDEX                 */ (Stz*)"no such index",
  /* EC_NO_SUCH_JOB                   */ (Stz*)"no such job",
  /* EC_NO_SUCH_NODE                  */ (Stz*)"no such node",
  /* EC_NO_SUCH_SCHEMA                */ (Stz*)"no such schema",
  /* EC_NO_SUCH_TABLE                 */ (Stz*)"no such table",
  /* EC_NO_SUCH_TABLE2                */ (Stz*)"EC_NO_SUCH_TABLE2",     // SQL Compiler Subsystem
  /* EC_NO_SUCH_TENANT                */ (Stz*)"no such tenant",
  /* EC_NO_SUCH_WORD                  */ (Stz*)"no such word",
  /* EC_NODE_EXISTS                   */ (Stz*)"node exists",
  /* EC_NOT_A_CATALOG                 */ (Stz*)"not a catalog",
  /* EC_NOT_A_FOLDER                  */ (Stz*)"not a folder",
  /* EC_NOT_AN_INDEX                  */ (Stz*)"not an index",
  /* EC_NOT_A_SCHEMA                  */ (Stz*)"not a schema",
  /* EC_NOT_A_TABLE                   */ (Stz*)"not a table",
  /* EC_NOT_A_TENANT                  */ (Stz*)"not a tenant",
  /* EC_NOT_IMPLEMENTED               */ (Stz*)"EC_NOT_IMPLEMENTED",
  /* EC_NOT_IN_CATALOGS               */ (Stz*)"not in catalogs",
  /* EC_NOT_IN_COLUMNS                */ (Stz*)"not in columns",
  /* EC_NOT_IN_INDEXES                */ (Stz*)"not in indexes",
  /* EC_NOT_IN_PARTITIONS             */ (Stz*)"not in partitions",
  /* EC_NOT_IN_SCHEMAS                */ (Stz*)"not in schemas",
  /* EC_NOT_IN_TABLE                  */ (Stz*)"not in table",
  /* EC_NOT_IN_TABLES                 */ (Stz*)"not in tables",
  /* EC_NOT_IN_TENANTS                */ (Stz*)"not in tenants",
  /* EC_NULL_INTERFACE                */ (Stz*)"null interface",
  /* EC_NULLPTR                       */ (Stz*)"nullptr",

  /* _O_ */
  /* EC_OBJECT_ADDRESS_INVALID        */ (Stz*)"EC_OBJECT_ADDRESS_INVALID",
  /* EC_OBJECT_CLASSNO_INVALID        */ (Stz*)"EC_OBJECT_CLASSNO_INVALID",
  /* EC_OBJECT_FOOTER_INVALID         */ (Stz*)"EC_OBJECT_FOOTER_INVALID",
  /* EC_OBJECT_MAGIC_INVALID          */ (Stz*)"EC_OBJECT_MAGIC_INVALID",
  /* EC_ORDERBY_NOT_SUPPORTED         */ (Stz*)"order by currently not supported",

  /* _P_ */
  /* EC_PIPELINE_PROTOCOL_ERROR       */ (Stz*)"pipeline protocol error",

  /* _Q_ */

  /* _R_ */
  /* EC_RESPONSE_PENDING              */ (Stz*)"response pending",

  /* _S_ */
  /* EC_SCALE_OUT_OF_BOUNDS           */ (Stz*)"scale out of bounds",
  /* EC_SCHEMA_EXISTS                 */ (Stz*)"schema exists",
  /* EC_SE_NOT_RUNNING                */ (Stz*)"SE is not running",
  /* EC_SE_UNKNOWN_ERROR              */ (Stz*)"SE unknown error",
  /* EC_SEMANTIC_ERROR                */ (Stz*)"semantic error",              // SQL Compiler Subsystem
  /* EC_SEND_FAILURE                  */ (Stz*)"send failure",
  /* EC_SUBQUERY_SELF_REFERENCE       */ (Stz*)"EC_SUBQUERY_SELF_REFERENCE",  // SQL Compiler Subsystem
  /* EC_SUBQUERY_PROJ_TOO_MANY_COLS   */ (Stz*)"EC_SUBQUERY_PROJ_TOO_MANY_COLS", // SQL Compiler Subsystem
  /* EC_SYNTAX_ERROR                  */ (Stz*)"syntax error",                // SQL Compiler Subsystem
  /* EC_SYSTEM_CLOSED                 */ (Stz*)"system is closed",

  /* _T_ */
  /* EC_TABLE_ALREADY_LOADED          */ (Stz*)"table already loaded",
  /* EC_TABLE_EXISTS                  */ (Stz*)"table exists",
  /* EC_TABLE_NOT_EMPTY               */ (Stz*)"table not empty",
  /* EC_TABLE_IN_USE                  */ (Stz*)"table in use",
  /* EC_TABLE_SUBQUERY_OUT_OF_SCOPE   */ (Stz*)"EC_TABLE_SUBQUERY_OUT_OF_SCOPE",  // SQL Compiler Subsystem
  /* EC_TENANT_EXISTS                 */ (Stz*)"tenant exists",
  /* EC_TINYC_ERROR                   */ (Stz*)"tinyC compilation error",
  /* EC_TOO_MANY_NAMED_COLUMNS        */ (Stz*)"too many named columns",
  /* EC_TX_BEGIN_FAILED               */ (Stz*)"begin transaction failed",
  /* EC_TX_COMMIT_FAILED              */ (Stz*)"commit transaction failed",
  /* EC_TX_END_FAILED                 */ (Stz*)"end transaction failed",
  /* EC_TX_ROLLBACK_FAILED            */ (Stz*)"rollback transaction failed",

  /* _U_ */
  /* EC_UNRESOLVED_COLUMN_WILDCARD    */ (Stz*)"EC_UNRESOLVED_COLUMN_WILDCARD", // SQL Compiler Subsystem
  /* EC_USING_COLUMN_NOT_FOUND        */ (Stz*)"EC_USING_COLUMN_NOT_FOUND",     // SQL Compiler Subsystem

  /* _V_ */
  /* EC_VALIDATION_FAILURE            */ (Stz*)"validation failure",
  /* EV_VALUE_OUT_OF_RANGE            */ (Stz*)"EV_VALUE_OUT_OF_RANGE",         // SQL Compiler Subsystem
  /* EC_VERSION_MISMATCH              */ (Stz*)"version mismatch",

  /* _W_ */

  /* _X_ */

  /* _Y_ */

  /* _Z_ */
  /* EC_ZZZ_OTHER                     */ (Stz*)"zzz other error (temp filler)",
  
  /* EC_ENUMS                         */ (Stz*)"enums"
};

// This should contain the text string representation of the EC enum.
// Not the error message.
static Stz* aEC2[] = // error code
{
  /* */ (Stz*)"EC_NONE",

  /* */ (Stz*)"EC_DONE",

  // - - - - - - - - - - - - - - - - - - -

  /* _A_ */
  /* EC_AGG_EXPR_IN_WHERE_CLAUSE      */ (Stz*)"EC_AGG_EXPR_IN_WHERE_CLAUSE",       // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_ALIAS               */ (Stz*)"EC_AMBIGUOUS_ALIAS",                // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_COLUMN              */ (Stz*)"EC_AMBIGUOUS_COLUMN",               // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_TABLE               */ (Stz*)"EC_AMBIGUOUS_TABLE",                // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_USING_COLUMN        */ (Stz*)"EC_AMBIGUOUS_USING_COLUMN",         // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_WITH_QUERY          */ (Stz*)"EC_AMBIGUOUS_WITH_QUERY",           // SQL Compiler Subsystem
  /* EC_ARITHMETIC_OVERFLOW,          */ (Stz*)"EC_ARITHMETIC_OVERFLOW",            // Arithmetic operation Overflow
  /* EC_ARITHMETIC_UNDERFLOW,         */ (Stz*)"EC_ARITHMETIC_UNDERFLOW",           // Arithmetic operation Underflow
  /* EC_ASSERT                        */ (Stz*)"EC_ASSERT",

  /* _B_ */
  /* EC_BLANKLINE                     */ (Stz*)"EC_BLANKLINE",

  /* _C_ */
  /* EC_CATALOG_EXISTS                */ (Stz*)"EC_CATALOG_EXISTS",
  /* EC_CLUSTER_NOT_CONNECTED         */ (Stz*)"EC_CLUSTER_NOT_CONNECTED",
  /* EC_CLUSTER_PARTITIONS_MISMATCH   */ (Stz*)"EC_CLUSTER_PARTITIONS_MISMATCH",
  /* EC_CLUSTER_SIZE_MISMATCH         */ (Stz*)"EC_CLUSTER_SIZE_MISMATCH",
  /* EC_COLUMN_COUNT_MISMATCH         */ (Stz*)"EC_COLUMN_COUNT_MISMATCH",          // SQL Compiler Subsystem
  /* EC_COLUMN_TYPE_MISMATCH          */ (Stz*)"EC_COLUMN_TYPE_MISMATCH",           // SQL Compiler Subsystem
  /* EC_CORRBY_COLUMN_NOT_FOUND       */ (Stz*)"EC_CORRBY_COLUMN_NOT_FOUND",        // SQL Compiler Subsystem
  /* EC_CORRBY_COLUMN_TYPE_MISMATCH   */ (Stz*)"EC_CORRBY_COLUMN_TYPE_MISMATCH",    // SQL Compiler Subsystem
  /* EC_CORRBY_NO_COMMON_COLUMNS      */ (Stz*)"EC_CORRBY_NO_COMMON_COLUMNS",       // SQL Compiler Subsystem

  /* EC_CSV_BOOLEAN_INVALID           */ (Stz*)"EC_CSV_BOOLEAN_INVALID",
  /* EC_CSV_BOOLEAN_TOO_LARGE         */ (Stz*)"EC_CSV_BOOLEAN_TOO_LARGE",
  /* EC_CSV_DATE_INVALID              */ (Stz*)"EC_CSV_DATE_INVALID",
  /* EC_CSV_DATE_SEP_MISSING          */ (Stz*)"EC_CSV_DATE_SEP_MISSING",
  /* EC_CSV_DAY_INVALID               */ (Stz*)"EC_CSV_DAY_INVALID",
  /* EC_CSV_DAY_NOT_LEAPYEAR          */ (Stz*)"EC_CSV_DAY_NOT_LEAPYEAR",
  /* EC_CSV_DAY_TOO_LARGE             */ (Stz*)"EC_CSV_DAY_TOO_LARGE",
  /* EC_CSV_DECIMAL_INVALID           */ (Stz*)"EC_CSV_DECIMAL_INVALID",
  /* EC_CSV_DECIMAL_TOO_LARGE         */ (Stz*)"EC_CSV_DECIMAL_TOO_LARGE",
  /* EC_CSV_DECIMAL_TOO_SMALL         */ (Stz*)"EC_CSV_DECIMAL_TOO_SMALL",
  /* EC_CSV_EXPONENT_MISSING          */ (Stz*)"EC_CSV_EXPONENT_MISSING",
  /* EC_CSV_FLOAT_INVALID             */ (Stz*)"EC_CSV_FLOAT_INVALID",
  /* EC_CSV_HOUR_INVALID              */ (Stz*)"EC_CSV_HOUR_INVALID",
  /* EC_CSV_HOUR_TOO_LARGE            */ (Stz*)"EC_CSV_HOUR_TOO_LARGE",
  /* EC_CSV_INTEGER_INVALID           */ (Stz*)"EC_CSV_INTEGER_INVALID",
  /* EC_CSV_INTEGER_TOO_LARGE         */ (Stz*)"EC_CSV_INTEGER_TOO_LARGE",
  /* EC_CSV_INTERVAL_INVALID          */ (Stz*)"EC_CSV_INTERVAL_INVALID",
  /* EC_CSV_INTERVAL_SECS_TOO_LARGE   */ (Stz*)"EC_CSV_INTERVAL_SECS_TOO_LARGE",
  /* EC_CSV_INTERVAL_USECS_TOO_LARGE  */ (Stz*)"EC_CSV_INTERVAL_USECS_TOO_LARGE",
  /* EC_CSV_MINUTE_INVALID            */ (Stz*)"EC_CSV_MINUTE_INVALID",
  /* EC_CSV_MINUTE_TOO_LARGE          */ (Stz*)"EC_CSV_MINUTE_TOO_LARGE",
  /* EC_CSV_MONTH_INVALID             */ (Stz*)"EC_CSV_MONTH_INVALID",
  /* EC_CSV_MONTH_TOO_LARGE           */ (Stz*)"EC_CSV_MONTH_TOO_LARGE",
  /* EC_CSV_SECOND_INVALID            */ (Stz*)"EC_CSV_SECOND_INVALID",
  /* EC_CSV_SECOND_TOO_LARGE          */ (Stz*)"EC_CSV_SECOND_TOO_LARGE",
  /* EC_CSV_TIME_INVALID              */ (Stz*)"EC_CSV_TIME_INVALID",
  /* EC_CSV_TIME_SEP_MISSING          */ (Stz*)"EC_CSV_TIME_SEP_MISSING",
  /* EC_CSV_TIMESTAMP_INVALID         */ (Stz*)"EC_CSV_TIMESTAMP_INVALID",
  /* EC_CSV_VARCHAR_INVALID           */ (Stz*)"EC_CSV_VARCHAR_INVALID",
  /* EC_CSV_YEAR_INVALID              */ (Stz*)"EC_CSV_YEAR_INVALID",
  /* EC_CSV_YEAR_TOO_SMALL            */ (Stz*)"EC_CSV_YEAR_TOO_SMALL",
  /* EC_CSV_YEAR_TOO_LARGE            */ (Stz*)"EC_CSV_YEAR_TOO_LARGE",

  /* _D_ */
  /* EC_DECIMAL_NOT_SUPPORTED         */ (Stz*)"EC_DECIMAL_NOT_SUPPORTED",
  /* EC_DIVIDE_BY_ZERO                */ (Stz*)"EC_DIVIDE_BY_ZERO",
  /* EC_DUPLICATE_COLUMN              */ (Stz*)"EC_DUPLICATE_COLUMN",
  /* EC_DUPLICATE_KEY                 */ (Stz*)"EC_DUPLICATE_KEY",

  /* _E_ */
  /* EC_ESCAPE_SINGLE_CHAR            */ (Stz*)"EC_ESCAPE_SINGLE_CHAR",             // SQL Compiler Subsystem
  /* EC_EXPLAIN_INVALID_FORMAT        */ (Stz*)"EC_EXPLAIN_INVALID_FORMAT",         // SQL Compiler Subsystem
  /* EC_EXPR_AGG_MIX_NO_GROUPBY_LIST  */ (Stz*)"EC_EXPR_AGG_MIX_NO_GROUPBY_LIST",   // SQL Compiler Subsystem
  /* EC_EXPR_INVALID_COMBINATION      */ (Stz*)"EC_EXPR_INVALID_COMBINATION",       // SQL Compiler Subsystem
  /* EC_EXPR_IS_AGG_IN_GROUPBY_LIST   */ (Stz*)"EC_EXPR_IS_AGG_IN_GROUPBY_LIST",    // SQL Compiler Subsystem
  /* EC_EXPR_IS_LIT_IN_GROUPBY_LIST   */ (Stz*)"EC_EXPR_IS_LIT_IN_GROUPBY_LIST",    // SQL Compiler Subsystem
  /* EC_EXPR_MUST_BE_BOOLEAN          */ (Stz*)"EC_EXPR_MUST_BE_BOOLEAN",           // SQL Compiler Subsystem
  /* EC_EXPR_MUST_BE_EQUIJOIN         */ (Stz*)"EC_EXPR_MUST_BE_EQUIJOIN",          // SQL Compiler Subsystem
  /* EC_EXPR_NOT_IN_GROUPBY_LIST      */ (Stz*)"EC_EXPR_NOT_IN_GROUPBY_LIST",       // SQL Compiler Subsystem
  /* EC_EXPR_NOT_PROJECTED            */ (Stz*)"EC_EXPR_NOT_PROJECTED",             // SQL Compiler Subsystem
  /* EC_EXPR_TYPE_INCOMPATIBLE        */ (Stz*)"EC_EXPR_TYPE_INCOMPATIBLE",             // SQL Compiler Subsystem

  /* _F_ */
  /* EC_FOLDER_IN_USE                 */ (Stz*)"EC_FOLDER_IN_USE",
  /* EC_FOLDER_NOT_EMPTY              */ (Stz*)"EC_FOLDER_NOT_EMPTY",

  /* _G_ */

  /* _H_ */
  /* EC_HAVING_EXPR_NOT_AGG_OR_IN_GROUPBY */ (Stz*)"EC_HAVING_EXPR_NOT_AGG_OR_IN_GROUPBY",  // SQL Compiler Subsystem
  /* EC_HINT_VALUE_UNRECOGNISED           */ (Stz*)"EC_HINT_VALUE_UNRECOGNISED",            // SQL Compiler Subsystem
  /* EC_HINT_VALUE_MISSING                */ (Stz*)"EC_HINT_VALUE_MISSING",                 // SQL Compiler Subsystem

  /* _I_ */
  /* EC_IDXVFY_ADJACENT_ENTRIES       */ (Stz*)"EC_IDXVFY_ADJACENT_ENTRIES",
  /* EC_IDXVFY_BUFFER_REFS            */ (Stz*)"EC_IDXVFY_BUFFER_REFS",
  /* EC_IDXVFY_FIRST_KEYS             */ (Stz*)"EC_IDXVFY_FIRST_KEYS",
  /* EC_IDXVFY_ITEMS                  */ (Stz*)"EC_IDXVFY_ITEMS",
  /* EC_IDXVFY_KEY_BUFFER             */ (Stz*)"EC_IDXVFY_KEY_BUFFER",
  /* EC_IDXVFY_KEY_ORDER              */ (Stz*)"EC_IDXVFY_KEY_ORDER",
  /* EC_IDXVFY_NEXT_PREV              */ (Stz*)"EC_IDXVFY_NEXT_PREV",
  /* EC_IDXVFY_ROW_REFS               */ (Stz*)"EC_IDXVFY_ROW_REFS",
  /* EC_IDXVFY_SKIPLIST_ENTRY         */ (Stz*)"EC_IDXVFY_SKIPLIST_ENTRY",
  /* EC_IDXVFY_SKIPLIST_LEVELS        */ (Stz*)"EC_IDXVFY_SKIPLIST_LEVELS",
  /* EC_INDEX_EXISTS                  */ (Stz*)"EC_INDEX_EXISTS",
  /* EC_INSUFFICIENT_EXPRS            */ (Stz*)"EC_INSUFFICIENT_EXPRS",
  /* EC_INTERNAL_ERROR                */ (Stz*)"EC_INTERNAL_ERROR",
  /* EC_INTERVAL_NOT_SUPPORTED        */ (Stz*)"EC_INTERVAL_NOT_SUPPORTED",
  /* EC_INVALID_CLASS                 */ (Stz*)"EC_INVALID_CLASS",
  /* EC_INVALID_COLUMN                */ (Stz*)"EC_INVALID_COLUMN",
  /* EC_INVALID_COLUMNS_UNIQUE_IND    */ (Stz*)"EC_INVALID_COLUMNS_UNIQUE_INDEX",
  /* EC_INVALID_COMMAND               */ (Stz*)"EC_INVALID_COMMAND",
  /* EC_INVALID_DATE_FORMAT           */ (Stz*)"EC_INVALID_DATE_FORMAT",            // SQL Compiler Subsystem
  /* EC_INVALID_FILE                  */ (Stz*)"EC_INVALID_FILE",
  /* EC_INVALID_FLOAT_FORMAT          */ (Stz*)"EC_INVALID_FLOAT_FORMAT",           // SQL Compiler Subsystem
  /* EC_INVALID_INTEGER_FORMAT        */ (Stz*)"EC_INVALID_INTEGER_FORMAT",         // SQL Compiler Subsystem
  /* EC_INVALID_INTERFACE             */ (Stz*)"EC_INVALID_INTERFACE",
  /* EC_INVALID_LIMIT_VALUE           */ (Stz*)"EC_INVALID_LIMIT_VALUE",            // SQL Compiler Subsystem
  /* EC_INVALID_OFFSET_VALUE          */ (Stz*)"EC_INVALID_OFFSET_VALUE",           // SQL Compiler Subsystem
  /* EC_INVALID_PARAMETER             */ (Stz*)"EC_INVALID_PARAMETER",
  /* EC_INVALID_PARAMETERS            */ (Stz*)"EC_INVALID_PARAMETERS",
  /* EC_INVALID_PARTITION             */ (Stz*)"EC_INVALID_PARTITION",
  /* EC_INVALID_PARTITION_KEY         */ (Stz*)"EC_INVALID_PARTITION_KEY",
  /* EC_INVALID_PIPELINE              */ (Stz*)"EC_INVALID_PIPELINE",
  /* EC_INVALID_INDEX_COLUMN          */ (Stz*)"EC_INVALID_INDEX_COLUMN",
  /* EC_INVALID_INSTANCE              */ (Stz*)"EC_INVALID_INSTANCE",
  /* EC_INVALID_SIZE                  */ (Stz*)"EC_INVALID_SIZE",
  /* EC_INVALID_SYNTAX                */ (Stz*)"EC_INVALID_SYNTAX",
  /* EC_INVALID_TIME_FORMAT           */ (Stz*)"EC_INVALID_TIME_FORMAT",            // SQL Compiler Subsystem
  /* EC_INVALID_TIMESTAMP_FORMAT      */ (Stz*)"EC_INVALID_TIMESTAMP_FORMAT",       // SQL Compiler Subsystem
  /* EC_INVALID_TYPES                 */ (Stz*)"EC_INVALID_TYPES",
  /* EC_INVALID_VALUE                 */ (Stz*)"EC_INVALID_VALUE",

  /* _J_ */

  /* _K_ */

  /* _L_ */
  /* EC_LOW_MEMORY                    */ (Stz*)"EC_LOW_MEMORY",

  /* _M_ */
  /* EC_MAX_INDEXES                   */ (Stz*)"EC_MAX_INDEXES",
  /* EC_MAX_COLUMNS                   */ (Stz*)"EC_MAX_COLUMNS",
  /* EC_MAX_ROW_LENGTH                */ (Stz*)"EC_MAX_ROW_LENGTH",

  /* _N_ */
  /* EC_NO_COLUMNS_DIC                */ (Stz*)"EC_NO_COLUMNS_DIC",
  /* EC_NO_SUCH_CATALOG               */ (Stz*)"EC_NO_SUCH_CATALOG",
  /* EC_NO_SUCH_COLUMN                */ (Stz*)"EC_NO_SUCH_COLUMN",                 // SQL Compiler Subsystem
  /* EC_NO_SUCH_DIRECTORY             */ (Stz*)"EC_NO_SUCH_DIRECTORY",
  /* EC_NO_SUCH_FILE                  */ (Stz*)"EC_NO_SUCH_FILE",
  /* EC_NO_SUCH_FOLDER                */ (Stz*)"EC_NO_SUCH_FOLDER",
  /* EC_NO_SUCH_FUNCTION              */ (Stz*)"EC_NO_SUCH_FUNCTION",
  /* EC_NO_SUCH_INDEX                 */ (Stz*)"EC_NO_SUCH_INDEX",
  /* EC_NO_SUCH_JOB                   */ (Stz*)"EC_NO_SUCH_JOB",
  /* EC_NO_SUCH_NODE                  */ (Stz*)"EC_NO_SUCH_NODE",
  /* EC_NO_SUCH_SCHEMA                */ (Stz*)"EC_NO_SUCH_SCHEMA",
  /* EC_NO_SUCH_TABLE                 */ (Stz*)"EC_NO_SUCH_TABLE",
  /* EC_NO_SUCH_TABLE2                */ (Stz*)"EC_NO_SUCH_TABLE2",                 // SQL Compiler Subsystem
  /* EC_NO_SUCH_TENANT                */ (Stz*)"EC_NO_SUCH_TENANT",
  /* EC_NO_SUCH_WORD                  */ (Stz*)"EC_NO_SUCH_WORD",
  /* EC_NODE_EXISTS                   */ (Stz*)"EC_NODE_EXISTS",
  /* EC_NOT_A_CATALOG                 */ (Stz*)"EC_NOT_A_CATALOG",
  /* EC_NOT_A_FOLDER                  */ (Stz*)"EC_NOT_A_FOLDER",
  /* EC_NOT_AN_INDEX                  */ (Stz*)"EC_NOT_AN_INDEX",
  /* EC_NOT_A_SCHEMA                  */ (Stz*)"EC_NOT_A_SCHEMA",
  /* EC_NOT_A_TABLE                   */ (Stz*)"EC_NOT_A_TABLE",
  /* EC_NOT_A_TENANT                  */ (Stz*)"EC_NOT_A_TENANT",
  /* EC_NOT_IMPLEMENTED               */ (Stz*)"EC_NOT_IMPLEMENTED",
  /* EC_NOT_IN_CATALOGS               */ (Stz*)"EC_NOT_IN_CATALOGS",
  /* EC_NOT_IN_COLUMNS                */ (Stz*)"EC_NOT_IN_COLUMNS",
  /* EC_NOT_IN_INDEXES                */ (Stz*)"EC_NOT_IN_INDEXES",
  /* EC_NOT_IN_PARTITIONS             */ (Stz*)"EC_NOT_IN_PARTITIONS",
  /* EC_NOT_IN_SCHEMAS                */ (Stz*)"EC_NOT_IN_SCHEMAS",
  /* EC_NOT_IN_TABLE                  */ (Stz*)"EC_NOT_IN_TABLE",
  /* EC_NOT_IN_TABLES                 */ (Stz*)"EC_NOT_IN_TABLES",
  /* EC_NOT_IN_TENANTS                */ (Stz*)"EC_NOT_IN_TENANTS",
  /* EC_NULL_INTERFACE                */ (Stz*)"EC_NULL_INTERFACE",
  /* EC_NULLPTR                       */ (Stz*)"EC_NULLPTR",

  /* _O_ */
  /* EC_OBJECT_ADDRESS_INVALID        */ (Stz*)"EC_OBJECT_ADDRESS_INVALID",
  /* EC_OBJECT_CLASSNO_INVALID        */ (Stz*)"EC_OBJECT_CLASSNO_INVALID",
  /* EC_OBJECT_FOOTER_INVALID         */ (Stz*)"EC_OBJECT_FOOTER_INVALID",
  /* EC_OBJECT_MAGIC_INVALID          */ (Stz*)"EC_OBJECT_MAGIC_INVALID",
  /* EC_ORDERBY_NOT_SUPPORTED         */ (Stz*)"EC_ORDERBY_NOT_SUPPORTED",

  /* _P_ */
  /* EC_PIPELINE_PROTOCOL_ERROR       */ (Stz*)"EC_PIPELINE_PROTOCOL_ERROR",

  /* _Q_ */

  /* _R_ */
  /* EC_RESPONSE_PENDING              */ (Stz*)"EC_RESPONSE_PENDING",

  /* _S_ */
  /* EC_SCALE_OUT_OF_BOUNDS           */ (Stz*)"EC_SCALE_OUT_OF_BOUNDS",
  /* EC_SCHEMA_EXISTS                 */ (Stz*)"EC_SCHEMA_EXISTS",
  /* EC_SE_NOT_RUNNING                */ (Stz*)"EC_SE_NOT_RUNNING",
  /* EC_SE_UNKNOWN_ERROR              */ (Stz*)"EC_SE_UNKNOWN_ERROR",
  /* EC_SEMANTIC_ERROR                */ (Stz*)"EC_SEMANTIC_ERROR",                 // SQL Compiler Subsystem
  /* EC_SEND_FAILURE                  */ (Stz*)"EC_SEND_FAILURE",
  /* EC_SUBQUERY_SELF_REFERENCE       */ (Stz*)"EC_SUBQUERY_SELF_REFERENCE",        // SQL Compiler Subsystem
  /* EC_SUBQUERY_PROJ_TOO_MANY_COLS   */ (Stz*)"EC_SUBQUERY_PROJ_TOO_MANY_COLS",    // SQL Compiler Subsystem
  /* EC_SYNTAX_ERROR                  */ (Stz*)"EC_SYNTAX_ERROR",                   // SQL Compiler Subsystem
  /* EC_SYSTEM_CLOSED                 */ (Stz*)"EC_SYSTEM_CLOSED",

  /* _T_ */
  /* EC_TABLE_ALREADY_LOADED          */ (Stz*)"EC_TABLE_ALREADY_LOADED",
  /* EC_TABLE_EXISTS                  */ (Stz*)"EC_TABLE_EXISTS",
  /* EC_TABLE_NOT_EMPTY               */ (Stz*)"EC_TABLE_NOT_EMPTY",
  /* EC_TABLE_IN_USE                  */ (Stz*)"EC_TABLE_IN_USE",
  /* EC_TABLE_SUBQUERY_OUT_OF_SCOPE   */ (Stz*)"EC_TABLE_SUBQUERY_OUT_OF_SCOPE",
  /* EC_TENANT_EXISTS                 */ (Stz*)"EC_TENANT_EXISTS",
  /* EC_TINYC_ERROR                   */ (Stz*)"EC_TINYC_ERROR",
  /* EC_TOO_MANY_NAMED_COLUMNS        */ (Stz*)"EC_TOO_MANY_NAMED_COLUMNS",
  /* EC_TX_BEGIN_FAILED               */ (Stz*)"EC_TX_BEGIN_FAILED",
  /* EC_TX_COMMIT_FAILED              */ (Stz*)"EC_TX_COMMIT_FAILED",
  /* EC_TX_END_FAILED                 */ (Stz*)"EC_TX_END_FAILED",
  /* EC_TX_ROLLBACK_FAILED            */ (Stz*)"EC_TX_ROLLBACK_FAILED",

  /* _U_ */
  /* EC_UNRESOLVED_COLUMN_WILDCARD    */ (Stz*)"EC_UNRESOLVED_COLUMN_WILDCARD",     // SQL Compiler Subsystem
  /* EC_USING_COLUMN_NOT_FOUND        */ (Stz*)"EC_USING_COLUMN_NOT_FOUND",         // SQL Compiler Subsystem

  /* _V_ */
  /* EC_VALIDATION_FAILURE            */ (Stz*)"EC_VALIDATION_FAILURE",
  /* EV_VALUE_OUT_OF_RANGE            */ (Stz*)"EV_VALUE_OUT_OF_RANGE",             // SQL Compiler Subsystem
  /* EC_VERSION_MISMATCH              */ (Stz*)"EC_VERSION_MISMATCH",

  /* _W_ */

  /* _X_ */

  /* _Y_ */

  /* _Z_ */
    /* EC_ZZZ_OTHER                     */ (Stz*)"EC_ZZZ_OTHER",

    /* EC_ENUMS                         */ (Stz*)"EC_ENUMS"
};


// Error messages corresponding to the EC enum above.
//
// A good practice here would be to write error message such that any
// parameters are printed at the end of the message rather than
// throughout the error message. That way it is easier to search for
// and easier to document.
//
// e.g. rather than printing "The table %s is not empty", a better way
// of structuring this is "Table is not empty: %s".
// That way the error can be easily documented as "Table is not empty".
// Parameters could be dynamically appended to these error messages as
// is necessary.
static Stz* aErrorMsg[] =
{
  /* EC_NONE                          */ (Stz*)"none",

  /* EC_DONE                          */ (Stz*)"done",

  // - - - - - - - - - - - - - - - - - - -
  /* _A_ */
  /* EC_AGG_EXPR_IN_WHERE_CLAUSE      */ (Stz*)"Cannot specify an aggregate expression in a WHERE clause", // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_ALIAS               */ (Stz*)"Ambiguous alias name",      // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_COLUMN              */ (Stz*)"Ambiguous column name",     // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_TABLE               */ (Stz*)"Ambiguous table name",      // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_USING_COLUMN        */ (Stz*)"Ambiguous column name in USING clause",     // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_WITH_QUERY          */ (Stz*)"Ambiguous alias name in WITH clause",       // SQL Compiler Subsystem
  /* EC_ARITHMETIC_OVERFLOW           */ (Stz*)"Arithmetic overflow",     // Arithmetic overflow
  /* EC_ARITHMETIC_UNDERFLOW          */ (Stz*)"Arithmetic underflow",    // Arithmetic overflow
  /* EC_ASSERT                        */ (Stz*)"assert",

  /* _B_ */
  /* EC_BLANKLINE                     */ (Stz*)"blank line",

  /* _C_ */
  /* EC_CATALOG_EXISTS                */ (Stz*)"catalog exists",
  /* EC_CLUSTER_NOT_CONNECTED         */ (Stz*)"cluster not connected (use connectCluster command)",
  /* EC_CLUSTER_PARTITIONS_MISMATCH   */ (Stz*)"cluster partitions per node mismatch",
  /* EC_CLUSTER_SIZE_MISMATCH         */ (Stz*)"cluster node size mismatch",
  /* EC_COLUMN_COUNT_MISMATCH         */ (Stz*)"The number of columns of each subquery do not match",   // SQL Compiler Subsystem
  /* EC_COLUMN_TYPE_MISMATCH          */ (Stz*)"Column types do not match",                             // SQL Compiler Subsystem
  /* EC_CORRBY_COLUMN_NOT_FOUND       */ (Stz*)"Column in CORRESPONDING BY list is not found in select list",   // SQL Compiler Subsystem
  /* EC_CORRBY_COLUMN_TYPE_MISMATCH   */ (Stz*)"Column in CORRESPONDING BY list has mismatched types in child queries", // SQL Compiler Subsystem
  /* EC_CORRBY_NO_COMMON_COLUMNS      */ (Stz*)"No common column names of the subqueries could be found for the CORRESPONDING list",  // SQL Compiler Subsystem

  /* EC_CSV_BOOLEAN_INVALID           */ (Stz*)"EC_CSV_BOOLEAN_INVALID",
  /* EC_CSV_BOOLEAN_TOO_LARGE         */ (Stz*)"EC_CSV_BOOLEAN_TOO_LARGE",
  /* EC_CSV_DATE_INVALID              */ (Stz*)"EC_CSV_DATE_INVALID",
  /* EC_CSV_DATE_SEP_MISSING          */ (Stz*)"EC_CSV_DATE_SEP_MISSING",
  /* EC_CSV_DAY_INVALID               */ (Stz*)"EC_CSV_DAY_INVALID",
  /* EC_CSV_DAY_NOT_LEAPYEAR          */ (Stz*)"EC_CSV_DAY_NOT_LEAPYEAR",
  /* EC_CSV_DAY_TOO_LARGE             */ (Stz*)"EC_CSV_DAY_TOO_LARGE",
  /* EC_CSV_DECIMAL_INVALID           */ (Stz*)"EC_CSV_DECIMAL_INVALID",
  /* EC_CSV_DECIMAL_TOO_LARGE         */ (Stz*)"EC_CSV_DECIMAL_TOO_LARGE",
  /* EC_CSV_DECIMAL_TOO_SMALL         */ (Stz*)"EC_CSV_DECIMAL_TOO_SMALL",
  /* EC_CSV_EXPONENT_MISSING          */ (Stz*)"EC_CSV_EXPONENT_MISSING",
  /* EC_CSV_FLOAT_INVALID             */ (Stz*)"EC_CSV_FLOAT_INVALID",
  /* EC_CSV_HOUR_INVALID              */ (Stz*)"EC_CSV_HOUR_INVALID",
  /* EC_CSV_HOUR_TOO_LARGE            */ (Stz*)"EC_CSV_HOUR_TOO_LARGE",
  /* EC_CSV_INTEGER_INVALID           */ (Stz*)"EC_CSV_INTEGER_INVALID",
  /* EC_CSV_INTEGER_TOO_LARGE         */ (Stz*)"EC_CSV_INTEGER_TOO_LARGE",
  /* EC_CSV_INTERVAL_INVALID          */ (Stz*)"EC_CSV_INTERVAL_INVALID",
  /* EC_CSV_INTERVAL_SECS_TOO_LARGE   */ (Stz*)"EC_CSV_INTERVAL_SECS_TOO_LARGE",
  /* EC_CSV_INTERVAL_USECS_TOO_LARGE  */ (Stz*)"EC_CSV_INTERVAL_USECS_TOO_LARGE",
  /* EC_CSV_MINUTE_INVALID            */ (Stz*)"EC_CSV_MINUTE_INVALID",
  /* EC_CSV_MINUTE_TOO_LARGE          */ (Stz*)"EC_CSV_MINUTE_TOO_LARGE",
  /* EC_CSV_MONTH_INVALID             */ (Stz*)"EC_CSV_MONTH_INVALID",
  /* EC_CSV_MONTH_TOO_LARGE           */ (Stz*)"EC_CSV_MONTH_TOO_LARGE",
  /* EC_CSV_SECOND_INVALID            */ (Stz*)"EC_CSV_SECOND_INVALID",
  /* EC_CSV_SECOND_TOO_LARGE          */ (Stz*)"EC_CSV_SECOND_TOO_LARGE",
  /* EC_CSV_TIME_INVALID              */ (Stz*)"EC_CSV_TIME_INVALID",
  /* EC_CSV_TIME_SEP_MISSING          */ (Stz*)"EC_CSV_TIME_SEP_MISSING",
  /* EC_CSV_TIMESTAMP_INVALID         */ (Stz*)"EC_CSV_TIMESTAMP_INVALID",
  /* EC_CSV_VARCHAR_INVALID           */ (Stz*)"EC_CSV_VARCHAR_INVALID",
  /* EC_CSV_YEAR_INVALID              */ (Stz*)"EC_CSV_YEAR_INVALID",
  /* EC_CSV_YEAR_TOO_SMALL            */ (Stz*)"EC_CSV_YEAR_TOO_SMALL",
  /* EC_CSV_YEAR_TOO_LARGE            */ (Stz*)"EC_CSV_YEAR_TOO_LARGE",

  /* _D_ */
  /* EC_DECIMAL_NOT_SUPPORTED         */ (Stz*)"decimal only with scale 2 currently supported",
  /* EC_DECIMAL_NOT_SUPPORTED         */ (Stz*)"divide by zero",
  /* EC_DUPLICATE_COLUMN              */ (Stz*)"duplicate column",
  /* EC_DUPLICATE_KEY                 */ (Stz*)"duplicate key",

  /* _E_ */
  /* EC_ESCAPE_SINGLE_CHAR            */ (Stz*)"ESCAPE clause can only contain a single character", // SQL Compiler Subsystem
  /* EC_EXPLAIN_INVALID_FORMAT        */ (Stz*)"Invalid EXPLAIN format. Must be 'PRETTY' or 'ROW'", // SQL Compiler Subsystem
  /* EC_EXPR_AGG_MIX_NO_GROUPBY_LIST  */ (Stz*)"Cannot select both aggregate and non-aggregate expressions without a GROUP BY clause", // SQL Compiler Subsystem
  /* EC_EXPR_INVALID_COMBINATION      */ (Stz*)"Expression type combination is invalid or unsupported for the expression operator",    // SQL Compiler Subsystem
  /* EC_EXPR_IS_AGG_IN_GROUPBY_LIST   */ (Stz*)"Cannot have an aggregate expression in the GROUP BY list", // SQL Compiler Subsystem
  /* EC_EXPR_IS_LIT_IN_GROUPBY_LIST   */ (Stz*)"Cannot have a literal expression in the GROUP BY list",    // SQL Compiler Subsystem
  /* EC_EXPR_MUST_BE_BOOLEAN          */ (Stz*)"Predicate expression must have a boolean result",          // SQL Compiler Subsystem
  /* EC_EXPR_MUST_BE_EQUIJOIN         */ (Stz*)"Join predicate expression must be an equi-join and contain an equality comparison",           // SQL Compiler Subsystem
  /* EC_EXPR_NOT_IN_GROUPBY_LIST      */ (Stz*)"Expression is not present in the GROUP BY list",           // SQL Compiler Subsystem
  /* EC_EXPR_NOT_PROJECTED            */ (Stz*)"Invalid expression because it is not being projected",     // SQL Compiler Subsystem
  /* EC_EXPR_TYPE_INCOMPATIBLE        */ (Stz*)"Incompatible expression types",                            // SQL Compiler Subsystem

  /* _F_ */
  /* EC_FOLDER_IN_USE                 */ (Stz*)"folder in use",
  /* EC_FOLDER_NOT_EMPTY              */ (Stz*)"folder not empty",

  /* _G_ */

  /* _H_ */
  /* EC_HAVING_EXPR_NOT_AGG_OR_IN_GROUPBY */ (Stz*)"The HAVING expression is not an aggregate or not in the GROUP BY list",  // SQL Compiler Subsystem
  /* EC_HINT_VALUE_UNRECOGNISED           */ (Stz*)"Unrecognised hint value",             // SQL Compiler Subsystem
  /* EC_HINT_VALUE_MISSING                */ (Stz*)"A value must be specified for hint",  // SQL Compiler Subsystem

  /* _I_ */
  /* EC_IDXVFY_ADJACENT_ENTRIES       */ (Stz*)"EC_IDXVFY_ADJACENT_ENTRIES",
  /* EC_IDXVFY_BUFFER_REFS            */ (Stz*)"EC_IDXVFY_BUFFER_REFS",
  /* EC_IDXVFY_FIRST_KEYS             */ (Stz*)"EC_IDXVFY_FIRST_KEYS",
  /* EC_IDXVFY_ITEMS                  */ (Stz*)"EC_IDXVFY_ITEMS",
  /* EC_IDXVFY_KEY_BUFFER             */ (Stz*)"EC_IDXVFY_KEY_BUFFER",
  /* EC_IDXVFY_KEY_ORDER              */ (Stz*)"EC_IDXVFY_KEY_ORDER",
  /* EC_IDXVFY_NEXT_PREV              */ (Stz*)"EC_IDXVFY_NEXT_PREV",
  /* EC_IDXVFY_ROW_REFS               */ (Stz*)"EC_IDXVFY_ROW_REFS",
  /* EC_IDXVFY_SKIPLIST_ENTRY         */ (Stz*)"EC_IDXVFY_SKIPLIST_ENTRY",
  /* EC_IDXVFY_SKIPLIST_LEVELS        */ (Stz*)"EC_IDXVFY_SKIPLIST_LEVELS",
  /* EC_INDEX_EXISTS                  */ (Stz*)"index exists",
  /* EC_INSUFFICIENT_EXPRS            */ (Stz*)"insufficient expressions specified",          // SQL Compiler Subsystem
  /* EC_INTERNAL_ERROR                */ (Stz*)"internal error",
  /* EC_INTERVAL_NOT_SUPPORTED        */ (Stz*)"interval currently not supported",
  /* EC_INVALID_CLASS                 */ (Stz*)"invalid class",
  /* EC_INVALID_COLUMN                */ (Stz*)"invalid column",
  /* EC_INVALID_COLUMNS_UNIQUE_IND    */ (Stz*)"invalid columns for unique index (include all partitioning columns)",
  /* EC_INVALID_COMMAND               */ (Stz*)"invalid command",
  /* EC_INVALID_DATE_FORMAT           */ (Stz*)"Invalid date string format. Must be of the form 'YYYY-MM-DD'",    // SQL Compiler Subsystem
  /* EC_INVALID_FILE                  */ (Stz*)"File does not exist : '%v'",
  /* EC_INVALID_FLOAT_FORMAT          */ (Stz*)"Invalid format of floating point literal",    // SQL Compiler Subsystem
  /* EC_INVALID_INTEGER_FORMAT        */ (Stz*)"Invalid format of integer literal",           // SQL Compiler Subsystem
  /* EC_INVALID_INTERFACE             */ (Stz*)"invalid interface",
  /* EC_INVALID_LIMIT_VALUE           */ (Stz*)"LIMIT value must be >= 0",                    // SQL Compiler Subsystem
  /* EC_INVALID_OFFSET_VALUE          */ (Stz*)"OFFSET value must be >= 0",                   // SQL Compiler Subsystem
  /* EC_INVALID_PARAMETER             */ (Stz*)"invalid parameter",
  /* EC_INVALID_PARAMETERS            */ (Stz*)"invalid parameters",
  /* EC_INVALID_PARTITION             */ (Stz*)"invalid partition",
  /* EC_INVALID_PARTITION_KEY         */ (Stz*)"invalid partition key",
  /* EC_INVALID_PIPELINE              */ (Stz*)"invalid pipeline",
  /* EC_INVALID_INDEX_COLUMN          */ (Stz*)"invalid index column",
  /* EC_INVALID_INSTANCE              */ (Stz*)"invalid instance",
  /* EC_INVALID_SIZE                  */ (Stz*)"invalid size",
  /* EC_INVALID_SYNTAX                */ (Stz*)"invalid syntax",
  /* EC_INVALID_TIME_FORMAT           */ (Stz*)"Invalid time string format. Must be of the form: 'hh:mm:ss'",    // SQL Compiler Subsystem
  /* EC_INVALID_TIMESTAMP_FORMAT      */ (Stz*)"Invalid timestamp string format. Must be of the form: 'YYYY-MM-DD hh:mm:ss[.ffffff]'",    // SQL Compiler Subsystem
  /* EC_INVALID_TYPES                 */ (Stz*)"Invalid types in arithmetic/logical/relational operation",
  /* EC_INVALID_VALUE                 */ (Stz*)"invalid value",

  /* _J_ */

  /* _K_ */

  /* _L_ */
  /* EC_LOW_MEMORY                    */ (Stz*)"low memory",

  /* _M_ */
  /* EC_MAX_INDEXES                   */ (Stz*)"max allowed indexes already created",
  /* EC_MAX_COLUMNS                   */ (Stz*)"max 150 columns supported in a table",
  /* EC_MAX_ROW_LENGTH                */ (Stz*)"max row length allowed is 16k bytes",

  /* _N_ */
  /* EC_NO_COLUMNS_DIC                */ (Stz*)"no columns dictionary",
  /* EC_NO_SUCH_CATALOG               */ (Stz*)"no such catalog",
  /* EC_NO_SUCH_COLUMN                */ (Stz*)"No such column",
  /* EC_NO_SUCH_DIRECTORY             */ (Stz*)"no such directory",
  /* EC_NO_SUCH_FILE                  */ (Stz*)"no such file",
  /* EC_NO_SUCH_FOLDER                */ (Stz*)"no such folder",
  /* EC_NO_SUCH_FUNCTION              */ (Stz*)"no such function",
  /* EC_NO_SUCH_INDEX                 */ (Stz*)"no such index",
  /* EC_NO_SUCH_JOB                   */ (Stz*)"no such job",
  /* EC_NO_SUCH_NODE                  */ (Stz*)"no such node",
  /* EC_NO_SUCH_SCHEMA                */ (Stz*)"no such schema",
  /* EC_NO_SUCH_TABLE                 */ (Stz*)"Table does not exist : '%v'",
  /* EC_NO_SUCH_TABLE2                */ (Stz*)"No such table",          // SQL Compiler Subsystem
  /* EC_NO_SUCH_TENANT                */ (Stz*)"no such tenant",
  /* EC_NO_SUCH_WORD                  */ (Stz*)"no such word",
  /* EC_NODE_EXISTS                   */ (Stz*)"node exists",
  /* EC_NOT_A_CATALOG                 */ (Stz*)"not a catalog",
  /* EC_NOT_A_FOLDER                  */ (Stz*)"not a folder",
  /* EC_NOT_AN_INDEX                  */ (Stz*)"'%v' not an index",
  /* EC_NOT_A_SCHEMA                  */ (Stz*)"not a schema",
  /* EC_NOT_A_TABLE                   */ (Stz*)"not a table",
  /* EC_NOT_A_TENANT                  */ (Stz*)"not a tenant",
  /* EC_NOT_IMPLEMENTED               */ (Stz*)"Feature not yet implemented",   // SQL Compiler Subsystem
  /* EC_NOT_IN_CATALOGS               */ (Stz*)"not in catalogs",
  /* EC_NOT_IN_COLUMNS                */ (Stz*)"not in columns",
  /* EC_NOT_IN_INDEXES                */ (Stz*)"not in indexes",
  /* EC_NOT_IN_PARTITIONS             */ (Stz*)"not in partitions",
  /* EC_NOT_IN_SCHEMAS                */ (Stz*)"not in schemas",
  /* EC_NOT_IN_TABLE                  */ (Stz*)"not in table",
  /* EC_NOT_IN_TABLES                 */ (Stz*)"not in tables",
  /* EC_NOT_IN_TENANTS                */ (Stz*)"not in tenants",
  /* EC_NULL_INTERFACE                */ (Stz*)"null interface",
  /* EC_NULLPTR                       */ (Stz*)"nullptr",

  /* _O_ */
  /* EC_OBJECT_ADDRESS_INVALID        */ (Stz*)"Object address is invalid",
  /* EC_OBJECT_CLASSNO_INVALID        */ (Stz*)"EC_OBJECT_CLASSNO_INVALID",
  /* EC_OBJECT_FOOTER_INVALID         */ (Stz*)"EC_OBJECT_FOOTER_INVALID",
  /* EC_OBJECT_MAGIC_INVALID          */ (Stz*)"EC_OBJECT_MAGIC_INVALID",
  /* EC_ORDERBY_NOT_SUPPORTED         */ (Stz*)"order by currently not supported",
  /* _P_ */
  /* EC_PIPELINE_PROTOCOL_ERROR       */ (Stz*)"pipeline protocol error",

  /* _Q_ */

  /* _R_ */
  /* EC_RESPONSE_PENDING              */ (Stz*)"response pending",

  /* _S_ */
  /* EC_SCALE_OUT_OF_BOUNDS           */ (Stz*)"scale out of bounds",
  /* EC_SCHEMA_EXISTS                 */ (Stz*)"schema exists",
  /* EC_SE_NOT_RUNNING                */ (Stz*)"SE is not running",
  /* EC_SE_UNKNOWN_ERROR              */ (Stz*)"SE unknown error",
  /* EC_SEMANTIC_ERROR                */ (Stz*)"semantic error",              // SQL Compiler Subsystem
  /* EC_SEND_FAILURE                  */ (Stz*)"send failure",
  /* EC_SUBQUERY_SELF_REFERENCE       */ (Stz*)"Non-recursive subquery cannot refer to itself",  // SQL Compiler Subsystem
  /* EC_SUBQUERY_PROJ_TOO_MANY_COLS   */ (Stz*)"Subquery cannot project more than one column",   // SQL Compiler Subsystem
  /* EC_SYNTAX_ERROR                  */ (Stz*)"syntax error",                // SQL Compiler Subsystem. Custom error string from ANTLR.
  /* EC_SYSTEM_CLOSED                 */ (Stz*)"system closed",

  /* _T_ */
  /* EC_TABLE_ALREADY_LOADED          */ (Stz*)"table already loaded",
  /* EC_TABLE_EXISTS                  */ (Stz*)"table exists",
  /* EC_TABLE_NOT_EMPTY               */ (Stz*)"table not empty",
  /* EC_TABLE_IN_USE                  */ (Stz*)"table in use",
  /* EC_TABLE_SUBQUERY_OUT_OF_SCOPE   */ (Stz*)"Table name refers to a table subquery alias that is not in scope",    // SQL Compiler Subsystem
  /* EC_TENANT_EXISTS                 */ (Stz*)"tenant exists",
  /* EC_TINYC_ERROR                   */ (Stz*)"tinyC compilation error",
  /* EC_TOO_MANY_NAMED_COLUMNS        */ (Stz*)"too many named columns in table subquery",
  /* EC_TX_BEGIN_FAILED               */ (Stz*)"begin transaction failed",
  /* EC_TX_COMMIT_FAILED              */ (Stz*)"commit transaction failed",
  /* EC_TX_END_FAILED                 */ (Stz*)"end transaction failed",
  /* EC_TX_ROLLBACK_FAILED            */ (Stz*)"rollback transaction failed",

  /* _U_ */
  /* EC_UNRESOLVED_COLUMN_WILDCARD    */ (Stz*)"Column wildcard does not resolve to any tables in the query",
  /* EC_USING_COLUMN_NOT_FOUND        */ (Stz*)"Column name in USING clause is not found",

  /* _V_ */
  /* EC_VALIDATION_FAILURE            */ (Stz*)"validation failure",
  /* EV_VALUE_OUT_OF_RANGE            */ (Stz*)"Value is out of range",
  /* EC_VERSION_MISMATCH              */ (Stz*)"version mismatch",

  /* _W_ */

  /* _X_ */

  /* _Y_ */

  /* _Z_ */
  /* EC_ZZZ_OTHER                     */ (Stz*)"zzz other error (temp filler)",

  // - - - - - - - - - - - - - - - - - - -

  /* EC_ENUMS                         */ (Stz*)"enums"

};
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
