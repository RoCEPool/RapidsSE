/*============================================================================*/
/* eMA.cpp -- Management Actions                                              */
/*============================================================================*/
#include "BASE.h"
#include "eMA.h"

Stz* aMA[MA_MNG_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "MA_MGT_ANALYZE_INDEX"_Stz,
  /*    */ "MA_MGT_ANALYZE_TABLE"_Stz,
  /*    */ "MA_MGT_COMPACT_INDEX"_Stz,
  /*    */ "MA_MGT_CONNECT_CLUSTER"_Stz,
  /*    */ "MA_MGT_EXPORT"_Stz,
  /*    */ "MA_MGT_INSERT_ROW"_Stz,
  /*    */ "MA_MGT_INSERT_TABLE"_Stz,
  /*    */ "MA_MGT_LOAD_TABLE"_Stz,
  /*    */ "MA_MGT_LIST_PARTITIONS"_Stz,
  /*    */ "MA_MGT_PUBLISH"_Stz,
  /*    */ "MA_MGT_PURGE"_Stz,
  /*    */ "MA_MGT_SANITY"_Stz,
  /*    */ "MA_MGT_SIZE_INDEX"_Stz,
  /*    */ "MA_MGT_SIZE_TABLE"_Stz,
  /*    */ "MA_MGT_SPACE"_Stz,
  /*    */ "MA_MGT_START_CLUSTER"_Stz,
  /*    */ "MA_MGT_STATUS"_Stz,
  /*    */ "MA_MGT_TABLE_SIZES"_Stz,
  /*    */ "MA_MGT_TRUNCATE_TABLE"_Stz,
  /*    */ "MA_MGT_VERIFY_INDEX"_Stz,
  /*    */ "MA_MGT_VERIFY_TABLE"_Stz,
  /*    */ "MA_MGT_WAIT_CLUSTER"_Stz,
  /*    */ "MA_MGT_WALK"_Stz,

//  /*    */ "ENUMS"_Stz,
};

Stz* aMA_short[MA_MNG_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "analyze index"_Stz,
  /*    */ "analyze table"_Stz,
  /*    */ "compact index"_Stz,
  /*    */ "connect cluster"_Stz,
  /*    */ "export"_Stz,
  /*    */ "insert row"_Stz,
  /*    */ "insert table"_Stz,
  /*    */ "load table"_Stz,
  /*    */ "list partitions"_Stz,
  /*    */ "publish"_Stz,
  /*    */ "purge"_Stz,
  /*    */ "sanity"_Stz,
  /*    */ "size index"_Stz,
  /*    */ "size table"_Stz,
  /*    */ "space"_Stz,
  /*    */ "start cluster"_Stz,
  /*    */ "status"_Stz,
  /*    */ "table sizes"_Stz,
  /*    */ "truncate table"_Stz,
  /*    */ "verify index"_Stz,
  /*    */ "verify table"_Stz,
  /*    */ "wait cluster"_Stz,
  /*    */ "walk"_Stz,

//  /*    */ "ENUMS"_Stz,
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
