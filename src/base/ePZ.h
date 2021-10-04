/*============================================================================*/
/* ePZ.h -- Topology                                                          */
/*============================================================================*/
#pragma once

enum ePZ // topology
{
  /*  0 */ PZ_NONE=0,
  
  /*  1 */ PZ_NEXT_OP,
  /*  1 */ PZ_PARTITION_STREAM,
  /*  2 */ PZ_STAGE_STREAM,
  /*  3 */ PZ_STAGE_STREAM_0,
  /*  4 */ PZ_NODE_STREAM,

  /*    */ PZ_ENUMS,
};

static Stz* aPZ[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "PZ_NEXT_OP"_Stz,
  /*    */ "PZ_PARTITION_STREAM"_Stz,
  /*    */ "PZ_STAGE_STREAM"_Stz,
  /*    */ "PZ_STAGE_STREAM_0"_Stz,
  /*    */ "PZ_NODE_STREAM"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
