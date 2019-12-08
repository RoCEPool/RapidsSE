/*============================================================================*/
/* eZK.h -- Operand Kind                                                      */
/*============================================================================*/
#pragma once

enum eZK // operand kind
{
  /*  0 */ ZK_NONE=0,

  /*    */ ZK_CANCELMARKER,
  /*    */ ZK_CRWBUFFER,
  /*    */ ZK_CSVBUFFER,
  /*    */ ZK_ENDMARKER,
  /*    */ ZK_ENTBUFFER,
  /*    */ ZK_IRWBUFFER,
  /*    */ ZK_SRTBUFFER,
  /*    */ ZK_STARTMARKER,
  /*    */ ZK_STOPMARKER,
  /*    */ ZK_STREAMDEF,

  /*    */ ZK_ENUMS
};

static Stz* aZK[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "ZK_CANCELMARKER"_Stz,
  /*    */ "ZK_CRWBUFFER"_Stz,
  /*    */ "ZK_CSVBUFFER"_Stz,
  /*    */ "ZK_ENDMARKER"_Stz,
  /*    */ "ZK_ENTBUFFER"_Stz,
  /*    */ "ZK_IRWBUFFER"_Stz,
  /*    */ "ZK_SRTBUFFER"_Stz,
  /*    */ "ZK_STARTMARKER"_Stz,
  /*    */ "ZK_STOPMARKER"_Stz,
  /*    */ "ZK_STREAMDEF"_Stz,

  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
