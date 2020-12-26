/*============================================================================*/
/* eLA.h -- Lock Actions                                                      */
/*============================================================================*/
#pragma once

enum eLA // lock actions
{
  /*  0 */ LA_LCK_NONE=0,
  
  /*    */ LA_LCK_CONSTRUCT,
  /*    */ LA_LCK_DESTRUCT,
  
  /*    */ LA_LCK_READ_ABLE,
  /*    */ LA_LCK_WRITE_ABLE,
  /*    */ LA_LCK_READ_UNABLE,
  /*    */ LA_LCK_WRITE_UNABLE,

  /*    */ LA_LCK_ENUMS,
};

static Stz* aLA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "LA_LCK_CONSTRUCT"_Stz,
  /*    */ "LA_LCK_DESTRUCT"_Stz,
  
  /*    */ "LA_LCK_READ_ABLE"_Stz,
  /*    */ "LA_LCK_WRITE_ABLE"_Stz,
  /*    */ "LA_LCK_READ_UNABLE"_Stz,
  /*    */ "LA_LCK_WRITE_UNABLE"_Stz,

  /*    */ "ENUMS"_Stz,
};

static Stz* aLA_short[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "Lock construct"_Stz,
  /*    */ "Lock destruct"_Stz,
  
  /*    */ "Lock read able"_Stz,
  /*    */ "Lock write able"_Stz,
  /*    */ "Lock read unable"_Stz,
  /*    */ "Lock write unable"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
