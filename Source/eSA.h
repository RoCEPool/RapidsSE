/*============================================================================*/
/* eSA.h -- Scheduler Action                                                  */
/*============================================================================*/
#pragma once

enum eSA // Scheduler Action
{
  /* 0 */ SA_NONE=0,
  /* 1 */ SA_BIDE,    // task to bide (for an operand (data arrival))
  /* 2 */ SA_DELETE,  // task to be deleted
  /* 3 */ SA_EXECUTE, // task to execute
  /* 4 */ SA_PAUSE,   // task to pause
  /* 5 */ SA_SIGNAL,  // task to signal waiter
  /* 6 */ SA_SLEEP,   // task to sleep (time arrival)
  /* 6 */ SA_WAIT,    // task to wait (for a signal (signal arrival))
  /* 8 */ SA_YIELD,   // task to yield (move back to ready queue (nice))
  /* 8 */ SA_ENUMS
};
  
static Stz* aSA[]
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
