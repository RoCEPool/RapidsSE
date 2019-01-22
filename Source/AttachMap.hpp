/*============================================================================*/
/* AttachMap.hpp - map of addresses when process attaches                     */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class AttachMap : public Buffer
  {
  public:
    i8            m_iPid;
    Since         m_oSince;
    Time          m_oTime;
    s32           m_sUTC;
    
    s32           m_sRegionName;
    i8            m_iRegionSize;
    i8            m_iJumboSize;
    i8            m_nSubIngesters;
    i8            m_nSubInserters;
    i8            m_nSubLoaders;
        
    i8            m_iEpoch;
    bool          m_bBootError;
  
    BufferPool*   m_pBufferPool;
  
    CarryOver*    m_pCarryOver;
  
    HeapMem       m_xHeapMem;
  
    Dic           m_oRootDic;   // the root dictionary
    TxSet*        m_pTxSet;
    Grid*         m_pGrid;
    ApiState*     m_pApiState;
    
    Word*         m_pWord;
    Word*         m_pTenants;
  
    Atomic        m_ziThreads;
  
    Queues*       m_pQueues;
    JobList*      m_pOriginatingJobs;
    JobList*      m_pParticipatingJobs;
  
    StepTiming*   m_pStepTiming;
    
    bool          m_bVerbose;
    bool          m_bTracepoint;
    bool          m_bProgress;
    bool          m_bMetadata;
   
    i8            m_nIndexEntries;  // no of entries per buffer
    i8            m_nIndexLevels;   // no of levels per index buffer entry
    i8            m_nIndexOffsets;  // no of offsets per index buffer entry
        
    i8            m_iQuit;
    i8            m_iStop;
    
    LockTracker   m_oLockTracker;
    
    Lock          m_oDdlLock;
    Lock          m_oDicLock;
       
    Atomic        m_ziPlanId;
    
    Formatters    m_oFormatters; // record formatters
  
    ActivityMap*  m_aActivityMap;   // there is an activity map
    i8            m_nActivityMarks; // number of activity marks

    bool          m_bMarkApiAll;
    bool          m_bMarkApi[AC_ENUMS];
    bool          m_bMarkJobAll;
    bool          m_bMarkJob[CN_ENUMS];

    Statistics    m_oStatistics;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    AttachMap(void);
    ~AttachMap(void) noexcept(false);
    
    void vGetGlobals(void);
    void vPrintAttachMap(void);
    void vRecordGlobals(Stz* sRegionName,
                        i8 iRegionSize, i8 iJumboSize,
                        i8 nSubIngesters, i8 nSubLoaders);
    
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
