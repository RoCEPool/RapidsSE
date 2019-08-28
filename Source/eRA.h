/*============================================================================*/
/* eRA.h -- Resource Actions                                                  */
/*============================================================================*/
#pragma once

enum eRA // resource actions
{
  /*  0 */ RA_NONE=0,

  /*    */ RA_DIR_CLOSED,
  /*    */ RA_DIR_CONSTRUCTOR,
  /*    */ RA_DIR_DESTRUCTOR,
  /*    */ RA_DIR_OPENED,

  /*    */ RA_FIL_CLOSED,
  /*    */ RA_FIL_CONSTRUCTOR,
  /*    */ RA_FIL_DESTRUCTOR,
  /*    */ RA_FIL_OPENED,

  /*    */ RA_FIO_CONSTRUCTOR,
  /*    */ RA_FIO_DESTRUCTOR,
  /*    */ RA_FIO_MADE,
  /*    */ RA_FIO_REMOVED,
  
  /*    */ RA_MTX_CONSTRUCTOR,
  /*    */ RA_MTX_DESTRUCTOR,
  /*    */ RA_MTX_LOCKED,
  /*    */ RA_MTX_SIGNAL,
  /*    */ RA_MTX_SIGNALED,
  /*    */ RA_MTX_TIMEOUT,
  /*    */ RA_MTX_UNLOCKED,
  /*    */ RA_MTX_WAIT_INDEF,
  /*    */ RA_MTX_WAIT_TIMED,

  /*    */ RA_SKT_ACCEPTED,
  /*    */ RA_SKT_CLOSED,
  /*    */ RA_SKT_CONNECTED,
  /*    */ RA_SKT_CONSTRUCTOR,
  /*    */ RA_SKT_CREATED,
  /*    */ RA_SKT_DESTRUCTOR,
  /*    */ RA_SKT_RECEIVED,
  /*    */ RA_SKT_SENT,

  /*    */ RA_THD_AWAKE,
  /*    */ RA_THD_CONSTRUCTOR,
  /*    */ RA_THD_DESTRUCTOR,
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

  /*    */ "RA_DIR_CLOSED"_Stz,
  /*    */ "RA_DIR_CONSTRUCTOR"_Stz,
  /*    */ "RA_DIR_DESTRUCTOR"_Stz,
  /*    */ "RA_DIR_OPENED"_Stz,

  /*    */ "RA_FIL_CLOSED"_Stz,
  /*    */ "RA_FIL_CONSTRUCTOR"_Stz,
  /*    */ "RA_FIL_DESTRUCTOR"_Stz,
  /*    */ "RA_FIL_OPENED"_Stz,

  /*    */ "RA_FIO_CONSTRUCTOR"_Stz,
  /*    */ "RA_FIO_DESTRUCTOR"_Stz,
  /*    */ "RA_FIO_MADE"_Stz,
  /*    */ "RA_FIO_REMOVED"_Stz,
  
  /*    */ "RA_MTX_CONSTRUCTOR"_Stz,
  /*    */ "RA_MTX_CREATE"_Stz,
  /*    */ "RA_MTX_LOCK"_Stz,
  /*    */ "RA_MTX_SIGNAL"_Stz,
  /*    */ "RA_MTX_SIGNALED"_Stz,
  /*    */ "RA_MTX_TIMEOUT"_Stz,
  /*    */ "RA_MTX_UNLOCK"_Stz,
  /*    */ "RA_MTX_WAIT_INDEF"_Stz,
  /*    */ "RA_MTX_WAIT_TIMED"_Stz,

  /*    */ "RA_SKT_ACCEPTED"_Stz,
  /*    */ "RA_SKT_CLOSED"_Stz,
  /*    */ "RA_SKT_CONNECTED"_Stz,
  /*    */ "RA_SKT_CONSTRUCTOR"_Stz,
  /*    */ "RA_SKT_CREATED"_Stz,
  /*    */ "RA_SKT_DESTRUCTOR"_Stz,
  /*    */ "RA_SKT_RECEIVED"_Stz,
  /*    */ "RA_SKT_SENT"_Stz,

  /*    */ "RA_THD_AWAKE"_Stz,
  /*    */ "RA_THD_CONSTRUCTOR"_Stz,
  /*    */ "RA_THD_DESTRUCTOR"_Stz,
  /*    */ "RA_THD_ENTRY"_Stz,
  /*    */ "RA_THD_EXIT"_Stz,
  /*    */ "RA_THD_SLEEP"_Stz,
  /*    */ "RA_THD_SLEEP_INDEFINITE"_Stz,
  /*    */ "RA_THD_SPAWNED"_Stz,
  
  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
