/*============================================================================*/
/* eLS.h -- Lock Kind                                                         */
/*============================================================================*/
#pragma once

enum eLS // lock kind
{
  /*  0 */ LS_NONE=0,
  
  /*  1 */ LS_READ_BLOCKING,
  /*  2 */ LS_READ_NONBLOCK,
  /*  3 */ LS_WRITE_BLOCKING,
  /*  4 */ LS_WRITE_NONBLOCK,

  /*    */ LS_ENUMS
};

static Stz* aLS[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  1 */ "LS_READ_BLOCKING"_Stz,
  /*  2 */ "LS_READ_NONBLOCK"_Stz,
  /*  3 */ "LS_WRITE_BLOCKING"_Stz,
  /*  4 */ "LS_WRITE_NONBLOCK"_Stz,
  
  /* 41 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
