/*============================================================================*/
/* eOM.h -- Operator Markers                                                  */
/*============================================================================*/
#pragma once

enum eOM // operator markers
{
  /*  0 */ OM_NONE=0,
  
  /*    */ OM_EOSMARKER,
  /*    */ OM_ERRORMARKER,
  /*    */ OM_CANCELMARKER,
  /*    */ OM_COMMENCEMARKER,
  /*    */ OM_COMMENCEDMARKER,
  /*    */ OM_PULSEMARKER,
  /*    */ OM_STARTMARKER,
  /*    */ OM_STARTEDMARKER,
  /*    */ OM_STOPMARKER,
  /*    */ OM_STOPPEDMARKER,

  /*    */ OM_ENUMS,
};

static Stz* aOM[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "EOSMARKER"_Stz,
  /*    */ "ERRORMARKER"_Stz,
  /*    */ "CANCELMARKER"_Stz,
  /*    */ "COMMENCEMARKER"_Stz,
  /*    */ "COMMENCEDMARKER"_Stz,
  /*    */ "PULSEMARKER"_Stz,
  /*    */ "STARTMARKER"_Stz,
  /*    */ "STARTEDMARKER"_Stz,
  /*    */ "STOPMARKER"_Stz,
  /*    */ "STOPPEDMARKER"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
