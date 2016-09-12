/*============================================================================*/
/* Task.hpp - task                                                            */
/*============================================================================*/

#pragma once

#include "Tasks.h"

extern pthread_key_t  giTLS_TaskKey;

namespace Interface
{
  class SEitf;
}

namespace Threads
{
  class Thread;
}

namespace Tasks
{
  enum eTS // Task State
  {
  /* 0 */ TS_NONE=0,
  /* 1 */ TS_RUNNING,
  /* 2 */ TS_SWITCH,
  /* 3 */ TS_WAITING,
  /* 4 */ TS_SLEEPING,
  /* 5 */ TS_BLOCKED,
  /* 6 */ TS_EXITED,
  /* 7 */ TS_ENUMS
  };
  
  static const char* const pTS[]
  {
    /* 0 */ "NONE",
    /* 1 */ "RUNNING",
    /* 2 */ "SWITCH",
    /* 3 */ "WAITING",
    /* 4 */ "SLEEPING",
    /* 5 */ "BLOCKED",
    /* 6 */ "EXITED",
    /* 7 */ "ENUMS",
  };
  
  typedef NP<Task,CN_TASK> NP_Task;
  typedef NNP<Task,CN_TASK> NNP_Task;

  class Task : public Object,
               public ListNode_t<Task>
  {
    #define GET_THROW(_1,_2,NAME,...) NAME
    
    #define THROW(...) GET_THROW(__VA_ARGS__, THROW2, THROW1)(__VA_ARGS__)
      
    #define THROW1(p1) \
    { \
      s256 sFile(__FILE__); \
      s256 sException; \
      String::vSprint(sException.a(),p1); \
      Task* pTask = reinterpret_cast<Task*>(PosixThread::pTlsGet(giTLS_TaskKey)); \
      pTask->vException(sFile,sException); \
      throw (i8)__LINE__; \
    }
      
    #define THROW2(p1, p2) \
    { \
      s256 sFile(__FILE__); \
      s256 sException; \
      String::vSprint(sException.a(),p1,p2); \
      Task* pTask = reinterpret_cast<Task*>(PosixThread::pTlsGet(giTLS_TaskKey)); \
      pTask->vException(sFile,sException); \
      throw (i8)__LINE__; \
    }
    
    // throw invalid parameter
    #define THROW_INVALID_PARAM(primitive) \
    { \
      THROW("Invalid param stack for: %s",primitive); \
    }

    #define ENTRY(p1) \
      static StepAccum* pStepAccum = nullptr; \
      if (! pStepAccum) \
        pStepAccum = gpStepArray->pAllocAccum(p1); \
      i8 _iTscEntry = iRdtscp(); \
      nnpTask->vPrimitive(p1);

    #define EXIT() \
      ASSERT(pStepAccum); \
      i8 _iTscExit = iRdtscp(); \
      i8 _iTscDiff = _iTscExit - _iTscEntry; \
      pStepAccum->m_nInvoked++; \
      pStepAccum->m_nCycles += _iTscDiff;
  
  private:
    Lock  m_xLock;
    i8    m_iTaskNo;    // task
    s32   m_sName;      // task name
    eTS   m_eState;     // state (running, sleeping, ...)
    i8    m_iTxId;      // transaction id
    i8    m_iWakeAt;    // time at which to wake this tx
    
    NP_Task m_npNextInSet; // next task in set
    NP_Task m_npPrevInSet; // prev task in set
    
    DictionaryStack m_xDictionaryStack;
    
    Queue* m_pQueue;        // processing queue
    
    Stack  m_xParamStack;   // parameter stack
    Stack  m_xReturnStack;  // return stack

    s256      m_sLambda;    // lambda input
    Tokeniser m_xTokeniser; // input stream tokenizer
    s256      m_sToken;     // current token
    eTK       m_eToken;     // token type
    
    union {
      Cell*       m_pCell;
      CodeField*  m_pCodeField;
      Value*      m_pValue;
    };
    
    
//  u8    m_uMarker1;
    Cell* m_pResetIP;
    Cell* m_pBeginIP;
    Word* m_pWord;
    bool  m_bCompiling;
    bool  m_bTracing;
    
    // exception
    s256  m_sFile;
    s256  m_sException;

    Interface::SEitf* m_pSEitf;
    
    // test
    Dictionary m_xTaskDictionary;
    
    // always last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void* operator new(size_t, Task* pTask);
    void  operator delete(void*);
    
    Task(void);
    Task(NNP_Task nnpTask);
    Task(const char* aLambda);
    void vInit(const char* aLambda);
    ~Task(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    void vAdjustNext(i8 iBytes);
    void vAddConstant(const char* aName, i8 iVal);
    void vAddConstant(const char* aName, const char* aValue);
    void vAddConstants(void);
    void vAddPrimitive(const char* aName, pFUN pFunc, bool bImmediate);
    void vAddPrimitives(void);
    void vAddRoutines(void);
    Object* pArgObject(void);
    void vCheckLocked(void);
    Word* pCompileColon(s& sDefinition);
    Word* pCurrentWord(void);
    static void vDebug(void);
    void vDump(void);
    void vException(s& sFile, s& eException);
    Word* pFindWord(s& sName);
    Word* pFindWord(const char* aName);
    bool  bForgetWord(s& sName);
    void vInitialiseDictionary(void);
    bool  bIsName(s& sName);
    Word* pLocateWord(Cell* pCell);
    void vLoop(void);
    NP_Task npNextInSet(void);
    s&   sName(void);
    void vName(s& sName);
    void vNextInSet(NP_Task npNext);
    void vSeedDicCompileSen(void);
    void vSeedDocol(void);
    void vSeedWord(const char *aWord);
    NP_Task npPrevInSet(void);
    void vPrevInSet(NP_Task npPrev);
    void vPrimitive(const char *aPrimitive);
    void vReset(void);
    void vRetry(void);
    Threads::Thread* pRootThread(void);
    void vRoutineCall(Word* pWord);
    eTS  eState(void);
    void vState(eTS eState);
    void vTokenNext(void);
    i8   iTxId(void);
    void vTxId(i8 iTxId);
    void vUndirect(void);
    
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/

    bool bGuardParam(i8 iCount);
    
    bool bGuardParam(eCN eClass0);
    bool bGuardParam(eCN eClass0, eCN eClass1);
    bool bGuardParam(eCN eClass0, eCN eClass1, eCN eClass2);
    
    bool bGuardReturn(i8 iCount);
    
    bool bGuardReturn(eCN eClass0);
    bool bGuardReturn(eCN eClass0, eCN eClass1);
    bool bGuardReturn(eCN eClass0, eCN eClass1, eCN eClass2);
    
    bool bPopParam(Cell*& pCell);
    bool bPopParam(i8& iArg1);
    bool bPopParam(i8& iArg1, i8& iArg2);
    bool bPopParam(i8& iArg1, Value& xArg1);
    bool bPopParam(i8& iArg1, s8K& sArg2);
    bool bPopParam(i8& iArg1, i8& iArg2, Value& xArg3);
    bool bPopParam(i8& iArg1, i8& iArg2, s8K& sArg3);
    bool bPopParam(Object*& pObject);
    bool bPopParam(Object*& pObject1, Object*& pObject2);
    bool bPopParam(Object*& pObject, Value& xValue);
    bool bPopParam(Value& xArg1);
    bool bPopParam(Value& xArg1, Value& xArg2);
    bool bPopParam(Value& xArg1, Value& xArg2, Value& xArg3);
    bool bPopParam(s32&  sArg1);
    bool bPopParam(s256& sArg1);
    bool bPopParam(s8K& sArg1);
    bool bPopParam(s8K& sArg1, s8K& sArg2);
    bool bPopParam(Task*& pTask);
    bool bPopParam(Word*& pArg1);
    bool bPopParamInt(i8& iArg1);

    void vPopParam(Value* pValue);
    
    void vPushParam(Cell* pCell);
    void vPushParam(const char* aString);
    void vPushParam(i8 iArg1);
    void vPushParam(Object* pObject);
    //    void vPushParam(Object* pObject);
    void vPushParam(Value& xArg1);
    void vPushParam(Value* pValue);
    void vPushParam(s32&  sArg1);
    void vPushParam(s256& sArg1);
    void vPushParam(s8K& sArg1);
    void vPushParam(Threads::Thread* pThread);
    void vPushParam(NNP_Task nnpTask);
    void vPushParam(Word* pWord);
    
    void vPushReturn(Cell* pCell);
    void vPushReturn(const char* aString);
    void vPushReturn(i8 iArg1);
    void vPushReturn(Value* pValue);
    void vPushReturn(Value& xValue);
    void vPushReturn(s32& sArg1);
    void vPushReturn(s256& sArg1);
    void vPushReturn(s8K& sArg1);

    void vPopReturn(Cell*& pCell);
    bool bPopReturn(i8& iArg1);
    bool bPopReturn(i8& iArg1, i8& iArg2);
    bool bPopReturn(Value& xValue);
    bool bPopReturn(Value& xValue1, Value& xValue2);
    void vPopReturn(Value* pValue);
    bool bPopReturn(s32&  sArg1);
    bool bPopReturn(s256& sArg1);
    bool bPopReturn(s8K& sArg1);
    
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    
    static void vAccept(NNP_Task nnpTask);
    static void vAdd(NNP_Task nnpTask);
    static void vAllot(NNP_Task nnpTask);          
    static void vAgain(NNP_Task nnpTask);          
    static void vAgain_(NNP_Task nnpTask);         
    static void vAnd(NNP_Task nnpTask);            
    static void vBegin(NNP_Task nnpTask);          
    static void vBegin_(NNP_Task nnpTask);         
    static void vBoot(NNP_Task nnpTask);           
    static void vBpa(NNP_Task nnpTask);
    static void vBpdump(NNP_Task nnpTask);
    static void vClassFetch(NNP_Task nnpTask);
    static void vClassStore(NNP_Task nnpTask);     
    static void vCodeAssign(NNP_Task nnpTask);
    static void vColon(NNP_Task nnpTask);          
    static void vComma(NNP_Task nnpTask);          
    static void vComment(NNP_Task nnpTask);        
    static void vCompiling(NNP_Task nnpTask);
    static void vCursorFetch(NNP_Task nnpTask);
    static void vCursorStore(NNP_Task nnpTask);
    static void vDivide(NNP_Task nnpTask);
    static void vDepth(NNP_Task nnpTask);          
    static void vDo(NNP_Task nnpTask);             
    static void vDo_RT(NNP_Task nnpTask);          
    static void vDrop(NNP_Task nnpTask);           
    static void vDup(NNP_Task nnpTask);            
    static void vDump(NNP_Task nnpTask);           
    static void vElse(NNP_Task nnpTask);           
    static void vEncodeCall(NNP_Task nnpTask);     
    static void vEncodeDoes(NNP_Task nnpTask);     
    static void vEncodeEmb(NNP_Task nnpTask);      
    static void vEncodeFun(NNP_Task nnpTask);      
    static void vEncodeVal(NNP_Task nnpTask);      
    static void vEnter(NNP_Task nnpTask);          
    static void vEnterCompiler(NNP_Task nnpTask);
    static void vEnterInterpreter(NNP_Task nnpTask);
    static void vEnvGet(NNP_Task nnpTask);
    static void vEqual(NNP_Task nnpTask);          
    static void vEqualAddress(NNP_Task nnpTask);
    static void vExecute(NNP_Task nnpTask);
    static void vExit(NNP_Task nnpTask);           
    static void vFetch(NNP_Task nnpTask);
    static void vFreePercent(NNP_Task nnpTask);
    static void vFreeSpace(NNP_Task nnpTask);
    static void vGreater(NNP_Task nnpTask);
    static void vGreaterEqual(NNP_Task nnpTask);   
    static void vHazard(NNP_Task nnpTask);
    static void vHash(NNP_Task nnpTask);
    static void vI(NNP_Task nnpTask);
    static void vIf(NNP_Task nnpTask);             
    static void vImmediate(NNP_Task nnpTask);      
    static void vIterate(NNP_Task nnpTask);
    static void vIterate_RT_A(NNP_Task nnpTask);
    static void vIterate_RT_B(NNP_Task nnpTask);
    static void vInterpreting(NNP_Task nnpTask);
    static void vInUseSpace(NNP_Task nnpTask);
    static void vIsBooted(NNP_Task nnpTask);       
    static void vIsCompiling(NNP_Task nnpTask);    
    static void vIsImmediate(NNP_Task nnpTask);    
    static void vIsInterpreting(NNP_Task nnpTask); 
    static void vIsPrimitive(NNP_Task nnpTask);
    static void vJ(NNP_Task nnpTask);              
    static void vJump(NNP_Task nnpTask);           
    static void vJumpFalse(NNP_Task nnpTask);      
    static void vJumpTrue(NNP_Task nnpTask);       
    static void vLess(NNP_Task nnpTask);           
    static void vLessEqual(NNP_Task nnpTask);      
    static void vLocate(NNP_Task nnpTask);
    static void vLoop(NNP_Task nnpTask);           
    static void vLoop_RT(NNP_Task nnpTask);        
    static void vLoopPlus(NNP_Task nnpTask);       
    static void vLoopPlus_RT(NNP_Task nnpTask);    
    static void vMagicFetch(NNP_Task nnpTask);     
    static void vMagicMark(NNP_Task nnpTask);      
    static void vModulo(NNP_Task nnpTask);
    static void vMultiply(NNP_Task nnpTask);
    static void vNameWord(NNP_Task nnpTask);       
    static void vNeg(NNP_Task nnpTask);
    static void vNext(NNP_Task nnpTask);
    static void vNop(NNP_Task nnpTask);
    static void vNot(NNP_Task nnpTask);
    static void vNotEqual(NNP_Task nnpTask);       
    static void vObjectFetch(NNP_Task nnpTask);
    static void vObjectMark(NNP_Task nnpTask);
    static void vObjectStore(NNP_Task nnpTask);
    static void vOr(NNP_Task nnpTask);
    static void vPick(NNP_Task nnpTask);
    static void vPrevWord(NNP_Task nnpTask);       
    static void vPrint(NNP_Task nnpTask);          
    static void vPstackDepth(NNP_Task nnpTask);
    static void vPush(NNP_Task nnpTask);
    static void vPushEmbedded(NNP_Task nnpTask);
    static void vPut(NNP_Task nnpTask);
    static void vPutNewline(NNP_Task nnpTask);     
    static void vPutVerbose(NNP_Task nnpTask);
    static void vRdtscp(NNP_Task nnpTask);
    static void vRepeat(NNP_Task nnpTask);         
    static void vRetryPoint(NNP_Task nnpTask);
    static void vRot(NNP_Task nnpTask);
    static void vSee(NNP_Task nnpTask);
    static void vSemicolon(NNP_Task nnpTask);      
    static void vSignal(NNP_Task nnpTask);         
    static void vSnooze(NNP_Task nnpTask);         
    static void vSleep(NNP_Task nnpTask);
    static void vStore(NNP_Task nnpTask);
    static void vSubtract(NNP_Task nnpTask);
    static void vSwap(NNP_Task nnpTask);           
    static void vSwitch(NNP_Task nnpTask);
    static void vTest(NNP_Task nnpTask);
    static void vThen(NNP_Task nnpTask);           
    static void vThrow(NNP_Task nnpTask);          
    static void vTick(NNP_Task nnpTask);           
    static void vToBody(NNP_Task nnpTask);         
    static void vToCmd(NNP_Task nnpTask);          
    static void vToDigitName(NNP_Task nnpTask);
    static void vToInt(NNP_Task nnpTask);
    static void vToPstack(NNP_Task nnpTask);
    static void vToRstack(NNP_Task nnpTask);
    static void vToSEitf(NNP_Task nnpTask);
    static void vToStr(NNP_Task nnpTask);
    static void vTotalSpace(NNP_Task nnpTask);
    static void vToTask(NNP_Task nnpTask);
    static void vToValSet(NNP_Task nnpTask);
    static void vToValues(NNP_Task nnpTask);
    static void vTraceOff(NNP_Task nnpTask);
    static void vTraceOn(NNP_Task nnpTask);        
    static void vUnloop(NNP_Task nnpTask);         
    static void vUntil(NNP_Task nnpTask);          
    static void vVersion(NNP_Task nnpTask);        
    static void vWait(NNP_Task nnpTask);           
    static void vWake(NNP_Task nnpTask);           
    static void vWhile(NNP_Task nnpTask);          
    static void vZeroLess(NNP_Task nnpTask);       
    static void vZeroLessEqual(NNP_Task nnpTask);  
    static void vZeroEqual(NNP_Task nnpTask);      
    static void vZeroNotEqual(NNP_Task nnpTask);   
    static void vZeroGreaterEqual(NNP_Task nnpTask); 
    static void vZeroGreater(NNP_Task nnpTask);      

    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/

    // args
    static void vArgsPartition(NNP_Task nnpTask);
    static void vArgsPrint(NNP_Task nnpTask);

    // assert
    static void vAssertStack_0(NNP_Task nnpTask);
    static void vAssertStack_0more(NNP_Task nnpTask);
    static void vAssertStack_1(NNP_Task nnpTask);
    static void vAssertStack_1more(NNP_Task nnpTask);
    static void vAssertStack_2(NNP_Task nnpTask);
    static void vAssertStack_2more(NNP_Task nnpTask);
    static void vAssertStack_3(NNP_Task nnpTask);
    static void vAssertStack_3more(NNP_Task nnpTask);
    static void vAssertStack_4(NNP_Task nnpTask);
    static void vAssertStack_4more(NNP_Task nnpTask);
    
    // csv
    static void vCsvFirstValue(NNP_Task nnpTask);
    
    // dic
    static void vDicCount(NNP_Task nnpTask);
    static void vDicDrop(NNP_Task nnpTask);       
    static void vDicFrozenGet(NNP_Task nnpTask);
    static void vDicFrozenSet(NNP_Task nnpTask);
    static void vDicInit(NNP_Task nnpTask);
    static void vDicLast(NNP_Task nnpTask);
    static void vDicMain(NNP_Task nnpTask);
    static void vDicName(NNP_Task nnpTask);
    static void vDicPick(NNP_Task nnpTask);
    static void vDicPush(NNP_Task nnpTask);       
    static void vDicStack(NNP_Task nnpTask);
    
    // dir
    static void vDirExists(NNP_Task nnpTask);
    static void vDirMake(NNP_Task nnpTask);
    static void vDirRemove(NNP_Task nnpTask);
    static void vGwd(NNP_Task nnpTask);
    static void vCwd(NNP_Task nnpTask);
    
    static void vDirClose(NNP_Task nnpTask);
    static void vDirDelete(NNP_Task nnpTask);
    static void vDirEntryName(NNP_Task nnpTask);
    static void vDirEntryType(NNP_Task nnpTask);
    static void vDirInit(NNP_Task nnpTask);
    static void vDirIsClosed(NNP_Task nnpTask);
    static void vDirIsOpen(NNP_Task nnpTask);
    static void vDirNew(NNP_Task nnpTask);
    static void vDirOpen(NNP_Task nnpTask);
    static void vDirPathGet(NNP_Task nnpTask);
    static void vDirPathSet(NNP_Task nnpTask);
    static void vDirRead(NNP_Task nnpTask);
    static void vDirRewind(NNP_Task nnpTask);
    static void vDirSeek(NNP_Task nnpTask);
    static void vDirTell(NNP_Task nnpTask);
    
    static void vDebugTrap(NNP_Task nnpTask);

    // file
    static void vFileClose(NNP_Task nnpTask);     
    static void vFileDelete(NNP_Task nnpTask);
    static void vFileInit(NNP_Task nnpTask);
    static void vFileNew(NNP_Task nnpTask);
    static void vFileOpen(NNP_Task nnpTask);
    static void vFilePathGet(NNP_Task nnpTask);
    static void vFilePathSet(NNP_Task nnpTask);   
    static void vFileRead(NNP_Task nnpTask);
    static void vFileWrite(NNP_Task nnpTask);
    
    // grid
    static void vGridAppendNode(NNP_Task nnpTask);
    static void vGridInit(NNP_Task nnpTask);
    
    // input
    static void vInputClose(NNP_Task nnpTask);
    static void vInputDepth(NNP_Task nnpTask);
    static void vInputNext(NNP_Task nnpTask);
    static void vInputOpen(NNP_Task nnpTask);
    static void vInputPop(NNP_Task nnpTask);
    static void vInputPush(NNP_Task nnpTask);
    static void vInputStdin(NNP_Task nnpTask);
    static void vInputText(NNP_Task nnpTask);
    static void vInputToken(NNP_Task nnpTask);
    static void vInputType(NNP_Task nnpTask);
    
    // loader
    static void vLoaderNew(NNP_Task nnpTask);
    static void vLoaderDelete(NNP_Task nnpTask);
    static void vLoaderInit(NNP_Task nnpTask);
    static void vLoaderTableStore(NNP_Task nnpTask);
    static void vLoaderPathStore(NNP_Task nnpTask);
    static void vLoaderTxRefStore(NNP_Task nnpTask);
    static void vLoaderOpenFile(NNP_Task nnpTask);
    static void vLoaderBegin(NNP_Task nnpTask);
    static void vLoaderFetchRecord(NNP_Task nnpTask);
    static void vLoaderPushRecord(NNP_Task nnpTask);
    static void vLoaderInsertRow(NNP_Task nnpTask);
    static void vLoaderEnd(NNP_Task nnpTask);
    static void vLoaderCloseFile(NNP_Task nnpTask);

    // lock
    static void vLockDropRead(NNP_Task nnpTask);
    static void vLockDropWrite(NNP_Task nnpTask);
    static void vLockGrabRead(NNP_Task nnpTask);
    static void vLockGrabWrite(NNP_Task nnpTask);
    static void vLockInit(NNP_Task nnpTask);
    
    // memory
    static void vMemoryByteDump(NNP_Task nnpTask);
    static void vMemoryWordDump(NNP_Task nnpTask);
    
    // node (of grid)
    static void vNodeInit(NNP_Task nnpTask);
    static void vNodeNew(NNP_Task nnpTask);
    static void vNodeNo(NNP_Task nnpTask);
    
    // nodeCursor
    static void vNodeCursorBegin(NNP_Task nnpTask);
    static void vNodeCursorDelete(NNP_Task nnpTask);
    static void vNodeCursorEnd(NNP_Task nnpTask);
    static void vNodeCursorFirst(NNP_Task nnpTask);
    static void vNodeCursorGet(NNP_Task nnpTask);
    static void vNodeCursorInit(NNP_Task nnpTask);
    static void vNodeCursorNew(NNP_Task nnpTask);
    static void vNodeCursorNext(NNP_Task nnpTask);
    
    // object
    static void vObjectDump(NNP_Task nnpTask);

    // process
    static void vProcessGetPid(NNP_Task nnpTask);
    static void vProcessQuit(NNP_Task nnpTask);
    static void vProcessTestPid(NNP_Task nnpTask);
    
    // pstack
    static void vPstackClass(NNP_Task nnpTask);
    
    // queue
    static void vQueueAppend(NNP_Task nnpTask);
    static void vQueueDump(NNP_Task nnpTask);
    static void vQueueInit(NNP_Task nnpTask);     
    static void vQueuePop(NNP_Task nnpTask);      

    // rowCursor
    static void vRowCursorBegin(NNP_Task nnpTask);
    static void vRowCursorDelete(NNP_Task nnpTask);
    static void vRowCursorEnd(NNP_Task nnpTask);
    static void vRowCursorFirst(NNP_Task nnpTask);
    static void vRowCursorGet(NNP_Task nnpTask);
    static void vRowCursorInit(NNP_Task nnpTask);
    static void vRowCursorNew(NNP_Task nnpTask);
    static void vRowCursorNext(NNP_Task nnpTask);
    static void vRowCursorTableFetch(NNP_Task nnpTask);
    static void vRowCursorTableStore(NNP_Task nnpTask);
    
    // rstack
    static void vRstackDepth(NNP_Task nnpTask);
    static void vRstackDrop(NNP_Task nnpTask);
    static void vRstackFetch(NNP_Task nnpTask);
    static void vRstackPick(NNP_Task nnpTask);
    static void vRstack0(NNP_Task nnpTask);
    static void vRstack1(NNP_Task nnpTask);
    static void vRstack2(NNP_Task nnpTask);
    static void vRstack3(NNP_Task nnpTask);
    static void vRstack4(NNP_Task nnpTask);
    static void vRstack5(NNP_Task nnpTask);
    static void vRstack6(NNP_Task nnpTask);
    static void vRstack7(NNP_Task nnpTask);
    static void vRstack8(NNP_Task nnpTask);
    static void vRstack9(NNP_Task nnpTask);
    
    // seitf
    static void vSEitfDelete(NNP_Task nnpTask);
    static void vSEitfDump(NNP_Task nnpTask);
    static void vSEitfNew(NNP_Task nnpTask);
    static void vSEitfReqFetch(NNP_Task nnpTask);
    static void vSEitfReqStore(NNP_Task nnpTask);
    static void vSEitfRspFetch(NNP_Task nnpTask);
    static void vSEitfRspStore(NNP_Task nnpTask);

    // seitfCursor
    static void vSEitfCursorBegin(NNP_Task nnpTask);
    static void vSEitfCursorDelete(NNP_Task nnpTask);
    static void vSEitfCursorEnd(NNP_Task nnpTask);
    static void vSEitfCursorFirst(NNP_Task nnpTask);
    static void vSEitfCursorGet(NNP_Task nnpTask);
    static void vSEitfCursorInit(NNP_Task nnpTask);
    static void vSEitfCursorNew(NNP_Task nnpTask);
    static void vSEitfCursorNext(NNP_Task nnpTask);
    
    // seitfList
    static void vSEitfListDump(NNP_Task nnpTask);
    static void vSEitfListInit(NNP_Task nnpTask);
    
    // stack
    static void vStackClass(NNP_Task nnpTask);
    static void vStackClass0(NNP_Task nnpTask);
    static void vStackClass1(NNP_Task nnpTask);
    static void vStackClass2(NNP_Task nnpTask);
    static void vStackClass3(NNP_Task nnpTask);
    static void vStackClass4(NNP_Task nnpTask);
    static void vStackClass5(NNP_Task nnpTask);
    static void vStackClass6(NNP_Task nnpTask);
    static void vStackClass7(NNP_Task nnpTask);
    static void vStackClass8(NNP_Task nnpTask);
    static void vStackClass9(NNP_Task nnpTask);
    
    // steps
    static void vStepsList(NNP_Task nnpTask);
    static void vStepsReset(NNP_Task nnpTask);
    
    // string
    static void vStringPad(NNP_Task nnpTask);
    static void vStrEql(NNP_Task nnpTask);
    static void vStrLen(NNP_Task nnpTask);
    static void vSubStr(NNP_Task nnpTask);
    static void vConcat(NNP_Task nnpTask);
    static void vClip(NNP_Task nnpTask);
    
    static void vS256Init(NNP_Task nnpTask);
    
    // table
    static void vTableDic(NNP_Task nnpTask);
    static void vTableDrop(NNP_Task nnpTask);
    static void vTableDump(NNP_Task nnpTask);
    static void vTableFirstBuffer(NNP_Task nnpTask);
    static void vTableInit(NNP_Task nnpTask);
    static void vTableInsert(NNP_Task nnpTask);
    static void vTableList(NNP_Task nnpTask);
    static void vTableSpace(NNP_Task nnpTask);
    static void vTableTruncate(NNP_Task nnpTask);
    
    // task
    static void vTaskDic(NNP_Task nnpTask);
    static void vTaskDump(NNP_Task nnpTask);
    static void vTaskInit(NNP_Task nnpTask);
    static void vTaskSeitfFetch(NNP_Task nnpTask);
    static void vTaskSeitfStore(NNP_Task nnpTask);
    static void vTaskNameFetch(NNP_Task nnpTask);
    static void vTaskNameStore(NNP_Task nnpTask);
    static void vTaskNew(NNP_Task nnpTask);
    static void vTaskThis(NNP_Task nnpTask);
    static void vTaskQfetch(NNP_Task nnpTask);
    static void vTaskQstore(NNP_Task nnpTask);
    static void vTaskSeed(NNP_Task nnpTask);
    static void vTaskStateFetch(NNP_Task nnpTask);
    static void vTaskStateStore(NNP_Task nnpTask);
    
    // taskCursor
    static void vTaskCursorBegin(NNP_Task nnpTask);
    static void vTaskCursorDelete(NNP_Task nnpTask);
    static void vTaskCursorEnd(NNP_Task nnpTask);
    static void vTaskCursorFirst(NNP_Task nnpTask);
    static void vTaskCursorGet(NNP_Task nnpTask);
    static void vTaskCursorInit(NNP_Task nnpTask);
    static void vTaskCursorNew(NNP_Task nnpTask);
    static void vTaskCursorNext(NNP_Task nnpTask);
    
    // taskList
    static void vTaskList(NNP_Task nnpTask);
    static void vTaskListAppend(NNP_Task nnpTask);
    static void vTaskListDump(NNP_Task nnpTask);
    static void vTaskListInit(NNP_Task nnpTask);
    
    // taskSet
    static void vTaskSetAppend(NNP_Task nnpTask); 
    static void vTaskSetDump(NNP_Task nnpTask);   
    static void vTaskSetInit(NNP_Task nnpTask);   
    
    // thread
    static void vThreadAppend(NNP_Task nnpTask);  
    static void vThreadBind(NNP_Task nnpTask);
    static void vThreadCpu(NNP_Task nnpTask);
    static void vThreadDump(NNP_Task nnpTask);
    static void vThreadId(NNP_Task nnpTask);
    static void vThreadInit(NNP_Task nnpTask);
    static void vThreadNameFetch(NNP_Task nnpTask);
    static void vThreadNameStore(NNP_Task nnpTask);
    static void vThreadSfetch(NNP_Task nnpTask);
    static void vThreadSstore(NNP_Task nnpTask);
    static void vThreadSleep_s(NNP_Task nnpTask);
    static void vThreadSleep_ms(NNP_Task nnpTask);
    static void vThreadSleep_us(NNP_Task nnpTask);
    static void vThreadSleep_ns(NNP_Task nnpTask);
    static void vThreadSpawn(NNP_Task nnpTask);   

    // threadCursor
    static void vThreadCursorBegin(NNP_Task nnpTask);
    static void vThreadCursorDelete(NNP_Task nnpTask);
    static void vThreadCursorEnd(NNP_Task nnpTask);
    static void vThreadCursorFirst(NNP_Task nnpTask);
    static void vThreadCursorGet(NNP_Task nnpTask);
    static void vThreadCursorInit(NNP_Task nnpTask);
    static void vThreadCursorNew(NNP_Task nnpTask);
    static void vThreadCursorNext(NNP_Task nnpTask);
    
    // threadList
    static void vThreadListDump(NNP_Task nnpTask);
    static void vThreadListInit(NNP_Task nnpTask);
    
    // timer
    static void vTimerDelete(NNP_Task nnpTask);
    static void vTimerInit(NNP_Task nnpTask);
    static void vTimerMicrosecs(NNP_Task nnpTask);
    static void vTimerMillisecs(NNP_Task nnpTask);
    static void vTimerNanosecs(NNP_Task nnpTask);
    static void vTimerNew(NNP_Task nnpTask);
    static void vTimerReset(NNP_Task nnpTask);
    static void vTimerSecs(NNP_Task nnpTask);
    static void vTimerStart(NNP_Task nnpTask);
    static void vTimerTicks(NNP_Task nnpTask);
    static void vTimerUpdate(NNP_Task nnpTask);
    
    // tx
    static void vTxDump(NNP_Task nnpTask);
    
    // txCollection
    static void vTxCollectionDump(NNP_Task nnpTask);
    static void vTxCollectionInit(NNP_Task nnpTask);
    
    // txCursor
    static void vTxCursorBegin(NNP_Task nnpTask);
    static void vTxCursorDelete(NNP_Task nnpTask);
    static void vTxCursorEnd(NNP_Task nnpTask);
    static void vTxCursorFirst(NNP_Task nnpTask);
    static void vTxCursorGet(NNP_Task nnpTask);
    static void vTxCursorInit(NNP_Task nnpTask);
    static void vTxCursorNew(NNP_Task nnpTask);
    static void vTxCursorNext(NNP_Task nnpTask);
    
    // txMap
    static void vTxMapInit(NNP_Task nnpTask);
    static void vTxRefBegin(NNP_Task nnpTask);
    static void vTxRefCommit(NNP_Task nnpTask);
    static void vTxRefRollback(NNP_Task nnpTask);
    static void vTxRefEnd(NNP_Task nnpTask);

    // txRef
    static void vTxRefInit(NNP_Task nnpTask);
    
    // word
    static void vCreate(NNP_Task nnpTask);
    static void vCreateFrom(NNP_Task nnpTask);
    static void vDoes(NNP_Task nnpTask);
    static void vFind(NNP_Task nnpTask);
    static void vForget(NNP_Task nnpTask);
    static void vWord(NNP_Task nnpTask);
    static void vWordDic(NNP_Task nnpTask);
    static void vWordDump(NNP_Task nnpTask);
    static void vWordForget(NNP_Task nnpTask);
    static void vWordKindGet(NNP_Task nnpTask);
    static void vWordKindSet(NNP_Task nnpTask);
    static void vWordName(NNP_Task nnpTask);
    static void vWordThis(NNP_Task nnpTask);
    
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    
    // socket
    static void vSocketAccept(NNP_Task nnpTask);   
    static void vSocketClose(NNP_Task nnpTask);    
    static void vSocketConnect(NNP_Task nnpTask);  
    static void vSocketListen(NNP_Task nnpTask);   
    static void vSocketOpen(NNP_Task nnpTask);     
    static void vSocketRecv(NNP_Task nnpTask);     
    static void vSocketSend(NNP_Task nnpTask);     
    
    static void vClient(NNP_Task nnpTask);         
    static void vServer(NNP_Task nnpTask);
    
    // utilities
    static void vBytesMagnitude(NNP_Task nnpTask);
    
    Footer* pFooter(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
