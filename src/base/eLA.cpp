/*============================================================================*/
/* eLA.cpp -- Lock Actions                                                    */
/*============================================================================*/
#include "BASE.h"
#include "eLA.h"

Stz* aLA[LA_LCK_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "LA_LCK_CONSTRUCT"_Stz,
  /*    */ "LA_LCK_DESTRUCT"_Stz,
  
  /*    */ "LA_LCK_READ_ABLE"_Stz,
  /*    */ "LA_LCK_WRITE_ABLE"_Stz,
  /*    */ "LA_LCK_READ_UNABLE"_Stz,
  /*    */ "LA_LCK_WRITE_UNABLE"_Stz,

//  /*    */ "ENUMS"_Stz,
};

Stz* aLA_short[LA_LCK_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "Lock construct"_Stz,
  /*    */ "Lock destruct"_Stz,
  
  /*    */ "Lock read able"_Stz,
  /*    */ "Lock write able"_Stz,
  /*    */ "Lock read unable"_Stz,
  /*    */ "Lock write unable"_Stz,

//  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
