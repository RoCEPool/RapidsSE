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

    /*  4 */ EC_FOLDER_IN_USE,
    /*  5 */ EC_CLUSTER_NOT_STARTED,
    
    /*  6 */ EC_INVALID_CLASS,
    /*  7 */ EC_INVALID_COMMAND,
    /*  8 */ EC_INVALID_FILE,
    /*  9 */ EC_INVALID_INTERFACE,
    /* 10 */ EC_INVALID_PARAMETER,
    /* 11 */ EC_INVALID_PARAMETERS,
    /* 12 */ EC_INVALID_INSTANCE,
    /* 13 */ EC_INVALID_SYNTAX,

    /* 14 */ EC_LOW_BUFFERS,
    
    /* 15 */ EC_NO_COLUMNS_DIC,
    
    /* 16 */ EC_NO_SUCH_CATALOG,
    /* 17 */ EC_NO_SUCH_DIRECTORY,
    /* 18 */ EC_NO_SUCH_FOLDER,
    /* 19 */ EC_NO_SUCH_SCHEMA,
    /* 20 */ EC_NO_SUCH_TABLE,
    /* 21 */ EC_NO_SUCH_TENANT,
    /* 22 */ EC_NO_SUCH_WORD,
    
    /* 23 */ EC_NOT_A_CATALOG,
    /* 24 */ EC_NOT_A_FOLDER,
    /* 25 */ EC_NOT_A_SCHEMA,
    /* 26 */ EC_NOT_A_TABLE,
    /* 27 */ EC_NOT_A_TENANT,
    
    /* 28 */ EC_NOT_IN_CATALOGS,
    /* 29 */ EC_NOT_IN_COLUMNS,
    /* 30 */ EC_NOT_IN_SCHEMAS,
    /* 31 */ EC_NOT_IN_TABLES,
    /* 32 */ EC_NOT_IN_TENANTS,

    /* 33 */ EC_NULL_INTERFACE,
    /* 34 */ EC_NULLPTR,
    
    /* 35 */ EC_RESPONSE_PENDING,
    /* 36 */ EC_SCHEMA_EXISTS,
    /* 37 */ EC_SEND_FAILURE,
    /* 38 */ EC_SE_NOT_RUNNING,

    /* 39 */ EC_TABLE_EXISTS,
    /* 40 */ EC_TABLE_IN_USE,
    /* 41 */ EC_TENANT_EXISTS,
    
    /* 42 */ EC_VERSION_MISMATCH,
    /* 43 */ EC_ENUMS
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
