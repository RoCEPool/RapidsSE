/*============================================================================*/
/* eOA.h -- Operator Actions                                                  */
/*============================================================================*/
#pragma once

enum eOA // operator actions
{
  /*  0 */ OA_NONE=0,
  
  /*    */ OA_OP_ACK,
  /*    */ OA_OP_APPEND,
  /*    */ OA_OP_CALL,
  /*    */ OA_OP_CONSTRUCT,
  /*    */ OA_OP_DESTRUCT,
  /*    */ OA_OP_DRAIN,
  /*    */ OA_OP_DRAINED,
  /*    */ OA_OP_INF,
  /*    */ OA_OP_POP,
  /*    */ OA_OP_RETURNED,

  /*    */ OA_ENUMS,
};

static Stz* aOA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "OA_OP_ACK"_Stz,
  /*    */ "OA_OP_APPEND"_Stz,
  /*    */ "OA_OP_CALL"_Stz,
  /*    */ "OA_OP_CONSTRUCT"_Stz,
  /*    */ "OA_OP_DESTRUCT"_Stz,
  /*    */ "OA_OP_DRAIN"_Stz,
  /*    */ "OA_OP_DRAINED"_Stz,
  /*    */ "OA_OP_INF"_Stz,
  /*    */ "OA_OP_POP"_Stz,
  /*    */ "OA_OP_RETURNED"_Stz,

  /*    */ "ENUMS"_Stz,
};

static Stz* aOA_short[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "Op ack"_Stz,
  /*    */ "Op append"_Stz,
  /*    */ "Op call"_Stz,
  /*    */ "Op construct"_Stz,
  /*    */ "Op destruct"_Stz,
  /*    */ "Op drain"_Stz,
  /*    */ "Op drained"_Stz,
  /*    */ "Op information"_Stz,
  /*    */ "Op pop"_Stz,
  /*    */ "Op returned"_Stz,

  /*    */ "ENUMS"_Stz,
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
