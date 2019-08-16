/*============================================================================*/
/* eAN.h -- Action Numbers                                                    */
/*============================================================================*/
#pragma once

enum eAN // action numbers
{
/*  0 */ AN_NONE=0,

/*    */ AN_ACK_TASK,
/*    */ AN_ACKS_REQUIRED,
/*    */ AN_ADD_EXECUTING_SET,
/*    */ AN_ADD_READY_QUEUE,
/*    */ AN_ADD_READY_QUEUE_LP,
/*    */ AN_ADD_SEND_QUEUE,
/*    */ AN_ADD_SLEEPING_QUEUE,
/*    */ AN_ADD_SLOT_QUEUE,
/*    */ AN_ADD_WAITING_SET,
/*    */ AN_ADDED_ORIGINATOR,
/*    */ AN_ADDED_PARTICIPANT,
/*    */ AN_AM_DECIDER,
/*    */ AN_CONSTRUCTOR,
/*    */ AN_DESTRUCTOR,
/*    */ AN_END,
/*    */ AN_ERROR_CODE,
/*    */ AN_FIND_ORIGINATOR,
/*    */ AN_FIND_PARTICIPANT,
/*    */ AN_JOB_BEGIN,
/*    */ AN_JOB_CHUNK,
/*    */ AN_JOB_END,
/*    */ AN_JOB_PHASE,
/*    */ AN_NEW_JOB,
/*    */ AN_POST_PROMPT,
/*    */ AN_POST_RECV,
/*    */ AN_PRE_PROMPT,
/*    */ AN_PRE_SEND,
/*    */ AN_RECV_JOB,
/*    */ AN_RECV_JOB_LOW_MEMORY,
/*    */ AN_REMOVED_ORIGINATOR,
/*    */ AN_REMOVED_PARTICIPANT,
/*    */ AN_RESOURCES_CHECKED,
/*    */ AN_RESOURCES_RELEASED,
/*    */ AN_ROOT,
/*    */ AN_SEED_CAN,
/*    */ AN_SEED_END,
/*    */ AN_SEED_EXE,
/*    */ AN_SEED_RCV,
/*    */ AN_SEED_REQ,
/*    */ AN_SEED_RUN,
/*    */ AN_SEND_JOB,
/*    */ AN_SPAWN_JOB,
/*    */ AN_TABLE_USE,
/*    */ AN_TASK_ACKED,
/*    */ AN_TASK_LOCKED,
/*    */ AN_TASK_SLEEP,
/*    */ AN_TASK_STATE,
/*    */ AN_TASK_STATE_SET,
/*    */ AN_TASK_WAIT,
/*    */ AN_TASK_YIELD,
/*    */ AN_THREAD_EXECUTE,

/*    */ AN_ENUMS,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
