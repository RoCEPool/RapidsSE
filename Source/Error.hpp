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
  
  /*  2 */ EC_BLANKLINE,

  /*    */ EC_CATALOG_EXISTS,
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
  /* 18 */ EC_INVALID_PARAMETER,
  /* 19 */ EC_INVALID_PARAMETERS,
  /*    */ EC_INVALID_PARTITION,
  /*    */ EC_INVALID_INDEX_COLUMN,
  /* 20 */ EC_INVALID_INSTANCE,
  /*    */ EC_INVALID_SIZE,
  /* 21 */ EC_INVALID_SYNTAX,
  /* 22 */ EC_INVALID_VALUE,
  
  /* 23 */ EC_LOW_BUFFERS,
  
  /* 24 */ EC_MAX_INDEXES,
  /*    */ EC_MAX_COLUMNS,
  /*    */ EC_MAX_ROW_LENGTH,

  /* 25 */ EC_NO_COLUMNS_DIC,
  /* 26 */ EC_NO_SUCH_CATALOG,
  /* 27 */ EC_NO_SUCH_DIRECTORY,
  /* 28 */ EC_NO_SUCH_FOLDER,
  /* 29 */ EC_NO_SUCH_INDEX,
  /* 30 */ EC_NO_SUCH_SCHEMA,
  /* 31 */ EC_NO_SUCH_TABLE,
  /* 32 */ EC_NO_SUCH_TENANT,
  /* 33 */ EC_NO_SUCH_WORD,
  /* 34 */ EC_NOT_A_CATALOG,
  /* 35 */ EC_NOT_A_FOLDER,
  /* 36 */ EC_NOT_AN_INDEX,
  /* 37 */ EC_NOT_A_SCHEMA,
  /* 38 */ EC_NOT_A_TABLE,
  /* 39 */ EC_NOT_A_TENANT,
  /* 40 */ EC_NOT_IN_CATALOGS,
  /* 41 */ EC_NOT_IN_COLUMNS,
  /* 42 */ EC_NOT_IN_INDEXES,
  /* 43 */ EC_NOT_IN_PARTITIONS,
  /* 44 */ EC_NOT_IN_SCHEMAS,
  /* 45 */ EC_NOT_IN_TABLE,
  /* 46 */ EC_NOT_IN_TABLES,
  /* 47 */ EC_NOT_IN_TENANTS,
  /* 48 */ EC_NULL_INTERFACE,
  /* 49 */ EC_NULLPTR,
  
  /* 50 */ EC_RESPONSE_PENDING,
  
  /* 51 */ EC_SCHEMA_EXISTS,
  /* 52 */ EC_SE_NOT_RUNNING,
  /* 53 */ EC_SE_UNKNOWN_ERROR,
  /* 54 */ EC_SEND_FAILURE,
  
  /*    */ EC_TABLE_ALREADY_LOADED,
  /* 55 */ EC_TABLE_EXISTS,
  /* 56 */ EC_TABLE_NOT_EMPTY,
  /* 57 */ EC_TABLE_IN_USE,
  /* 58 */ EC_TENANT_EXISTS,
  /*    */ EC_TX_BEGIN_FAILED,
  /*    */ EC_TX_COMMIT_FAILED,
  /*    */ EC_TX_END_FAILED,
  /*    */ EC_TX_ROLLBACK_FAILED,
    
  /* 59 */ EC_VALIDATION_FAILURE,
  /* 60 */ EC_VERSION_MISMATCH,
  
  /* 61 */ EC_ENUMS
  };
  
  static Stz* aEC[]
  {
  /*  0 */ "none"_Stz,
  
  /*  1 */ "assert"_Stz,
  /*  2 */ "blank line"_Stz,
  
  /*    */ "catalog exists"_Stz,
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
  /* 17 */ "invalid interface"_Stz,
  /* 18 */ "invalid parameter"_Stz,
  /* 19 */ "invalid parameters"_Stz,
  /*    */ "invalid partition"_Stz,
  /*    */ "invalid index column"_Stz,
  /* 20 */ "invalid instance"_Stz,
  /*    */ "invalid size"_Stz,
  /* 21 */ "invalid syntax"_Stz,
  /* 22 */ "invalid value"_Stz,
  
  /* 23 */ "low buffers"_Stz,
  
  /* 24 */ "max allowed indexes already created"_Stz,
  /*    */ "max 150 columns supported in a table"_Stz,
  /*    */ "max row length allowed is 16k bytes"_Stz,

  /* 25 */ "no columns dictionary"_Stz,
  /* 26 */ "no such catalog"_Stz,
  /* 27 */ "no such directory"_Stz,
  /* 28 */ "no such folder"_Stz,
  /* 29 */ "no such index"_Stz,
  /* 30 */ "no such schema"_Stz,
  /* 31 */ "no such table"_Stz,
  /* 32 */ "no such tenant"_Stz,
  /* 33 */ "no such word"_Stz,
  /* 34 */ "not a catalog"_Stz,
  /* 35 */ "not a folder"_Stz,
  /* 36 */ "not an index"_Stz,
  /* 37 */ "not a schema"_Stz,
  /* 38 */ "not a table"_Stz,
  /* 39 */ "not a tenant"_Stz,
  /* 40 */ "not in catalogs"_Stz,
  /* 41 */ "not in columns"_Stz,
  /* 42 */ "not in indexes"_Stz,
  /* 43 */ "not in partitions"_Stz,
  /* 44 */ "not in schemas"_Stz,
  /* 45 */ "not in table"_Stz,
  /* 46 */ "not in tables"_Stz,
  /* 47 */ "not in tenants"_Stz,
  /* 48 */ "null interface"_Stz,
  /* 49 */ "nullptr"_Stz,

  /* 50 */ "response pending"_Stz,
  
  /* 51 */ "schema exists"_Stz,
  /* 52 */ "SE is not running"_Stz,
  /* 53 */ "SE unknown error"_Stz,
  /* 54 */ "send failure"_Stz,
  
  /*    */ "table already loaded"_Stz,
  /* 55 */ "table exists"_Stz,
  /* 56 */ "table not empty"_Stz,
  /* 57 */ "table in use"_Stz,
  /* 58 */ "tenant exists"_Stz,
    
  /*    */ "begin transaction failed"_Stz,
  /*    */ "commit transaction failed"_Stz,
  /*    */ "end transaction failed"_Stz,
  /*    */ "rollback transaction failed"_Stz,
    
  /* 59 */ "validation failure"_Stz,
  /* 60 */ "version mismatch"_Stz,

  /* 61 */ "enums"_Stz
  
  };
  
  class Error
  {
  private:
    eEC   m_eErrorCode;
    char  m_caFileName[256];
    i8    m_iLineNo;
    char  m_caErrorMsg[256];
  
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
