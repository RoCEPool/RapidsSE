/*============================================================================*/
/* Error -- errors                                                            */
/*============================================================================*/

#pragma once

#include "Magic.h"

namespace Magic
{
  enum eEC // error code
  {
    /*  0 */ EC_NONE=0,
    /*  1 */ EC_ASSERT,
    /*  2 */ EC_BEGIN_TX_FAILED,
    /*  3 */ EC_CATALOG_EXISTS,
    /*  4 */ EC_CLUSTER_NOT_STARTED,
    
    /*  5 */ EC_FOLDER_IN_USE,
    /*  6 */ EC_FOLDER_NOT_EMPTY,
    
    /*  7 */ EC_INVALID_CLASS,
    /*  8 */ EC_INVALID_COMMAND,
    /*  9 */ EC_INVALID_FILE,
    /* 10 */ EC_INVALID_INTERFACE,
    /* 11 */ EC_INVALID_PARAMETER,
    /* 12 */ EC_INVALID_PARAMETERS,
    /* 13 */ EC_INVALID_INSTANCE,
    /* 14 */ EC_INVALID_SYNTAX,

    /* 15 */ EC_LOW_BUFFERS,
    
    /* 16 */ EC_NO_COLUMNS_DIC,
    
    /* 17 */ EC_NO_SUCH_CATALOG,
    /* 18 */ EC_NO_SUCH_DIRECTORY,
    /* 19 */ EC_NO_SUCH_FOLDER,
    /* 20 */ EC_NO_SUCH_SCHEMA,
    /* 21 */ EC_NO_SUCH_TABLE,
    /* 22 */ EC_NO_SUCH_TENANT,
    /* 23 */ EC_NO_SUCH_WORD,
    
    /* 24 */ EC_NOT_A_CATALOG,
    /* 25 */ EC_NOT_A_FOLDER,
    /* 26 */ EC_NOT_A_SCHEMA,
    /* 27 */ EC_NOT_A_TABLE,
    /* 28 */ EC_NOT_A_TENANT,
    
    /* 29 */ EC_NOT_IN_CATALOGS,
    /* 30 */ EC_NOT_IN_COLUMNS,
    /* 31 */ EC_NOT_IN_SCHEMAS,
    /* 32 */ EC_NOT_IN_TABLES,
    /* 33 */ EC_NOT_IN_TENANTS,

    /* 34 */ EC_NULL_INTERFACE,
    /* 35 */ EC_NULLPTR,
    
    /* 36 */ EC_RESPONSE_PENDING,
    /* 37 */ EC_SCHEMA_EXISTS,
    /* 38 */ EC_SEND_FAILURE,
    /* 39 */ EC_SE_NOT_RUNNING,

    /* 40 */ EC_TABLE_EXISTS,
    /* 41 */ EC_TABLE_IN_USE,
    /* 42 */ EC_TENANT_EXISTS,
    
    /* 43 */ EC_VERSION_MISMATCH,
    /* 44 */ EC_ENUMS
  };
  
  static const char* const pEC[]
  {
    /*  0 */ "none",
    /*  1 */ "assert",
    /*  2 */ "begin transaction failed",
    /*  3 */ "catalog exists",
    /*  4 */ "folder in use",
    
    /*  5 */ "cluster not started",
    
    /*  6 */ "invalid class",
    /*  7 */ "invalid command",
    /*  8 */ "invalid file",
    /*  9 */ "invalid interface",
    /* 10 */ "invalid parameter",
    /* 11 */ "invalid parameters",
    /* 12 */ "invalid instance",
    /* 13 */ "invalid syntax",
    
    /* 14 */ "low buffers",
    
    /* 15 */ "no columns dictionary",
    
    /* 16 */ "no such catalog",
    /* 17 */ "no such directory",
    /* 18 */ "no such folder",
    /* 19 */ "no such schema",
    /* 20 */ "no such table",
    /* 21 */ "no such tenant",
    /* 22 */ "no such word",
    
    /* 23 */ "not a catalog",
    /* 24 */ "not a folder",
    /* 25 */ "not a schema",
    /* 26 */ "not a table",
    /* 27 */ "not a tenant",
    
    /* 28 */ "not in catalogs",
    /* 29 */ "not in columns",
    /* 30 */ "not in schemas",
    /* 31 */ "not in tables",
    /* 32 */ "not in tenants",
    
    /* 33 */ "null interface",
    /* 34 */ "nullptr",
    
    /* 35 */ "response pending",
  
    /* 36 */ "schema exists",
    /* 37 */ "SE is not running",
    /* 38 */ "send failure",
    /* 39 */ "table exists",
    /* 40 */ "table in use",
    /* 41 */ "tenant exists",

    /* 42 */ "version mismatch",

    /* 43 */ "enums"

  };

  class Error
  {
  private:
    eEC   m_eErrorCode;
    s256  m_sFileName;
    i8    m_iLineNumber;
    s256  m_sErrorMsg;
    
    Footer m_xFooter;
    
  public:
    Error(eEC eErrorCode, const char* sFile, i8 iLine);
    Error(eEC eErrorCode, const char *errormsg, const char* sFile, i8 iLine);
    
    const char* pErrorMsg(void);
    char*       pFileName(void);
    i8          iLineNumber(void);
    
    Footer* pFooter(void);
  };
  
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
