/*============================================================================*/
/* eHA.cpp -- Thread Actions                                                  */
/*============================================================================*/
#include "BASE.h"
#include "eHA.h"

Stz* aHA[HA_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  
  /*  0 */ "HA_THD_ADD_DEDICATED"_Stz,
  /*  0 */ "HA_THD_ADD_GENERAL"_Stz,
  /*  0 */ "HA_THD_APPEND_IDLE"_Stz,
  /*  0 */ "HA_THD_BEGIN_LOOP"_Stz,
  /*  0 */ "HA_THD_CONSTRUCT"_Stz,
  /*  0 */ "HA_THD_CONTINUE_TASK"_Stz,
  /*  0 */ "HA_THD_DESTRUCT"_Stz,
  /*  0 */ "HA_THD_END_LOOP"_Stz,
  /*  0 */ "HA_THD_EXECUTING"_Stz,
  /*  0 */ "HA_THD_EXECUTE_DEDICATED"_Stz,
  /*  0 */ "HA_THD_GET_TASK"_Stz,
  /*  0 */ "HA_THD_INF"_Stz,
  /*  0 */ "HA_THD_REMOVE_DEDICATED"_Stz,
  /*  0 */ "HA_THD_REMOVE_GENERAL"_Stz,
  /*  0 */ "HA_THD_UNASSIGN_TASK"_Stz,
  /*  0 */ "HA_THD_WAIT_ASSIGN"_Stz,

  /*  0 */ "HA_THD_POP_READY"_Stz,
  
//  /*    */ "ENUMS"_Stz,
};

Stz* aHA_short[HA_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "Thread add dedicated"_Stz,
  /*    */ "Thread add general"_Stz,
  /*    */ "Thread append idle queue"_Stz,
  /*    */ "Thread begin loop"_Stz,
  /*    */ "Thread construct"_Stz,
  /*    */ "Thread continue same task"_Stz,
  /*    */ "Thread destruct"_Stz,
  /*    */ "Thread end loop"_Stz,
  /*    */ "Thread executing task"_Stz,
  /*    */ "Thread execute dedicated task"_Stz,
  /*    */ "Thread get task assignment"_Stz,
  /*    */ "Thread information"_Stz,
  /*    */ "Thread remove dedicated"_Stz,
  /*    */ "Thread remove general"_Stz,
  /*    */ "Thread unassign task"_Stz,
  /*    */ "Thread wait assignment"_Stz,

  /*  0 */ "Thread pops ready task"_Stz,

//  /*    */ "ENUMS"_Stz,
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
