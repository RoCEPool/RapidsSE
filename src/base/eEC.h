/*============================================================================*/
/* eEC.h -- Error Codes                                                       */
/*============================================================================*/
#pragma once

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// NOTE:
// When modifying this enum, also refer to the corresponding *.cpp file for
// modifying the corresponding global variables declared below.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


enum eEC // error codes
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
  EC_COMMAND_TOO_LONG,
  EC_COMPILATION_FAILED,
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
  EC_DATE_OUT_OF_RANGE,
  EC_DECIMAL_NOT_SUPPORTED,
  EC_DISABLED_STATEMENT,
  EC_DISK_SPACE_CHECK_FAILED,
  EC_DIVIDE_BY_ZERO,
  EC_DUPLICATE_COLUMN,
  EC_DUPLICATE_KEY,
  EC_DYN_LINK_FAILED,
  EC_DYN_SYM_RESOLVE_FAILED,
  EC_DYN_UNLINK_FAILED,

  /* _E_ */
  EC_ESCAPE_SINGLE_CHAR,            // SQL Compiler Subsystem
  EC_EXPLAIN_INVALID_FORMAT,        // SQL Compiler Subsystem
  EC_EXPR_AGG_MIX_NO_GROUPBY_LIST,  // SQL Compiler Subsystem
  EC_EXPR_INVALID_COMBINATION,      // SQL Compiler Subsystem
  EC_EXPR_IS_AGG_IN_GROUPBY_LIST,   // SQL Compiler Subsystem
  EC_EXPR_IS_LIT_IN_GROUPBY_LIST,   // SQL Compiler Subsystem
  EC_EXPR_MUST_BE_BOOLEAN,          // SQL Compiler Subsystem
  EC_EXPR_MUST_BE_EQUIJOIN,         // SQL Compiler Subsystem
  EC_EXPR_MUST_BE_LITERAL,          // SQL Compiler Subsystem
  EC_EXPR_NESTED_AGGREGATE,         // SQL Compiler Subsystem
  EC_EXPR_NOT_IN_GROUPBY_LIST,      // SQL Compiler Subsystem
  EC_EXPR_NOT_PROJECTED,            // SQL Compiler Subsystem
  EC_EXPR_TYPE_INCOMPATIBLE,        // SQL Compiler Subsystem

  /* _F_ */
  EC_FILE_IS_EMPTY,
  EC_FILE_TOO_LARGE,
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
  EC_INTERVAL_OUT_OF_RANGE,
  EC_INVALID_CAST,
  EC_INVALID_CLASS,
  EC_INVALID_COLUMN,
  EC_INVALID_COLUMNS_UNIQUE_INDEX,
  EC_INVALID_COMMAND,
  EC_INVALID_DATE_FORMAT,           // SQL Compiler Subsystem
  EC_INVALID_FILE,
  EC_INVALID_FLOAT_FORMAT,          // SQL Compiler Subsystem
  EC_INVALID_INTEGER_FORMAT,        // SQL Compiler Subsystem
  EC_INVALID_INTERFACE,
  EC_INVALID_LOAD_OPT_DATE_FORMAT,
  EC_INVALID_LOAD_OPT_ENCL_TRAILING,
  EC_INVALID_LOAD_OPT_RECORD_ENDS,
  EC_INVALID_LIMIT_VALUE,           // SQL Compiler Subsystem
  EC_INVALID_OFFSET_VALUE,          // SQL Compiler Subsystem
  EC_INVALID_PARAMETER,
  EC_INVALID_PARAMETERS,
  EC_INVALID_PARTITION,
  EC_INVALID_PARTITION_KEY,
  EC_INVALID_PIPELINE,
  EC_INVALID_INDEX_COLUMN,
  EC_INVALID_INSTANCE,
  EC_INVALID_INTERVAL_FORMAT,      // SQL Compiler Subsystem
  EC_INVALID_SIZE,
  EC_INVALID_SYNTAX,
  EC_INVALID_TIME_FORMAT,           // SQL Compiler Subsystem
  EC_INVALID_TIMESTAMP_FORMAT,      // SQL Compiler Subsystem
  EC_INVALID_TRACE_NAME,
  EC_INVALID_TOKEN,
  EC_INVALID_TYPES,
  EC_INVALID_VALUE,
  EC_IO_ERROR,

  /* _J_ */

  /* _K_ */

  /* _L_ */
  EC_LOW_DISK_SPACE,
  EC_LOW_MEMORY,

  /* _M_ */
  EC_MAX_IDENTIFIER_NAME_LENGTH,
  EC_MAX_INDEX_NAME_LENGTH,
  EC_MAX_INDEXES,
	EC_MAX_COLUMN_NAME_LENGTH,
  EC_MAX_COLUMNS,
  EC_MAX_ROW_LENGTH,
  EC_MAX_TABLE_NAME_LENGTH,
  EC_MEMORY_ALREADY_TRACKED,
  EC_MEMORY_NOT_BEING_TRACKED,
  EC_MISSING_JOIN_INDEX,
  EC_MISSING_OP,
  EC_MISSING_SCAN_OP,
	EC_MULTIPLE_COLUMN_PARTITION_KEY,

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
  EC_NODE_LOSS,
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
  EC_ORDERBY_WITH_AGGREGATES,


  /* _P_ */
  EC_PIPELINE_PROTOCOL_ERROR,

  /* _Q_ */

  /* _R_ */
  EC_RESERVED_WORD,
  EC_RESPONSE_PENDING,

  /* _S_ */
  EC_SCALE_OUT_OF_BOUNDS,
  EC_SCHEMA_EXISTS,
  EC_SE_NOT_RUNNING,
  EC_SE_UNKNOWN_ERROR,
  EC_SEMANTIC_ERROR,                // SQL Compiler Subsystem
  EC_SEND_FAILURE,
  EC_STATEMENT_TOO_LONG,            // SQL Compiler Subsystem
  EC_SUBQUERY_SELF_REFERENCE,       // SQL Compiler Subsystem
  EC_SUBQUERY_PROJ_TOO_MANY_COLS,   // SQL Compiler Subsystem
  EC_SYNTAX_ERROR,                  // SQL Compiler Subsystem
  EC_SYNTHCODE_COMPILATION_ERROR,
  EC_SYNTHCODE_LINKING_ERROR,
  EC_SYSTEM_CLOSED,

  /* _T_ */
  EC_TABLE_ALREADY_LOADED,
  EC_TABLE_EXISTS,
  EC_TABLE_NOT_EMPTY,
  EC_TABLE_IN_USE,
  EC_TABLE_SUBQUERY_OUT_OF_SCOPE,   // SQL Compiler Subsystem
  EC_TENANT_EXISTS,
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
  EC_WL_DISCONNECT_REQ,
  EC_WL_FEATURE_NOT_SUPPORTED,
  EC_WL_INTERNAL_ERROR,
  EC_WL_MSG_NOT_SUPPORTED,
  EC_WL_MSG_TOO_BIG,
  EC_WL_PROTOCOL_ERROR,
  EC_WL_PROTOCOL_VER_NOT_SUPPORTED,
  EC_WL_RESULTSET_RETURNED,
  EC_WL_ROW_TOO_BIG,
  EC_WL_SOCKET_CLOSED,

  /* _X_ */

  /* _Y_ */

  /* _Z_ */
  EC_ZZZ_OTHER,

  EC_ENUMS
};


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// NOTE: Refer to the corresponding *.cpp file for modifying the below global variables.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// This should contain the text string representation of the EC enum.
// Not the error message.
extern Stz* aEC[EC_ENUMS];

// This should contain the text string representation of the EC enum.
// Not the error message.
extern Stz* aEC2[EC_ENUMS];

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
extern Stz* aErrorMsg[EC_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
