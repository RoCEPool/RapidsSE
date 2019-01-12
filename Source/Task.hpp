/*============================================================================*/
/* Task.hpp - task                                                            */
/*============================================================================*/
#pragma once

extern pthread_key_t  giTLS_TaskKey;

namespace Interface
{
  class Session;
}

namespace GridModel
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
  enum eTS // Task State
  {
  /* 0 */ TS_NONE=0,
  /* 1 */ TS_EXECUTING, // task is currenty being executed by Thread::vLoop
  /* 2 */ TS_EXITED,    // task has completed
  /* 3 */ TS_READY,     // task is ready to be executed
  /* 4 */ TS_SEND,      // task is wants to send
  /* 5 */ TS_SLEEPING,  // task is sleeping
  /* 6 */ TS_WAITING,   // task is waiting on ack from cooperating task
  /* 7 */ TS_YIELDED,   // task is yielded, not running and is in a TaskSet
  /* 8 */ TS_ENUMS
  };
  
  static Stz* aTS[]
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
  
  typedef NP<Task,CN_TASK> NP_Task;
  
  class Task : public Object,
               public ListNode_t<Task>
  {
  friend class Steps::Step;
  friend class Word;

  private:
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // keep

    Job*    m_pJob;         // the job for this task

    Threads::Thread* m_pThread; // current thread
  
    eTS     m_eState;       // state (running, sleeping, ...)
    eQR     m_eQueue;       // which queue the task is in
    i8      m_iQueueOrder;  // queue ordering number
    Lock    m_oLock;        // lock for this task
    Decider m_oAcksRemaining; // number of acks still waiting for
    Since   m_oWakeupTime;  // wakeup time

    i8      m_iPartition;   // which partition if a partition task
    i8      m_iDdlLocked;   // ? has the ddl lock?
    i8      m_iTracing;     // tracing mask

    GridModel::Node* m_pSendNode; // node to send to

    Stack*   m_pDataStack;   // data stack
    Stack*   m_pReturnStack; // return stack
    Stack*   m_pAuxStack;    // auxiliary stack
    DicStack m_oPathStack;   // dictionary stack
    
    RegFrame* m_pLocalsFrame; // local register frame chain

    union {                 // forth instruction pointer
      Cell*   m_aCell;      // points to cell inside a word
      Value*  m_aEmbValue;  // points to an embedded value within a word
    } FIP;
    StepFp* m_pEntryCell; // points to does arg (entry cell of word)
  
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
    
    bool bAckAndDecide(void);
    void vAdjustNext(i8 iBytes);
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
    void vNewStacks(void);
    i8   iPartition(void);
    void vPerformancePoint(Stz* aWordName, Stz* aPointNote);
    void vPerformancePrint(void);
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
    GridModel::Node* pSendNode(void);
    Interface::Session* pSession(void);
    void vSession(Interface::Session* pSession);
    void vSleepUs(i8 iMicroSeconds);
    i8   iSlot(void);
    void vSprint(s48K& sFormat, s48K& sString);
    void vStackContents(Stack* pStack, s48K& sStackContents);
    eTS  eState(void);
    void vState(eTS eState);
    void vTaskLoop(void);
    Threads::Thread* pThread(void);
    void vThread(Threads::Thread* pThread);
    void vTracePoint(Stz* aMark);
    void vTraceStep(Stz* aStepNm);
    bool bWakeupNow(void);
    i8   iWakeupTime(void);
    Since& oWakeupTime(void);
    void vWakeupTask(void);
    eQR  eWhichQueue(void);
    void vWriteMark(Stz* aPrefix);

    /*------------------------------------------------------------------------*/

    void vAddBaseWords(void);
    void vAddMunged(MP_Task mpTask);
    void vAddRoutines(void);
    Word* pCompileColon(s48K& sDefinition);
    void  vCompileConstant(s48K& sDefinition);

    /*------------------------------------------------------------------------*/

  public:
    static bool   bAppendRow(FetchBuffer* pFetchBuffer, Entry* pEntry, Job_FetchRows* pJob_FetchRows);
    
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
