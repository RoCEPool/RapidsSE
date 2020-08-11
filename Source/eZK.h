/*============================================================================*/
/* eZK.h -- Operand Kind                                                      */
/*============================================================================*/
#pragma once

enum eZK // operand kind
{
  /*  0 */ ZK_NONE=0,

  /*    */ ZK_CANCELMARKER,
  /*    */ ZK_CANCELLEDMARKER,
  /*    */ ZK_CRWBUFFER,
  /*    */ ZK_CSVBUFFER,
  /*    */ ZK_ENTBUFFER,
  /*    */ ZK_EOSMARKER,
  /*    */ ZK_ERRORMARKER,
  /*    */ ZK_ERROREDMARKER,
  /*    */ ZK_EXITMARKER,
  /*    */ ZK_EXITEDMARKER,
  /*    */ ZK_INDEXDEF,
  /*    */ ZK_LOCKSTATS,
  /*    */ ZK_NODEINFO,
  /*    */ ZK_PROFILE,
  /*    */ ZK_PROGRESSUNITS,
  /*    */ ZK_PULSEMARKER,
  /*    */ ZK_PULSEDMARKER,
  /*    */ ZK_RSBBUFFER,
  /*    */ ZK_SEEKOFFSETS,
  /*    */ ZK_SRTBUFFER,
  /*    */ ZK_STARTMARKER,
  /*    */ ZK_STARTEDMARKER,
  /*    */ ZK_STREAMDEF,
  /*    */ ZK_STREAMSET,
  /*    */ ZK_TOTALUNITS,
  /*    */ ZK_ENUMS
};

static Stz* aZK[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "ZK_CANCELMARKER"_Stz,
  /*    */ "ZK_CANCELLEDMARKER"_Stz,
  /*    */ "ZK_CRWBUFFER"_Stz,
  /*    */ "ZK_CSVBUFFER"_Stz,
  /*    */ "ZK_ENTBUFFER"_Stz,
  /*    */ "ZK_EOSMARKER"_Stz,
  /*    */ "ZK_ERRORMARKER"_Stz,
  /*    */ "ZK_ERROREDMARKER"_Stz,
  /*    */ "ZK_EXITMARKER"_Stz,
  /*    */ "ZK_EXITEDMARKER"_Stz,
  /*    */ "ZK_INDEXDEF"_Stz,
  /*    */ "ZK_LOCKSTATS"_Stz,
  /*    */ "ZK_NODEINFO"_Stz,
  /*    */ "ZK_PROFILE"_Stz,
  /*    */ "ZK_PROGRESSUNITS"_Stz,
  /*    */ "ZK_PULSEMARKER"_Stz,
  /*    */ "ZK_PULSEDMARKER"_Stz,
  /*    */ "ZK_RSBBUFFER"_Stz,
  /*    */ "ZK_SEEKOFFSETS"_Stz,
  /*    */ "ZK_SRTBUFFER"_Stz,
  /*    */ "ZK_STARTMARKER"_Stz,
  /*    */ "ZK_STARTEDMARKER"_Stz,
  /*    */ "ZK_STREAMDEF"_Stz,
  /*    */ "ZK_STREAMSET"_Stz,
  /*    */ "ZK_TOTALUNITS"_Stz,
  
  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
