/*============================================================================*/
/* eOI.h -- Operator Infoset                                                  */
/*============================================================================*/
#pragma once

enum eOI // operator Infoset
{
  /*  0 */ OI_NONE=0,
  
  /*    */ OI_CALLPIPELINE,
  /*    */ OI_COUNTINFO,
  /*    */ OI_LOCKSTATS,
  /*    */ OI_NODEINFO,
  /*    */ OI_PATHINFO,
  /*    */ OI_PROGRESSUNITS,
  /*    */ OI_RETURNPIPELINE,
  /*    */ OI_SEEKOFFSETS,
  /*    */ OI_TOTALUNITS,
 
  /*    */ OI_ENUMS,
};

static Stz* aOI[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "CALLPIPELINE"_Stz,
  /*    */ "COUNTINFO"_Stz,
  /*    */ "LOCKSTATS"_Stz,
  /*    */ "NODEINFO"_Stz,
  /*    */ "PATHINFO"_Stz,
  /*    */ "PROGRESSUNITS"_Stz,
  /*    */ "RETURNPIPELINE"_Stz,
  /*    */ "SEEKOFFSETS"_Stz,
  /*    */ "TOTALUNITS"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
