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
  /*    */ ZK_INFOSET,
  /*    */ ZK_IRWBUFFER,
  /*    */ ZK_PULSEMARKER,
  /*    */ ZK_PULSEDMARKER,
  /*    */ ZK_SRTBUFFER,
  /*    */ ZK_STARTMARKER,
  /*    */ ZK_STARTEDMARKER,
  /*    */ ZK_STREAMDEF,
  /*    */ ZK_STREAMSET,

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
  /*    */ "ZK_INFOSET"_Stz,
  /*    */ "ZK_IRWBUFFER"_Stz,
  /*    */ "ZK_PULSEMARKER"_Stz,
  /*    */ "ZK_PULSEDMARKER"_Stz,
  /*    */ "ZK_SRTBUFFER"_Stz,
  /*    */ "ZK_STARTMARKER"_Stz,
  /*    */ "ZK_STARTEDMARKER"_Stz,
  /*    */ "ZK_STREAMDEF"_Stz,
  /*    */ "ZK_STREAMSET"_Stz,
  
  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
