/*============================================================================*/
/* eEV.h -- Events                                                            */
/*============================================================================*/
#pragma once

enum eEV // events
{
  /*  0 */ EV_NONE=0,

  /*    */ EV_D_CRWBUFFER,
  /*    */ EV_D_CSVBUFFER,
  /*    */ EV_D_ENTBUFFER,
  /*    */ EV_D_GRID,
  /*    */ EV_D_INDEX,
  /*    */ EV_D_INDEXDEF,
  /*    */ EV_D_JOB,
  /*    */ EV_D_JOBMETRICS,
  /*    */ EV_D_METAGET,
  /*    */ EV_D_NAMEDBUFFER,
  /*    */ EV_D_OP,
  /*    */ EV_D_OTHEROPERAND,
  /*    */ EV_D_PAGINGMETRICS,
  /*    */ EV_D_PARCEL,
  /*    */ EV_D_PAYLOAD,
  /*    */ EV_D_PLANOPERAND,
  /*    */ EV_D_PROFILE,
  /*    */ EV_D_ROWBUFFER,
  /*    */ EV_D_SRTBUFFER,
  /*    */ EV_D_STREAMDEF,
  /*    */ EV_D_STREAMDEFRHT,
  /*    */ EV_D_STREAMSET,
  /*    */ EV_D_TABLE,
  /*    */ EV_D_TABLEPTN,

  /*    */ EV_I_CALLPIPELINE,
  /*    */ EV_I_COMMAND,
  /*    */ EV_I_COMPILEERRORS,
  /*    */ EV_I_COUNT,
  /*    */ EV_I_EXECERRORS,
  /*    */ EV_I_FORKCOMMAND,
  /*    */ EV_I_PAGEINREQUEST,
  /*    */ EV_I_PATH,
  /*    */ EV_I_WARNINGS,

  /*    */ EV_M_CANCEL,
  /*    */ EV_M_CANCELLING,
  /*    */ EV_M_CLEANING,
  /*    */ EV_M_CLEANED,
  /*    */ EV_M_CANCELLED,
  /*    */ EV_M_DATAPULSE,
  /*    */ EV_M_DESTROY,
  /*    */ EV_M_END,
  /*    */ EV_M_ENDED,
  /*    */ EV_M_ENDING,
  /*    */ EV_M_EOS,
  /*    */ EV_M_ERROR,
  /*    */ EV_M_ERRORED,
  /*    */ EV_M_ERRORING,
  /*    */ EV_M_EXITING,
  /*    */ EV_M_EXITED,
  /*    */ EV_M_FINISHED,
  /*    */ EV_M_FLUSH,
  /*    */ EV_M_HARDABORT,
  /*    */ EV_M_INTERNALERROR,
  /*    */ EV_M_INVOKE,
  /*    */ EV_M_LEAVE,
  /*    */ EV_M_LEAVING,
  /*    */ EV_M_LEFT,
  /*    */ EV_M_LOWMEMORY,
  /*    */ EV_M_NEXTBUFFER,
  /*    */ EV_M_NODELOSSABORT,
  /*    */ EV_M_STARTED,
  /*    */ EV_M_START,
  /*    */ EV_M_STREAMDEFRHT,
  /*    */ EV_M_TRIGGER,
  /*    */ EV_M_TRIGGERED,

  /*    */ EV_X_CHILDEXIT,
  /*    */ EV_X_EXIT,

  /*    */ EV_ENUMS
};

extern Stz* aEV[EV_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
