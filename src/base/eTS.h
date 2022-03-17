/*============================================================================*/
/* eTS.h -- Task State                                                        */
/*============================================================================*/
#pragma once

enum eTS // Task State
{
  /* 0 */ TS_NONE=0,
  /* 1 */ TS_BIDING,      // task is biding till operand arrives and triggers
  /* 2 */ TS_COMPLETED,   // task has completed and can be deleted
  /* 3 */ TS_DELETING,    // task is being deleted
  /* 4 */ TS_DESISTED,    // task has desisted / is under control of scheduler
  /* 5 */ TS_EXECUTING,   // task is currenty being executing by a thread
  /* 6 */ TS_HIBERNATING, // task is hibernating with its job in plist
  /* 7 */ TS_READY,       // task is queue ready to be executed
  /* 8 */ TS_REAPABLE,    // task is not in any queue/set and is repeable
  /* 9 */ TS_RETIRED,     // task is retired waiting to be reaped
  /*10 */ TS_SLEEPING,    // task is sleeping till wakeup time arrives
  /*11 */ TS_WAITING,     // task is waiting on ack from cooperating task
  /*12 */ TS_ENUMS
};
  
static Stz* aTS[]
{
  /* 0 */ "TS_NONE"_Stz,
  /* 1 */ "TS_BIDING"_Stz,
  /* 2 */ "TS_COMPLETED"_Stz,
  /* 3 */ "TS_DELETED"_Stz,
  /* 4 */ "TS_DESISTED"_Stz,
  /* 5 */ "TS_EXECUTING"_Stz,
  /* 6 */ "TS_HIBERNATING"_Stz,
  /* 7 */ "TS_READY"_Stz,
  /* 8 */ "TS_REAPABLE"_Stz,
  /* 9 */ "TS_RETIRED"_Stz,
  /*10 */ "TS_SLEEPING"_Stz,
  /*11 */ "TS_WAITING"_Stz,
  /*12 */ "TS_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
