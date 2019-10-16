/*============================================================================*/
/* eTS.h -- Task State                                                        */
/*============================================================================*/
#pragma once

enum eTS // Task State
{
  /* 0 */ TS_NONE=0,
  /* 1 */ TS_ABLE,      // task is able to be executed
  /* 2 */ TS_BIDING,    // task is biding till operand arrives and triggers
  /* 3 */ TS_COMPLETED, // task has completed and can be deleted
  /* 4 */ TS_DESISTED,  // task has desisted from executing
  /* 5 */ TS_EXECUTING, // task is currenty being executing by a thread
  /* 6 */ TS_PAUSED,    // task is paused till continued
  /* 7 */ TS_READY,     // task is queue ready to be executed
  /* 8 */ TS_SLEEPING,  // task is sleeping till wakeup time arrives
  /* 9 */ TS_WAITING,   // task is waiting on ack from cooperating task
  /*10 */ TS_ENUMS
};
  
static Stz* aTS[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "ABLE"_Stz,
  /* 2 */ "BIDING"_Stz,
  /* 3 */ "COMPLETED"_Stz,
  /* 4 */ "DESISTED"_Stz,
  /* 5 */ "EXECUTING"_Stz,
  /* 6 */ "PAUSED"_Stz,
  /* 7 */ "READY"_Stz,
  /* 8 */ "WAITING"_Stz,
  /* 9 */ "YIELDED"_Stz,
  /*10 */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
