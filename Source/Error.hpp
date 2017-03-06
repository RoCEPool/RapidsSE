/*============================================================================*/
/* Error.hpp -- runtime errors                                                */
/*============================================================================*/
#pragma once

namespace Debugging
{
  enum eEC // error code
  {
  /*  0 */ EC_NONE=0,
  /*  1 */ EC_ASSERT,
  /*  2 */ EC_BEGIN_TX_FAILED,
  /*  3 */ EC_CATALOG_EXISTS,
  /*  4 */ EC_CLUSTER_NOT_STARTED,
  
  /*  5 */ EC_DUPLICATE_COLUMN,
  /*  6 */ EC_FOLDER_IN_USE,
  /*  7 */ EC_FOLDER_NOT_EMPTY,
  
  /*  8 */ EC_INDEX_EXISTS,
    
  /*  9 */ EC_INVALID_CLASS,
  /* 10 */ EC_INVALID_COMMAND,
  /* 11 */ EC_INVALID_FILE,
  /* 12 */ EC_INVALID_INTERFACE,
  /* 13 */ EC_INVALID_PARAMETER,
  /* 14 */ EC_INVALID_PARAMETERS,
  /* 15 */ EC_INVALID_INSTANCE,
  /* 16 */ EC_INVALID_SYNTAX,
  
  /* 17 */ EC_LOW_BUFFERS,
  
  /* 18 */ EC_NO_COLUMNS_DIC,
  
  /* 19 */ EC_NO_SUCH_CATALOG,
  /* 20 */ EC_NO_SUCH_DIRECTORY,
  /* 21 */ EC_NO_SUCH_FOLDER,
  /* 22 */ EC_NO_SUCH_INDEX,
  /* 23 */ EC_NO_SUCH_SCHEMA,
  /* 24 */ EC_NO_SUCH_TABLE,
  /* 25 */ EC_NO_SUCH_TENANT,
  /* 26 */ EC_NO_SUCH_WORD,
  
  /* 27 */ EC_NOT_A_CATALOG,
  /* 28 */ EC_NOT_A_FOLDER,
  /* 29   */ EC_NOT_AN_INDEX,
  /* 30 */ EC_NOT_A_SCHEMA,
  /* 31 */ EC_NOT_A_TABLE,
  /* 32 */ EC_NOT_A_TENANT,
    
  /* 33 */ EC_NOT_IN_CATALOGS,
  /* 34 */ EC_NOT_IN_COLUMNS,
  /* 35 */ EC_NOT_IN_INDEXES,
  /* 36 */ EC_NOT_IN_SCHEMAS,
  /* 37 */ EC_NOT_IN_TABLE,
  /* 38 */ EC_NOT_IN_TABLES,
  /* 39 */ EC_NOT_IN_TENANTS,
  
  /* 40 */ EC_NULL_INTERFACE,
  /* 41 */ EC_NULLPTR,
  
  /* 42 */ EC_RESPONSE_PENDING,
  /* 43 */ EC_SCHEMA_EXISTS,
  /* 44 */ EC_SE_NOT_RUNNING,
  /* 45 */ EC_SEND_FAILURE,
  
  /* 46 */ EC_TABLE_EXISTS,
  /* 47 */ EC_TABLE_IN_USE,
  /* 48 */ EC_TENANT_EXISTS,
  
  /* 49 */ EC_VERSION_MISMATCH,
  /* 50 */ EC_ENUMS
  };
  
  static const char* const pEC[]
  {
  /*  0 */ "none",
  /*  1 */ "assert",
  /*  2 */ "begin transaction failed",
  /*  3 */ "catalog exists",
  /*  4 */ "cluster not started",
  
  /*  5 */ "duplicate column",
  /*  6 */ "folder in use",
  /*  7 */ "folder not empty",
  
  /*  8 */ "index exists",
    
  /*  9 */ "invalid class",
  /* 10 */ "invalid command",
  /* 11 */ "invalid file",
  /* 12 */ "invalid interface",
  /* 13 */ "invalid parameter",
  /* 14 */ "invalid parameters",
  /* 15 */ "invalid instance",
  /* 16 */ "invalid syntax",
  
  /* 17 */ "low buffers",
  
  /* 18 */ "no columns dictionary",
  
  /* 19 */ "no such catalog",
  /* 20 */ "no such directory",
  /* 21 */ "no such folder",
  /* 22 */ "no such index",
  /* 23 */ "no such schema",
  /* 24 */ "no such table",
  /* 25 */ "no such tenant",
  /* 26 */ "no such word",
  
  /* 27 */ "not a catalog",
  /* 28 */ "not a folder",
  /* 29 */ "not an index",
  /* 30 */ "not a schema",
  /* 31 */ "not a table",
  /* 32 */ "not a tenant",
    
  /* 33 */ "not in catalogs",
  /* 34 */ "not in columns",
  /* 35 */ "not in indexes",
  /* 36 */ "not in schemas",
  /* 37 */ "not in table",
  /* 38 */ "not in tables",
  /* 39 */ "not in tenants",
  
  /* 40 */ "null interface",
  /* 41 */ "nullptr",
  
  /* 42 */ "response pending",
  
  /* 43 */ "schema exists",
  /* 44 */ "SE is not running",
  /* 45 */ "send failure",
  /* 46 */ "table exists",
  /* 47 */ "table in use",
  /* 48 */ "tenant exists",

  /* 49 */ "version mismatch",
  
  /* 50 */ "enums"
  
  };
  
  class Error
  {
  private:
    i8    m_iErrorCode;
    char  m_aFileName[256];
    i8    m_iLineNo;
    char  m_aErrorMsg[256];
  
  public:
    Error(i8 iErrorCode, const char* aFileName, i8 iLineNo);
    Error(i8 iErrorCode, const char* aErrorMsg, const char* aFileName, i8 iLineNo);
    
    i8          iErrorCode(void);
    const char* aErrorMsg(void);
    const char* aFileName(void);
    i8          iLineNo(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
