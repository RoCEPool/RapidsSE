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
  /*    */ EC_INVALID_COLUMN,
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
  /* 29 */ EC_NOT_AN_INDEX,
  /* 30 */ EC_NOT_A_SCHEMA,
  /* 31 */ EC_NOT_A_TABLE,
  /* 32 */ EC_NOT_A_TENANT,
    
  /* 33 */ EC_NOT_IN_CATALOGS,
  /* 34 */ EC_NOT_IN_COLUMNS,
  /* 35 */ EC_NOT_IN_INDEXES,
  /*    */ EC_NOT_IN_PARTITIONS,
  /* 36 */ EC_NOT_IN_SCHEMAS,
  /* 37 */ EC_NOT_IN_TABLE,
  /* 38 */ EC_NOT_IN_TABLES,
  /* 39 */ EC_NOT_IN_TENANTS,
  
  /* 40 */ EC_NULL_INTERFACE,
  /* 41 */ EC_NULLPTR,
  
  /* 42 */ EC_RESPONSE_PENDING,
  /* 43 */ EC_SCHEMA_EXISTS,
  /* 44 */ EC_SE_NOT_RUNNING,
  /* 45 */ EC_SE_UNKNOWN_ERROR,
  /* 46 */ EC_SEND_FAILURE,
  
  /* 47 */ EC_TABLE_EXISTS,
  /*    */ EC_TABLE_NOT_EMPTY,
  /* 48 */ EC_TABLE_IN_USE,
  /* 49 */ EC_TENANT_EXISTS,
  
  /* 50 */ EC_VALIDATION_FAILURE,
  /* 51 */ EC_VERSION_MISMATCH,
  /* 52 */ EC_ENUMS
  };
  
  static Stz* aEC[]
  {
  /*  0 */ "none"_Stz,
  /*  1 */ "assert"_Stz,
  /*  2 */ "begin transaction failed"_Stz,
  /*  3 */ "catalog exists"_Stz,
  /*  4 */ "cluster not started"_Stz,
  
  /*  5 */ "duplicate column"_Stz,
  /*  6 */ "folder in use"_Stz,
  /*  7 */ "folder not empty"_Stz,
  
  /*  8 */ "index exists"_Stz,
    
  /*  9 */ "invalid class"_Stz,
  /*    */ "invalid column"_Stz,
  /* 10 */ "invalid command"_Stz,
  /* 11 */ "invalid file"_Stz,
  /* 12 */ "invalid interface"_Stz,
  /* 13 */ "invalid parameter"_Stz,
  /* 14 */ "invalid parameters"_Stz,
  /* 15 */ "invalid instance"_Stz,
  /* 16 */ "invalid syntax"_Stz,
  
  /* 17 */ "low buffers"_Stz,
  
  /* 18 */ "no columns dictionary"_Stz,
  
  /* 19 */ "no such catalog"_Stz,
  /* 20 */ "no such directory"_Stz,
  /* 21 */ "no such folder"_Stz,
  /* 22 */ "no such index"_Stz,
  /* 23 */ "no such schema"_Stz,
  /* 24 */ "no such table"_Stz,
  /* 25 */ "no such tenant"_Stz,
  /* 26 */ "no such word"_Stz,
  
  /* 27 */ "not a catalog"_Stz,
  /* 28 */ "not a folder"_Stz,
  /* 29 */ "not an index"_Stz,
  /* 30 */ "not a schema"_Stz,
  /* 31 */ "not a table"_Stz,
  /* 32 */ "not a tenant"_Stz,
    
  /* 33 */ "not in catalogs"_Stz,
  /* 34 */ "not in columns"_Stz,
  /* 35 */ "not in indexes"_Stz,
  /*    */ "not in partitions"_Stz,
  /* 36 */ "not in schemas"_Stz,
  /* 37 */ "not in table"_Stz,
  /* 38 */ "not in tables"_Stz,
  /* 39 */ "not in tenants"_Stz,
  
  /* 40 */ "null interface"_Stz,
  /* 41 */ "nullptr"_Stz,
  
  /* 42 */ "response pending"_Stz,
  
  /* 43 */ "schema exists"_Stz,
  /* 44 */ "SE is not running"_Stz,
  /* 45 */ "SE unknown error"_Stz,
  /* 46 */ "send failure"_Stz,
  /* 47 */ "table exists"_Stz,
  /*    */ "table not empty"_Stz,
  /* 48 */ "table in use"_Stz,
  /* 49 */ "tenant exists"_Stz,

  /* 50 */ "validation failure"_Stz,
  /* 51 */ "version mismatch"_Stz,

  /* 52 */ "enums"_Stz
  
  };
  
  class Error
  {
  private:
    eEC   m_eErrorCode;
    char  m_cFileName[256];
    i8    m_iLineNo;
    char  m_cErrorMsg[256];
  
  public:
    Error(eEC eErrorCode, Stz* aFileName, i8 iLineNo);
    Error(eEC eErrorCode, Stz* aErrorMsg, Stz* aFileName, i8 iLineNo);
    
    eEC  eErrorCode(void);
    Stz* aErrorMsg(void);
    Stz* aFileName(void);
    i8   iLineNo(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
