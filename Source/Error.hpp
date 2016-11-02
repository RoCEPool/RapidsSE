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
    /*  2 */ EC_CATALOG_EXISTS,

    /*  3 */ EC_INVALID_CLASS,
    /*  4 */ EC_INVALID_COMMAND,
    /*  5 */ EC_INVALID_INTERFACE,
    /*  6 */ EC_INVALID_PARAMETER,
    /*  7 */ EC_INVALID_INSTANCE,
    /*  8 */ EC_INVALID_SYNTAX,

    /*  9 */ EC_LOW_BUFFERS,
    
    /* 10 */ EC_NO_COLUMNS_DIC,
    
    /* 11 */ EC_NO_SUCH_CATALOG,
    /* 12 */ EC_NO_SUCH_DIRECTORY,
    /* 13 */ EC_NO_SUCH_FOLDER,
    /* 14 */ EC_NO_SUCH_SCHEMA,
    /* 15 */ EC_NO_SUCH_TABLE,
    /* 16 */ EC_NO_SUCH_TENANT,
    /* 17 */ EC_NO_SUCH_WORD,
    
    /* 18 */ EC_NOT_A_CATALOG,
    /* 19 */ EC_NOT_A_FOLDER,
    /* 20 */ EC_NOT_A_SCHEMA,
    /* 21 */ EC_NOT_A_TABLE,
    /* 22 */ EC_NOT_A_TENANT,
    
    /* 23 */ EC_NOT_IN_CATALOGS,
    /* 24 */ EC_NOT_IN_COLUMNS,
    /* 25 */ EC_NOT_IN_SCHEMAS,
    /* 26 */ EC_NOT_IN_TABLES,
    /* 27 */ EC_NOT_IN_TENANTS,

    /* 28 */ EC_NULL_INTERFACE,
    /* 29 */ EC_NULLPTR,
    
    /* 30 */ EC_PARAMETER_COUNT_MISMATCH,
    
    /* 31 */ EC_RESPONSE_PENDING,
    /* 32 */ EC_SCHEMA_EXISTS,

    /* 33 */ EC_TABLE_EXISTS,
    /* 34 */ EC_TENANT_EXISTS,
    
    /* 35 */ EC_VERSION_MISMATCH,
    /* 36 */ EC_ENUMS
  };
  
  static const char* const pEC[]
  {
    /*  0 */ "none",
    /*  1 */ "assert",
    /*  2 */ "catalog exists",
    
    /*  3 */ "invalid class",
    /*  4 */ "invalid command",
    /*  5 */ "invalid interface",
    /*  6 */ "invalid parameter",
    /*  7 */ "invalid instance",
    /*  8 */ "invalid syntax",
    
    /*  9 */ "low buffers",
    
    /* 10 */ "no columns dictionary",
    
    /* 11 */ "no such catalog",
    /* 12 */ "no such directory",
    /* 13 */ "no such folder",
    /* 14 */ "no such schema",
    /* 15 */ "no such table",
    /* 16 */ "no such tenant",
    /* 17 */ "no such word",
    
    /* 18 */ "not a catalog",
    /* 19 */ "not a folder",
    /* 20 */ "not a schema",
    /* 21 */ "not a table",
    /* 22 */ "not a tenant",
    
    /* 23 */ "not in catalogs",
    /* 24 */ "not in columns",
    /* 25 */ "not in schemas",
    /* 26 */ "not in tables",
    /* 27 */ "not in tenants",
    
    /* 28  */ "null interface",
    /* 29 */ "nullptr",
    /* 30 */ "parameter count mismatch",
    
    /* 31 */ "response pending",
  
    /* 32 */ "schema exists",
    /* 33 */ "table exists",
    /* 34 */ "tenant exists",

    /* 35 */ "enums"

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
