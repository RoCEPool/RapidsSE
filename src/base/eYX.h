/*============================================================================*/
/* eYX.h -- Cross Topology                                                    */
/*============================================================================*/
#pragma once

enum eYX // cross topology
{
  /*  0 */ YX_NONE=0,
  
  /*  1 */ YX_ALL_NODES_TO_ALL_NODES,
  /*  2 */ YX_ALL_NODES_TO_ALL_PARTITIONS,
  /*  3 */ YX_ALL_NODES_TO_ORIGIN,
  /*  4 */ YX_ALL_NODES_TO_SPECIFIC_NODE,
  
  /*  6 */ YX_ALL_PARTITIONS_TO_ALL_NODES,
  /*  7 */ YX_ALL_PARTITIONS_TO_ALL_PARTITIONS,
  /*  8 */ YX_ALL_PARTITIONS_TO_ORIGIN,
  /*  9 */ YX_ALL_PARTITIONS_TO_SPECIFIC_NODE,
  
  /*    */ YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS,
  /*    */ YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_NODES,

  /* 13 */ YX_NEXT_NODE_TO_ORIGIN,
  
  /* 11 */ YX_ORIGIN_TO_ALL_NODES,
  /* 12 */ YX_ORIGIN_TO_ALL_PARTITIONS,
  /* 12 */ YX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN,
  /* 13 */ YX_ORIGIN_TO_ORIGIN,
  /* 13 */ YX_ORIGIN_TO_NEXT_NODE,
  /*    */ YX_ORIGIN_TO_SPECIFIC_NODE,
  
  /*    */ YX_SPECIFIC_NODE_TO_ALL_NODES,
  /*    */ YX_SPECIFIC_NODE_TO_ALL_PARTITIONS,
  /*    */ YX_SPECIFIC_NODE_TO_ORIGIN,
  /*    */ YX_SPECIFIC_NODE_TO_SPECIFIC_NODE,
    
  /*    */ YX_ENUMS,
};

static Stz* aYX[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "YX_ALL_NODES_TO_ALL_NODES"_Stz,
  /*    */ "YX_ALL_NODES_TO_ALL_PARTITIONS"_Stz,
  /*    */ "YX_ALL_NODES_TO_ORIGIN"_Stz,
  /*    */ "YX_ALL_NODES_TO_SPECIFIC_NODE"_Stz,
  
  /*    */ "YX_ALL_PARTITIONS_TO_ALL_NODES"_Stz,
  /*    */ "YX_ALL_PARTITIONS_TO_ALL_PARTITIONS"_Stz,
  /*    */ "YX_ALL_PARTITIONS_TO_ORIGIN"_Stz,
  /*    */ "YX_ALL_PARTITIONS_TO_SPECIFIC_NODE"_Stz,
  
  /*    */ "YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS"_Stz,
  /*    */ "YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_NODES"_Stz,

  /*    */ "YX_NEXT_NODE_TO_ORIGIN"_Stz,

  /*    */ "YX_ORIGIN_TO_ALL_NODES"_Stz,
  /*    */ "YX_ORIGIN_TO_ALL_PARTITIONS"_Stz,
  /*    */ "YX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN"_Stz,
  /*    */ "YX_ORIGIN_TO_ORIGIN"_Stz,
  /*    */ "YX_ORIGIN_TO_NEXT_NODE"_Stz,
  /*    */ "YX_ORIGIN_TO_SPECIFIC_NODE"_Stz,
  
  /*    */ "YX_SPECIFIC_NODE_TO_ALL_NODES"_Stz,
  /*    */ "YX_SPECIFIC_NODE_TO_ALL_PARTITIONS"_Stz,
  /*    */ "YX_SPECIFIC_NODE_TO_ORIGIN"_Stz,
  /*    */ "YX_SPECIFIC_NODE_TO_SPECIFIC_NODE"_Stz,
  
  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
