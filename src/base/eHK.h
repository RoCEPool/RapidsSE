/*============================================================================*/
/* eHK.h -- Handler kind                                                      */
/*============================================================================*/
#pragma once

enum eHK // Handler kind
{
  /*  0 */ HK_NONE=0,

  /*    */ HK_CRWBUFFER,
  /*    */ HK_EOS,
  /*    */ HK_INDEXBUFFER,
  /*    */ HK_ROWBUFFER,
  /*    */ HK_STREAMDEF,

  /*    */ HK_ENUMS,
};

static Stz* aHK[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "HK_CRWBUFFER"_Stz,
  /*    */ "HK_EOS"_Stz,
  /*    */ "HK_INDEXBUFFER"_Stz,
  /*    */ "HK_ROWBUFFER"_Stz,
  /*    */ "HK_STREAMDEF"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
