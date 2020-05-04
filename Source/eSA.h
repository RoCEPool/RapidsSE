/*============================================================================*/
/* eSA.h -- Scheduler Action                                                  */
/*============================================================================*/
#pragma once

enum eSA // Scheduler Action
{
  /* 0 */ SA_NONE=0,
  
  /* 1 */ SA_BIDE_FOR_OPERAND,   // bide for an operand (data arrival)
  /* 2 */ SA_DELETE_THIS_TASK,   // task to be deleted
  /* 3 */ SA_SIGNAL_COMPLETION,  // signal cond-waiting thread & task completes
  /* 4 */ SA_SLEEP_SOME_TIME,    // task to sleep (time arrival)
  /* 5 */ SA_WAIT_FOR_RESUME,    // wait for a resume from another tsk
  /* 6 */ SA_YIELD_FOR_OTHERS,   // yield so as to give tasks a thread
  
  /* 7 */ SA_ENUMS
};
  
static Stz* aSA[]
{
  /* 0 */ "SA_NONE"_Stz,
  
  /* 1 */ "SA_BIDE_FOR_OPERAND"_Stz,
  /* 2 */ "SA_DELETE_THIS_TASK"_Stz,
  /* 3 */ "SA_SIGNAL_COMPLETION"_Stz,
  /* 4 */ "SA_SLEEP_SOME_TIME"_Stz,
  /* 5 */ "SA_WAIT_FOR_RESUME"_Stz,
  /* 6 */ "SA_YIELD_FOR_OTHERS"_Stz,
  
  /* 7 */ "SA_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
