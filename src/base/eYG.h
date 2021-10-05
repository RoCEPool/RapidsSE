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
  /*    */ YG_ORIGIN,
  /*    */ YG_SPECIFIC_NODE,
  /*    */ YG_SPECIFIC_PARTITION,

  /*    */ YG_ENUMS,
};

static Stz* aYG[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "YG_ALL_NODES"_Stz,
  /*    */ "YG_ALL_PARTITIONS"_Stz,
  /*    */ "YG_ALL_PARTITIONS_ON_ORIGIN"_Stz,
  /*    */ "YG_ORIGIN"_Stz,
  /*    */ "YG_SPECIFIC_NODE"_Stz,
  /*    */ "YG_SPECIFIC_PARTITION"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
