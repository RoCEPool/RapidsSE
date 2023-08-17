/*============================================================================*/
/* eYX.h -- Cross Topology                                                    */
/*============================================================================*/
#include "BASE.h"
#include "eYX.h"

Stz* aYX[YX_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "YX_ALL_NODES_TO_ALL_NODES"_Stz,
  /*    */ "YX_ALL_NODES_TO_ALL_PARTITIONS"_Stz,
  /*    */ "YX_ALL_NODES_TO_ORIGIN"_Stz,
  
  /*    */ "YX_ALL_PARTITIONS_TO_ALL_NODES"_Stz,
  /*    */ "YX_ALL_PARTITIONS_TO_ALL_PARTITIONS"_Stz,
  /*    */ "YX_ALL_PARTITIONS_TO_ORIGIN"_Stz,
  
  /*    */ "YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS"_Stz,
  /*    */ "YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_NODES"_Stz,

  /*    */ "YX_NEXT_NODE_TO_ORIGIN"_Stz,

  /*    */ "YX_ORIGIN_TO_ALL_NODES"_Stz,
  /*    */ "YX_ORIGIN_TO_ALL_PARTITIONS"_Stz,
  /*    */ "YX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN"_Stz,
  /*    */ "YX_ORIGIN_TO_ORIGIN"_Stz,
  /*    */ "YX_ORIGIN_TO_NEXT_NODE"_Stz,
    
//  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
