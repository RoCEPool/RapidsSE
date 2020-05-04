/*============================================================================*/
/* eSA.h -- Scheduler Action                                                  */
/*============================================================================*/
#pragma once

enum eSA // Scheduler Action
{
  /* 0 */ SA_NONE=0,
  /* 1 */ SA_BIDE,    // task to bide (for an operand (data arrival))
  /* 2 */ SA_DELETE,  // task to be deleted
  /* 3 */ SA_SIGNAL,  // task to signal waiter
  /* 4 */ SA_SLEEP,   // task to sleep (time arrival)
  /* 5 */ SA_WAIT,    // task to wait (for a signal (signal arrival))
  /* 6 */ SA_YIELD,   // task to yield (move back to ready queue (nice))
  /* 7 */ SA_ENUMS
};
  
static Stz* aSA[]
{
  /* 0 */ "SA_NONE"_Stz,
  /* 1 */ "SA_BIDE"_Stz,
  /* 2 */ "SA_DELETE"_Stz,
  /* 3 */ "SA_SIGNAL"_Stz,
  /* 4 */ "SA_SLEEP"_Stz,
  /* 5 */ "SA_WAIT"_Stz,
  /* 6 */ "SA_YIELD"_Stz,
  /* 7 */ "SA_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
