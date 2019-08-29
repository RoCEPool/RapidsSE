/*============================================================================*/
/* eAN.h -- Action Numbers                                                    */
/*============================================================================*/
#pragma once

enum eAN // action numbers
{
/*  0 */ AN_NONE=0,

/*    */ AN_CONSTRUCT,
/*    */ AN_DESTRUCT,
/*    */ AN_ERROR_CODE,
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
/*    */ AN_RESOURCES_CHECKED,
/*    */ AN_RESOURCES_RELEASED,
/*    */ AN_SEED_CAN,
/*    */ AN_SEED_END,
/*    */ AN_SEED_EXE,
/*    */ AN_SEED_RCV,
/*    */ AN_SEED_REQ,
/*    */ AN_SEED_RUN,
/*    */ AN_SEND_JOB,
/*    */ AN_SPAWN_JOB,

/*    */ AN_ENUMS,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
