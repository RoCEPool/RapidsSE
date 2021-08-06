/*============================================================================*/
/* eOM.h -- Operator Markers                                                  */
/*============================================================================*/
#pragma once

enum eOM // operator markers
{
  /*  0 */ OM_NONE=0,
  
  /*    */ OM_CALLMARKER,
  /*    */ OM_CANCELMARKER,
  /*    */ OM_CANCELLEDMARKER,
  /*    */ OM_CLEANUPMARKER,
  /*    */ OM_CLEANEDUPMARKER,
  /*    */ OM_COMMENCEMARKER,
  /*    */ OM_COMMENCEDMARKER,
  /*    */ OM_DATAPULSE,
  /*    */ OM_DESTROYMARKER,
  /*    */ OM_EACHSCANEOSMARKER,
  /*    */ OM_EARLYEXITMARKER,
  /*    */ OM_EARLYEXITEDMARKER,
  /*    */ OM_EARLYEXITINGMARKER,
  /*    */ OM_ENDMARKER,
  /*    */ OM_ENDEDMARKER,
  /*    */ OM_ENDINGMARKER,
  /*    */ OM_END_CRWRHTMARKER,
  /*    */ OM_END_ROWRHTMARKER,
  /*    */ OM_EOSMARKER,
  /*    */ OM_ERRORMARKER,
  /*    */ OM_ERROREDMARKER,
  /*    */ OM_ERRORINGMARKER,
  /*    */ OM_EXITMARKER,
  /*    */ OM_EXITEDMARKER,
  /*    */ OM_LEAVEMARKER,
  /*    */ OM_LEAVINGMARKER,
  /*    */ OM_LEFTMARKER,
  /*    */ OM_LEFTOUTER_CRWLFT,
  /*    */ OM_LEFTOUTER_ROWLFT,
  /*    */ OM_NEXTBUFFERMARKER,
  /*    */ OM_PULSEDMARKER,
  /*    */ OM_RETURNMARKER,
  /*    */ OM_RETURNEDMARKER,
  /*    */ OM_STARTEDMARKER,
  /*    */ OM_START_CRWRHTMARKER,
  /*    */ OM_STARTMARKER,
  /*    */ OM_START_ROWRHTMARKER,
  /*    */ OM_TRIGGER,

  /*    */ OM_ENUMS,
};

static Stz* aOM[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "CALLMARKER"_Stz,
  /*    */ "CANCELMARKER"_Stz,
  /*    */ "CANCELLEDMARKER"_Stz,
  /*    */ "CLEANUPMARKER"_Stz,
  /*    */ "CLEANEDUPMARKER"_Stz,
  /*    */ "COMMENCEMARKER"_Stz,
  /*    */ "COMMENCEDMARKER"_Stz,
  /*    */ "DATAPULSE"_Stz,
  /*    */ "DESTROYMARKER"_Stz,
  /*    */ "EACHSCANEOSMARKER"_Stz,
  /*    */ "EARLYEXITMARKER"_Stz,
  /*    */ "EARLYEXITEDMARKER"_Stz,
  /*    */ "EARLYEXITINGMARKER"_Stz,
  /*    */ "ENDMARKER"_Stz,
  /*    */ "ENDEDMARKER"_Stz,
  /*    */ "ENDINGMARKER"_Stz,
  /*    */ "END_CRWRHTMARKER"_Stz,
  /*    */ "END_ROWRHTMARKER"_Stz,
  /*    */ "EOSMARKER"_Stz,
  /*    */ "ERRORMARKER"_Stz,
  /*    */ "ERROREDMARKER"_Stz,
  /*    */ "ERRORINGMARKER"_Stz,
  /*    */ "EXITMARKER"_Stz,
  /*    */ "EXITEDMARKER"_Stz,
  /*    */ "LEAVEMARKER"_Stz,
  /*    */ "LEAVINGMARKER"_Stz,
  /*    */ "LEFTMARKER"_Stz,
  /*    */ "LEFTOUTER_CRWLFT"_Stz,
  /*    */ "LEFTOUTER_ROWLFT"_Stz,
  /*    */ "NEXTBUFFERMARKER"_Stz,
  /*    */ "PULSEDMARKER"_Stz,
  /*    */ "RETURNMARKER"_Stz,
  /*    */ "RETURNEDMARKER"_Stz,
  /*    */ "STARTEDMARKER"_Stz,
  /*    */ "START_CRWRHTMARKER"_Stz,
  /*    */ "STARTMARKER"_Stz,
  /*    */ "START_ROWRHTMARKER"_Stz,
  /*    */ "TRIGGER"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
