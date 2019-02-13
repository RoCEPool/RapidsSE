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
  /*    */ AN_ADDED_ORIGINATOR,
  /*    */ AN_ADDED_PARTICIPANT,
  /*    */ AN_AM_DECIDER,
  /*    */ AN_CLOSE_FILE,
  /*    */ AN_CONSTRUCTOR,
  /*    */ AN_DESTRUCTOR,
  /*    */ AN_JOB_BEGIN,
  /*    */ AN_JOB_CHUNK,
  /*    */ AN_JOB_END,
  /*    */ AN_OPEN_FILE,
  /*    */ AN_POST_PROMPT,
  /*    */ AN_POST_RECV,
  /*    */ AN_PRE_PROMPT,
  /*    */ AN_PRE_SEND,
  /*    */ AN_RECV_JOB,
  /*    */ AN_RECV_JOB_LOW_MEMORY,
  /*    */ AN_REMOVED_ORIGINATOR,
  /*    */ AN_REMOVED_PARTICIPANT,
  /*    */ AN_ROOT,
  /*    */ AN_SEED_CAN,
  /*    */ AN_SEED_END,
  /*    */ AN_SEED_EXE,
  /*    */ AN_SEED_RCV,
  /*    */ AN_SEED_REQ,
  /*    */ AN_SEED_RUN,
  /*    */ AN_SEND_JOB,
  /*    */ AN_SET_ACKS_REQUIRED,
  /*    */ AN_TASK_LOCKED,
  /*    */ AN_TASK_SCHEDULE,
  /*    */ AN_TASK_SLEEP,
  /*    */ AN_TASK_STATE,
  /*    */ AN_TASK_STATE_SET,
  /*    */ AN_TASK_WAIT,
  /*    */ AN_TASK_YIELD,

  /*    */ AN_ENUMS,
  };
  
  static Stz* aAN[]
  {
  /*  0 */ "NONE"_Stz,

  /*    */ "ACK_TASK"_Stz,
  /*    */ "ADDED_ORIGINATOR"_Stz,
  /*    */ "ADDED_PARTICIPANT"_Stz,
  /*    */ "AM_DECIDER"_Stz,
  /*    */ "CLOSE_FILE"_Stz,
  /*    */ "CONSTRUCTOR"_Stz,
  /*    */ "DESTRUCTOR"_Stz,
  /*    */ "JOB_BEGIN"_Stz,
  /*    */ "JOB_CHUNK"_Stz,
  /*    */ "JOB_END"_Stz,
  /*    */ "OPEN_FILE"_Stz,
  /*    */ "POST_PROMPT"_Stz,
  /*    */ "POST_RECV"_Stz,
  /*    */ "PRE_PROMPT"_Stz,
  /*    */ "PRE_SEND"_Stz,
  /*    */ "RECV_JOB"_Stz,
  /*    */ "RECV_JOB_LOW_MEMORY"_Stz,
  /*    */ "REMOVED_ORIGINATOR"_Stz,
  /*    */ "REMOVED_PARTICIPANT"_Stz,
  /*    */ "ROOT"_Stz,
  /*    */ "SEED_CAN"_Stz,
  /*    */ "SEED_END"_Stz,
  /*    */ "SEED_EXE"_Stz,
  /*    */ "SEED_RCV"_Stz,
  /*    */ "SEED_REQ"_Stz,
  /*    */ "SEED_RUN"_Stz,
  /*    */ "SEND_JOB"_Stz,
  /*    */ "SET_ACKS_REQUIRED"_Stz,
  /*    */ "TASK_LOCKED"_Stz,
  /*    */ "TASK_SCHEDULE"_Stz,
  /*    */ "TASK_SLEEP"_Stz,
  /*    */ "TASK_STATE"_Stz,
  /*    */ "TASK_STATE_SET"_Stz,
  /*    */ "TASK_WAIT"_Stz,
  /*    */ "TASK_YIELD"_Stz,

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
  
    char  m_caText[128];

    Link_t<i8> m_wLink;
  
    // must be last
    Footer m_xFooter;
    
  public:
    Action();
    ~Action() noexcept(false);
  
    void vInit(i8 i_Action);
    bool bIsRoot(void);
    void vReset(void);
    void vWrite(File* pFile);
    void vWrite_Default(File* pFile);

    Footer* aFooter(void);
  };
  
  Action* _pAction(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
