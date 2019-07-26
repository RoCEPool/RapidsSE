/*============================================================================*/
/* AttachMap.hpp - map of addresses when process attaches                     */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class AttachMap : public PoolBuffer
  {
  friend class AM;
  friend class Engine;
  friend class Prompt;

  private:
    i8    m_iPid;
    Birth m_oBirth;
    Now   m_oNow;
  
  public:
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
  
    ApiState*     m_pApiState;
    bool          m_bBootError;
    BufferPool*   m_pBufferPool;
    CarryOver*    m_pCarryOver;
    Lock          m_okDdlLock;
    Lock          m_okDicLock;
    Grid*         m_pGrid;
    i8            m_iEpoch;
    HeapMem       m_xHeapMem;
    Formatters    m_oFormatters;
    LockTracker   m_oLockTracker;
    bool          m_bMarkApiAll;
    bool          m_bMarkApi[AC_ENUMS];
    bool          m_bMarkJobAll;
    bool          m_bMarkJob[CN_ENUMS];
    bool          m_bMarkResourceAll;
    bool          m_bMarkResource[SR_ENUMS];
    EnumFlags     m_oMarkAction;
    EnumFlags     m_oMarkTrace;
    Atomic        m_ixMonitorTake;
    Atomic        m_ixNextDiagram;
    Atomic        m_ixNextJobId;
    i8            m_nObjectsInPool[CN_ENUMS];
    JobList*      m_pOriginatingJobs;
    JobList*      m_pParticipatingJobs;
    Atomic        m_ixPlanId;
    Queues*       m_pQueues;
    i8            m_iQuit;
    ResourceTracker m_oResourceTracker;
    Dic           m_oRootDic;
    Lock          m_okSchLock;
    Statistics    m_oStatistics;
    StepTiming*   m_pStepTiming;
    i8            m_iStop;
    PoolBuffer*   m_pTakenBuffers;
    i8            m_nTakenBytes;
    ForthWord*    m_pTenants;
    TempName      m_oTempName;
    Atomic        m_ixThreads;
    TxSet*        m_pTxSet;
    ForthWord*    m_pWord;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // these should be in Args
  private:
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
    void vGetGlobals(void);
    void vIndexEntries(i8 nIndexEntries);
    i8   iPid(void);
    void vPrintAttachMap(void);
    void vPrintStartupInfo(void);
  
    void vRecordArguments(
                          bool bDeveloper,
                          i8   nDummyLoop,
                          i8   nDummySleep,
                          i8   nDummyThreads,
                          i8   iJumboSize,
                          bool bLog,
                          i8   nMaxLoadErrors,
                          i8   nMaxReadyLP,
                          i8   nNodes,
                          Stz* sRegionName,
                          i8   iRegionSize,
                          i8   nSlotsPerNode,
                          i8   nSubIngesters,
                          i8   nSubLoaders,
                          i8   nThreads
                          );
  
    Footer* aFooter(void);
  };
  
  AttachMap* _pAttachMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
