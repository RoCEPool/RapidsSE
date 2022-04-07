/*============================================================================*/
/* eTR.h -- Transition Request                                                */
/*============================================================================*/
#pragma once

enum eTR // Transition Request
{
  /* 0 */ TR_NONE=0,
  
  /* 1 */ TR_BIDE_FOR_OPERAND,   // bide for an operand (data arrival)
  /* 2 */ TR_DELETE_THIS_TASK,   // task to be deleted
  /* 3 */ TR_HIBERNATE_THIS_TASK,// task to be hibernated
  /* 4 */ TR_RETIRE_THIS_TASK,   // task to retired
  /* 5 */ TR_SLEEP_SOME_TIME,    // task to sleep (time arrival)
  /* 6 */ TR_WAIT_FOR_RESUME,    // wait for a resume from another tsk
  /* 7 */ TR_YIELD_FOR_OTHERS,   // yield so as to give tasks a thread
  
  /* 8 */ TR_ENUMS
};
  
static Stz* aTR[]
{
  /* 0 */ "TR_NONE"_Stz,
  
  /* 1 */ "TR_BIDE"_Stz,
  /* 2 */ "TR_DELETE"_Stz,
  /* 3 */ "TR_HIBERNATE"_Stz,
  /* 4 */ "TR_RETIRE"_Stz,
  /* 5 */ "TR_SLEEP"_Stz,
  /* 6 */ "TR_WAIT"_Stz,
  /* 7 */ "TR_YIELD"_Stz,
  
  /* 8 */ "TR_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
