/*============================================================================*/
/* eOM.h -- Operator Markers                                                  */
/*============================================================================*/
#pragma once

enum eOM // operator markers
{
  /*  0 */ OM_NONE=0,
  
  /*    */ OM_EOSMARKER,
  /*    */ OM_ERRORMARKER,
  /*    */ OM_ERROREDMARKER,
  /*    */ OM_EXITMARKER,
  /*    */ OM_EXITEDMARKER,
  /*    */ OM_CANCELMARKER,
  /*    */ OM_COMMENCEMARKER,
  /*    */ OM_COMMENCEDMARKER,
  /*    */ OM_PULSEMARKER,
  /*    */ OM_PULSEDMARKER,
  /*    */ OM_STARTMARKER,
  /*    */ OM_STARTEDMARKER,
  /*    */ OM_UNNESTEDMARKER,

  /*    */ OM_ENUMS,
};

static Stz* aOM[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "EOSMARKER"_Stz,
  /*    */ "ERRORMARKER"_Stz,
  /*    */ "ERROREDMARKER"_Stz,
  /*    */ "EXITMARKER"_Stz,
  /*    */ "EXITEDMARKER"_Stz,
  /*    */ "CANCELMARKER"_Stz,
  /*    */ "COMMENCEMARKER"_Stz,
  /*    */ "COMMENCEDMARKER"_Stz,
  /*    */ "PULSEMARKER"_Stz,
  /*    */ "PULSEDMARKER"_Stz,
  /*    */ "STARTMARKER"_Stz,
  /*    */ "STARTEDMARKER"_Stz,
  /*    */ "UNNESTEDMARKER"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
