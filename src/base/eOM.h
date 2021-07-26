/*============================================================================*/
/* eOM.h -- Operator Markers                                                  */
/*============================================================================*/
#pragma once

enum eOM // operator markers
{
  /*  0 */ OM_NONE=0,
  
  /*    */ OM_CALLMARKER,
  /*    */ OM_CANCELMARKER,
  /*    */ OM_COMMENCEMARKER,
  /*    */ OM_COMMENCEDMARKER,
  /*    */ OM_DATAPULSE,
  /*    */ OM_DESTROYMARKER,
  /*    */ OM_EACHSCANEOSMARKER,
  /*    */ OM_ENDCRW_RHTMARKER,
  /*    */ OM_ENDROW_RHTMARKER,
  /*    */ OM_EOSMARKER,
  /*    */ OM_ERRORMARKER,
  /*    */ OM_ERROREDMARKER,
  /*    */ OM_EXITMARKER,
  /*    */ OM_EXITEARLYMARKER,
  /*    */ OM_EXITEDMARKER,
  /*    */ OM_EXITEDEARLYMARKER,
  /*    */ OM_NEXTBUFFERMARKER,
  /*    */ OM_PULSEDMARKER,
  /*    */ OM_RETURNMARKER,
  /*    */ OM_RETURNEDMARKER,
  /*    */ OM_STARTEDMARKER,
  /*    */ OM_STARTCRW_RHTMARKER,
  /*    */ OM_STARTMARKER,
  /*    */ OM_STARTROW_RHTMARKER,

  /*    */ OM_ENUMS,
};

static Stz* aOM[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "CALLMARKER"_Stz,
  /*    */ "CANCELMARKER"_Stz,
  /*    */ "COMMENCEMARKER"_Stz,
  /*    */ "COMMENCEDMARKER"_Stz,
  /*    */ "DATAPULSE"_Stz,
  /*    */ "DESTROYMARKER"_Stz,
  /*    */ "EACHSCANEOSMARKER"_Stz,
  /*    */ "ENDCRW_RHTMARKER"_Stz,
  /*    */ "ENDROW_RHTMARKER"_Stz,
  /*    */ "EOSMARKER"_Stz,
  /*    */ "ERRORMARKER"_Stz,
  /*    */ "ERROREDMARKER"_Stz,
  /*    */ "EXITMARKER"_Stz,
  /*    */ "EXITEARLYMARKER"_Stz,
  /*    */ "EXITEDMARKER"_Stz,
  /*    */ "EXITEDEARLYMARKER"_Stz,
  /*    */ "NEXTBUFFERMARKER"_Stz,
  /*    */ "PULSEDMARKER"_Stz,
  /*    */ "RETURNMARKER"_Stz,
  /*    */ "RETURNEDMARKER"_Stz,
  /*    */ "STARTEDMARKER"_Stz,
  /*    */ "STARTCRW_RHTMARKER"_Stz,
  /*    */ "STARTMARKER"_Stz,
  /*    */ "STARTROW_RHTMARKER"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
