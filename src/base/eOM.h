/*============================================================================*/
/* eOM.h -- Operator Markers                                                  */
/*============================================================================*/
#pragma once

enum eOM // operator markers
{
  /*  0 */ OM_NONE=0,

  /*    */ OM_CANCELMARKER,
  /*    */ OM_CANCELLING,
  /*    */ OM_CANCELLEDMARKER,
  /*    */ OM_CHILDEXIT,
  /*    */ OM_CLEANING,
  /*    */ OM_CLEANED,
  
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

  /*    */ OM_NODELOSSABORT,
  
  /*    */ OM_STARTEDMARKER,
  /*    */ OM_STARTMARKER,
  
  /*    */ OM_TRIGGER,
  /*    */ OM_TRIGGERED,

  /*    */ OM_ENUMS,
};

extern Stz* aOM[OM_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
