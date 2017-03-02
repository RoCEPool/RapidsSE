/*============================================================================*/
/* Error.hpp -- runtime errors                                                */
/*============================================================================*/
#pragma once

#include "Debugging.h"

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
  
  /* 16 */ EC_LOW_BUFFERS,
  
  /* 17 */ EC_NO_COLUMNS_DIC,
  
  /* 18 */ EC_NO_SUCH_CATALOG,
  /* 19 */ EC_NO_SUCH_DIRECTORY,
  /* 20 */ EC_NO_SUCH_FOLDER,
  /* 21 */ EC_NO_SUCH_INDEX,
  /* 22 */ EC_NO_SUCH_SCHEMA,
  /* 23 */ EC_NO_SUCH_TABLE,
  /* 24 */ EC_NO_SUCH_TENANT,
  /* 25 */ EC_NO_SUCH_WORD,
  
  /* 26 */ EC_NOT_A_CATALOG,
  /* 27 */ EC_NOT_A_FOLDER,
  /* 28 */ EC_NOT_A_SCHEMA,
  /* 29 */ EC_NOT_A_TABLE,
  /* 30 */ EC_NOT_A_TENANT,
    
  /* 31 */ EC_NOT_IN_CATALOGS,
  /* 32 */ EC_NOT_IN_COLUMNS,
  /* 33 */ EC_NOT_IN_INDEXES,
  /* 34 */ EC_NOT_IN_SCHEMAS,
  /* 35 */ EC_NOT_IN_TABLES,
  /* 36 */ EC_NOT_IN_TENANTS,
  
  /* 37 */ EC_NULL_INTERFACE,
  /* 38 */ EC_NULLPTR,
  
  /* 39 */ EC_RESPONSE_PENDING,
  /* 40 */ EC_SCHEMA_EXISTS,
  /* 41 */ EC_SEND_FAILURE,
  /* 42 */ EC_SE_NOT_RUNNING,
  
  /* 44 */ EC_TABLE_EXISTS,
  /* 45 */ EC_TABLE_IN_USE,
  /* 46 */ EC_TENANT_EXISTS,
  
  /* 47 */ EC_VERSION_MISMATCH,
  /* 48 */ EC_ENUMS
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
  
  /*  8 */ "invalid class",
  /*  9 */ "invalid command",
  /* 10 */ "invalid file",
  /* 11 */ "invalid interface",
  /* 12 */ "invalid parameter",
  /* 13 */ "invalid parameters",
  /* 14 */ "invalid instance",
  /* 15 */ "invalid syntax",
  
  /* 16 */ "low buffers",
  
  /* 17 */ "no columns dictionary",
  
  /* 18 */ "no such catalog",
  /* 19 */ "no such directory",
  /* 20 */ "no such folder",
  /* 21 */ "no such index",
  /* 22 */ "no such schema",
  /* 23 */ "no such table",
  /* 24 */ "no such tenant",
  /* 25 */ "no such word",
  
  /* 26 */ "not a catalog",
  /* 27 */ "not a folder",
  /* 28 */ "not a schema",
  /* 29 */ "not a table",
  /* 30 */ "not a tenant",
    
  /* 31 */ "not in catalogs",
  /* 32 */ "not in columns",
  /* 33 */ "not in indexes",
  /* 34 */ "not in schemas",
  /* 35 */ "not in tables",
  /* 36 */ "not in tenants",
  
  /* 37 */ "null interface",
  /* 38 */ "nullptr",
  
  /* 39 */ "response pending",
  
  /* 40 */ "schema exists",
  /* 41 */ "SE is not running",
  /* 42 */ "send failure",
  /* 43 */ "table exists",
  /* 44 */ "table in use",
  /* 45 */ "tenant exists",
  
  /* 46 */ "version mismatch",
  
  /* 47 */ "enums"
  
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
