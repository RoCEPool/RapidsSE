/*============================================================================*/
/* eTS.h -- Task State                                                        */
/*============================================================================*/
#pragma once

enum eTS // Task State
{
  /* 0 */ TS_NONE=0,
  /* 1 */ TS_BIDING,    // task is biding till operand arrives and triggers
  
  // TO BE REMOVED
  /* 2 */ TS_COMPLETED, // task has completed and can be deleted
  /* 3 */ TS_DELETING,  // task is being deleted
  /* 4 */ TS_DESISTED,  // task has desisted / is under control of scheduler
  /* 5 */ TS_EXECUTING, // task is currenty being executing by a thread
  /* 6 */ TS_READY,     // task is queue ready to be executed
  /* 7 */ TS_SLEEPING,  // task is sleeping till wakeup time arrives
  /* 8 */ TS_WAITING,   // task is waiting on ack from cooperating task
  /* 9 */ TS_ENUMS
};
  
static Stz* aTS[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "BIDING"_Stz,
  /* 2 */ "COMPLETED"_Stz,
  /* 3 */ "DELETED"_Stz,
  /* 4 */ "DESISTED"_Stz,
  /* 5 */ "EXECUTING"_Stz,
  /* 6 */ "READY"_Stz,
  /* 7 */ "WAITING"_Stz,
  /* 8 */ "YIELDED"_Stz,
  /* 9 */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
