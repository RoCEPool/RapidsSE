/*============================================================================*/
/* eMA.h -- Management Actions                                                */
/*============================================================================*/
#pragma once

enum eMA // management actions
{
  /*  0 */ MA_MNG_NONE=0,
 
  /*    */ MA_MGT_ANALYZE_INDEX,
  /*    */ MA_MGT_ANALYZE_TABLE,
  /*    */ MA_MGT_COMPACT_INDEX,
  /*    */ MA_MGT_CONNECT_CLUSTER,
  /*    */ MA_MGT_EXPORT,
  /*    */ MA_MGT_INSERT_ROW,
  /*    */ MA_MGT_INSERT_TABLE,
  /*    */ MA_MGT_LOAD_TABLE,
  /*    */ MA_MGT_LIST_PARTITIONS,
  /*    */ MA_MGT_PUBLISH,
  /*    */ MA_MGT_PURGE,
  /*    */ MA_MGT_SANITY,
  /*    */ MA_MGT_SIZE_INDEX,
  /*    */ MA_MGT_SIZE_TABLE,
  /*    */ MA_MGT_SPACE,
  /*    */ MA_MGT_START_CLUSTER,
  /*    */ MA_MGT_STATUS,
  /*    */ MA_MGT_TABLE_SIZES,
  /*    */ MA_MGT_TRUNCATE_TABLE,
  /*    */ MA_MGT_VERIFY_INDEX,
  /*    */ MA_MGT_VERIFY_TABLE,
  /*    */ MA_MGT_WAIT_CLUSTER,
  /*    */ MA_MGT_WALK,
  
  /*    */ MA_MNG_ENUMS,
};

static Stz* aMA[]
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

  /*    */ "ENUMS"_Stz,
};

static Stz* aMA_short[]
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

  /*    */ "ENUMS"_Stz,
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
