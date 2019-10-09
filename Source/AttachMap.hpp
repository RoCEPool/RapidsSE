/*============================================================================*/
/* AttachMap.hpp - map of addresses when process attaches                     */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class AttachMap : public Focal
  {
  friend class AM;
  friend class Engine;
  friend class Prompt;

  private:
    i8    m_iPid;
    Birth m_oBirth;
    Now   m_oNow;
    i8    m_iTimePoint;
  
    struct {
      bool m_bDeveloper;
      i8   m_nDummyLoop;    // loop count for dummy threads
      i8   m_nDummySleep;   // sleep period for dummy threads
      i8   m_nDummyThreads; // number of dummy threads
      i8   m_iJumboSize;
      bool m_bLog;
      i8   m_nMaxLoadErrors;
      i8   m_nMaxReadyLP;
      i8   m_nNodes;
      s32  m_sRegionName;
      i8   m_iRegionSize;
      i8   m_nSlotsPerNode;
      i8   m_nSubIngesters;
      i8   m_nSubInserters;
      i8   m_nSubLoaders;
      i8   m_nThreads;      // number of general threads
    } Arg;
    
    i8            m_nObjectsInPool[CN_ENUMS];
  
    ApiState*     m_pApiState;
    bool          m_bBootError;
    BufferPool*   m_pBufferPool;
    CarryOver*    m_pCarryOver;
    EnumFlags     m_oClassFlags;
    Lock          m_okDdlLock;
    Lock          m_okDicLock;
    i8            m_iEpoch;
    FocalObjects* m_pFocalObjects;
    Formatters    m_oFormatters;
    Grid*         m_pGrid;
    HeapMem       m_xHeapMem;
    Article*      m_pLexiconArticle;
    LockTracker   m_oLockTracker;
    NextId        m_inNextDiagram;
    NextId        m_inNextJobId;
    NextId        m_inNextLockId;
    NextId        m_inNextMonitor;
    NextId        m_inNextOpId;
    NextId        m_inNextPlanId;
    NextId        m_inNextPromptId;
    NextId        m_inNextRequest;
    NextId        m_inNextResourceId;
    NextId        m_inNextSessionId;
    NextId        m_inNextStreamId;
    NextId        m_inNextTaskId;
    NextId        m_inNextThreadId;
    NextId        m_inNextTxId;
    Queues*       m_pQueues;
    i8            m_iQuit;
    ResourceTracker m_oResourceTracker;
    Article*      m_pRootArcticle;
    Dic*          m_pRootDic;
    Word*         m_pRootWord;
    Statistics    m_oStatistics;
    StepTiming*   m_pStepTiming;
    i8            m_iStop;
    PoolBuffer*   m_pTakenBuffers;
    i8            m_nTakenBytes;
    Word*         m_pTenants;
    TempName      m_oTempName;
    Atomic        m_ixThreads;
    TxSet*        m_pTxSet;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // these should be in Args
    bool          m_bVerbose;
    bool          m_bTracepoint;
    bool          m_bProgress;
    bool          m_bMetadata;
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // remove?
  
    i8            m_nIndexEntries;  // no of entries per buffer
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    AttachMap(void);
    ~AttachMap(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    eAC  eFindApi(Stz* aApiNm);
    void vFocalOutline(s4K& sOutline);
    void vGetGlobals(void);
    void vIndexEntries(i8 nIndexEntries);
    void vNextTempName(s32& sName);
    i8   iPid(void);
    void vPrintAttachMap(void);
    void vPrintStartupInfo(void);
  
    void vRecordArguments(
                          bool bDeveloper,
                          i8   nDummyLoop,
                          i8   nDummySleep,
                          i8   nDummyThreads,
                          i8   nIndexEntries,
                          i8   iJumboSize,
                          bool bLog,
                          i8   nMaxLoadErrors,
                          i8   nMaxReadyLP,
                          bool bMetadata,
                          i8   nNodes,
                          bool bProgress,
                          Stz* sRegionName,
                          i8   iRegionSize,
                          i8   nSlotsPerNode,
                          i8   nSubIngesters,
                          i8   nSubLoaders,
                          i8   nThreads,
                          bool bTracepoint,
                          bool bVerbose
                         );
  
    Footer* aFooter(void);
  };
  
  AttachMap* _pAttachMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
