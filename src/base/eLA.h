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

extern Stz* aLA[LA_LCK_ENUMS];
extern Stz* aLA_short[LA_LCK_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
