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
  /* 4 */ SA_SIGNAL,  // task to signal waiter
  /* 5 */ SA_SLEEP,   // task to sleep (time arrival)
  /* 6 */ SA_WAIT,    // task to wait (for a signal (signal arrival))
  /* 7 */ SA_YIELD,   // task to yield (move back to ready queue (nice))
  /* 8 */ SA_ENUMS
};
  
static Stz* aSA[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "BIDE"_Stz,
  /* 2 */ "DELETE"_Stz,
  /* 3 */ "EXECUTE"_Stz,
  /* 4 */ "SIGNAL"_Stz,
  /* 5 */ "SLEEP"_Stz,
  /* 6 */ "WAIT"_Stz,
  /* 7 */ "YIELD"_Stz,
  /* 8 */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
