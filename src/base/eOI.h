/*============================================================================*/
/* eOI.h -- Operator Infoset                                                  */
/*============================================================================*/
#pragma once

enum eOI // operator Infoset
{
  /*  0 */ OI_NONE=0,

  /*    */ OI_COMMAND,
  /*    */ OI_COMPILEERRORS,
  /*    */ OI_COUNT,
  /*    */ OI_EXECERRORS,
  /*    */ OI_LOADRESULT,
  /*    */ OI_LOCKSTATS,
  /*    */ OI_NODEINFO,
  /*    */ OI_PAGEINREQUEST,
  /*    */ OI_PATH,
  /*    */ OI_PROGRESSUNITS,
  /*    */ OI_PROMPTOUT,
  /*    */ OI_RETURNPIPELINE,
  /*    */ OI_SEEKOFFSETS,
  /*    */ OI_TOTALUNITS,
 
  /*    */ OI_ENUMS,
};

extern Stz* aOI[OI_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
