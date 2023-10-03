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
  /*    */ YG_ORIGIN_NODE,

  /*    */ YG_ENUMS,
};

extern Stz* aYG[YG_ENUMS];
extern Stz* aYG_15s[YG_ENUMS];
extern Stz* aYG_weave[YG_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
