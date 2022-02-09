/*============================================================================*/
/* eST.h -- Stage Topology                                                    */
/*============================================================================*/
#pragma once

enum eST // stage topology
{
  /*  0 */ ST_NONE=0,
  
  /*  1 */ ST_ALL_PARTITIONS,
  /*  2 */ ST_ALL_NODES,
  /*  3 */ ST_ON_ORIGIN,
  /*  4 */ ST_SAME_NODE,
  /*  5 */ ST_SAME_PARTITION,
  /*  6 */ ST_SPECIFIC_NODE,

  /*  7 */ ST_ENUMS
};

static Stz* aST[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  1 */ "ST_ALL_PARTITIONS"_Stz,
  /*  2 */ "ST_ALL_NODES"_Stz,
  /*  3 */ "ST_ON_ORIGIN"_Stz,
  /*  4 */ "ST_SAME_NODE"_Stz,
  /*  5 */ "ST_SAME_PARTITION"_Stz,
  /*  6 */ "ST_SPECIFIC_NODE"_Stz,
  
  /*  7 */ "ST_ENUMS"_Stz,
};

static Stz* aST_weave[]
{
  /*  0 */ "ST_NONE"_Stz,
  
  /*  1 */ "all partitions"_Stz,
  /*  2 */ "all nodes"_Stz,
  /*  3 */ "origin"_Stz,
  /*  4 */ "same node"_Stz,
  /*  5 */ "same partition"_Stz,
  /*  6 */ "specific node"_Stz,
  
  /*  7 */ "ST_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
