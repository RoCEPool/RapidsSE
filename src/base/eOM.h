/*============================================================================*/
/* eOM.h -- Operator Markers                                                  */
/*============================================================================*/
#pragma once

enum eOM // operator markers
{
  /*  0 */ OM_NONE=0,
  
  /*    */ OM_CALLMARKER,
  /*    */ OM_CANCELMARKER,
  /*    */ OM_CANCELLING,
  /*    */ OM_CANCELLEDMARKER,
  /*    */ OM_CHILDEXIT,
  /*    */ OM_CLEANING,
  /*    */ OM_CLEANED,
  /*    */ OM_COMMENCEMARKER,
  /*    */ OM_COMMENCEDMARKER,
  
  /*    */ OM_DATAPULSE,
  /*    */ OM_DESTROYMARKER,

  /*    */ OM_ENDMARKER,
  /*    */ OM_ENDEDMARKER,
  /*    */ OM_ENDINGMARKER,
  /*    */ OM_EOSMARKER,
  /*    */ OM_ERRORMARKER,
  /*    */ OM_ERROREDMARKER,
  /*    */ OM_ERRORINGMARKER,
  /*    */ OM_EXIT,
  /*    */ OM_EXITING,
  /*    */ OM_EXITED,
  
  /*    */ OM_FINISHED,
  /*    */ OM_FLUSH,

  /*    */ OM_HARDABORT,

  /*    */ OM_INTERNALERROR,
  /*    */ OM_INVOKEMARKER,
  
  /*    */ OM_LEAVEMARKER,
  /*    */ OM_LEAVINGMARKER,
  /*    */ OM_LEFTMARKER,
  /*    */ OM_LOWMEMORY,
  
  /*    */ OM_NEXTBUFFERMARKER,
  /*    */ OM_NODELOSSABORT,
  
  /*    */ OM_PULSEDMARKER,
  
  /*    */ OM_RETURNMARKER,
  /*    */ OM_RETURNEDMARKER,
  
  /*    */ OM_STARTEDMARKER,
  /*    */ OM_STARTMARKER,
  /*    */ OM_STREAMDEFRHT,
  
  /*    */ OM_TRIGGER,
  /*    */ OM_TRIGGERED,

  /*    */ OM_ENUMS,
};

static Stz* aOM[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "CALLMARKER"_Stz,
  /*    */ "CANCELMARKER"_Stz,
  /*    */ "CANCELLING"_Stz,
  /*    */ "CANCELLEDMARKER"_Stz,
  /*    */ "CHILDEXIT"_Stz,
  /*    */ "CLEANING"_Stz,
  /*    */ "CLEANED"_Stz,
  /*    */ "COMMENCEMARKER"_Stz,
  /*    */ "COMMENCEDMARKER"_Stz,
  
  /*    */ "DATAPULSE"_Stz,
  /*    */ "DESTROYMARKER"_Stz,

  /*    */ "ENDMARKER"_Stz,
  /*    */ "ENDEDMARKER"_Stz,
  /*    */ "ENDINGMARKER"_Stz,
  /*    */ "EOSMARKER"_Stz,
  /*    */ "ERRORMARKER"_Stz,
  /*    */ "ERROREDMARKER"_Stz,
  /*    */ "ERRORINGMARKER"_Stz,
  /*    */ "EXIT"_Stz,
  /*    */ "EXITINGMARKER"_Stz,
  /*    */ "EXITEDMARKER"_Stz,
  
  /*    */ "FINISHED"_Stz,
  /*    */ "FLUSH"_Stz,

  /*    */ "HARDABORT"_Stz,
  /*    */ "INTERNALERROR"_Stz,
  /*    */ "INVOKEMARKER"_Stz,
  
  /*    */ "LEAVEMARKER"_Stz,
  /*    */ "LEAVINGMARKER"_Stz,
  /*    */ "LEFTMARKER"_Stz,
  /*    */ "LOWMEMORY"_Stz,
  
  /*    */ "NEXTBUFFERMARKER"_Stz,
  /*    */ "NODELOSSABORT"_Stz,
  
  /*    */ "PULSEDMARKER"_Stz,
  
  /*    */ "RETURNMARKER"_Stz,
  /*    */ "RETURNEDMARKER"_Stz,
  
  /*    */ "STARTEDMARKER"_Stz,
  /*    */ "STARTMARKER"_Stz,
  /*    */ "STREAMDEFRHT"_Stz,
  
  /*    */ "TRIGGER"_Stz,
  /*    */ "TRIGGEREDMARKER"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
