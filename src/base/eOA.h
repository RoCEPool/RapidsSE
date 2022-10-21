/*============================================================================*/
/* eOA.h -- Optimiser Actions                                                 */
/*============================================================================*/
#pragma once

enum eOA // optimiser actions
{
  /*  0 */ OA_NONE=0,

  /*    */ OA_DISABLE_CUEING,
  /*    */ OA_REPRINT_BOARD,
  /*    */ OA_CONTINUE_TRANSFORMING,
  /*    */ OA_INPUT_AGAIN,

  /*    */ OA_ENUMS,
};

static Stz* aOA[]
{
  /*  0 */ "NONE"_Stz,

  /*    */ "OA_DISABLE_CUEING"_Stz,
  /*    */ "OA_REPRINT_BOARD"_Stz,
  /*    */ "OA_CONTINUE_TRANSFORMING"_Stz,
  /*    */ "OA_INPUT_AGAIN"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
