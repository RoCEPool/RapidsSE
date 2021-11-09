/*============================================================================*/
/* eEC.h -- Error Codes                                                       */
/*============================================================================*/
#pragma once

enum eEC // error code
{
  EC_NONE=0,
  
  EC_DONE,
  
  // - - - - - - - - - - - - - - - - - - -
  /* _A_ */
  EC_AGG_EXPR_IN_WHERE_CLAUSE,      // SQL Compiler Subsystem
  EC_AMBIGUOUS_ALIAS,               // SQL Compiler Subsystem
  EC_AMBIGUOUS_COLUMN,              // SQL Compiler Subsystem
  EC_AMBIGUOUS_TABLE,               // SQL Compiler Subsystem
  EC_AMBIGUOUS_USING_COLUMN,        // SQL Compiler Subsystem
  EC_AMBIGUOUS_WITH_QUERY,          // SQL Compiler Subsystem
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
  EC_INVALID_INDEX_COLUMN,
  EC_INVALID_INSTANCE,
  EC_INVALID_SIZE,
  EC_INVALID_SYNTAX,
  EC_INVALID_TIME_FORMAT,           // SQL Compiler Subsystem
  EC_INVALID_TIMESTAMP_FORMAT,      // SQL Compiler Subsystem
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
  EC_UNRECOGNISED_HINT_VALUE,       // SQL Compiler Subsystem
  EC_UNRESOLVED_COLUMN_WILDCARD,    // SQL Compiler Subsystem
  EC_USING_COLUMN_NOT_FOUND,        // SQL Compiler Subsystem

  /* _V_ */
  EC_VALIDATION_FAILURE,
  EV_VALUE_OUT_OF_RANGE,            // SQL Compiler Subsystem
  EC_VERSION_MISMATCH,

  /* _W_ */

  /* _X_ */

  /* _Y_ */

  /* _Z_ */
  EC_ZZZ_OTHER,
  
  EC_ENUMS
};

