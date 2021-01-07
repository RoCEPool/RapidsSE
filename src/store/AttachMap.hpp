/*============================================================================*/
/* AttachMap.hpp - map of addresses when process attaches                     */
/*============================================================================*/
#pragma once

#include "ResourceTracker.hpp" //?


namespace STORE
{
  static const u8 C_CREATED = 0x2444455441455243;   // $CREATED
  static const u8 C_READY   = 0x2424594441455224;   // $READY$$
  
  class AttachMap : public Focal
  {
  friend class OBLIQUE::AM;
  friend class Engine;
  friend class PROMPT::SePrompt;

  private:
    i8    m_iPid;
    Birth m_oBirth;
    Now   m_oNow;
    i8    m_iTimePoint;
    
    s256  m_sTraceFile;
    s256  m_sLogRegion;
    s256  m_sLogCsvFile;
    s256  m_sLogTxtFile;
  
    SysArgs m_xSysArgs;
        
    bool          m_bBootError;
    BucketMap*    m_pBucketMap;
    BufferPool*   m_pBufferPool;
    EnumFlags     m_oClassFlags;
    Lock          m_okDdlLock;
    Lock          m_okDicLock;
    i8            m_iEpoch;
    Format        m_oFormat;
    Grid*         m_pGrid;
    HeapMem       m_xHeapMem;
    Article*      m_pLexiconArticle;
    LockTracker   m_oLockTracker;
    NextGid       m_inNextDiagram;
    NextGid       m_inNextJobId;
    NextGid       m_inNextLockId;
    NextGid       m_inNextMonitor;
    NextGid       m_inNextOpId;
    NextGid       m_inNextPlanId;
    NextGid       m_inNextPromptId;
    NextGid       m_inNextRequest;
    NextGid       m_inNextResourceId;
    NextGid       m_inNextSessionId;
    NextGid       m_inNextStreamId;
    NextGid       m_inNextTaskId;
    NextGid       m_inNextThreadId;
    NextGid       m_inNextTxId;
    Atomic        m_nxOperands;
    PtnMap*       m_pPtnMap;
    SystemSchedule*       m_pQueues;
    i8            m_iQuit;
    ResourceTracker m_oResourceTracker;
    Article*      m_pRootArcticle;
    Word*         m_pRootConstant;
    Dic*          m_pRootDic;
    Word*         m_pRootWord;
    SessionArray* m_pSessionArray;
    Statistics    m_oStatistics;
    StepTiming*   m_pStepTiming;
    i8            m_iStop;
    PoolBuffer*   m_pTakenBuffers;
    i8            m_nTakenBytes;
    Lock          m_okTccLock;
    Word*         m_pTenants;
    TempName      m_oTempName;
    Atomic        m_ixThreads;
    TxSet*        m_pTxSet;
  
  
    JobList*      m_pDemonJobs;
    JobList*      m_pLocalJobs;
    JobList*      m_pOriginatingJobs;
    JobList*      m_pParticipatingJobs;

  

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // these should be in Arg
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // remove?
  
    union {
      u8    m_uAttachMapState;         // attachmap is ready
      char  m_caAttachMapState[8];
    };
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    AttachMap(void);
    ~AttachMap(void) noexcept(false);
    
    static void vSubSystem(SelfFn* aSelfFn, TokenVec* pTokenVec);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void vAttachMapState(u8 uState);
    u8   uAttachMapState(void);
    eAC  eFindApi(Stz* aApiNm);
    void vFocalOutline(s4K& sOutline);
    void vGetGlobals(void);
    void vIndexEntries(i8 nIndexEntries);
    void vNextTempName(s32& sName);
    i8   iPid(void);
    void vPrintAttachMap(void);
    void vPrintStartupInfo(void);
  
    void vRecordArguments(CmdLineArgs* pArgs);
    SessionArray* pSessionArray(void);

    Footer* aFooter(void);
  };
  
  AttachMap* _pAttachMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
