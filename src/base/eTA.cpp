/*============================================================================*/
/* eTA.cpp -- Task Actions                                                    */
/*============================================================================*/
#include "BASE.h"
#include "eTA.h"

Stz* aTA[TA_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "TA_DDL_READ_DROPPED"_Stz,
  /*    */ "TA_DDL_READ_GRAB"_Stz,
  /*    */ "TA_DDL_READ_GRABBED"_Stz,
  /*    */ "TA_DDL_WRITE_DROPPED"_Stz,
  /*    */ "TA_DDL_WRITE_GRAB"_Stz,
  /*    */ "TA_DDL_WRITE_GRABBED"_Stz,

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
  /*    */ "TA_TSK_GRABBED"_Stz,
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
  
  /*  0 */ "TA_TSK_ASSIGN_THREAD"_Stz,
  /*  0 */ "TA_TSK_ASSIGNED_THREAD"_Stz,
  /*  0 */ "TA_TSK_SIGNAL_THREAD"_Stz,
  /*  0 */ "TA_TSK_SPAWN_DEDICATED"_Stz,

//  /*    */ "ENUMS"_Stz,
};

Stz* aTA_short[TA_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "Ddl read dropped"_Stz,
  /*    */ "Ddl read grab"_Stz,
  /*    */ "Ddl read grabbed"_Stz,
  /*    */ "Ddl write dropped"_Stz,
  /*    */ "Ddl write grab"_Stz,
  /*    */ "Ddl write grabbed"_Stz,

  /*    */ "Task able"_Stz,
  /*    */ "Task ack"_Stz,
  /*    */ "Task add"_Stz,
  /*    */ "Task bide"_Stz,
  /*    */ "Task construct"_Stz,
  /*    */ "Task desist"_Stz,
  /*    */ "Task destruct"_Stz,
  /*    */ "Task drop"_Stz,
  /*    */ "Task execute"_Stz,
  /*    */ "Task exit"_Stz,
  /*    */ "Task grab"_Stz,
  /*    */ "Task grabbed"_Stz,
  /*    */ "Task inf"_Stz,
  /*    */ "Task pause"_Stz,
  /*    */ "Task pulse"_Stz,
  /*    */ "Task ready"_Stz,
  /*    */ "Task remove"_Stz,
  /*    */ "Task requires"_Stz,
  /*    */ "Task resume"_Stz,
  /*    */ "Task schedule"_Stz,
  /*    */ "Task sleep"_Stz,
  /*    */ "Task thread"_Stz,
  /*    */ "Task trigger"_Stz,
  /*    */ "Task wait"_Stz,
  /*    */ "Task wakeup"_Stz,
  /*    */ "Task yield"_Stz,
  
  /*  0 */ "Task assign thread"_Stz,
  /*  0 */ "Task assigned thread"_Stz,
  /*  0 */ "Task signals thread"_Stz,
  /*  0 */ "Task spawn dedicated"_Stz,
  
//  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