// This should contain the text string representation of the EC enum.
// Not the error message.
static Stz* aEC[]
{
  /* */ "none"_Stz,
  
  /* */ "done"_Stz,

  // - - - - - - - - - - - - - - - - - - -

  /* _A_ */
  /* EC_AGG_EXPR_IN_WHERE_CLAUSE      */ "EC_AGG_EXPR_IN_WHERE_CLAUSE"_Stz, // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_ALIAS               */ "EC_AMBIGUOUS_ALIAS"_Stz,          // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_COLUMN              */ "EC_AMBIGUOUS_COLUMN"_Stz,         // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_TABLE               */ "EC_AMBIGUOUS_TABLE"_Stz,          // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_USING_COLUMN        */ "EC_AMBIGUOUS_USING_COLUMN"_Stz,   // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_WITH_QUERY          */ "EC_AMBIGUOUS_WITH_QUERY"_Stz,     // SQL Compiler Subsystem
  /* EC_ASSERT                        */ "assert"_Stz,

  /* _B_ */
  /* EC_BLANKLINE                     */ "blank line"_Stz,

  /* _C_ */
  /* EC_CATALOG_EXISTS                */ "catalog exists"_Stz,
  /* EC_CLUSTER_NOT_CONNECTED         */ "cluster not connected (use connectCluster command)"_Stz,
  /* EC_CLUSTER_PARTITIONS_MISMATCH   */ "cluster partitions per node mismatch"_Stz,
  /* EC_CLUSTER_SIZE_MISMATCH         */ "cluster node size mismatch"_Stz,
  /* EC_COLUMN_COUNT_MISMATCH         */ "EC_COLUMN_COUNT_MISMATCH"_Stz,        // SQL Compiler Subsystem
  /* EC_COLUMN_TYPE_MISMATCH          */ "EC_COLUMN_TYPE_MISMATCH"_Stz,         // SQL Compiler Subsystem
  /* EC_CORRBY_COLUMN_NOT_FOUND       */ "EC_CORRBY_COLUMN_NOT_FOUND"_Stz,      // SQL Compiler Subsystem
  /* EC_CORRBY_COLUMN_TYPE_MISMATCH   */ "EC_CORRBY_COLUMN_TYPE_MISMATCH"_Stz,  // SQL Compiler Subsystem
  /* EC_CORRBY_NO_COMMON_COLUMNS      */ "EC_CORRBY_NO_COMMON_COLUMNS"_Stz,     // SQL Compiler Subsystem
  /* EC_CSV_BOOLEAN_INVALID           */ "EC_CSV_BOOLEAN_INVALID"_Stz,
  /* EC_CSV_BOOLEAN_TOO_LARGE         */ "EC_CSV_BOOLEAN_TOO_LARGE"_Stz,
  /* EC_CSV_DATE_INVALID              */ "EC_CSV_DATE_INVALID"_Stz,
  /* EC_CSV_DATE_SEP_MISSING          */ "EC_CSV_DATE_SEP_MISSING"_Stz,
  /* EC_CSV_DAY_INVALID               */ "EC_CSV_DAY_INVALID"_Stz,
  /* EC_CSV_DAY_NOT_LEAPYEAR          */ "EC_CSV_DAY_NOT_LEAPYEAR"_Stz,
  /* EC_CSV_DAY_TOO_LARGE             */ "EC_CSV_DAY_TOO_LARGE"_Stz,
  /* EC_CSV_DECIMAL_INVALID           */ "EC_CSV_DECIMAL_INVALID"_Stz,
  /* EC_CSV_DECIMAL_TOO_LARGE         */ "EC_CSV_DECIMAL_TOO_LARGE"_Stz,
  /* EC_CSV_DECIMAL_TOO_SMALL         */ "EC_CSV_DECIMAL_TOO_SMALL"_Stz,
  /* EC_CSV_EXPONENT_MISSING          */ "EC_CSV_EXPONENT_MISSING"_Stz,
  /* EC_CSV_FLOAT_INVALID             */ "EC_CSV_FLOAT_INVALID"_Stz,
  /* EC_CSV_HOUR_INVALID              */ "EC_CSV_HOUR_INVALID"_Stz,
  /* EC_CSV_HOUR_TOO_LARGE            */ "EC_CSV_HOUR_TOO_LARGE"_Stz,
  /* EC_CSV_INTEGER_INVALID           */ "EC_CSV_INTEGER_INVALID"_Stz,
  /* EC_CSV_INTEGER_TOO_LARGE         */ "EC_CSV_INTEGER_TOO_LARGE"_Stz,
  /* EC_CSV_INTERVAL_INVALID          */ "EC_CSV_INTERVAL_INVALID"_Stz,
  /* EC_CSV_INTERVAL_SECS_TOO_LARGE   */ "EC_CSV_INTERVAL_SECS_TOO_LARGE"_Stz,
  /* EC_CSV_INTERVAL_USECS_TOO_LARGE  */ "EC_CSV_INTERVAL_USECS_TOO_LARGE"_Stz,
  /* EC_CSV_MINUTE_INVALID            */ "EC_CSV_MINUTE_INVALID"_Stz,
  /* EC_CSV_MINUTE_TOO_LARGE          */ "EC_CSV_MINUTE_TOO_LARGE"_Stz,
  /* EC_CSV_MONTH_INVALID             */ "EC_CSV_MONTH_INVALID"_Stz,
  /* EC_CSV_MONTH_TOO_LARGE           */ "EC_CSV_MONTH_TOO_LARGE"_Stz,
  /* EC_CSV_SECOND_INVALID            */ "EC_CSV_SECOND_INVALID"_Stz,
  /* EC_CSV_SECOND_TOO_LARGE          */ "EC_CSV_SECOND_TOO_LARGE"_Stz,
  /* EC_CSV_TIME_INVALID              */ "EC_CSV_TIME_INVALID"_Stz,
  /* EC_CSV_TIME_SEP_MISSING          */ "EC_CSV_TIME_SEP_MISSING"_Stz,
  /* EC_CSV_TIMESTAMP_INVALID         */ "EC_CSV_TIMESTAMP_INVALID"_Stz,
  /* EC_CSV_VARCHAR_INVALID           */ "EC_CSV_VARCHAR_INVALID"_Stz,
  /* EC_CSV_YEAR_INVALID              */ "EC_CSV_YEAR_INVALID"_Stz,
  /* EC_CSV_YEAR_TOO_SMALL            */ "EC_CSV_YEAR_TOO_SMALL"_Stz,
  /* EC_CSV_YEAR_TOO_LARGE            */ "EC_CSV_YEAR_TOO_LARGE"_Stz,

  /* _D_ */
  /* EC_DECIMAL_NOT_SUPPORTED         */ "decimal only with scale 2 currently supported"_Stz,
  /* EC_DUPLICATE_COLUMN              */ "duplicate column"_Stz,
  /* EC_DUPLICATE_KEY                 */ "duplicate key"_Stz,

  /* _E_ */
  /* EC_ESCAPE_SINGLE_CHAR            */ "EC_ESCAPE_SINGLE_CHAR"_Stz,           // SQL Compiler Subsystem
  /* EC_EXPLAIN_INVALID_FORMAT        */ "EC_EXPLAIN_INVALID_FORMAT"_Stz,       // SQL Compiler Subsystem
  /* EC_EXPR_AGG_MIX_NO_GROUPBY_LIST  */ "EC_EXPR_AGG_MIX_NO_GROUPBY_LIST"_Stz, // SQL Compiler Subsystem
  /* EC_EXPR_INVALID_COMBINATION      */ "EC_EXPR_INVALID_COMBINATION"_Stz,     // SQL Compiler Subsystem
  /* EC_EXPR_IS_AGG_IN_GROUPBY_LIST   */ "EC_EXPR_IS_AGG_IN_GROUPBY_LIST"_Stz,  // SQL Compiler Subsystem
  /* EC_EXPR_IS_LIT_IN_GROUPBY_LIST   */ "EC_EXPR_IS_LIT_IN_GROUPBY_LIST"_Stz,  // SQL Compiler Subsystem
  /* EC_EXPR_MUST_BE_BOOLEAN          */ "EC_EXPR_MUST_BE_BOOLEAN"_Stz,         // SQL Compiler Subsystem
  /* EC_EXPR_MUST_BE_EQUIJOIN         */ "EC_EXPR_MUST_BE_EQUIJOIN"_Stz,        // SQL Compiler Subsystem
  /* EC_EXPR_NOT_IN_GROUPBY_LIST      */ "EC_EXPR_NOT_IN_GROUPBY_LIST"_Stz,     // SQL Compiler Subsystem
  /* EC_EXPR_NOT_PROJECTED            */ "EC_EXPR_NOT_PROJECTED"_Stz,           // SQL Compiler Subsystem
  /* EC_EXPR_TYPE_INCOMPATIBLE        */ "EC_EXPR_TYPE_INCOMPATIBLE"_Stz,           // SQL Compiler Subsystem

  /* _F_ */
  /* EC_FOLDER_IN_USE                 */ "folder in use"_Stz,
  /* EC_FOLDER_NOT_EMPTY              */ "folder not empty"_Stz,

  /* _G_ */

  /* _H_ */
  /* EC_HAVING_EXPR_NOT_AGG_OR_IN_GROUPBY */ "EC_HAVING_EXPR_NOT_AGG_OR_IN_GROUPBY"_Stz, // SQL Compiler Subsystem

  /* _I_ */
  /* EC_IDXVFY_ADJACENT_ENTRIES       */ "EC_IDXVFY_ADJACENT_ENTRIES"_Stz,
  /* EC_IDXVFY_BUFFER_REFS            */ "EC_IDXVFY_BUFFER_REFS"_Stz,
  /* EC_IDXVFY_FIRST_KEYS             */ "EC_IDXVFY_FIRST_KEYS"_Stz,
  /* EC_IDXVFY_ITEMS                  */ "EC_IDXVFY_ITEMS"_Stz,
  /* EC_IDXVFY_KEY_BUFFER             */ "EC_IDXVFY_KEY_BUFFER"_Stz,
  /* EC_IDXVFY_KEY_ORDER              */ "EC_IDXVFY_KEY_ORDER"_Stz,
  /* EC_IDXVFY_NEXT_PREV              */ "EC_IDXVFY_NEXT_PREV"_Stz,
  /* EC_IDXVFY_ROW_REFS               */ "EC_IDXVFY_ROW_REFS"_Stz,
  /* EC_IDXVFY_SKIPLIST_ENTRY         */ "EC_IDXVFY_SKIPLIST_ENTRY"_Stz,
  /* EC_IDXVFY_SKIPLIST_LEVELS        */ "EC_IDXVFY_SKIPLIST_LEVELS"_Stz,
  /* EC_INDEX_EXISTS                  */ "index exists"_Stz,
  /* EC_INSUFFICIENT_EXPRS            */ "EC_INSUFFICIENT_EXPRS"_Stz,         // SQL Compiler Subsystem
  /* EC_INTERNAL_ERROR                */ "EC_INTERNAL_ERROR"_Stz,
  /* EC_INTERVAL_NOT_SUPPORTED        */ "interval currently not supported"_Stz,
  /* EC_INVALID_CLASS                 */ "invalid class"_Stz,
  /* EC_INVALID_COLUMN                */ "invalid column"_Stz,
  /* EC_INVALID_COLUMNS_UNIQUE_INDEX  */ "invalid columns for unique index (include all partitioning columns)"_Stz,
  /* EC_INVALID_COMMAND               */ "invalid command"_Stz,
  /* EC_INVALID_DATE_FORMAT           */ "EC_INVALID_DATE_FORMAT"_Stz,        // SQL Compiler Subsystem
  /* EC_INVALID_FILE                  */ "invalid file"_Stz,
  /* EC_INVALID_FLOAT_FORMAT          */ "EC_INVALID_FLOAT_FORMAT"_Stz,       // SQL Compiler Subsystem
  /* EC_INVALID_INTEGER_FORMAT        */ "EC_INVALID_INTEGER_FORMAT"_Stz,     // SQL Compiler Subsystem
  /* EC_INVALID_INTERFACE             */ "invalid interface"_Stz,
  /* EC_INVALID_LIMIT_VALUE           */ "EC_INVALID_LIMIT_VALUE"_Stz,        // SQL Compiler Subsystem
  /* EC_INVALID_OFFSET_VALUE          */ "EC_INVALID_OFFSET_VALUE"_Stz,       // SQL Compiler Subsystem
  /* EC_INVALID_PARAMETER             */ "invalid parameter"_Stz,
  /* EC_INVALID_PARAMETERS            */ "invalid parameters"_Stz,
  /* EC_INVALID_PARTITION             */ "invalid partition"_Stz,
  /* EC_INVALID_PARTITION_KEY         */ "invalid partition key"_Stz,
  /* EC_INVALID_INDEX_COLUMN          */ "invalid index column"_Stz,
  /* EC_INVALID_INSTANCE              */ "invalid instance"_Stz,
  /* EC_INVALID_SIZE                  */ "invalid size"_Stz,
  /* EC_INVALID_SYNTAX                */ "invalid syntax"_Stz,
  /* EC_INVALID_TIME_FORMAT           */ "EC_INVALID_TIME_FORMAT"_Stz,        // SQL Compiler Subsystem
  /* EC_INVALID_TIMESTAMP_FORMAT      */ "EC_INVALID_TIMESTAMP_FORMAT"_Stz,   // SQL Compiler Subsystem
  /* EC_INVALID_VALUE                 */ "invalid value"_Stz,

  /* _J_ */

  /* _K_ */

  /* _L_ */
  /* EC_LOW_MEMORY                    */ "low memory"_Stz,

  /* _M_ */
  /* EC_MAX_INDEXES                   */ "max allowed indexes already created"_Stz,
  /* EC_MAX_COLUMNS                   */ "max 150 columns supported in a table"_Stz,
  /* EC_MAX_ROW_LENGTH                */ "max row length allowed is 16k bytes"_Stz,

  /* _N_ */
  /* EC_NO_COLUMNS_DIC                */ "no columns dictionary"_Stz,
  /* EC_NO_SUCH_CATALOG               */ "no such catalog"_Stz,
  /* EC_NO_SUCH_COLUMN                */ "EC_NO_SUCH_COLUMN"_Stz,     // SQL Compiler Subsystem
  /* EC_NO_SUCH_DIRECTORY             */ "no such directory"_Stz,
  /* EC_NO_SUCH_FILE                  */ "no such file"_Stz,
  /* EC_NO_SUCH_FOLDER                */ "no such folder"_Stz,
  /* EC_NO_SUCH_INDEX                 */ "no such index"_Stz,
  /* EC_NO_SUCH_JOB                   */ "no such job"_Stz,
  /* EC_NO_SUCH_NODE                  */ "no such node"_Stz,
  /* EC_NO_SUCH_SCHEMA                */ "no such schema"_Stz,
  /* EC_NO_SUCH_TABLE                 */ "no such table"_Stz,
  /* EC_NO_SUCH_TABLE2                */ "EC_NO_SUCH_TABLE2"_Stz,     // SQL Compiler Subsystem
  /* EC_NO_SUCH_TENANT                */ "no such tenant"_Stz,
  /* EC_NO_SUCH_WORD                  */ "no such word"_Stz,
  /* EC_NODE_EXISTS                   */ "node exists"_Stz,
  /* EC_NOT_A_CATALOG                 */ "not a catalog"_Stz,
  /* EC_NOT_A_FOLDER                  */ "not a folder"_Stz,
  /* EC_NOT_AN_INDEX                  */ "not an index"_Stz,
  /* EC_NOT_A_SCHEMA                  */ "not a schema"_Stz,
  /* EC_NOT_A_TABLE                   */ "not a table"_Stz,
  /* EC_NOT_A_TENANT                  */ "not a tenant"_Stz,
  /* EC_NOT_IMPLEMENTED               */ "EC_NOT_IMPLEMENTED"_Stz,
  /* EC_NOT_IN_CATALOGS               */ "not in catalogs"_Stz,
  /* EC_NOT_IN_COLUMNS                */ "not in columns"_Stz,
  /* EC_NOT_IN_INDEXES                */ "not in indexes"_Stz,
  /* EC_NOT_IN_PARTITIONS             */ "not in partitions"_Stz,
  /* EC_NOT_IN_SCHEMAS                */ "not in schemas"_Stz,
  /* EC_NOT_IN_TABLE                  */ "not in table"_Stz,
  /* EC_NOT_IN_TABLES                 */ "not in tables"_Stz,
  /* EC_NOT_IN_TENANTS                */ "not in tenants"_Stz,
  /* EC_NULL_INTERFACE                */ "null interface"_Stz,
  /* EC_NULLPTR                       */ "nullptr"_Stz,

  /* _O_ */
  /* EC_OBJECT_ADDRESS_INVALID        */ "EC_OBJECT_ADDRESS_INVALID"_Stz,
  /* EC_OBJECT_CLASSNO_INVALID        */ "EC_OBJECT_CLASSNO_INVALID"_Stz,
  /* EC_OBJECT_FOOTER_INVALID         */ "EC_OBJECT_FOOTER_INVALID"_Stz,
  /* EC_OBJECT_MAGIC_INVALID          */ "EC_OBJECT_MAGIC_INVALID"_Stz,
  /* EC_ORDERBY_NOT_SUPPORTED         */ "order by currently not supported"_Stz,

  /* _P_ */
  /* EC_PIPELINE_PROTOCOL_ERROR       */ "pipeline protocol error"_Stz,

  /* _Q_ */

  /* _R_ */
  /* EC_RESPONSE_PENDING              */ "response pending"_Stz,

  /* _S_ */
  /* EC_SCHEMA_EXISTS                 */ "schema exists"_Stz,
  /* EC_SE_NOT_RUNNING                */ "SE is not running"_Stz,
  /* EC_SE_UNKNOWN_ERROR              */ "SE unknown error"_Stz,
  /* EC_SEMANTIC_ERROR                */ "semantic error"_Stz,              // SQL Compiler Subsystem
  /* EC_SEND_FAILURE                  */ "send failure"_Stz,
  /* EC_SUBQUERY_SELF_REFERENCE       */ "EC_SUBQUERY_SELF_REFERENCE"_Stz,  // SQL Compiler Subsystem
  /* EC_SUBQUERY_PROJ_TOO_MANY_COLS   */ "EC_SUBQUERY_PROJ_TOO_MANY_COLS"_Stz, // SQL Compiler Subsystem
  /* EC_SYNTAX_ERROR                  */ "syntax error"_Stz,                // SQL Compiler Subsystem
  /* EC_SYSTEM_CLOSED                 */ "system is closed"_Stz,

  /* _T_ */
  /* EC_TABLE_ALREADY_LOADED          */ "table already loaded"_Stz,
  /* EC_TABLE_EXISTS                  */ "table exists"_Stz,
  /* EC_TABLE_NOT_EMPTY               */ "table not empty"_Stz,
  /* EC_TABLE_IN_USE                  */ "table in use"_Stz,
  /* EC_TABLE_SUBQUERY_OUT_OF_SCOPE   */ "EC_TABLE_SUBQUERY_OUT_OF_SCOPE"_Stz,  // SQL Compiler Subsystem
  /* EC_TENANT_EXISTS                 */ "tenant exists"_Stz,
  /* EC_TINYC_ERROR                   */ "tinyC compilation error"_Stz,
  /* EC_TOO_MANY_NAMED_COLUMNS        */ "too many named columns"_Stz,
  /* EC_TX_BEGIN_FAILED               */ "begin transaction failed"_Stz,
  /* EC_TX_COMMIT_FAILED              */ "commit transaction failed"_Stz,
  /* EC_TX_END_FAILED                 */ "end transaction failed"_Stz,
  /* EC_TX_ROLLBACK_FAILED            */ "rollback transaction failed"_Stz,

  /* _U_ */
  /* EC_UNRECOGNISED_HINT_VALUE       */ "EC_UNRECOGNISED_HINT_VALUE"_Stz, // SQL Compiler Subsystem
  /* EC_UNRESOLVED_COLUMN_WILDCARD    */ "EC_UNRESOLVED_COLUMN_WILDCARD"_Stz, // SQL Compiler Subsystem
  /* EC_USING_COLUMN_NOT_FOUND        */ "EC_USING_COLUMN_NOT_FOUND"_Stz,     // SQL Compiler Subsystem

  /* _V_ */
  /* EC_VALIDATION_FAILURE            */ "validation failure"_Stz,
  /* EV_VALUE_OUT_OF_RANGE            */ "EV_VALUE_OUT_OF_RANGE"_Stz,         // SQL Compiler Subsystem
  /* EC_VERSION_MISMATCH              */ "version mismatch"_Stz,

  /* _W_ */

  /* _X_ */

  /* _Y_ */

  /* _Z_ */
  /* EC_ZZZ_OTHER                     */ "zzz other error (temp filler)"_Stz,
  
  /* EC_ENUMS                         */ "enums"_Stz
};

