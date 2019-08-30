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
  /*    */ HA_THD_CONTINUE_SAME,
  /*    */ HA_THD_DESTRUCT,
  /*    */ HA_THD_END_LOOP,
  /*    */ HA_THD_EXECUTE_DEDICATED,
  /*    */ HA_THD_GET_ASSIGN,
  /*    */ HA_THD_INF,
  /*    */ HA_THD_REMOVE_DEDICATED,
  /*    */ HA_THD_REMOVE_GENERAL,
  /*    */ HA_THD_UNASSIGN_TASK,
  
  /*    */ HA_THD_POP,

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
  /*  0 */ "HA_THD_CONTINUE_SAME"_Stz,
  /*  0 */ "HA_THD_DESTRUCT"_Stz,
  /*  0 */ "HA_THD_END_LOOP"_Stz,
  /*  0 */ "HA_THD_EXECUTE_DEDICATED"_Stz,
  /*  0 */ "HA_THD_GET_ASSIGN"_Stz,
  /*  0 */ "HA_THD_INF"_Stz,
  /*  0 */ "HA_THD_REMOVE_DEDICATED"_Stz,
  /*  0 */ "HA_THD_REMOVE_GENERAL"_Stz,
  /*  0 */ "HA_THD_UNASSIGN_TASK"_Stz,

  /*  0 */ "HA_THD_POP"_Stz,
  
  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
