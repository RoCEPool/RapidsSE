/*============================================================================*/
/* eRA.h -- Resource Actions                                                  */
/*============================================================================*/
#pragma once

enum eRA // resource actions
{
  /*  0 */ RA_NONE=0,

  /*    */ RA_DIR_CLOSED,
  /*    */ RA_DIR_CONSTRUCT,
  /*    */ RA_DIR_DESTRUCT,
  /*    */ RA_DIR_OPENED,

  /*    */ RA_FIL_CLOSED,
  /*    */ RA_FIL_CONSTRUCT,
  /*    */ RA_FIL_DESTRUCT,
  /*    */ RA_FIL_OPENED,

  /*    */ RA_FIO_CONSTRUCT,
  /*    */ RA_FIO_DESTRUCT,
  /*    */ RA_FIO_MADE,
  /*    */ RA_FIO_REMOVED,
  
  /*    */ RA_MTX_CONSTRUCT,
  /*    */ RA_MTX_DESTRUCT,
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
  /*    */ RA_SKT_CONSTRUCT,
  /*    */ RA_SKT_CREATED,
  /*    */ RA_SKT_DESTRUCT,
  /*    */ RA_SKT_RECEIVED,
  /*    */ RA_SKT_SENT,

  /*    */ RA_THD_AWAKE,
  /*    */ RA_THD_CONSTRUCT,
  /*    */ RA_THD_DESTRUCT,
  /*    */ RA_THD_ENTRY,
  /*    */ RA_THD_EXIT,
  /*    */ RA_THD_SLEEP,
  /*    */ RA_THD_SLEEP_INDEF,
  /*    */ RA_THD_SPAWNED,
  
  /*    */ RA_ENUMS,
};

static Stz* aRA[]
{
  /*  0 */ "NONE"_Stz,

  /*    */ "RA_DIR_CLOSED"_Stz,
  /*    */ "RA_DIR_CONSTRUCT"_Stz,
  /*    */ "RA_DIR_DESTRUCT"_Stz,
  /*    */ "RA_DIR_OPENED"_Stz,

  /*    */ "RA_FIL_CLOSED"_Stz,
  /*    */ "RA_FIL_CONSTRUCT"_Stz,
  /*    */ "RA_FIL_DESTRUCT"_Stz,
  /*    */ "RA_FIL_OPENED"_Stz,

  /*    */ "RA_FIO_CONSTRUCT"_Stz,
  /*    */ "RA_FIO_DESTRUCT"_Stz,
  /*    */ "RA_FIO_MADE"_Stz,
  /*    */ "RA_FIO_REMOVED"_Stz,
  
  /*    */ "RA_MTX_CONSTRUCT"_Stz,
  /*    */ "RA_MTX_DESTRUCT"_Stz,
  /*    */ "RA_MTX_LOCKED"_Stz,
  /*    */ "RA_MTX_SIGNAL"_Stz,
  /*    */ "RA_MTX_SIGNALED"_Stz,
  /*    */ "RA_MTX_TIMEOUT"_Stz,
  /*    */ "RA_MTX_UNLOCK"_Stz,
  /*    */ "RA_MTX_WAIT_INDEF"_Stz,
  /*    */ "RA_MTX_WAIT_TIMED"_Stz,

  /*    */ "RA_SKT_ACCEPTED"_Stz,
  /*    */ "RA_SKT_CLOSED"_Stz,
  /*    */ "RA_SKT_CONNECTED"_Stz,
  /*    */ "RA_SKT_CONSTRUCT"_Stz,
  /*    */ "RA_SKT_CREATED"_Stz,
  /*    */ "RA_SKT_DESTRUCT"_Stz,
  /*    */ "RA_SKT_RECEIVED"_Stz,
  /*    */ "RA_SKT_SENT"_Stz,

  /*    */ "RA_THD_AWAKE"_Stz,
  /*    */ "RA_THD_CONSTRUCT"_Stz,
  /*    */ "RA_THD_DESTRUCT"_Stz,
  /*    */ "RA_THD_ENTRY"_Stz,
  /*    */ "RA_THD_EXIT"_Stz,
  /*    */ "RA_THD_SLEEP"_Stz,
  /*    */ "RA_THD_SLEEP_INDEF"_Stz,
  /*    */ "RA_THD_SPAWNED"_Stz,
  
  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
