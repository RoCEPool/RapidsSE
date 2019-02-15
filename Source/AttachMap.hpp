/*============================================================================*/
/* AttachMap.hpp - map of addresses when process attaches                     */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class AttachMap : public PoolBuffer
  {
  public:
    i8    m_iPid;
    Since m_oSince;
    Time  m_oTime;
    s32   m_sUTC;
  
    struct {
      bool m_bActionSequence;
      bool m_bActivityMap;
      bool m_bDeveloper;
      i8   m_nDummyLoop;    // loop count for dummy threads
      i8   m_nDummySleep;   // sleep period for dummy threads
      i8   m_nDummyThreads; // number of dummy threads
      i8   m_iJumboSize;
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
    Atomic        m_ixMonitorTake;
    Atomic        m_ixNextJobId;
    i8            m_nObjectsInPool[CN_ENUMS];
    JobList*      m_pOriginatingJobs;
    JobList*      m_pParticipatingJobs;
    Atomic        m_ixPlanId;
    Queues*       m_pQueues;
    i8            m_iQuit;
    ResourceTracker m_oResourceTracker;
    Dic           m_oRootDic;
    Statistics    m_oStatistics;
    StepTiming*   m_pStepTiming;
    i8            m_iStop;
    PoolBuffer*   m_pTakenBuffers;
    i8            m_nTakenBytes;
    Word*         m_pTenants;
    Atomic        m_ixThreads;
    TxSet*        m_pTxSet;
    Word*         m_pWord;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // these should be in Args
  bool          m_bVerbose;
  bool          m_bTracepoint;
  bool          m_bProgress;
  bool          m_bMetadata;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // remove?
  i8            m_nIndexEntries;  // no of entries per buffer
  i8            m_nIndexHeight;   // no of buffer height
  i8            m_nIndexLevels;  // no of offsets per index buffer entry
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    AttachMap(void);
    ~AttachMap(void) noexcept(false);
    
    void vGetGlobals(void);
    void vPrintAttachMap(void);
    void vRecordArguments(
                          bool bActionSequence,
                          bool bActivityMap,
                          bool bDeveloper,
                          i8   nDummyLoop,
                          i8   nDummySleep,
                          i8   nDummyThreads,
                          i8   iJumboSize,
                          i8   nNodes,
                          Stz* sRegionName,
                          i8   iRegionSize,
                          i8   nSlotsPerNode,
                          i8   nSubIngesters,
                          i8   nSubLoaders,
                          i8   nThreads
                          );
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vPrintStartupInfo(void);
  
    eAC  eFindApi(Stz* aApiNm);
    
    Footer* aFooter(void);
  };
  
  AttachMap* _pAttachMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
