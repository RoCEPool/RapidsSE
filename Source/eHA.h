/*============================================================================*/
/* eHA.h -- Thread Actions                                                    */
/*============================================================================*/
#pragma once

enum eHA // thread actions
{
  /*  0 */ HA_NONE=0,
  
  /*    */ HA_THD_ADD_DEDICATED,
  /*    */ HA_THD_ADD_GENERAL,
  /*    */ HA_THD_APPEND_IDLE,
  /*    */ HA_THD_BEGIN_LOOP,
  /*    */ HA_THD_CONSTRUCT,
  /*    */ HA_THD_CONTINUE_TASK,
  /*    */ HA_THD_DESTRUCT,
  /*    */ HA_THD_END_LOOP,
  /*    */ HA_THD_EXECUTING,
  /*    */ HA_THD_EXECUTE_DEDICATED,
  /*    */ HA_THD_GET_TASK,
  /*    */ HA_THD_INF,
  /*    */ HA_THD_REMOVE_DEDICATED,
  /*    */ HA_THD_REMOVE_GENERAL,
  /*    */ HA_THD_UNASSIGN_TASK,
  /*    */ HA_THD_WAIT_ASSIGN,

  /*    */ HA_THD_POP_READY,

  /*    */ HA_ENUMS,
};

static Stz* aHA[]
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
  
  /*    */ "ENUMS"_Stz,
};

static Stz* aHA_short[]
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

  /*    */ "ENUMS"_Stz,
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
