/*============================================================================*/
/* eTS.h -- Task State                                                        */
/*============================================================================*/
#pragma once

enum eTS // Task State
{
  /* 0 */ TS_NONE=0,
  /* 1 */ TS_BIDING,    // task is biding till operand arrives and triggers
  /* 2 */ TS_COMPLETED, // task has completed and can be deleted
  /* 3 */ TS_DESISTED,  // task has desisted / not executing
  /* 4 */ TS_EXECUTING, // task is currenty being executing by a thread
  /* 5 */ TS_READY,     // task is queue ready to be executed
  /* 6 */ TS_SLEEPING,  // task is sleeping till wakeup time arrives
  /* 7 */ TS_WAITING,   // task is waiting on ack from cooperating task
  /* 8 */ TS_ENUMS
};
  
static Stz* aTS[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "BIDING"_Stz,
  /* 2 */ "COMPLETED"_Stz,
  /* 3 */ "DESISTED"_Stz,
  /* 4 */ "EXECUTING"_Stz,
  /* 5 */ "READY"_Stz,
  /* 6 */ "WAITING"_Stz,
  /* 7 */ "YIELDED"_Stz,
  /* 8 */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
