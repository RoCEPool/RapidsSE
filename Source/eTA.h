/*============================================================================*/
/* eTA.h -- Task Actions                                                      */
/*============================================================================*/
#pragma once

enum eTA // task actions
{
  /*  0 */ TA_NONE=0,
  
  /*    */ TA_DDL_DROP_READ,
  /*    */ TA_DDL_DROP_WRITE,
  /*    */ TA_DDL_GRAB_READ,
  /*    */ TA_DDL_GRAB_WRITE,

  /*    */ TA_TSK_ABLE,
  /*    */ TA_TSK_ACK,
  /*    */ TA_TSK_ADD,
  /*    */ TA_TSK_BIDE,
  /*    */ TA_TSK_CONSTRUCT,
  /*    */ TA_TSK_DESIST,
  /*    */ TA_TSK_DESTRUCT,
  /*    */ TA_TSK_DROP,
  /*    */ TA_TSK_EXECUTE,
  /*    */ TA_TSK_EXIT,
  /*    */ TA_TSK_GRAB,
  /*    */ TA_TSK_INF,
  /*    */ TA_TSK_PAUSE,
  /*    */ TA_TSK_PULSE,
  /*    */ TA_TSK_READY,
  /*    */ TA_TSK_REMOVE,
  /*    */ TA_TSK_REQUIRES,
  /*    */ TA_TSK_RESUME,
  /*    */ TA_TSK_SCHEDULE,
  /*    */ TA_TSK_SLEEP,
  /*    */ TA_TSK_THREAD,
  /*    */ TA_TSK_TRIGGER,
  /*    */ TA_TSK_WAIT,
  /*    */ TA_TSK_WAKEUP,
  /*    */ TA_TSK_YIELD,
  
  /*    */ TA_TSK_ASSIGN_IDLE,
  /*    */ TA_TSK_SPAWN_DEDICATED,

  /*    */ TA_ENUMS,
  
};

static Stz* aTA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "TA_DDL_DROP_READ"_Stz,
  /*    */ "TA_DDL_DROP_WRITE"_Stz,
  /*    */ "TA_DDL_GRAB_READ"_Stz,
  /*    */ "TA_DDL_GRAB_WRITE"_Stz,

  /*    */ "TA_TSK_ABLE"_Stz,
  /*    */ "TA_TSK_ACK"_Stz,
  /*    */ "TA_TSK_ADD"_Stz,
  /*    */ "TA_TSK_BIDE"_Stz,
  /*    */ "TA_TSK_CONSTRUCT"_Stz,
  /*    */ "TA_TSK_DESIST"_Stz,
  /*    */ "TA_TSK_DESTRUCT"_Stz,
  /*    */ "TA_TSK_DROP"_Stz,
  /*    */ "TA_TSK_EXECUTE"_Stz,
  /*    */ "TA_TSK_EXIT"_Stz,
  /*    */ "TA_TSK_GRAB"_Stz,
  /*    */ "TA_TSK_INF"_Stz,
  /*    */ "TA_TSK_PAUSE"_Stz,
  /*    */ "TA_TSK_PULSE"_Stz,
  /*    */ "TA_TSK_READY"_Stz,
  /*    */ "TA_TSK_REMOVE"_Stz,
  /*    */ "TA_TSK_REQUIRES"_Stz,
  /*    */ "TA_TSK_RESUME"_Stz,
  /*    */ "TA_TSK_SCHEDULE"_Stz,
  /*    */ "TA_TSK_SLEEP"_Stz,
  /*    */ "TA_TSK_THREAD"_Stz,
  /*    */ "TA_TSK_TRIGGER"_Stz,
  /*    */ "TA_TSK_WAIT"_Stz,
  /*    */ "TA_TSK_WAKEUP"_Stz,
  /*    */ "TA_TSK_YIELD"_Stz,
  
  /*  0 */ "TA_TSK_ASSIGN_IDLE"_Stz,
  /*  0 */ "TA_TSK_SPAWN_DEDICATED"_Stz,

  
  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
