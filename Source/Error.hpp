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
  /*  5 */ EC_CLUSTER_PARTITIONS_MISMATCH,
  /*  6 */ EC_CLUSTER_SIZE_MISMATCH,
  
  /*  7 */ EC_DUPLICATE_COLUMN,
  /*  8 */ EC_DUPLICATE_KEY,
  /*  9 */ EC_FOLDER_IN_USE,
  /* 10 */ EC_FOLDER_NOT_EMPTY,
  
  /* 11 */ EC_INDEX_EXISTS,
    
  /* 12 */ EC_INVALID_CLASS,
  /* 13 */ EC_INVALID_COLUMN,
  /* 14 */ EC_INVALID_COLUMNS_UNIQUE_INDEX,
  /* 15 */ EC_INVALID_COMMAND,
  /* 16 */ EC_INVALID_FILE,
  /* 17 */ EC_INVALID_INTERFACE,
  /* 14 */ EC_INVALID_PARAMETER,
  /* 15 */ EC_INVALID_PARAMETERS,
  /* 16 */ EC_INVALID_INSTANCE,
  /* 17 */ EC_INVALID_SYNTAX,
  
  /* 18 */ EC_LOW_BUFFERS,
  
  /* 19 */ EC_MAX_INDEXES,
    
  /* 20 */ EC_NO_COLUMNS_DIC,
  
  /* 21 */ EC_NO_SUCH_CATALOG,
  /* 22 */ EC_NO_SUCH_DIRECTORY,
  /* 23 */ EC_NO_SUCH_FOLDER,
  /* 24 */ EC_NO_SUCH_INDEX,
  /* 25 */ EC_NO_SUCH_SCHEMA,
  /* 26 */ EC_NO_SUCH_TABLE,
  /* 27 */ EC_NO_SUCH_TENANT,
  /* 28 */ EC_NO_SUCH_WORD,
  
  /* 29 */ EC_NOT_A_CATALOG,
  /* 30 */ EC_NOT_A_FOLDER,
  /* 31 */ EC_NOT_AN_INDEX,
  /* 32 */ EC_NOT_A_SCHEMA,
  /* 33 */ EC_NOT_A_TABLE,
  /* 34 */ EC_NOT_A_TENANT,
  
  /* 35 */ EC_NOT_IN_CATALOGS,
  /* 36 */ EC_NOT_IN_COLUMNS,
  /* 37 */ EC_NOT_IN_INDEXES,
  /* 38 */ EC_NOT_IN_PARTITIONS,
  /* 39 */ EC_NOT_IN_SCHEMAS,
  /* 40 */ EC_NOT_IN_TABLE,
  /* 41 */ EC_NOT_IN_TABLES,
  /* 42 */ EC_NOT_IN_TENANTS,
  
  /* 43 */ EC_NULL_INTERFACE,
  /* 44 */ EC_NULLPTR,
  
  /* 45 */ EC_RESPONSE_PENDING,
  /* 46 */ EC_SCHEMA_EXISTS,
  /* 47 */ EC_SE_NOT_RUNNING,
  /* 48 */ EC_SE_UNKNOWN_ERROR,
  /* 49 */ EC_SEND_FAILURE,
  
  /* 50 */ EC_TABLE_EXISTS,
  /* 51 */ EC_TABLE_NOT_EMPTY,
  /* 52 */ EC_TABLE_IN_USE,
  /* 53 */ EC_TENANT_EXISTS,
  
  /* 54 */ EC_VALIDATION_FAILURE,
  /* 55 */ EC_VERSION_MISMATCH,
  /* 56 */ EC_ENUMS
  };
  
  static Stz* aEC[]
  {
  /*  0 */ "none"_Stz,
  /*  1 */ "assert"_Stz,
  /*  2 */ "begin transaction failed"_Stz,
  /*  3 */ "catalog exists"_Stz,
  /*  4 */ "cluster not started"_Stz,
  /*  5 */ "cluster partitions per node mismatch"_Stz,
  /*  6 */ "cluster node size mismatch"_Stz,
  
  /*  7 */ "duplicate column"_Stz,
  /*  8 */ "duplicate key"_Stz,
  /*  9 */ "folder in use"_Stz,
  /* 10 */ "folder not empty"_Stz,
  
  /* 11 */ "index exists"_Stz,
    
  /* 12 */ "invalid class"_Stz,
  /* 13 */ "invalid column"_Stz,
  /* 14 */ "invalid columns for unique index (include all partitioning columns)"_Stz,
  /* 15 */ "invalid command"_Stz,
  /* 16 */ "invalid file"_Stz,
  /* 13 */ "invalid interface"_Stz,
  /* 14 */ "invalid parameter"_Stz,
  /* 15 */ "invalid parameters"_Stz,
  /* 16 */ "invalid instance"_Stz,
  /* 17 */ "invalid syntax"_Stz,
  
  /* 18 */ "low buffers"_Stz,
  /* 19 */ "max allowed indexes already created"_Stz,
    
  /* 20 */ "no columns dictionary"_Stz,
  
  /* 21 */ "no such catalog"_Stz,
  /* 22 */ "no such directory"_Stz,
  /* 23 */ "no such folder"_Stz,
  /* 24 */ "no such index"_Stz,
  /* 25 */ "no such schema"_Stz,
  /* 26 */ "no such table"_Stz,
  /* 27 */ "no such tenant"_Stz,
  /* 28 */ "no such word"_Stz,
  
  /* 29 */ "not a catalog"_Stz,
  /* 30 */ "not a folder"_Stz,
  /* 31 */ "not an index"_Stz,
  /* 32 */ "not a schema"_Stz,
  /* 33 */ "not a table"_Stz,
  /* 34 */ "not a tenant"_Stz,
    
  /* 35 */ "not in catalogs"_Stz,
  /* 36 */ "not in columns"_Stz,
  /* 37 */ "not in indexes"_Stz,
  /* 38 */ "not in partitions"_Stz,
  /* 39 */ "not in schemas"_Stz,
  /* 40 */ "not in table"_Stz,
  /* 41 */ "not in tables"_Stz,
  /* 42 */ "not in tenants"_Stz,
  
  /* 43 */ "null interface"_Stz,
  /* 44 */ "nullptr"_Stz,

  /* 45 */ "response pending"_Stz,
  
  /* 46 */ "schema exists"_Stz,
  /* 47 */ "SE is not running"_Stz,
  /* 48 */ "SE unknown error"_Stz,
  /* 49 */ "send failure"_Stz,
  /* 50 */ "table exists"_Stz,
  /* 51 */ "table not empty"_Stz,
  /* 52 */ "table in use"_Stz,
  /* 53 */ "tenant exists"_Stz,
  /* 54 */ "validation failure"_Stz,
  /* 55 */ "version mismatch"_Stz,

  /* 56 */ "enums"_Stz
  
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
