/*============================================================================*/
/* Task.hpp - task                                                            */
/*============================================================================*/
#pragma once

extern pthread_key_t  giTLS_TaskKey;

namespace Interface
{
  class Session;
}

namespace Grids
{
  class Node;
}

namespace Threads
{
  class Thread;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
namespace Tasks
{  
  enum eTK // Task State
  {
  /* 0 */ TK_NONE=0,
  /* 1 */ TK_EXECUTING, // task is currenty being executed by Thread::vLoop
  /* 2 */ TK_EXITED,    // task has completed
  /* 3 */ TK_READY,     // task is ready to be executed
  /* 4 */ TK_SEND,      // task is wants to send
  /* 5 */ TK_SLEEPING,  // task is sleeping
  /* 6 */ TK_WAITING,   // task is waiting on ack from cooperating task
  /* 7 */ TK_YIELDED,   // task is yielded, not running and is in a TaskSet
  /* 8 */ TK_ENUMS
  };
  
  static Stz* aTK[]
  {
    /* 0 */ "NONE"_Stz,
    /* 1 */ "EXECUTING"_Stz,
    /* 2 */ "EXITED"_Stz,
    /* 3 */ "READY"_Stz,
    /* 4 */ "SEND"_Stz,
    /* 5 */ "SLEEPING"_Stz,
    /* 6 */ "WAITING"_Stz,
    /* 7 */ "YIELDED"_Stz,
    /* 8 */ "ENUMS"_Stz,
  };
  
  class Task : public Object,
               public ListNode_t<Task>
  {
  friend class Steps::Step;
  friend class ForthWord;

  private:
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // keep

    Job*    m_pJob;         // the job for this task

    Threads::Thread* m_pThread; // current thread
  
    eTK     m_eState;       // state (running, sleeping, ...)
    eQR     m_eQueue;       // which queue the task is in
    i8      m_iQueueOrder;  // queue ordering number
    Lock    m_okLock;       // lock for this task
    Decider m_oAcksRemaining; // number of acks still waiting for
    Since   m_oWakeupTime;  // wakeup time

    i8      m_iPartition;   // which partition if a partition task
    i8      m_iDdlLocked;   // ? has the ddl lock?
    i8      m_iTracing;     // tracing mask
    i8      m_i_ActionRoot; // action root
    i8      m_i_NextRnd;    // which next repeatable random nummber?

    Grids::Node* m_pSendNode; // node to send to

    Stack*   m_pDataStack;   // data stack
    Stack*   m_pReturnStack; // return stack
    Stack*   m_pAuxStack;    // auxiliary stack
    DicStack m_oDicStack;    // dictionary stack
    
    LocalSet* m_pLocalsFrame; // local register frame chain

    union {                 // forth instruction pointer
      Cell*   m_aCell;      // points to cell inside a word
    } FIP;
  
    // exception
    s256  m_sFile;
    s256  m_sException;

    Compiler* m_pCompiler;  // word compiler
  
    s256  m_sPortrait;      // short text description of task
   
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // remove
  
    Interface::Session* m_pSession; // the session for this task

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // always last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Task(Job* pJob);
    ~Task(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static Task* pExecutingTask(void);
    static void vMonitorProfile_stats(s48K& sPortrait);
  
    bool bAckAndDecide(void);
    void vAdjustFIP(i8 nBytes);
    Object* pArgObject(void);
    static void vDebug(void);
    void vContentDump(void);
    void vDropLock(void);
    void vException(s256& sFile, s256& eException);
    void vGrabLock(void);
    bool bIsLocked(void);
    bool bIsNonPartitioned(void);
    bool bIsPartitioned(void);
    Job* pJob(void);
    void vJob(Job* pJob);
    Stz* aJobName(void);
    void vLocalsPop(void);
    void vLocalsPush(void);
    void vMonitorPortrait(s48K& sPortrait);
    void vNewStacks(void);
    i8&  i_NextRnd(void);
    i8   iPartition(void);
    s256& sPortrait(void);
    eQR  eQueue(void);
    void vQueue(eQR eQueue);
    void vQueueOrder(i8 iTaskOrder);
    i8   iQueueOrder(void);
    void vRefindJobClass(void);
    void vRenderPortrait(void);
    void vRetry(void);
    bool bSeeded(void);
    void vSeedJob(void);
    Grids::Node* pSendNode(void);
    Interface::Session* pSession(void);
    void vSession(Interface::Session* pSession);
    void vSleepUsec(i8 iMicroSeconds);
    i8   iSlot(void);
    void vSprint(s48K& sFormat, s48K& sString);
    void vStackContents(Stack* pStack, s48K& sStackContents);
    eTK  eState(void);
    void vState(eTK eState);
    void vTaskLoop(void);
    Threads::Thread* pThread(void);
    void vThread(Threads::Thread* pThread);
    void vTracePoint(Stz* aMark);
    void vTraceStep(Stz* aStepNm);
    i8   iWakeupInUsec(void);
    bool bWakeupNow(void);
    i8   iWakeupTime(void);
    Since& oWakeupTime(void);
    void vWakeupTask(void);
    eQR  eWhichQueue(void);
                                              
    /*------------------------------------------------------------------------*/

    void vAddBaseWords(void);
    void vAddMunged(MP_Task mpTask);
    void vAddRoutines(void);
    ForthWord* pCompileColon(s48K& sDefinition);
    void  vCompileConstant(s48K& sDefinition);

    /*------------------------------------------------------------------------*/

  public:
    static bool   bAppendRow(FetchBuffer* pFetchBuffer, Item* aItem, Job_FetchRows* pJob_FetchRows);
    /*------------------------------------------------------------------------*/
    
  private:
    Footer* aFooter(void);

    /*------------------------------------------------------------------------*/
    
  };

  Task* _pTask(void* aAddr);
  Task* _pTask(i8 iAddr);
}

bool bTaskCompareQueueOrder(void* pObjectInsert, void* pObjectScan, bool& bMatch);
bool bTaskCompareWakeupTime(void* pObjectInsert, void* pObjectScan, bool& bMatch);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
