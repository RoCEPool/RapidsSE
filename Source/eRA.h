/*============================================================================*/
/* eRA.h -- Resource Actions                                                  */
/*============================================================================*/
#pragma once

enum eRA // resource actions
{
  /*  0 */ RA_NONE=0,

  /*    */ RA_DIR_CLOSE,
  /*    */ RA_DIR_OPEN,

  /*    */ RA_FIL_CLOSE,
  /*    */ RA_FIL_OPEN,

  /*    */ RA_MTX_,
  
  /*    */ RA_PIP_,

  /*    */ RA_SKT_,
  
  /*    */ RA_PTH_,
  
  /*    */ RA_ENUMS,
};

static Stz* aRA[]
{
  /*  0 */ "NONE"_Stz,

  /*    */ "DIR_CLOSE"_Stz,
  /*    */ "DIR_OPEN"_Stz,

  /*    */ "FIL_CLOSE"_Stz,
  /*    */ "FIL_OPEN"_Stz,

  /*    */ "MTX_"_Stz,
  
  /*    */ "PIP_"_Stz,
  
  /*    */ "SKT_"_Stz,
  
  /*    */ "PTH_"_Stz,
  
  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
