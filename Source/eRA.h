/*============================================================================*/
/* eRA.h -- Resource Actions                                                  */
/*============================================================================*/
#pragma once

enum eRA // resource actions
{
  /*  0 */ RA_NONE=0,

  /*    */ RA_DIR_CLOSED,
  /*    */ RA_DIR_OPENED,

  /*    */ RA_FIL_CLOSED,
  /*    */ RA_FIL_OPENED,

  /*    */ RA_MTX_CREATE,
  /*    */ RA_MTX_DESTROY,
  /*    */ RA_MTX_LOCKED,
  /*    */ RA_MTX_SIGNAL,
  /*    */ RA_MTX_SIGNALED,
  /*    */ RA_MTX_TIMEOUT,
  /*    */ RA_MTX_UNLOCKED,
  /*    */ RA_MTX_WAIT_INDEFINITE,
  /*    */ RA_MTX_WAIT_TIMED,

  /*    */ RA_PIP_MADE,
  /*    */ RA_PIP_REMOVED,

  /*    */ RA_SKT_ACCEPTED,
  /*    */ RA_SKT_CLOSED,
  /*    */ RA_SKT_CONNECTED,
  /*    */ RA_SKT_CREATED,
  /*    */ RA_SKT_RECEIVED,
  /*    */ RA_SKT_SENT,

  /*    */ RA_THD_AWAKE,
  /*    */ RA_THD_ENTRY,
  /*    */ RA_THD_EXIT,
  /*    */ RA_THD_SLEEP,
  /*    */ RA_THD_SLEEP_INDEFINITE,
  /*    */ RA_THD_SPAWNED,
  
  /*    */ RA_ENUMS,
};

static Stz* aRA[]
{
  /*  0 */ "NONE"_Stz,

  /*    */ "DIR_CLOSED"_Stz,
  /*    */ "DIR_OPENED"_Stz,

  /*    */ "FIL_CLOSED"_Stz,
  /*    */ "FIL_OPENED"_Stz,

  /*    */ "MTX_CREATE"_Stz,
  /*    */ "MTX_DESTROY"_Stz,
  /*    */ "MTX_LOCK"_Stz,
  /*    */ "MTX_SIGNAL"_Stz,
  /*    */ "MTX_SIGNALED"_Stz,
  /*    */ "MTX_TIMEOUT"_Stz,
  /*    */ "MTX_UNLOCK"_Stz,
  /*    */ "MTX_WAIT_INDEFINITE"_Stz,
  /*    */ "MTX_WAIT_TIMED"_Stz,

  /*    */ "PIP_MADE"_Stz,
  /*    */ "PIP_REMOVED"_Stz,

  /*    */ "SKT_ACCEPTED"_Stz,
  /*    */ "SKT_CLOSED"_Stz,
  /*    */ "SKT_CONNECTED"_Stz,
  /*    */ "SKT_CREATED"_Stz,
  /*    */ "SKT_RECEIVED"_Stz,
  /*    */ "SKT_SENT"_Stz,

  /*    */ "THD_AWAKE"_Stz,
  /*    */ "THD_ENTRY"_Stz,
  /*    */ "THD_EXIT"_Stz,
  /*    */ "THD_SLEEP"_Stz,
  /*    */ "THD_SLEEP_INDEFINITE"_Stz,
  /*    */ "THD_SPAWNED"_Stz,
  
  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
