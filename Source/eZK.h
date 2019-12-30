/*============================================================================*/
/* eZK.h -- Operand Kind                                                      */
/*============================================================================*/
#pragma once

enum eZK // operand kind
{
  /*  0 */ ZK_NONE=0,

  /*    */ ZK_CANCELMARKER,
  /*    */ ZK_COMMENCEMARKER,
  /*    */ ZK_COMMENCEDMARKER,
  /*    */ ZK_COMPLETEDMARKER,
  /*    */ ZK_CRWBUFFER,
  /*    */ ZK_CSVBUFFER,
  /*    */ ZK_EOSMARKER,
  /*    */ ZK_ENTBUFFER,
  /*    */ ZK_ERRORMARKER,
  /*    */ ZK_IRWBUFFER,
  /*    */ ZK_PULSEMARKER,
  /*    */ ZK_SRTBUFFER,
  /*    */ ZK_STARTMARKER,
  /*    */ ZK_STARTEDMARKER,
  /*    */ ZK_STOPMARKER,
  /*    */ ZK_STOPPEDMARKER,
  /*    */ ZK_STREAMDEF,

  /*    */ ZK_ENUMS
};

static Stz* aZK[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "ZK_CANCELMARKER"_Stz,
  /*    */ "ZK_COMMENCEMARKER"_Stz,
  /*    */ "ZK_COMMENCEDMARKER"_Stz,
  /*    */ "ZK_COMPLETEDMARKER"_Stz,
  /*    */ "ZK_CRWBUFFER"_Stz,
  /*    */ "ZK_CSVBUFFER"_Stz,
  /*    */ "ZK_EOSMARKER"_Stz,
  /*    */ "ZK_ENTBUFFER"_Stz,
  /*    */ "ZK_ERRORMARKER"_Stz,
  /*    */ "ZK_IRWBUFFER"_Stz,
  /*    */ "ZK_PULSEMARKER"_Stz,
  /*    */ "ZK_SRTBUFFER"_Stz,
  /*    */ "ZK_STARTMARKER"_Stz,
  /*    */ "ZK_STARTEDMARKER"_Stz,
  /*    */ "ZK_STOPMARKER"_Stz,
  /*    */ "ZK_STOPPEDMARKER"_Stz,
  /*    */ "ZK_STREAMDEF"_Stz,

  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
