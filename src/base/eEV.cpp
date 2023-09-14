/*============================================================================*/
/* eEV.cpp -- Events                                                          */
/*============================================================================*/
#include "BASE.h"
#include "eEV.h"

Stz* aEV[EV_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  /*    */ "EV_D_CRWBUFFER"_Stz,
  /*    */ "EV_D_CSVBUFFER"_Stz,
  /*    */ "EV_D_ENTBUFFER"_Stz,
  /*    */ "EV_D_GRID"_Stz,
  /*    */ "EV_D_INDEX"_Stz,
  /*    */ "EV_D_INDEXDEF"_Stz,
  /*    */ "EV_D_JOB"_Stz,
  /*    */ "EV_D_JOBMETRICS"_Stz,
  /*    */ "EV_D_METAGET"_Stz,
  /*    */ "EV_D_NAMEDBUFFER"_Stz,
  /*    */ "EV_D_OP"_Stz,
  /*    */ "EV_D_OTHEROPERAND"_Stz,
  /*    */ "EV_D_PAGINGMETRICS"_Stz,
  /*    */ "EV_D_PARCEL"_Stz,
  /*    */ "EV_D_PAYLOAD"_Stz,
  /*    */ "EV_D_PLANOPERAND"_Stz,
  /*    */ "EV_D_PROFILE"_Stz,
  /*    */ "EV_D_ROWBUFFER"_Stz,
  /*    */ "EV_D_SRTBUFFER"_Stz,
  /*    */ "EV_D_STREAMDEF"_Stz,
  /*    */ "EV_D_STREAMDEFRHT"_Stz,
  /*    */ "EV_D_STREAMSET"_Stz,
  /*    */ "EV_D_TABLE"_Stz,
  /*    */ "EV_D_TABLEPTN"_Stz,

  /*    */ "EV_I_CALLPIPELINE"_Stz,
  /*    */ "EV_I_COMMAND"_Stz,
  /*    */ "EV_I_COMPILEERRORS"_Stz,
  /*    */ "EV_I_COUNT"_Stz,
  /*    */ "EV_I_EXECERRORS"_Stz,
  /*    */ "EV_I_FORKCOMMAND"_Stz,
  /*    */ "EV_I_LOADRESULT"_Stz,
  /*    */ "EV_I_LOCKSTATS"_Stz,
  /*    */ "EV_I_NODEINFO"_Stz,
  /*    */ "EV_I_PAGEINREQUEST"_Stz,
  /*    */ "EV_I_PATH"_Stz,
  /*    */ "EV_I_PROGRESSUNITS"_Stz,
  /*    */ "EV_I_PROMPTOUT"_Stz,
  /*    */ "EV_I_RETURNPIPELINE"_Stz,
  /*    */ "EV_I_SEEKOFFSETS"_Stz,
  /*    */ "EV_I_TOTALUNITS"_Stz,

  /*    */ "EV_M_CANCEL"_Stz,
  /*    */ "EV_M_CANCELLING"_Stz,
  /*    */ "EV_M_CLEANING"_Stz,
  /*    */ "EV_M_CLEANED"_Stz,
  /*    */ "EV_M_CANCELLED"_Stz,
  /*    */ "EV_M_DATAPULSE"_Stz,
  /*    */ "EV_M_DESTROY"_Stz,
  /*    */ "EV_M_END"_Stz,
  /*    */ "EV_M_ENDED"_Stz,
  /*    */ "EV_M_ENDING"_Stz,
  /*    */ "EV_M_EOS"_Stz,
  /*    */ "EV_M_ERROR"_Stz,
  /*    */ "EV_M_ERRORED"_Stz,
  /*    */ "EV_M_ERRORING"_Stz,
  /*    */ "EV_M_EXITING"_Stz,
  /*    */ "EV_M_EXITED"_Stz,
  /*    */ "EV_M_FINISHED"_Stz,
  /*    */ "EV_M_FLUSH"_Stz,
  /*    */ "EV_M_HARDABORT"_Stz,
  /*    */ "EV_M_INTERNALERROR"_Stz,
  /*    */ "EV_M_INVOKE"_Stz,
  /*    */ "EV_M_LEAVE"_Stz,
  /*    */ "EV_M_LEAVING"_Stz,
  /*    */ "EV_M_LEFT"_Stz,
  /*    */ "EV_M_LOWMEMORY"_Stz,
  /*    */ "EV_M_NEXTBUFFER"_Stz,
  /*    */ "EV_M_NODELOSSABORT"_Stz,
  /*    */ "EV_M_STARTED"_Stz,
  /*    */ "EV_M_START"_Stz,
  /*    */ "EV_M_STREAMDEFRHT"_Stz,
  /*    */ "EV_M_TRIGGER"_Stz,
  /*    */ "EV_M_TRIGGERED"_Stz,

  /*    */ "EV_X_CHILDEXIT"_Stz,
  /*    */ "EV_X_EXIT"_Stz,

//  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
