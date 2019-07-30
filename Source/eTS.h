/*============================================================================*/
/* eTS.h -- Task State                                                        */
/*============================================================================*/
#pragma once

namespace Tasks
{  
  enum eTS // Task State
  {
  /* 0 */ TS_NONE=0,
  /* 1 */ TS_ABLE,      // task is able to be executed
  /* 2 */ TS_BIDING,    // task is biding till operand arrives and triggers
  /* 3 */ TS_DESISTED,  // task has desisted from executing
  /* 4 */ TS_EXECUTING, // task is currenty being executing by a thread
  /* 5 */ TS_EXITED,    // task has completed
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
  /* 3 */ "DESISTED"_Stz,
  /* 4 */ "EXECUTING"_Stz,
  /* 5 */ "EXITED"_Stz,
  /* 6 */ "PAUSED"_Stz,
  /* 7 */ "READY"_Stz,
  /* 8 */ "WAITING"_Stz,
  /* 9 */ "YIELDED"_Stz,
  /*10 */ "ENUMS"_Stz,
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
