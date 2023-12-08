/*============================================================================*/
/* eOM.cpp -- Operator Markers                                                */
/*============================================================================*/
#include "BASE.h"
#include "eOM.h"

Stz* aOM[OM_ENUMS]
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

  /*    */ "NODELOSSABORT"_Stz,
  
  /*    */ "PULSEDMARKER"_Stz,
  
  /*    */ "RETURNMARKER"_Stz,
  /*    */ "RETURNEDMARKER"_Stz,
  
  /*    */ "STARTEDMARKER"_Stz,
  /*    */ "STARTMARKER"_Stz,
  /*    */ "STREAMDEFRHT"_Stz,
  
  /*    */ "TRIGGER"_Stz,
  /*    */ "TRIGGEREDMARKER"_Stz,

//  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
