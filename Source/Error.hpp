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
  
  static Str* aEC[]
  {
  /*  0 */ "none"_Str,
  /*  1 */ "assert"_Str,
  /*  2 */ "begin transaction failed"_Str,
  /*  3 */ "catalog exists"_Str,
  /*  4 */ "cluster not started"_Str,
  
  /*  5 */ "duplicate column"_Str,
  /*  6 */ "folder in use"_Str,
  /*  7 */ "folder not empty"_Str,
  
  /*  8 */ "index exists"_Str,
    
  /*  9 */ "invalid class"_Str,
  /* 10 */ "invalid command"_Str,
  /* 11 */ "invalid file"_Str,
  /* 12 */ "invalid interface"_Str,
  /* 13 */ "invalid parameter"_Str,
  /* 14 */ "invalid parameters"_Str,
  /* 15 */ "invalid instance"_Str,
  /* 16 */ "invalid syntax"_Str,
  
  /* 17 */ "low buffers"_Str,
  
  /* 18 */ "no columns dictionary"_Str,
  
  /* 19 */ "no such catalog"_Str,
  /* 20 */ "no such directory"_Str,
  /* 21 */ "no such folder"_Str,
  /* 22 */ "no such index"_Str,
  /* 23 */ "no such schema"_Str,
  /* 24 */ "no such table"_Str,
  /* 25 */ "no such tenant"_Str,
  /* 26 */ "no such word"_Str,
  
  /* 27 */ "not a catalog"_Str,
  /* 28 */ "not a folder"_Str,
  /* 29 */ "not an index"_Str,
  /* 30 */ "not a schema"_Str,
  /* 31 */ "not a table"_Str,
  /* 32 */ "not a tenant"_Str,
    
  /* 33 */ "not in catalogs"_Str,
  /* 34 */ "not in columns"_Str,
  /* 35 */ "not in indexes"_Str,
  /* 36 */ "not in schemas"_Str,
  /* 37 */ "not in table"_Str,
  /* 38 */ "not in tables"_Str,
  /* 39 */ "not in tenants"_Str,
  
  /* 40 */ "null interface"_Str,
  /* 41 */ "nullptr"_Str,
  
  /* 42 */ "response pending"_Str,
  
  /* 43 */ "schema exists"_Str,
  /* 44 */ "SE is not running"_Str,
  /* 45 */ "send failure"_Str,
  /* 46 */ "table exists"_Str,
  /* 47 */ "table in use"_Str,
  /* 48 */ "tenant exists"_Str,

  /* 49 */ "version mismatch"_Str,
  
  /* 50 */ "enums"_Str
  
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
