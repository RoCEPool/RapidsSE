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

  /*    */ RA_MTX_CREATE,
  /*    */ RA_MTX_DESTROY,
  /*    */ RA_MTX_LOCKED,
  /*    */ RA_MTX_SIGNAL,
  /*    */ RA_MTX_SIGNALED,
  /*    */ RA_MTX_TIMEOUT,
  /*    */ RA_MTX_UNLOCKED,
  /*    */ RA_MTX_WAIT_INDEFINITE,
  /*    */ RA_MTX_WAIT_TIMED,

  /*    */ RA_PIP_MAKE,
  /*    */ RA_PIP_REMOVE,

  /*    */ RA_SKT_,
  
  /*    */ RA_PTH_AWAKE,
  /*    */ RA_PTH_SLEEP,
  /*    */ RA_PTH_SLEEP_INDEFINITE,
  /*    */ RA_PTH_SPAWNED,
  
  /*    */ RA_ENUMS,
};

static Stz* aRA[]
{
  /*  0 */ "NONE"_Stz,

  /*    */ "DIR_CLOSE"_Stz,
  /*    */ "DIR_OPEN"_Stz,

  /*    */ "FIL_CLOSE"_Stz,
  /*    */ "FIL_OPEN"_Stz,

  /*    */ "MTX_CREATE"_Stz,
  /*    */ "MTX_DESTROY"_Stz,
  /*    */ "MTX_LOCK"_Stz,
  /*    */ "MTX_SIGNAL"_Stz,
  /*    */ "MTX_SIGNALED"_Stz,
  /*    */ "MTX_TIMEOUT"_Stz,
  /*    */ "MTX_UNLOCK"_Stz,
  /*    */ "MTX_WAIT_INDEFINITE"_Stz,
  /*    */ "MTX_WAIT_TIMED"_Stz,

  /*    */ "PIP_MAKE"_Stz,
  /*    */ "PIP_REMOVE"_Stz,

  /*    */ "SKT_"_Stz,
  
  /*    */ "PTH_AWAKE"_Stz,
  /*    */ "PTH_SLEEP"_Stz,
  /*    */ "PTH_SLEEP_INDEFINITE"_Stz,
  /*    */ "PTH_SPAWNED"_Stz,
  
  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
