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
  
  /*  8 */ EC_INVALID_CLASS,
  /*  9 */ EC_INVALID_COMMAND,
  /* 10 */ EC_INVALID_FILE,
  /* 11 */ EC_INVALID_INTERFACE,
  /* 12 */ EC_INVALID_PARAMETER,
  /* 13 */ EC_INVALID_PARAMETERS,
  /* 14 */ EC_INVALID_INSTANCE,
  /* 15 */ EC_INVALID_SYNTAX,
  
  /* 16 */ EC_LOW_BUFFERS,
  
  /* 17 */ EC_NO_COLUMNS_DIC,
  
  /* 18 */ EC_NO_SUCH_CATALOG,
  /* 19 */ EC_NO_SUCH_DIRECTORY,
  /* 20 */ EC_NO_SUCH_FOLDER,
  /* 21 */ EC_NO_SUCH_SCHEMA,
  /* 22 */ EC_NO_SUCH_TABLE,
  /* 23 */ EC_NO_SUCH_TENANT,
  /* 24 */ EC_NO_SUCH_WORD,
  
  /* 25 */ EC_NOT_A_CATALOG,
  /* 26 */ EC_NOT_A_FOLDER,
  /* 27 */ EC_NOT_A_SCHEMA,
  /* 28 */ EC_NOT_A_TABLE,
  /* 29 */ EC_NOT_A_TENANT,
    
  /* 30 */ EC_NOT_IN_CATALOGS,
  /* 31 */ EC_NOT_IN_COLUMNS,
  /* 32 */ EC_NOT_IN_SCHEMAS,
  /* 33 */ EC_NOT_IN_TABLES,
  /* 34 */ EC_NOT_IN_TENANTS,
  
  /* 35 */ EC_NULL_INTERFACE,
  /* 36 */ EC_NULLPTR,
  
  /* 37 */ EC_RESPONSE_PENDING,
  /* 38 */ EC_SCHEMA_EXISTS,
  /* 39 */ EC_SEND_FAILURE,
  /* 40 */ EC_SE_NOT_RUNNING,
  
  /* 41 */ EC_TABLE_EXISTS,
  /* 42 */ EC_TABLE_IN_USE,
  /* 43 */ EC_TENANT_EXISTS,
  
  /* 44 */ EC_VERSION_MISMATCH,
  /* 45 */ EC_ENUMS
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
  /* 21 */ "no such schema",
  /* 22 */ "no such table",
  /* 23 */ "no such tenant",
  /* 24 */ "no such word",
  
  /* 25 */ "not a catalog",
  /* 26 */ "not a folder",
  /* 27 */ "not a schema",
  /* 28 */ "not a table",
  /* 29 */ "not a tenant",
    
  /* 30 */ "not in catalogs",
  /* 31 */ "not in columns",
  /* 32 */ "not in schemas",
  /* 33 */ "not in tables",
  /* 34 */ "not in tenants",
  
  /* 35 */ "null interface",
  /* 36 */ "nullptr",
  
  /* 37 */ "response pending",
  
  /* 38 */ "schema exists",
  /* 39 */ "SE is not running",
  /* 40 */ "send failure",
  /* 41 */ "table exists",
  /* 42 */ "table in use",
  /* 43 */ "tenant exists",
  
  /* 44 */ "version mismatch",
  
  /* 45 */ "enums"
  
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
