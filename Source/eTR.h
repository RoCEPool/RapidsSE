/*============================================================================*/
/* eTR.h -- Task State Transition                                             */
/*============================================================================*/
#pragma once

namespace Tasks
{  
  enum eTR // Task State Transition
  {
  /* 0 */ TR_NONE=0,
  /* 1 */ TR_BIDE,    // task to bide
  /* 2 */ TR_EXECUTE, // task to execute
  /* 3 */ TR_EXIT,    // task to exit
  /* 4 */ TR_PAUSE,   // task to pause
  /* 5 */ TR_SLEEP,   // task to sleep
  /* 6 */ TR_WAIT,    // task to wait
  /* 7 */ TR_YIELD,   // task to yield
  /* 8 */ TR_ENUMS
  };
  
  static Stz* aTR[]
  {
  /* 0 */ "NONE"_Stz,
  /* 1 */ "BIDE"_Stz,
  /* 2 */ "EXECUTE"_Stz,
  /* 3 */ "EXIT"_Stz,
  /* 4 */ "PAUSE"_Stz,
  /* 5 */ "SLEEP"_Stz,
  /* 6 */ "WAIT"_Stz,
  /* 7 */ "YIELD"_Stz,
  /* 8 */ "ENUMS"_Stz,
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
