/*============================================================================*/
/* ePX.h -- Cross Topology                                                    */
/*============================================================================*/
#pragma once

enum ePX // toPXlogy cross
{
  /*  0 */ PX_NONE=0,
  
  /*    */ PX_ALL_NODES_TO_ALL_NODES,
  /*    */ PX_ALL_NODES_TO_ALL_PARTITIONS,
  /*    */ PX_ALL_NODES_TO_ORIGIN,
  /*    */ PX_ALL_NODES_TO_SPECIFIC_NODE,
  /*    */ PX_ALL_NODES_TO_SPECIFIC_PARTITION,
  
  /*    */ PX_ALL_PARTITIONS_TO_ALL_NODES,
  /*    */ PX_ALL_PARTITIONS_TO_ALL_PARTITIONS,
  /*    */ PX_ALL_PARTITIONS_TO_ORIGIN,
  /*    */ PX_ALL_PARTITIONS_TO_SPECIFIC_NODE,
  /*    */ PX_ALL_PARTITIONS_TO_SPECIFIC_PARTITION,
  
  /*    */ PX_ORIGIN_TO_ALL_NODES,
  /*    */ PX_ORIGIN_TO_ALL_PARTITIONS,
  /*    */ PX_ORIGIN_TO_ORIGIN,
  /*    */ PX_ORIGIN_TO_SPECIFIC_NODE,
  /*    */ PX_ORIGIN_TO_SPECIFIC_PARTITION,
  
  /*    */ PX_SPECIFIC_NODE_TO_ALL_NODES,
  /*    */ PX_SPECIFIC_NODE_TO_ALL_PARTITIONS,
  /*    */ PX_SPECIFIC_NODE_TO_ORIGIN,
  /*    */ PX_SPECIFIC_NODE_TO_SPECIFIC_NODE,
  /*    */ PX_SPECIFIC_NODE_TO_SPECIFIC_PARTITION,
  
  /*    */ PX_SPECIFIC_PARTITION_TO_ALL_NODES,
  /*    */ PX_SPECIFIC_PARTITION_TO_ALL_PARTITIONS,
  /*    */ PX_SPECIFIC_PARTITION_TO_ORIGIN,
  /*    */ PX_SPECIFIC_PARTITION_SPECIFIC_NODE,
  /*    */ PX_SPECIFIC_PARTITION_TO_SPECIFIC_PARTITION,
  
  /*    */ PX_ENUMS,
};

static Stz* aPX[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "PX_ALL_NODES_TO_ALL_NODES"_Stz,
  /*    */ "PX_ALL_NODES_TO_ALL_PARTITIONS"_Stz,
  /*    */ "PX_ALL_NODES_TO_ORIGIN"_Stz,
  /*    */ "PX_ALL_NODES_TO_SPECIFIC_NODE"_Stz,
  /*    */ "PX_ALL_NODES_TO_SPECIFIC_PARTITION"_Stz,
  
  /*    */ "PX_ALL_PARTITIONS_TO_ALL_NODES"_Stz,
  /*    */ "PX_ALL_PARTITIONS_TO_ALL_PARTITIONS"_Stz,
  /*    */ "PX_ALL_PARTITIONS_TO_ORIGIN"_Stz,
  /*    */ "PX_ALL_PARTITIONS_TO_SPECIFIC_NODE"_Stz,
  /*    */ "PX_ALL_PARTITIONS_TO_SPECIFIC_PARTITION"_Stz,
  
  /*    */ "PX_ORIGIN_TO_ALL_NODES"_Stz,
  /*    */ "PX_ORIGIN_TO_ALL_PARTITIONS"_Stz,
  /*    */ "PX_ORIGIN_TO_ORIGIN"_Stz,
  /*    */ "PX_ORIGIN_TO_SPECIFIC_NODE"_Stz,
  /*    */ "PX_ORIGIN_TO_SPECIFIC_PARTITION"_Stz,
  
  /*    */ "PX_SPECIFIC_NODE_TO_ALL_NODES"_Stz,
  /*    */ "PX_SPECIFIC_NODE_TO_ALL_PARTITIONS"_Stz,
  /*    */ "PX_SPECIFIC_NODE_TO_ORIGIN"_Stz,
  /*    */ "PX_SPECIFIC_NODE_TO_SPECIFIC_NODE"_Stz,
  /*    */ "PX_SPECIFIC_NODE_TO_SPECIFIC_PARTITION"_Stz,
  
  /*    */ "PX_SPECIFIC_PARTITION_TO_ALL_NODES"_Stz,
  /*    */ "PX_SPECIFIC_PARTITION_TO_ALL_PARTITIONS"_Stz,
  /*    */ "PX_SPECIFIC_PARTITION_TO_ORIGIN"_Stz,
  /*    */ "PX_SPECIFIC_PARTITION_SPECIFIC_NODE"_Stz,
  /*    */ "PX_SPECIFIC_PARTITION_TO_SPECIFIC_PARTITION"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
