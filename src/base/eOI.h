/*============================================================================*/
/* eOI.h -- Operator Infoset                                                  */
/*============================================================================*/
#pragma once

enum eOI // operator Infoset
{
  /*  0 */ OI_NONE=0,
  
  /*    */ OI_AFFECTEDROWS,
  /*    */ OI_CALLPIPELINE,
  /*    */ OI_COMMAND,
  /*    */ OI_COMPILEERRORS,
  /*    */ OI_COUNT,
  /*    */ OI_EXECERRORS,
  /*    */ OI_FORKCOMMAND,
  /*    */ OI_LOADRESULT,
  /*    */ OI_LOCKSTATS,
  /*    */ OI_PAGEINREQUEST,
  /*    */ OI_PATH,
  /*    */ OI_PROMPTOUT,
  /*    */ OI_WARNINGS,

  /*    */ OI_ENUMS,
};

extern Stz* aOI[OI_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
