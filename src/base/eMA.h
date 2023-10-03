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

extern Stz* aMA[MA_MNG_ENUMS];
extern Stz* aMA_short[MA_MNG_ENUMS];


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
