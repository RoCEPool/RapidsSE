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

extern Stz* aHA[HA_ENUMS];
extern Stz* aHA_short[HA_ENUMS];


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
