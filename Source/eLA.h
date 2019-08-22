/*============================================================================*/
/* eLA.h -- Lock Actions                                                      */
/*============================================================================*/
#pragma once

enum eLA // lock actions
{
  /*  0 */ LA_NONE=0,
  
  /*    */ LA_CONSTRUCTOR,
  /*    */ LA_DESTRUCTOR,
  
  /*    */ LA_READ_ABLE,
  /*    */ LA_WRITE_ABLE,
  /*    */ LA_READ_UNABLE,
  /*    */ LA_WRITE_UNABLE,

  /*    */ LA_ENUMS,
};

static Stz* aLA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "LA_CONSTRUCTOR"_Stz,
  /*    */ "LA_DESTRUCTOR"_Stz,
  
  /*    */ "LA_READ_ABLE"_Stz,
  /*    */ "LA_WRITE_ABLE"_Stz,
  /*    */ "LA_READ_UNABLE"_Stz,
  /*    */ "LA_WRITE_UNABLE"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
