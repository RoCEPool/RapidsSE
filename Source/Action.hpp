/*============================================================================*/
/* Action.hpp - when did an action occur and what happened                    */
/*============================================================================*/
#pragma once

namespace Maps
{
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
  /*    */ AN_CLOSE_FILE,
  /*    */ AN_CONSTRUCTOR,
  /*    */ AN_DDL_DROPPED_READ,
  /*    */ AN_DDL_DROPPED_WRITE,
  /*    */ AN_DDL_GRAB_READ,
  /*    */ AN_DDL_GRAB_WRITE,
  /*    */ AN_DDL_GRABBED_READ,
  /*    */ AN_DDL_GRABBED_WRITE,
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
  /*    */ AN_OPEN_FILE,
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
  
  static Stz* aAN[]
  {
  /*  0 */ "NONE"_Stz,

  /*    */ "ACK_TASK"_Stz,
  /*    */ "ACKS_REQUIRED"_Stz,
  /*    */ "ADD_EXECUTING_SET"_Stz,
  /*    */ "ADD_READY_QUEUE"_Stz,
  /*    */ "ADD_READY_QUEUE_LP"_Stz,
  /*    */ "ADD_SEND_QUEUE"_Stz,
  /*    */ "ADD_SLEEPING_QUEUE"_Stz,
  /*    */ "ADD_SLOT_QUEUE"_Stz,
  /*    */ "ADD_WAITING_SET"_Stz,
  /*    */ "ADDED_ORIGINATOR"_Stz,
  /*    */ "ADDED_PARTICIPANT"_Stz,
  /*    */ "AM_DECIDER"_Stz,
  /*    */ "CLOSE_FILE"_Stz,
  /*    */ "CONSTRUCTOR"_Stz,
  /*    */ "DDL_DROPPED_READ"_Stz,
  /*    */ "DDL_DROPPED_WRITE"_Stz,
  /*    */ "DDL_GRAB_READ"_Stz,
  /*    */ "DDL_GRAB_WRITE"_Stz,
  /*    */ "DDL_GRABBED_READ"_Stz,
  /*    */ "DDL_GRABBED_WRITE"_Stz,
  /*    */ "DESTRUCTOR"_Stz,
  /*    */ "END"_Stz,
  /*    */ "ERROR_CODE"_Stz,
  /*    */ "FIND_ORIGINATOR"_Stz,
  /*    */ "FIND_PARTICIPANT"_Stz,
  /*    */ "JOB_BEGIN"_Stz,
  /*    */ "JOB_CHUNK"_Stz,
  /*    */ "JOB_END"_Stz,
  /*    */ "JOB_PHASE"_Stz,
  /*    */ "NEW_JOB"_Stz,
  /*    */ "OPEN_FILE"_Stz,
  /*    */ "POST_PROMPT"_Stz,
  /*    */ "POST_RECV"_Stz,
  /*    */ "PRE_PROMPT"_Stz,
  /*    */ "PRE_SEND"_Stz,
  /*    */ "RECV_JOB"_Stz,
  /*    */ "RECV_JOB_LOW_MEMORY"_Stz,
  /*    */ "REMOVED_ORG"_Stz,
  /*    */ "REMOVED_PRT"_Stz,
  /*    */ "RESOURCES_CHECKED"_Stz,
  /*    */ "RESOURCES_RELEASED"_Stz,
  /*    */ "ROOT"_Stz,
  /*    */ "SEED_CAN"_Stz,
  /*    */ "SEED_END"_Stz,
  /*    */ "SEED_EXE"_Stz,
  /*    */ "SEED_RCV"_Stz,
  /*    */ "SEED_REQ"_Stz,
  /*    */ "SEED_RUN"_Stz,
  /*    */ "SEND_JOB"_Stz,
  /*    */ "SPAWN_JOB"_Stz,
  /*    */ "TABLE_USE"_Stz,
  /*    */ "TASK_ACKED"_Stz,
  /*    */ "TASK_LOCKED"_Stz,
  /*    */ "TASK_SLEEP"_Stz,
  /*    */ "TASK_STATE"_Stz,
  /*    */ "TASK_STATE_SET"_Stz,
  /*    */ "TASK_WAIT"_Stz,
  /*    */ "TASK_YIELD"_Stz,
  /*    */ "THREAD_EXECUTE"_Stz,

  /*    */ "ENUMS"_Stz,
  };

  class Action : public Object
  {
  friend class ActionSequence;
  
  private:
    i8    m_i_Action;
    i8    m_i_Root;
    i8    m_iTick;
  
    void* m_aJob;
    i8    m_iJobId;
    eCN   m_eClassNo;
    i8    m_iParentId;
    bool  m_bWasRecv;
    i8    m_i_Activity;

    void* m_aJob_Arg;
    i8    m_iJobId_Arg;
    eCN   m_eClassNo_Arg;
    i8    m_iParentId_Arg;
    bool  m_bWasRecv_Arg;
    i8    m_i_Activity_Arg;

    eAN   m_eAction;
    i8    m_iArg0;
    i8    m_iArg1;
  
    char  m_caName[32];
    char  m_caText[128];

    Link_t<i8> m_wLink;
  
    // must be last
    Footer m_xFooter;
    
  public:
    Action();
    ~Action() noexcept(false);
  
    i8   iDeltaNextUsec(void);
    void vInit(i8 i_Action);
    bool bIsRoot(void);
    Action* pNext(void);
    void vReset(void);
    void vWrite(File* pFile);
    void vWriteJob(File* pFile);
    void vWriteLead(File* pFile);
    void vWriteRefer(File* pFile);
    void vWriteSysJob(File* pFile);

    void vWrite_Default(File* pFile);
    void vWrite_Penum(File* pFile);
    void vWrite_P(File* pFile);
    void vWrite_Pi8(File* pFile);
    void vWrite_Pjob(File* pFile);
    void vWrite_Proot(File* pFile);
    void vWrite_Psysjob(File* pFile);
    void vWrite_Psysjob_enum(File* pFile);
    void vWrite_Psysjob_i8(File* pFile);
    void vWrite_Ptxt(File* pFile);
    void vWrite_Pwrd(File* pFile);

    Footer* aFooter(void);
  };
  
  Action* _pAction(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
