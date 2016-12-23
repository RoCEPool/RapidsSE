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
    /*    */ EC_BEGIN_TX_FAILED,
    /*  2 */ EC_CATALOG_EXISTS,

    /*  3 */ EC_FOLDER_IN_USE,

    /*  4 */ EC_INVALID_CLASS,
    /*  5 */ EC_INVALID_COMMAND,
    /*  6 */ EC_INVALID_FILE,
    /*  7 */ EC_INVALID_INTERFACE,
    /*  8 */ EC_INVALID_PARAMETER,
    /*  9 */ EC_INVALID_PARAMETERS,
    /* 10 */ EC_INVALID_INSTANCE,
    /* 11 */ EC_INVALID_SYNTAX,

    /* 12 */ EC_LOW_BUFFERS,
    
    /* 13 */ EC_NO_COLUMNS_DIC,
    
    /* 14 */ EC_NO_SUCH_CATALOG,
    /* 15 */ EC_NO_SUCH_DIRECTORY,
    /* 16 */ EC_NO_SUCH_FOLDER,
    /* 17 */ EC_NO_SUCH_SCHEMA,
    /* 18 */ EC_NO_SUCH_TABLE,
    /* 19 */ EC_NO_SUCH_TENANT,
    /* 20 */ EC_NO_SUCH_WORD,
    
    /* 21 */ EC_NOT_A_CATALOG,
    /* 22 */ EC_NOT_A_FOLDER,
    /* 23 */ EC_NOT_A_SCHEMA,
    /* 24 */ EC_NOT_A_TABLE,
    /* 25 */ EC_NOT_A_TENANT,
    
    /* 26 */ EC_NOT_IN_CATALOGS,
    /* 27 */ EC_NOT_IN_COLUMNS,
    /* 28 */ EC_NOT_IN_SCHEMAS,
    /* 29 */ EC_NOT_IN_TABLES,
    /* 30 */ EC_NOT_IN_TENANTS,

    /* 31 */ EC_NULL_INTERFACE,
    /* 32 */ EC_NULLPTR,
    
    /* 33 */ EC_RESPONSE_PENDING,
    /* 34 */ EC_SCHEMA_EXISTS,
    /* 35 */ EC_SEND_FAILURE,
    /* 36 */ EC_SE_NOT_RUNNING,

    /* 37 */ EC_TABLE_EXISTS,
    /* 38 */ EC_TABLE_IN_USE,
    /* 39 */ EC_TENANT_EXISTS,
    
    /* 40 */ EC_VERSION_MISMATCH,
    /* 41 */ EC_ENUMS
  };
  
  static const char* const pEC[]
  {
    /*  0 */ "none",
    /*  1 */ "assert",
    /*    */ "begin transaction failed",
    /*  2 */ "catalog exists",
    /*  3 */ "folder in use",
    
    /*  4 */ "invalid class",
    /*  5 */ "invalid command",
    /*  6 */ "invalid file",
    /*  7 */ "invalid interface",
    /*  8 */ "invalid parameter",
    /*  9 */ "invalid parameters",
    /* 10 */ "invalid instance",
    /* 11 */ "invalid syntax",
    
    /* 12 */ "low buffers",
    
    /* 13 */ "no columns dictionary",
    
    /* 14 */ "no such catalog",
    /* 15 */ "no such directory",
    /* 16 */ "no such folder",
    /* 17 */ "no such schema",
    /* 18 */ "no such table",
    /* 19 */ "no such tenant",
    /* 20 */ "no such word",
    
    /* 21 */ "not a catalog",
    /* 22 */ "not a folder",
    /* 23 */ "not a schema",
    /* 24 */ "not a table",
    /* 25 */ "not a tenant",
    
    /* 26 */ "not in catalogs",
    /* 27 */ "not in columns",
    /* 28 */ "not in schemas",
    /* 29 */ "not in tables",
    /* 30 */ "not in tenants",
    
    /* 31 */ "null interface",
    /* 32 */ "nullptr",
    
    /* 33 */ "response pending",
  
    /* 34 */ "schema exists",
    /* 35 */ "SE is not running",
    /* 36 */ "send failure",
    /* 37 */ "table exists",
    /* 38 */ "table in use",
    /* 39 */ "tenant exists",

    /* 40 */ "version mismatch",

    /* 41 */ "enums"

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
