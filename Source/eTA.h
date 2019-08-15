/*============================================================================*/
/* eTA.h -- Task Actions                                                      */
/*============================================================================*/
#pragma once

enum eTA // task actions
{
  /*  0 */ TA_NONE=0,
  
  /*    */ TA_TASK_ABLE,
  /*    */ TA_TASK_ACK,
  /*    */ TA_TASK_ADD,
  /*    */ TA_TASK_BIDE,
  /*    */ TA_TASK_DESIST,
  /*    */ TA_TASK_EXECUTE,
  /*    */ TA_TASK_EXIT,
  /*    */ TA_TASK_INF,
  /*    */ TA_TASK_PAUSE,
  /*    */ TA_TASK_POP,
  /*    */ TA_TASK_PULSE,
  /*    */ TA_TASK_READY,
  /*    */ TA_TASK_REMOVE,
  /*    */ TA_TASK_RESUME,
  /*    */ TA_TASK_SCHEDULE,
  /*    */ TA_TASK_SLEEP,
  /*    */ TA_TASK_THREAD,
  /*    */ TA_TASK_TRIGGER,
  /*    */ TA_TASK_WAIT,
  /*    */ TA_TASK_WAKEUP,
  /*    */ TA_TASK_YIELD,

  /*    */ TA_ENUMS,
  
};

static Stz* aTA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "TA_TASK_ABLE"_Stz,
  /*    */ "TA_TASK_ACK"_Stz,
  /*    */ "TA_TASK_ADD"_Stz,
  /*    */ "TA_TASK_BIDE"_Stz,
  /*    */ "TA_TASK_DESIST"_Stz,
  /*    */ "TA_TASK_EXECUTE"_Stz,
  /*    */ "TA_TASK_EXIT"_Stz,
  /*    */ "TA_TASK_INF"_Stz,
  /*    */ "TA_TASK_PAUSE"_Stz,
  /*    */ "TA_TASK_POP"_Stz,
  /*    */ "TA_TASK_PULSE"_Stz,
  /*    */ "TA_TASK_READY"_Stz,
  /*    */ "TA_TASK_REMOVE"_Stz,
  /*    */ "TA_TASK_RESUME"_Stz,
  /*    */ "TA_TASK_SCHEDULE"_Stz,
  /*    */ "TA_TASK_SLEEP"_Stz,
  /*    */ "TA_TASK_THREAD"_Stz,
  /*    */ "TA_TASK_TRIGGER"_Stz,
  /*    */ "TA_TASK_WAIT"_Stz,
  /*    */ "TA_TASK_WAKEUP"_Stz,
  /*    */ "TA_TASK_YIELD"_Stz,
  
  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
