/*============================================================================*/
/* eOI.h -- Operator Infoset                                                  */
/*============================================================================*/
#pragma once

enum eOI // operator Infoset
{
  /*  0 */ OI_NONE=0,
  
  /*    */ OI_CALLPIPELINE,
  /*    */ OI_COMMAND,
  /*    */ OI_COMMANDERRORS,
  /*    */ OI_COUNT,
  /*    */ OI_EXECERRORS,
  /*    */ OI_FORKCOMMAND,
  /*    */ OI_LOADRESULT,
  /*    */ OI_LOCKSTATS,
  /*    */ OI_NODEINFO,
  /*    */ OI_PATH,
  /*    */ OI_PROGRESSUNITS,
  /*    */ OI_PROMPTOUT,
  /*    */ OI_RETURNPIPELINE,
  /*    */ OI_SEEKOFFSETS,
  /*    */ OI_TOTALUNITS,
 
  /*    */ OI_ENUMS,
};

static Stz* aOI[]
{
  /*  0 */ "OI_NONE"_Stz,
  
  /*    */ "OI_CALLPIPELINE"_Stz,
  /*    */ "OI_COMMAND"_Stz,
  /*    */ "OI_COMMANDERRORS"_Stz,
  /*    */ "OI_COUNT"_Stz,
  /*    */ "OI_EXECERRORS"_Stz,
  /*    */ "OI_FORKCOMMAND"_Stz,
  /*    */ "OI_LOADRESULT"_Stz,
  /*    */ "OI_LOCKSTATS"_Stz,
  /*    */ "OI_NODEINFO"_Stz,
  /*    */ "OI_PATH"_Stz,
  /*    */ "OI_PROGRESSUNITS"_Stz,
  /*    */ "OI_PROMPTOUT"_Stz,
  /*    */ "OI_RETURNPIPELINE"_Stz,
  /*    */ "OI_SEEKOFFSETS"_Stz,
  /*    */ "OI_TOTALUNITS"_Stz,

  /*    */ "OI_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
