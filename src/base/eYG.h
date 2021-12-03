/*============================================================================*/
/* eYG.h -- Stage Topology                                                    */
/*============================================================================*/
#pragma once

enum eYG // stage topology
{
  /*  0 */ YG_NONE=0,
  
  /*    */ YG_ALL_NODES,
  /*    */ YG_ALL_PARTITIONS,
  /*    */ YG_ALL_PARTITIONS_ON_ORIGIN,
  /*    */ YG_NEXT_NODE,
  /*    */ YG_ORIGIN,

  /*    */ YG_ENUMS,
};

static Stz* aYG[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "YG_ALL_NODES"_Stz,
  /*    */ "YG_ALL_PARTITIONS"_Stz,
  /*    */ "YG_ALL_PARTITIONS_ON_ORIGIN"_Stz,
  /*    */ "YG_NEXT_NODE"_Stz,
  /*    */ "YG_ORIGIN"_Stz,

  /*    */ "ENUMS"_Stz,
};

static Stz* aYG_15s[]
{
  /*  0 */ "NONE"_Stz,
  //        0        1
  //        123456789012345
  /*    */ "ALL_NODES"_Stz,
  /*    */ "ALL_PARTITIONS"_Stz,
  /*    */ "ALL_PTN_ORIGIN"_Stz,
  /*    */ "NEXT_NODE"_Stz,
  /*    */ "ORIGIN"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
