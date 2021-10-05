/*============================================================================*/
/* eYO.h -- Op Topology                                                       */
/*============================================================================*/
#pragma once

enum eYO // op topology
{
  /*  0 */ YO_NONE=0,
  
  /*  1 */ YO_NEXT_OP,
  /*  1 */ YO_PARTITION_STREAM,
  /*  2 */ YO_STAGE_STREAM,
  /*  3 */ YO_STAGE_STREAM_0,
  /*  4 */ YO_NODE_STREAM,

  /*    */ YO_ENUMS,
};

static Stz* aYO[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "YO_NEXT_OP"_Stz,
  /*    */ "YO_PARTITION_STREAM"_Stz,
  /*    */ "YO_STAGE_STREAM"_Stz,
  /*    */ "YO_STAGE_STREAM_0"_Stz,
  /*    */ "YO_NODE_STREAM"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
