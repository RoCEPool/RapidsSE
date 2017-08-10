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
  /* 10 */ EC_INVALID_COLUMN,
  /* 11 */ EC_INVALID_COMMAND,
  /* 12 */ EC_INVALID_FILE,
  /* 13 */ EC_INVALID_INTERFACE,
  /* 14 */ EC_INVALID_PARAMETER,
  /* 15 */ EC_INVALID_PARAMETERS,
  /* 16 */ EC_INVALID_INSTANCE,
  /* 17 */ EC_INVALID_SYNTAX,
  
  /* 18 */ EC_LOW_BUFFERS,
  
  /* 19 */ EC_NO_COLUMNS_DIC,
  
  /* 20 */ EC_NO_SUCH_CATALOG,
  /* 21 */ EC_NO_SUCH_DIRECTORY,
  /* 22 */ EC_NO_SUCH_FOLDER,
  /* 23 */ EC_NO_SUCH_INDEX,
  /* 24 */ EC_NO_SUCH_SCHEMA,
  /* 25 */ EC_NO_SUCH_TABLE,
  /* 26 */ EC_NO_SUCH_TENANT,
  /* 27 */ EC_NO_SUCH_WORD,
  
  /* 28 */ EC_NOT_A_CATALOG,
  /* 29 */ EC_NOT_A_FOLDER,
  /* 30 */ EC_NOT_AN_INDEX,
  /* 31 */ EC_NOT_A_SCHEMA,
  /* 33 */ EC_NOT_A_TABLE,
  /* 33 */ EC_NOT_A_TENANT,
    
  /* 34 */ EC_NOT_IN_CATALOGS,
  /* 35 */ EC_NOT_IN_COLUMNS,
  /* 36 */ EC_NOT_IN_INDEXES,
  /* 37 */ EC_NOT_IN_PARTITIONS,
  /* 38 */ EC_NOT_IN_SCHEMAS,
  /* 39 */ EC_NOT_IN_TABLE,
  /* 40 */ EC_NOT_IN_TABLES,
  /* 41 */ EC_NOT_IN_TENANTS,
  
  /* 42 */ EC_NULL_INTERFACE,
  /* 43 */ EC_NULLPTR,
  
  /* 44 */ EC_RESPONSE_PENDING,
  /* 45 */ EC_SCHEMA_EXISTS,
  /* 46 */ EC_SE_NOT_RUNNING,
  /* 47 */ EC_SE_UNKNOWN_ERROR,
  /* 48 */ EC_SEND_FAILURE,
  
  /* 49 */ EC_TABLE_EXISTS,
  /* 50 */ EC_TABLE_NOT_EMPTY,
  /* 51 */ EC_TABLE_IN_USE,
  /* 52 */ EC_TENANT_EXISTS,
  
  /* 53 */ EC_VALIDATION_FAILURE,
  /* 54 */ EC_VERSION_MISMATCH,
  /* 55 */ EC_ENUMS
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
  /* 10 */ "invalid column"_Stz,
  /* 11 */ "invalid command"_Stz,
  /* 12 */ "invalid file"_Stz,
  /* 13 */ "invalid interface"_Stz,
  /* 14 */ "invalid parameter"_Stz,
  /* 15 */ "invalid parameters"_Stz,
  /* 16 */ "invalid instance"_Stz,
  /* 17 */ "invalid syntax"_Stz,
  
  /* 18 */ "low buffers"_Stz,
  
  /* 19 */ "no columns dictionary"_Stz,
  
  /* 20 */ "no such catalog"_Stz,
  /* 21 */ "no such directory"_Stz,
  /* 22 */ "no such folder"_Stz,
  /* 23 */ "no such index"_Stz,
  /* 24 */ "no such schema"_Stz,
  /* 25 */ "no such table"_Stz,
  /* 26 */ "no such tenant"_Stz,
  /* 27 */ "no such word"_Stz,
  
  /* 28 */ "not a catalog"_Stz,
  /* 29 */ "not a folder"_Stz,
  /* 30 */ "not an index"_Stz,
  /* 31 */ "not a schema"_Stz,
  /* 32 */ "not a table"_Stz,
  /* 33 */ "not a tenant"_Stz,
    
  /* 34 */ "not in catalogs"_Stz,
  /* 35 */ "not in columns"_Stz,
  /* 36 */ "not in indexes"_Stz,
  /* 37 */ "not in partitions"_Stz,
  /* 38 */ "not in schemas"_Stz,
  /* 39 */ "not in table"_Stz,
  /* 40 */ "not in tables"_Stz,
  /* 41 */ "not in tenants"_Stz,
  
  /* 42 */ "null interface"_Stz,
  /* 43 */ "nullptr"_Stz,
  
  /* 44 */ "response pending"_Stz,
  
  /* 45 */ "schema exists"_Stz,
  /* 46 */ "SE is not running"_Stz,
  /* 47 */ "SE unknown error"_Stz,
  /* 48 */ "send failure"_Stz,
  /* 49 */ "table exists"_Stz,
  /* 50 */ "table not empty"_Stz,
  /* 51 */ "table in use"_Stz,
  /* 52 */ "tenant exists"_Stz,

  /* 53 */ "validation failure"_Stz,
  /* 54 */ "version mismatch"_Stz,

  /* 55 */ "enums"_Stz
  
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
