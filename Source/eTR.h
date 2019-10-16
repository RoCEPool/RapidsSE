/*============================================================================*/
/* eTR.h -- Task State Transition                                             */
/*============================================================================*/
#pragma once

enum eTR // Task State Transition
{
  /* 0 */ TR_NONE=0,
  /* 1 */ TR_BIDE,    // task to bide
  /* 2 */ TR_DELETE,  // task to be deleted
  /* 3 */ TR_EXECUTE, // task to execute
  /* 4 */ TR_PAUSE,   // task to pause
  /* 5 */ TR_SIGNAL,  // task to signal waiter
  /* 6 */ TR_SLEEP,   // task to sleep
  /* 6 */ TR_WAIT,    // task to wait
  /* 8 */ TR_YIELD,   // task to yield
  /* 8 */ TR_ENUMS
};
  
static Stz* aTR[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "BIDE"_Stz,
  /* 2 */ "DELETE"_Stz,
  /* 3 */ "EXECUTE"_Stz,
  /* 4 */ "PAUSE"_Stz,
  /* 5 */ "SIGNAL"_Stz,
  /* 6 */ "SLEEP"_Stz,
  /* 7 */ "WAIT"_Stz,
  /* 8 */ "YIELD"_Stz,
  /* 9 */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
