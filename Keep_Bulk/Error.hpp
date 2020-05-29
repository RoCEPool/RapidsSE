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
    /*  4 */ "cluster not started",
    
    /*  5 */ "folder in use",
    /*  6 */ "folder not empty",
    
    /*  7 */ "invalid class",
    /*  8 */ "invalid command",
    /*  9 */ "invalid file",
    /* 10 */ "invalid interface",
    /* 11 */ "invalid parameter",
    /* 12 */ "invalid parameters",
    /* 13 */ "invalid instance",
    /* 14 */ "invalid syntax",
    
    /* 15 */ "low buffers",
    
    /* 16 */ "no columns dictionary",
    
    /* 17 */ "no such catalog",
    /* 18 */ "no such directory",
    /* 19 */ "no such folder",
    /* 20 */ "no such schema",
    /* 21 */ "no such table",
    /* 22 */ "no such tenant",
    /* 23 */ "no such word",
    
    /* 24 */ "not a catalog",
    /* 25 */ "not a folder",
    /* 26 */ "not a schema",
    /* 27 */ "not a table",
    /* 28 */ "not a tenant",
    
    /* 29 */ "not in catalogs",
    /* 30 */ "not in columns",
    /* 31 */ "not in schemas",
    /* 32 */ "not in tables",
    /* 33 */ "not in tenants",
    
    /* 34 */ "null interface",
    /* 35 */ "nullptr",
    
    /* 36 */ "response pending",
  
    /* 37 */ "schema exists",
    /* 38 */ "SE is not running",
    /* 39 */ "send failure",
    /* 40 */ "table exists",
    /* 41 */ "table in use",
    /* 42 */ "tenant exists",

    /* 43 */ "version mismatch",

    /* 44 */ "enums"

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
