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
  /*    */ OM_END_CRWRHTMARKER,
  /*    */ OM_END_ROWRHTMARKER,
  /*    */ OM_EOSMARKER,
  /*    */ OM_ERRORMARKER,
  /*    */ OM_ERROREDMARKER,
  /*    */ OM_EXITMARKER,
  /*    */ OM_EXITEARLYMARKER,
  /*    */ OM_EXITEDMARKER,
  /*    */ OM_EXITEDEARLYMARKER,
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
  /*    */ "END_CRWRHTMARKER"_Stz,
  /*    */ "END_ROWRHTMARKER"_Stz,
  /*    */ "EOSMARKER"_Stz,
  /*    */ "ERRORMARKER"_Stz,
  /*    */ "ERROREDMARKER"_Stz,
  /*    */ "EXITMARKER"_Stz,
  /*    */ "EXITEARLYMARKER"_Stz,
  /*    */ "EXITEDMARKER"_Stz,
  /*    */ "EXITEDEARLYMARKER"_Stz,
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

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
