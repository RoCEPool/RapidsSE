/*============================================================================*/
/* eTA.h -- Task Actions                                                      */
/*============================================================================*/
#pragma once

enum eTA // task actions
{
  /*  0 */ TA_NONE=0,
  
  /*    */ TA_DDL_READ_DROPPED,
  /*    */ TA_DDL_READ_GRAB,
  /*    */ TA_DDL_READ_GRABBED,
  /*    */ TA_DDL_WRITE_DROPPED,
  /*    */ TA_DDL_WRITE_GRAB,
  /*    */ TA_DDL_WRITE_GRABBED,

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
  /*    */ TA_TSK_GRABBED,
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
  
  /*    */ TA_TSK_ASSIGN_THREAD,
  /*    */ TA_TSK_ASSIGNED_THREAD,
  /*    */ TA_TSK_SIGNAL_THREAD,
  /*    */ TA_TSK_SPAWN_DEDICATED,

  /*    */ TA_ENUMS,
  
};

extern Stz* aTA[TA_ENUMS];
extern Stz* aTA_short[TA_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
