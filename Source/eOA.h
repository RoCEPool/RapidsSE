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
  /*    */ OA_OP_CONSTRUCTOR,
  /*    */ OA_OP_DESTRUCTOR,
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
  /*    */ "OA_OP_CONSTRUCTOR"_Stz,
  /*    */ "OA_OP_DESTRUCTOR"_Stz,
  /*    */ "OA_OP_DRAIN"_Stz,
  /*    */ "OA_OP_DRAINED"_Stz,
  /*    */ "OA_OP_INF"_Stz,
  /*    */ "OA_OP_POP"_Stz,
  /*    */ "OA_OP_RETURNED"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
