/*============================================================================*/
/* eHA.h -- Thread Actions                                                    */
/*============================================================================*/
#pragma once

enum eHA // thread actions
{
  /*  0 */ HA_NONE=0,
  
  /*    */ HA_THREAD_ADD_DEDICATED,
  /*    */ HA_THREAD_ADD_GENERAL,
  /*    */ HA_THREAD_APPEND_IDLE,
  /*    */ HA_THREAD_BEGIN_LOOP,
  /*    */ HA_THREAD_CONSTRUCTOR,
  /*    */ HA_THREAD_CONTINUE_SAME,
  /*    */ HA_THREAD_DESTRUCTOR,
  /*    */ HA_THREAD_END_LOOP,
  /*    */ HA_THREAD_EXECUTE_DEDICATED,
  /*    */ HA_THREAD_INF,
  /*    */ HA_THREAD_REMOVE_DEDICATED,
  /*    */ HA_THREAD_REMOVE_GENERAL,
  /*    */ HA_THREAD_UNASSIGN_TASK,
  /*    */ HA_THREAD_WAIT_ASSIGN,
  
  /*    */ HA_THREAD_POP,

  /*    */ HA_ENUMS,
};

static Stz* aHA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  0 */ "HA_THREAD_ADD_DEDICATED"_Stz,
  /*  0 */ "HA_THREAD_ADD_GENERAL"_Stz,
  /*  0 */ "HA_THREAD_APPEND_IDLE"_Stz,
  /*  0 */ "HA_THREAD_BEGIN_LOOP"_Stz,
  /*  0 */ "HA_THREAD_CONSTRUCTOR"_Stz,
  /*  0 */ "HA_THREAD_CONTINUE_SAME"_Stz,
  /*  0 */ "HA_THREAD_DESTRUCTOR"_Stz,
  /*  0 */ "HA_THREAD_END_LOOP"_Stz,
  /*  0 */ "HA_THREAD_EXECUTE_DEDICATED"_Stz,
  /*  0 */ "HA_THREAD_INF"_Stz,
  /*  0 */ "HA_THREAD_REMOVE_DEDICATED"_Stz,
  /*  0 */ "HA_THREAD_REMOVE_GENERAL"_Stz,
  /*  0 */ "HA_THREAD_UNASSIGN_TASK"_Stz,
  /*  0 */ "HA_THREAD_WAIT_ASSIGN"_Stz,

  /*  0 */ "HA_THREAD_POP"_Stz,
  
  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