// This should contain the text string representation of the EC enum.
// Not the error message.
static Stz* aEC2[] // error code
{
  /* */ "EC_NONE"_Stz,

  /* */ "EC_DONE"_Stz,

  // - - - - - - - - - - - - - - - - - - -

  /* _A_ */
  /* EC_AGG_EXPR_IN_WHERE_CLAUSE      */ "EC_AGG_EXPR_IN_WHERE_CLAUSE"_Stz,       // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_ALIAS               */ "EC_AMBIGUOUS_ALIAS"_Stz,                // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_COLUMN              */ "EC_AMBIGUOUS_COLUMN"_Stz,               // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_TABLE               */ "EC_AMBIGUOUS_TABLE"_Stz,                // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_USING_COLUMN        */ "EC_AMBIGUOUS_USING_COLUMN"_Stz,         // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_WITH_QUERY          */ "EC_AMBIGUOUS_WITH_QUERY"_Stz,           // SQL Compiler Subsystem
  /* EC_ASSERT                        */ "EC_ASSERT"_Stz,

  /* _B_ */
  /* EC_BLANKLINE                     */ "EC_BLANKLINE"_Stz,

  /* _C_ */
  /* EC_CATALOG_EXISTS                */ "EC_CATALOG_EXISTS"_Stz,
  /* EC_CLUSTER_NOT_CONNECTED         */ "EC_CLUSTER_NOT_CONNECTED"_Stz,
  /* EC_CLUSTER_PARTITIONS_MISMATCH   */ "EC_CLUSTER_PARTITIONS_MISMATCH"_Stz,
  /* EC_CLUSTER_SIZE_MISMATCH         */ "EC_CLUSTER_SIZE_MISMATCH"_Stz,
  /* EC_COLUMN_COUNT_MISMATCH         */ "EC_COLUMN_COUNT_MISMATCH"_Stz,          // SQL Compiler Subsystem
  /* EC_COLUMN_TYPE_MISMATCH          */ "EC_COLUMN_TYPE_MISMATCH"_Stz,           // SQL Compiler Subsystem
  /* EC_CORRBY_COLUMN_NOT_FOUND       */ "EC_CORRBY_COLUMN_NOT_FOUND"_Stz,        // SQL Compiler Subsystem
  /* EC_CORRBY_COLUMN_TYPE_MISMATCH   */ "EC_CORRBY_COLUMN_TYPE_MISMATCH"_Stz,    // SQL Compiler Subsystem
  /* EC_CORRBY_NO_COMMON_COLUMNS      */ "EC_CORRBY_NO_COMMON_COLUMNS"_Stz,       // SQL Compiler Subsystem

  /* EC_CSV_BOOLEAN_INVALID           */ "EC_CSV_BOOLEAN_INVALID"_Stz,
  /* EC_CSV_BOOLEAN_TOO_LARGE         */ "EC_CSV_BOOLEAN_TOO_LARGE"_Stz,
  /* EC_CSV_DATE_INVALID              */ "EC_CSV_DATE_INVALID"_Stz,
  /* EC_CSV_DATE_SEP_MISSING          */ "EC_CSV_DATE_SEP_MISSING"_Stz,
  /* EC_CSV_DAY_INVALID               */ "EC_CSV_DAY_INVALID"_Stz,
  /* EC_CSV_DAY_NOT_LEAPYEAR          */ "EC_CSV_DAY_NOT_LEAPYEAR"_Stz,
  /* EC_CSV_DAY_TOO_LARGE             */ "EC_CSV_DAY_TOO_LARGE"_Stz,
  /* EC_CSV_DECIMAL_INVALID           */ "EC_CSV_DECIMAL_INVALID"_Stz,
  /* EC_CSV_DECIMAL_TOO_LARGE         */ "EC_CSV_DECIMAL_TOO_LARGE"_Stz,
  /* EC_CSV_DECIMAL_TOO_SMALL         */ "EC_CSV_DECIMAL_TOO_SMALL"_Stz,
  /* EC_CSV_EXPONENT_MISSING          */ "EC_CSV_EXPONENT_MISSING"_Stz,
  /* EC_CSV_FLOAT_INVALID             */ "EC_CSV_FLOAT_INVALID"_Stz,
  /* EC_CSV_HOUR_INVALID              */ "EC_CSV_HOUR_INVALID"_Stz,
  /* EC_CSV_HOUR_TOO_LARGE            */ "EC_CSV_HOUR_TOO_LARGE"_Stz,
  /* EC_CSV_INTEGER_INVALID           */ "EC_CSV_INTEGER_INVALID"_Stz,
  /* EC_CSV_INTEGER_TOO_LARGE         */ "EC_CSV_INTEGER_TOO_LARGE"_Stz,
  /* EC_CSV_INTERVAL_INVALID          */ "EC_CSV_INTERVAL_INVALID"_Stz,
  /* EC_CSV_INTERVAL_SECS_TOO_LARGE   */ "EC_CSV_INTERVAL_SECS_TOO_LARGE"_Stz,
  /* EC_CSV_INTERVAL_USECS_TOO_LARGE  */ "EC_CSV_INTERVAL_USECS_TOO_LARGE"_Stz,
  /* EC_CSV_MINUTE_INVALID            */ "EC_CSV_MINUTE_INVALID"_Stz,
  /* EC_CSV_MINUTE_TOO_LARGE          */ "EC_CSV_MINUTE_TOO_LARGE"_Stz,
  /* EC_CSV_MONTH_INVALID             */ "EC_CSV_MONTH_INVALID"_Stz,
  /* EC_CSV_MONTH_TOO_LARGE           */ "EC_CSV_MONTH_TOO_LARGE"_Stz,
  /* EC_CSV_SECOND_INVALID            */ "EC_CSV_SECOND_INVALID"_Stz,
  /* EC_CSV_SECOND_TOO_LARGE          */ "EC_CSV_SECOND_TOO_LARGE"_Stz,
  /* EC_CSV_TIME_INVALID              */ "EC_CSV_TIME_INVALID"_Stz,
  /* EC_CSV_TIME_SEP_MISSING          */ "EC_CSV_TIME_SEP_MISSING"_Stz,
  /* EC_CSV_TIMESTAMP_INVALID         */ "EC_CSV_TIMESTAMP_INVALID"_Stz,
  /* EC_CSV_VARCHAR_INVALID           */ "EC_CSV_VARCHAR_INVALID"_Stz,
  /* EC_CSV_YEAR_INVALID              */ "EC_CSV_YEAR_INVALID"_Stz,
  /* EC_CSV_YEAR_TOO_SMALL            */ "EC_CSV_YEAR_TOO_SMALL"_Stz,
  /* EC_CSV_YEAR_TOO_LARGE            */ "EC_CSV_YEAR_TOO_LARGE"_Stz,

  /* _D_ */
  /* EC_DECIMAL_NOT_SUPPORTED         */ "EC_DECIMAL_NOT_SUPPORTED"_Stz,
  /* EC_DUPLICATE_COLUMN              */ "EC_DUPLICATE_COLUMN"_Stz,
  /* EC_DUPLICATE_KEY                 */ "EC_DUPLICATE_KEY"_Stz,

  /* _E_ */
  /* EC_ESCAPE_SINGLE_CHAR            */ "EC_ESCAPE_SINGLE_CHAR"_Stz,             // SQL Compiler Subsystem
  /* EC_EXPLAIN_INVALID_FORMAT        */ "EC_EXPLAIN_INVALID_FORMAT"_Stz,         // SQL Compiler Subsystem
  /* EC_EXPR_AGG_MIX_NO_GROUPBY_LIST  */ "EC_EXPR_AGG_MIX_NO_GROUPBY_LIST"_Stz,   // SQL Compiler Subsystem
  /* EC_EXPR_INVALID_COMBINATION      */ "EC_EXPR_INVALID_COMBINATION"_Stz,       // SQL Compiler Subsystem
  /* EC_EXPR_IS_AGG_IN_GROUPBY_LIST   */ "EC_EXPR_IS_AGG_IN_GROUPBY_LIST"_Stz,    // SQL Compiler Subsystem
  /* EC_EXPR_IS_LIT_IN_GROUPBY_LIST   */ "EC_EXPR_IS_LIT_IN_GROUPBY_LIST"_Stz,    // SQL Compiler Subsystem
  /* EC_EXPR_MUST_BE_BOOLEAN          */ "EC_EXPR_MUST_BE_BOOLEAN"_Stz,           // SQL Compiler Subsystem
  /* EC_EXPR_MUST_BE_EQUIJOIN         */ "EC_EXPR_MUST_BE_EQUIJOIN"_Stz,          // SQL Compiler Subsystem
  /* EC_EXPR_NOT_IN_GROUPBY_LIST      */ "EC_EXPR_NOT_IN_GROUPBY_LIST"_Stz,       // SQL Compiler Subsystem
  /* EC_EXPR_NOT_PROJECTED            */ "EC_EXPR_NOT_PROJECTED"_Stz,             // SQL Compiler Subsystem
  /* EC_EXPR_TYPE_INCOMPATIBLE        */ "EC_EXPR_TYPE_INCOMPATIBLE"_Stz,             // SQL Compiler Subsystem

  /* _F_ */
  /* EC_FOLDER_IN_USE                 */ "EC_FOLDER_IN_USE"_Stz,
  /* EC_FOLDER_NOT_EMPTY              */ "EC_FOLDER_NOT_EMPTY"_Stz,

  /* _G_ */

  /* _H_ */
  /* EC_HAVING_EXPR_NOT_AGG_OR_IN_GROUPBY */ "EC_HAVING_EXPR_NOT_AGG_OR_IN_GROUPBY"_Stz,  // SQL Compiler Subsystem

  /* _I_ */
  /* EC_IDXVFY_ADJACENT_ENTRIES       */ "EC_IDXVFY_ADJACENT_ENTRIES"_Stz,
  /* EC_IDXVFY_BUFFER_REFS            */ "EC_IDXVFY_BUFFER_REFS"_Stz,
  /* EC_IDXVFY_FIRST_KEYS             */ "EC_IDXVFY_FIRST_KEYS"_Stz,
  /* EC_IDXVFY_ITEMS                  */ "EC_IDXVFY_ITEMS"_Stz,
  /* EC_IDXVFY_KEY_BUFFER             */ "EC_IDXVFY_KEY_BUFFER"_Stz,
  /* EC_IDXVFY_KEY_ORDER              */ "EC_IDXVFY_KEY_ORDER"_Stz,
  /* EC_IDXVFY_NEXT_PREV              */ "EC_IDXVFY_NEXT_PREV"_Stz,
  /* EC_IDXVFY_ROW_REFS               */ "EC_IDXVFY_ROW_REFS"_Stz,
  /* EC_IDXVFY_SKIPLIST_ENTRY         */ "EC_IDXVFY_SKIPLIST_ENTRY"_Stz,
  /* EC_IDXVFY_SKIPLIST_LEVELS        */ "EC_IDXVFY_SKIPLIST_LEVELS"_Stz,
  /* EC_INDEX_EXISTS                  */ "EC_INDEX_EXISTS"_Stz,
  /* EC_INSUFFICIENT_EXPRS            */ "EC_INSUFFICIENT_EXPRS"_Stz,
  /* EC_INTERNAL_ERROR                */ "EC_INTERNAL_ERROR"_Stz,
  /* EC_INTERVAL_NOT_SUPPORTED        */ "EC_INTERVAL_NOT_SUPPORTED"_Stz,
  /* EC_INVALID_CLASS                 */ "EC_INVALID_CLASS"_Stz,
  /* EC_INVALID_COLUMN                */ "EC_INVALID_COLUMN"_Stz,
  /* EC_INVALID_COLUMNS_UNIQUE_IND    */ "EC_INVALID_COLUMNS_UNIQUE_INDEX"_Stz,
  /* EC_INVALID_COMMAND               */ "EC_INVALID_COMMAND"_Stz,
  /* EC_INVALID_DATE_FORMAT           */ "EC_INVALID_DATE_FORMAT"_Stz,            // SQL Compiler Subsystem
  /* EC_INVALID_FILE                  */ "EC_INVALID_FILE"_Stz,
  /* EC_INVALID_FLOAT_FORMAT          */ "EC_INVALID_FLOAT_FORMAT"_Stz,           // SQL Compiler Subsystem
  /* EC_INVALID_INTEGER_FORMAT        */ "EC_INVALID_INTEGER_FORMAT"_Stz,         // SQL Compiler Subsystem
  /* EC_INVALID_INTERFACE             */ "EC_INVALID_INTERFACE"_Stz,
  /* EC_INVALID_LIMIT_VALUE           */ "EC_INVALID_LIMIT_VALUE"_Stz,            // SQL Compiler Subsystem
  /* EC_INVALID_OFFSET_VALUE          */ "EC_INVALID_OFFSET_VALUE"_Stz,           // SQL Compiler Subsystem
  /* EC_INVALID_PARAMETER             */ "EC_INVALID_PARAMETER"_Stz,
  /* EC_INVALID_PARAMETERS            */ "EC_INVALID_PARAMETERS"_Stz,
  /* EC_INVALID_PARTITION             */ "EC_INVALID_PARTITION"_Stz,
  /* EC_INVALID_PARTITION_KEY         */ "EC_INVALID_PARTITION_KEY"_Stz,
  /* EC_INVALID_INDEX_COLUMN          */ "EC_INVALID_INDEX_COLUMN"_Stz,
  /* EC_INVALID_INSTANCE              */ "EC_INVALID_INSTANCE"_Stz,
  /* EC_INVALID_SIZE                  */ "EC_INVALID_SIZE"_Stz,
  /* EC_INVALID_SYNTAX                */ "EC_INVALID_SYNTAX"_Stz,
  /* EC_INVALID_TIME_FORMAT           */ "EC_INVALID_TIME_FORMAT"_Stz,            // SQL Compiler Subsystem
  /* EC_INVALID_TIMESTAMP_FORMAT      */ "EC_INVALID_TIMESTAMP_FORMAT"_Stz,       // SQL Compiler Subsystem
  /* EC_INVALID_VALUE                 */ "EC_INVALID_VALUE"_Stz,

  /* _J_ */

  /* _K_ */

  /* _L_ */
  /* EC_LOW_MEMORY                    */ "EC_LOW_MEMORY"_Stz,

  /* _M_ */
  /* EC_MAX_INDEXES                   */ "EC_MAX_INDEXES"_Stz,
  /* EC_MAX_COLUMNS                   */ "EC_MAX_COLUMNS"_Stz,
  /* EC_MAX_ROW_LENGTH                */ "EC_MAX_ROW_LENGTH"_Stz,

  /* _N_ */
  /* EC_NO_COLUMNS_DIC                */ "EC_NO_COLUMNS_DIC"_Stz,
  /* EC_NO_SUCH_CATALOG               */ "EC_NO_SUCH_CATALOG"_Stz,
  /* EC_NO_SUCH_COLUMN                */ "EC_NO_SUCH_COLUMN"_Stz,                 // SQL Compiler Subsystem
  /* EC_NO_SUCH_DIRECTORY             */ "EC_NO_SUCH_DIRECTORY"_Stz,
  /* EC_NO_SUCH_FILE                  */ "EC_NO_SUCH_FILE"_Stz,
  /* EC_NO_SUCH_FOLDER                */ "EC_NO_SUCH_FOLDER"_Stz,
  /* EC_NO_SUCH_INDEX                 */ "EC_NO_SUCH_INDEX"_Stz,
  /* EC_NO_SUCH_JOB                   */ "EC_NO_SUCH_JOB"_Stz,
  /* EC_NO_SUCH_NODE                  */ "EC_NO_SUCH_NODE"_Stz,
  /* EC_NO_SUCH_SCHEMA                */ "EC_NO_SUCH_SCHEMA"_Stz,
  /* EC_NO_SUCH_TABLE                 */ "EC_NO_SUCH_TABLE"_Stz,
  /* EC_NO_SUCH_TABLE2                */ "EC_NO_SUCH_TABLE2"_Stz,                 // SQL Compiler Subsystem
  /* EC_NO_SUCH_TENANT                */ "EC_NO_SUCH_TENANT"_Stz,
  /* EC_NO_SUCH_WORD                  */ "EC_NO_SUCH_WORD"_Stz,
  /* EC_NODE_EXISTS                   */ "EC_NODE_EXISTS"_Stz,
  /* EC_NOT_A_CATALOG                 */ "EC_NOT_A_CATALOG"_Stz,
  /* EC_NOT_A_FOLDER                  */ "EC_NOT_A_FOLDER"_Stz,
  /* EC_NOT_AN_INDEX                  */ "EC_NOT_AN_INDEX"_Stz,
  /* EC_NOT_A_SCHEMA                  */ "EC_NOT_A_SCHEMA"_Stz,
  /* EC_NOT_A_TABLE                   */ "EC_NOT_A_TABLE"_Stz,
  /* EC_NOT_A_TENANT                  */ "EC_NOT_A_TENANT"_Stz,
  /* EC_NOT_IMPLEMENTED               */ "EC_NOT_IMPLEMENTED"_Stz,
  /* EC_NOT_IN_CATALOGS               */ "EC_NOT_IN_CATALOGS"_Stz,
  /* EC_NOT_IN_COLUMNS                */ "EC_NOT_IN_COLUMNS"_Stz,
  /* EC_NOT_IN_INDEXES                */ "EC_NOT_IN_INDEXES"_Stz,
  /* EC_NOT_IN_PARTITIONS             */ "EC_NOT_IN_PARTITIONS"_Stz,
  /* EC_NOT_IN_SCHEMAS                */ "EC_NOT_IN_SCHEMAS"_Stz,
  /* EC_NOT_IN_TABLE                  */ "EC_NOT_IN_TABLE"_Stz,
  /* EC_NOT_IN_TABLES                 */ "EC_NOT_IN_TABLES"_Stz,
  /* EC_NOT_IN_TENANTS                */ "EC_NOT_IN_TENANTS"_Stz,
  /* EC_NULL_INTERFACE                */ "EC_NULL_INTERFACE"_Stz,
  /* EC_NULLPTR                       */ "EC_NULLPTR"_Stz,

  /* _O_ */
  /* EC_OBJECT_ADDRESS_INVALID        */ "EC_OBJECT_ADDRESS_INVALID"_Stz,
  /* EC_OBJECT_CLASSNO_INVALID        */ "EC_OBJECT_CLASSNO_INVALID"_Stz,
  /* EC_OBJECT_FOOTER_INVALID         */ "EC_OBJECT_FOOTER_INVALID"_Stz,
  /* EC_OBJECT_MAGIC_INVALID          */ "EC_OBJECT_MAGIC_INVALID"_Stz,
  /* EC_ORDERBY_NOT_SUPPORTED         */ "EC_ORDERBY_NOT_SUPPORTED"_Stz,

  /* _P_ */
  /* EC_PIPELINE_PROTOCOL_ERROR       */ "EC_PIPELINE_PROTOCOL_ERROR"_Stz,

  /* _Q_ */

  /* _R_ */
  /* EC_RESPONSE_PENDING              */ "EC_RESPONSE_PENDING"_Stz,

  /* _S_ */
  /* EC_SCHEMA_EXISTS                 */ "EC_SCHEMA_EXISTS"_Stz,
  /* EC_SE_NOT_RUNNING                */ "EC_SE_NOT_RUNNING"_Stz,
  /* EC_SE_UNKNOWN_ERROR              */ "EC_SE_UNKNOWN_ERROR"_Stz,
  /* EC_SEMANTIC_ERROR                */ "EC_SEMANTIC_ERROR"_Stz,                 // SQL Compiler Subsystem
  /* EC_SEND_FAILURE                  */ "EC_SEND_FAILURE"_Stz,
  /* EC_SUBQUERY_SELF_REFERENCE       */ "EC_SUBQUERY_SELF_REFERENCE"_Stz,        // SQL Compiler Subsystem
  /* EC_SUBQUERY_PROJ_TOO_MANY_COLS   */ "EC_SUBQUERY_PROJ_TOO_MANY_COLS"_Stz,    // SQL Compiler Subsystem
  /* EC_SYNTAX_ERROR                  */ "EC_SYNTAX_ERROR"_Stz,                   // SQL Compiler Subsystem
  /* EC_SYSTEM_CLOSED                 */ "EC_SYSTEM_CLOSED"_Stz,

  /* _T_ */
  /* EC_TABLE_ALREADY_LOADED          */ "EC_TABLE_ALREADY_LOADED"_Stz,
  /* EC_TABLE_EXISTS                  */ "EC_TABLE_EXISTS"_Stz,
  /* EC_TABLE_NOT_EMPTY               */ "EC_TABLE_NOT_EMPTY"_Stz,
  /* EC_TABLE_IN_USE                  */ "EC_TABLE_IN_USE"_Stz,
  /* EC_TABLE_SUBQUERY_OUT_OF_SCOPE   */ "EC_TABLE_SUBQUERY_OUT_OF_SCOPE"_Stz,
  /* EC_TENANT_EXISTS                 */ "EC_TENANT_EXISTS"_Stz,
  /* EC_TINYC_ERROR                   */ "EC_TINYC_ERROR"_Stz,
  /* EC_TOO_MANY_NAMED_COLUMNS        */ "EC_TOO_MANY_NAMED_COLUMNS"_Stz,
  /* EC_TX_BEGIN_FAILED               */ "EC_TX_BEGIN_FAILED"_Stz,
  /* EC_TX_COMMIT_FAILED              */ "EC_TX_COMMIT_FAILED"_Stz,
  /* EC_TX_END_FAILED                 */ "EC_TX_END_FAILED"_Stz,
  /* EC_TX_ROLLBACK_FAILED            */ "EC_TX_ROLLBACK_FAILED"_Stz,

  /* _U_ */
  /* EC_UNRECOGNISED_HINT_VALUE       */ "EC_UNRECOGNISED_HINT_VALUE"_Stz,        // SQL Compiler Subsystem
  /* EC_UNRESOLVED_COLUMN_WILDCARD    */ "EC_UNRESOLVED_COLUMN_WILDCARD"_Stz,     // SQL Compiler Subsystem
  /* EC_USING_COLUMN_NOT_FOUND        */ "EC_USING_COLUMN_NOT_FOUND"_Stz,         // SQL Compiler Subsystem

  /* _V_ */
  /* EC_VALIDATION_FAILURE            */ "EC_VALIDATION_FAILURE"_Stz,
  /* EV_VALUE_OUT_OF_RANGE            */ "EV_VALUE_OUT_OF_RANGE"_Stz,             // SQL Compiler Subsystem
  /* EC_VERSION_MISMATCH              */ "EC_VERSION_MISMATCH"_Stz,

  /* _W_ */

  /* _X_ */

  /* _Y_ */

  /* _Z_ */
    /* EC_ZZZ_OTHER                     */ "EC_ZZZ_OTHER"_Stz,

    /* EC_ENUMS                         */ "EC_ENUMS"_Stz
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
static Stz* aErrorMsg[]
{
  /* EC_NONE                          */ "none"_Stz,

  /* EC_DONE                          */ "done"_Stz,

  // - - - - - - - - - - - - - - - - - - -
  /* _A_ */
  /* EC_AGG_EXPR_IN_WHERE_CLAUSE      */ "Cannot specify an aggregate expression in a WHERE clause"_Stz, // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_ALIAS               */ "Ambiguous alias name"_Stz,      // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_COLUMN              */ "Ambiguous column name"_Stz,     // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_TABLE               */ "Ambiguous table name"_Stz,      // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_USING_COLUMN        */ "Ambiguous column name in USING clause"_Stz,     // SQL Compiler Subsystem
  /* EC_AMBIGUOUS_WITH_QUERY          */ "Ambiguous alias name in WITH clause"_Stz,       // SQL Compiler Subsystem
  /* EC_ASSERT                        */ "assert"_Stz,

  /* _B_ */
  /* EC_BLANKLINE                     */ "blank line"_Stz,

  /* _C_ */
  /* EC_CATALOG_EXISTS                */ "catalog exists"_Stz,
  /* EC_CLUSTER_NOT_CONNECTED         */ "cluster not connected (use connectCluster command)"_Stz,
  /* EC_CLUSTER_PARTITIONS_MISMATCH   */ "cluster partitions per node mismatch"_Stz,
  /* EC_CLUSTER_SIZE_MISMATCH         */ "cluster node size mismatch"_Stz,
  /* EC_COLUMN_COUNT_MISMATCH         */ "The number of columns of each subquery do not match"_Stz,   // SQL Compiler Subsystem
  /* EC_COLUMN_TYPE_MISMATCH          */ "Column types do not match"_Stz,                             // SQL Compiler Subsystem
  /* EC_CORRBY_COLUMN_NOT_FOUND       */ "Column in CORRESPONDING BY list is not found in select list"_Stz,   // SQL Compiler Subsystem
  /* EC_CORRBY_COLUMN_TYPE_MISMATCH   */ "Column in CORRESPONDING BY list has mismatched types in child queries"_Stz, // SQL Compiler Subsystem
  /* EC_CORRBY_NO_COMMON_COLUMNS      */ "No common column names of the subqueries could be found for the CORRESPONDING list"_Stz,  // SQL Compiler Subsystem

  /* EC_CSV_BOOLEAN_INVALID           */ "EC_CSV_BOOLEAN_INVALID"_Stz,
  /* EC_CSV_BOOLEAN_TOO_LARGE         */ "EC_CSV_BOOLEAN_TOO_LARGE"_Stz,
  /* EC_CSV_DATE_INVALID              */ "EC_CSV_DATE_INVALID"_Stz,
  /* EC_CSV_DATE_SEP_MISSING          */ "EC_CSV_DATE_SEP_MISSING"_Stz,
  /* EC_CSV_DAY_INVALID               */ "EC_CSV_DAY_INVALID"_Stz,
  /* EC_CSV_DAY_NOT_LEAPYEAR          */ "EC_CSV_DAY_NOT_LEAPYEAR"_Stz,
  /* EC_CSV_DAY_TOO_LARGE             */ "EC_CSV_DAY_TOO_LARGE"_Stz,
  /* EC_CSV_DECIMAL_INVALID           */ "EC_CSV_DECIMAL_INVALID"_Stz,
  /* EC_CSV_DECIMAL_TOO_LARGE         */ "EC_CSV_DECIMAL_TOO_LARGE"_Stz,
  /* EC_CSV_DECIMAL_TOO_SMALL         */ "EC_CSV_DECIMAL_TOO_SMALL"_Stz,
  /* EC_CSV_EXPONENT_MISSING          */ "EC_CSV_EXPONENT_MISSING"_Stz,
  /* EC_CSV_FLOAT_INVALID             */ "EC_CSV_FLOAT_INVALID"_Stz,
  /* EC_CSV_HOUR_INVALID              */ "EC_CSV_HOUR_INVALID"_Stz,
  /* EC_CSV_HOUR_TOO_LARGE            */ "EC_CSV_HOUR_TOO_LARGE"_Stz,
  /* EC_CSV_INTEGER_INVALID           */ "EC_CSV_INTEGER_INVALID"_Stz,
  /* EC_CSV_INTEGER_TOO_LARGE         */ "EC_CSV_INTEGER_TOO_LARGE"_Stz,
  /* EC_CSV_INTERVAL_INVALID          */ "EC_CSV_INTERVAL_INVALID"_Stz,
  /* EC_CSV_INTERVAL_SECS_TOO_LARGE   */ "EC_CSV_INTERVAL_SECS_TOO_LARGE"_Stz,
  /* EC_CSV_INTERVAL_USECS_TOO_LARGE  */ "EC_CSV_INTERVAL_USECS_TOO_LARGE"_Stz,
  /* EC_CSV_MINUTE_INVALID            */ "EC_CSV_MINUTE_INVALID"_Stz,
  /* EC_CSV_MINUTE_TOO_LARGE          */ "EC_CSV_MINUTE_TOO_LARGE"_Stz,
  /* EC_CSV_MONTH_INVALID             */ "EC_CSV_MONTH_INVALID"_Stz,
  /* EC_CSV_MONTH_TOO_LARGE           */ "EC_CSV_MONTH_TOO_LARGE"_Stz,
  /* EC_CSV_SECOND_INVALID            */ "EC_CSV_SECOND_INVALID"_Stz,
  /* EC_CSV_SECOND_TOO_LARGE          */ "EC_CSV_SECOND_TOO_LARGE"_Stz,
  /* EC_CSV_TIME_INVALID              */ "EC_CSV_TIME_INVALID"_Stz,
  /* EC_CSV_TIME_SEP_MISSING          */ "EC_CSV_TIME_SEP_MISSING"_Stz,
  /* EC_CSV_TIMESTAMP_INVALID         */ "EC_CSV_TIMESTAMP_INVALID"_Stz,
  /* EC_CSV_VARCHAR_INVALID           */ "EC_CSV_VARCHAR_INVALID"_Stz,
  /* EC_CSV_YEAR_INVALID              */ "EC_CSV_YEAR_INVALID"_Stz,
  /* EC_CSV_YEAR_TOO_SMALL            */ "EC_CSV_YEAR_TOO_SMALL"_Stz,
  /* EC_CSV_YEAR_TOO_LARGE            */ "EC_CSV_YEAR_TOO_LARGE"_Stz,

  /* _D_ */
  /* EC_DECIMAL_NOT_SUPPORTED         */ "decimal only with scale 2 currently supported"_Stz,
  /* EC_DUPLICATE_COLUMN              */ "duplicate column"_Stz,
  /* EC_DUPLICATE_KEY                 */ "duplicate key"_Stz,

  /* _E_ */
  /* EC_ESCAPE_SINGLE_CHAR            */ "ESCAPE clause can only contain a single character"_Stz, // SQL Compiler Subsystem
  /* EC_EXPLAIN_INVALID_FORMAT        */ "Invalid EXPLAIN format. Must be 'PRETTY' or 'ROW'"_Stz, // SQL Compiler Subsystem
  /* EC_EXPR_AGG_MIX_NO_GROUPBY_LIST  */ "Cannot select both aggregate and non-aggregate expressions without a GROUP BY clause"_Stz, // SQL Compiler Subsystem
  /* EC_EXPR_INVALID_COMBINATION      */ "Expression type combination is invalid or unsupported for the expression operator"_Stz,    // SQL Compiler Subsystem
  /* EC_EXPR_IS_AGG_IN_GROUPBY_LIST   */ "Cannot have an aggregate expression in the GROUP BY list"_Stz, // SQL Compiler Subsystem
  /* EC_EXPR_IS_LIT_IN_GROUPBY_LIST   */ "Cannot have a literal expression in the GROUP BY list"_Stz,    // SQL Compiler Subsystem
  /* EC_EXPR_MUST_BE_BOOLEAN          */ "Predicate expression must have a boolean result"_Stz,          // SQL Compiler Subsystem
  /* EC_EXPR_MUST_BE_EQUIJOIN         */ "Join predicate expression must be an equi-join"_Stz,           // SQL Compiler Subsystem
  /* EC_EXPR_NOT_IN_GROUPBY_LIST      */ "Expression is not present in the GROUP BY list"_Stz,           // SQL Compiler Subsystem
  /* EC_EXPR_NOT_PROJECTED            */ "Invalid expression because it is not being projected"_Stz,     // SQL Compiler Subsystem
  /* EC_EXPR_TYPE_INCOMPATIBLE        */ "Incompatible expression types"_Stz,                            // SQL Compiler Subsystem

  /* _F_ */
  /* EC_FOLDER_IN_USE                 */ "folder in use"_Stz,
  /* EC_FOLDER_NOT_EMPTY              */ "folder not empty"_Stz,

  /* _G_ */

  /* _H_ */
  /* EC_HAVING_EXPR_NOT_AGG_OR_IN_GROUPBY */ "The HAVING expression is not an aggregate or not in the GROUP BY list"_Stz,  // SQL Compiler Subsystem

  /* _I_ */
  /* EC_IDXVFY_ADJACENT_ENTRIES       */ "EC_IDXVFY_ADJACENT_ENTRIES"_Stz,
  /* EC_IDXVFY_BUFFER_REFS            */ "EC_IDXVFY_BUFFER_REFS"_Stz,
  /* EC_IDXVFY_FIRST_KEYS             */ "EC_IDXVFY_FIRST_KEYS"_Stz,
  /* EC_IDXVFY_ITEMS                  */ "EC_IDXVFY_ITEMS"_Stz,
  /* EC_IDXVFY_KEY_BUFFER             */ "EC_IDXVFY_KEY_BUFFER"_Stz,
  /* EC_IDXVFY_KEY_ORDER              */ "EC_IDXVFY_KEY_ORDER"_Stz,
  /* EC_IDXVFY_NEXT_PREV              */ "EC_IDXVFY_NEXT_PREV"_Stz,
  /* EC_IDXVFY_ROW_REFS               */ "EC_IDXVFY_ROW_REFS"_Stz,
  /* EC_IDXVFY_SKIPLIST_ENTRY         */ "EC_IDXVFY_SKIPLIST_ENTRY"_Stz,
  /* EC_IDXVFY_SKIPLIST_LEVELS        */ "EC_IDXVFY_SKIPLIST_LEVELS"_Stz,
  /* EC_INDEX_EXISTS                  */ "index exists"_Stz,
  /* EC_INSUFFICIENT_EXPRS            */ "insufficient expressions specified"_Stz,          // SQL Compiler Subsystem
  /* EC_INTERNAL_ERROR                */ "internal error"_Stz,
  /* EC_INTERVAL_NOT_SUPPORTED        */ "interval currently not supported"_Stz,
  /* EC_INVALID_CLASS                 */ "invalid class"_Stz,
  /* EC_INVALID_COLUMN                */ "invalid column"_Stz,
  /* EC_INVALID_COLUMNS_UNIQUE_IND    */ "invalid columns for unique index (include all partitioning columns)"_Stz,
  /* EC_INVALID_COMMAND               */ "invalid command"_Stz,
  /* EC_INVALID_DATE_FORMAT           */ "Invalid date string format. Must be of the form 'YYYY-MM-DD'"_Stz,    // SQL Compiler Subsystem
  /* EC_INVALID_FILE                  */ "File does not exist : '%v'"_Stz,
  /* EC_INVALID_FLOAT_FORMAT          */ "Invalid format of floating point literal"_Stz,    // SQL Compiler Subsystem
  /* EC_INVALID_INTEGER_FORMAT        */ "Invalid format of integer literal"_Stz,           // SQL Compiler Subsystem
  /* EC_INVALID_INTERFACE             */ "invalid interface"_Stz,
  /* EC_INVALID_LIMIT_VALUE           */ "LIMIT value must be >= 0"_Stz,                    // SQL Compiler Subsystem
  /* EC_INVALID_OFFSET_VALUE          */ "OFFSET value must be >= 0"_Stz,                   // SQL Compiler Subsystem
  /* EC_INVALID_PARAMETER             */ "invalid parameter"_Stz,
  /* EC_INVALID_PARAMETERS            */ "invalid parameters"_Stz,
  /* EC_INVALID_PARTITION             */ "invalid partition"_Stz,
  /* EC_INVALID_PARTITION_KEY         */ "invalid partition key"_Stz,
  /* EC_INVALID_INDEX_COLUMN          */ "invalid index column"_Stz,
  /* EC_INVALID_INSTANCE              */ "invalid instance"_Stz,
  /* EC_INVALID_SIZE                  */ "invalid size"_Stz,
  /* EC_INVALID_SYNTAX                */ "invalid syntax"_Stz,
  /* EC_INVALID_TIME_FORMAT           */ "Invalid time string format. Must be of the form: 'hh:mm:ss'"_Stz,    // SQL Compiler Subsystem
  /* EC_INVALID_TIMESTAMP_FORMAT      */ "Invalid timestamp string format. Must be of the form: 'YYYY-MM-DD hh:mm:ss[.ffffff]'"_Stz,    // SQL Compiler Subsystem
  /* EC_INVALID_VALUE                 */ "invalid value"_Stz,

  /* _J_ */

  /* _K_ */

  /* _L_ */
  /* EC_LOW_MEMORY                    */ "low memory"_Stz,

  /* _M_ */
  /* EC_MAX_INDEXES                   */ "max allowed indexes already created"_Stz,
  /* EC_MAX_COLUMNS                   */ "max 150 columns supported in a table"_Stz,
  /* EC_MAX_ROW_LENGTH                */ "max row length allowed is 16k bytes"_Stz,

  /* _N_ */
  /* EC_NO_COLUMNS_DIC                */ "no columns dictionary"_Stz,
  /* EC_NO_SUCH_CATALOG               */ "no such catalog"_Stz,
  /* EC_NO_SUCH_COLUMN                */ "No such column"_Stz,
  /* EC_NO_SUCH_DIRECTORY             */ "no such directory"_Stz,
  /* EC_NO_SUCH_FILE                  */ "no such file"_Stz,
  /* EC_NO_SUCH_FOLDER                */ "no such folder"_Stz,
  /* EC_NO_SUCH_INDEX                 */ "no such index"_Stz,
  /* EC_NO_SUCH_JOB                   */ "no such job"_Stz,
  /* EC_NO_SUCH_NODE                  */ "no such node"_Stz,
  /* EC_NO_SUCH_SCHEMA                */ "no such schema"_Stz,
  /* EC_NO_SUCH_TABLE                 */ "Table does not exist : '%v'"_Stz,
  /* EC_NO_SUCH_TABLE2                */ "No such table"_Stz,          // SQL Compiler Subsystem
  /* EC_NO_SUCH_TENANT                */ "no such tenant"_Stz,
  /* EC_NO_SUCH_WORD                  */ "no such word"_Stz,
  /* EC_NODE_EXISTS                   */ "node exists"_Stz,
  /* EC_NOT_A_CATALOG                 */ "not a catalog"_Stz,
  /* EC_NOT_A_FOLDER                  */ "not a folder"_Stz,
  /* EC_NOT_AN_INDEX                  */ "'%v' not an index"_Stz,
  /* EC_NOT_A_SCHEMA                  */ "not a schema"_Stz,
  /* EC_NOT_A_TABLE                   */ "not a table"_Stz,
  /* EC_NOT_A_TENANT                  */ "not a tenant"_Stz,
  /* EC_NOT_IMPLEMENTED               */ "Feature not yet implemented"_Stz,   // SQL Compiler Subsystem
  /* EC_NOT_IN_CATALOGS               */ "not in catalogs"_Stz,
  /* EC_NOT_IN_COLUMNS                */ "not in columns"_Stz,
  /* EC_NOT_IN_INDEXES                */ "not in indexes"_Stz,
  /* EC_NOT_IN_PARTITIONS             */ "not in partitions"_Stz,
  /* EC_NOT_IN_SCHEMAS                */ "not in schemas"_Stz,
  /* EC_NOT_IN_TABLE                  */ "not in table"_Stz,
  /* EC_NOT_IN_TABLES                 */ "not in tables"_Stz,
  /* EC_NOT_IN_TENANTS                */ "not in tenants"_Stz,
  /* EC_NULL_INTERFACE                */ "null interface"_Stz,
  /* EC_NULLPTR                       */ "nullptr"_Stz,

  /* _O_ */
  /* EC_OBJECT_ADDRESS_INVALID        */ "Object address is invalid"_Stz,
  /* EC_OBJECT_CLASSNO_INVALID        */ "EC_OBJECT_CLASSNO_INVALID"_Stz,
  /* EC_OBJECT_FOOTER_INVALID         */ "EC_OBJECT_FOOTER_INVALID"_Stz,
  /* EC_OBJECT_MAGIC_INVALID          */ "EC_OBJECT_MAGIC_INVALID"_Stz,
  /* EC_ORDERBY_NOT_SUPPORTED         */ "order by currently not supported"_Stz,
  /* _P_ */
  /* EC_PIPELINE_PROTOCOL_ERROR       */ "pipeline protocol error"_Stz,

  /* _Q_ */

  /* _R_ */
  /* EC_RESPONSE_PENDING              */ "response pending"_Stz,

  /* _S_ */
  /* EC_SCHEMA_EXISTS                 */ "schema exists"_Stz,
  /* EC_SE_NOT_RUNNING                */ "SE is not running"_Stz,
  /* EC_SE_UNKNOWN_ERROR              */ "SE unknown error"_Stz,
  /* EC_SEMANTIC_ERROR                */ "semantic error"_Stz,              // SQL Compiler Subsystem
  /* EC_SEND_FAILURE                  */ "send failure"_Stz,
  /* EC_SUBQUERY_SELF_REFERENCE       */ "Non-recursive subquery cannot refer to itself"_Stz,  // SQL Compiler Subsystem
  /* EC_SUBQUERY_PROJ_TOO_MANY_COLS   */ "Subquery cannot project more than one column"_Stz,   // SQL Compiler Subsystem
  /* EC_SYNTAX_ERROR                  */ "syntax error"_Stz,                // SQL Compiler Subsystem. Custom error string from ANTLR.
  /* EC_SYSTEM_CLOSED                 */ "system closed"_Stz,

  /* _T_ */
  /* EC_TABLE_ALREADY_LOADED          */ "table already loaded"_Stz,
  /* EC_TABLE_EXISTS                  */ "table exists"_Stz,
  /* EC_TABLE_NOT_EMPTY               */ "table not empty"_Stz,
  /* EC_TABLE_IN_USE                  */ "table in use"_Stz,
  /* EC_TABLE_SUBQUERY_OUT_OF_SCOPE   */ "Table name refers to a table subquery alias that is not in scope"_Stz,    // SQL Compiler Subsystem
  /* EC_TENANT_EXISTS                 */ "tenant exists"_Stz,
  /* EC_TINYC_ERROR                   */ "tinyC compilation error"_Stz,
  /* EC_TOO_MANY_NAMED_COLUMNS        */ "too many named columns in WITH clause"_Stz,
  /* EC_TX_BEGIN_FAILED               */ "begin transaction failed"_Stz,
  /* EC_TX_COMMIT_FAILED              */ "commit transaction failed"_Stz,
  /* EC_TX_END_FAILED                 */ "end transaction failed"_Stz,
  /* EC_TX_ROLLBACK_FAILED            */ "rollback transaction failed"_Stz,

  /* _U_ */
  /* EC_UNRECOGNISED_HINT_VALUE       */ "Unrecognised hint value"_Stz,
  /* EC_UNRESOLVED_COLUMN_WILDCARD    */ "Column wildcard does not resolve to any tables in the query"_Stz,
  /* EC_USING_COLUMN_NOT_FOUND        */ "Column name in USING clause is not found"_Stz,

  /* _V_ */
  /* EC_VALIDATION_FAILURE            */ "validation failure"_Stz,
  /* EV_VALUE_OUT_OF_RANGE            */ "Value is out of range"_Stz,
  /* EC_VERSION_MISMATCH              */ "version mismatch"_Stz,

  /* _W_ */

  /* _X_ */

  /* _Y_ */

  /* _Z_ */
  /* EC_ZZZ_OTHER                     */ "zzz other error (temp filler)"_Stz,

  // - - - - - - - - - - - - - - - - - - -

  /* EC_ENUMS                         */ "enums"_Stz

};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
