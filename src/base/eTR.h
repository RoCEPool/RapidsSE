/*============================================================================*/
/* eTR.h -- Transition Request                                                */
/*============================================================================*/
#pragma once

enum eTR // Transition Request
{
  /* 0 */ TR_NONE=0,
  
  /* 1 */ TR_BIDE_FOR_OPERAND,   // bide for an operand (data arrival)
  /* 2 */ TR_DELETE_THIS_TASK,   // task to be deleted
  /* 4 */ TR_SLEEP_SOME_TIME,    // task to sleep (time arrival)
  /* 5 */ TR_WAIT_FOR_RESUME,    // wait for a resume from another tsk
  /* 6 */ TR_YIELD_FOR_OTHERS,   // yield so as to give tasks a thread
  
  /* 7 */ TR_ENUMS
};
  
static Stz* aTR[]
{
  /* 0 */ "TR_NONE"_Stz,
  
  /* 1 */ "TR_BIDE_FOR_OPERAND"_Stz,
  /* 2 */ "TR_DELETE_THIS_TASK"_Stz,
  /* 4 */ "TR_SLEEP_SOME_TIME"_Stz,
  /* 5 */ "TR_WAIT_FOR_RESUME"_Stz,
  /* 6 */ "TR_YIELD_FOR_OTHERS"_Stz,
  
  /* 7 */ "TR_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
