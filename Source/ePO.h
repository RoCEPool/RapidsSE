/*============================================================================*/
/* ePO.h -- Topology                                                          */
/*============================================================================*/
#pragma once

enum ePO // topology
{
  /*  0 */ PO_NONE=0,
  
  /*    */ PO_ALL_NODES,
  /*    */ PO_ALL_PARTITIONS,
  /*    */ PO_ALL_PARTITIONS_ON_ORIGIN,
  /*    */ PO_ORIGIN,
  /*    */ PO_SPECIFIC_NODE,
  /*    */ PO_SPECIFIC_PARTITION,

  /*    */ PO_ENUMS,
};

static Stz* aPO[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "PO_ALL_NODES"_Stz,
  /*    */ "PO_ALL_PARTITIONS"_Stz,
  /*    */ "PO_ALL_PARTITIONS_ON_ORIGIN"_Stz,
  /*    */ "PO_ORIGIN"_Stz,
  /*    */ "PO_SPECIFIC_NODE"_Stz,
  /*    */ "PO_SPECIFIC_PARTITION"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
