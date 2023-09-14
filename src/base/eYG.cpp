/*============================================================================*/
/* eYG.cpp -- Stage Topology                                                  */
/*============================================================================*/
#include "BASE.h"
#include "eYG.h"

Stz* aYG[YG_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "YG_ALL_NODES"_Stz,
  /*    */ "YG_ALL_PARTITIONS"_Stz,
  /*    */ "YG_ALL_PARTITIONS_ON_ORIGIN"_Stz,
  /*    */ "YG_NEXT_NODE"_Stz,
  /*    */ "YG_ORIGIN_NODE"_Stz,

//  /*    */ "ENUMS"_Stz,
};

Stz* aYG_15s[YG_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  //        0        1
  //        123456789012345
  /*    */ "ALL_NODES"_Stz,
  /*    */ "ALL_PARTITIONS"_Stz,
  /*    */ "ALL_PTN_ORIGIN"_Stz,
  /*    */ "NEXT_NODE"_Stz,
  /*    */ "ORIGIN_NODE"_Stz,

//  /*    */ "ENUMS"_Stz,
};

Stz* aYG_weave[YG_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "all nodes"_Stz,
  /*    */ "all partitions"_Stz,
  /*    */ "all partitions on origin"_Stz,
  /*    */ "next node"_Stz,
  /*    */ "origin"_Stz,

//  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
