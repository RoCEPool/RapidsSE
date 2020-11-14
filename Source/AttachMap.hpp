/*============================================================================*/
/* AttachMap.hpp - map of addresses when process attaches                     */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  static const u8 C_CREATED = 0x2444455441455243;   // $CREATED
  static const u8 C_READY   = 0x2424594441455224;   // $READY$$
  
  class AttachMap : public Focal
  {
  friend class Oblique::AM;
  friend class Engine;
  friend class Prompt::SePrompt;

  private:
    i8    m_iPid;
    Birth m_oBirth;
    Now   m_oNow;
    i8    m_iTimePoint;
    
    s256  m_sTraceFile;
    s256  m_sLogRegion;
    s256  m_sLogCsvFile;
    s256  m_sLogTxtFile;
    
    struct {
      i8     m_nBootNodes;     // number nodes supplied via SE -node
      bool   m_bDebugIndex;    // debug index
      bool   m_bDebugMode;     // debug mode
      bool   m_bDeveloper;     // developer mode
      i8     m_nDummyLoop;     // loop count for dummy threads
      i8     m_nDummySleep;    // sleep period for dummy threads
      i8     m_nDummyThreads;  // number of dummy threads
      i8     m_nIndexEntries;  // no of entries per buffer
      bool   m_bLog;
      i8     m_nLvlLevels;     // maximum number of levels 
      i8     m_iLvlRatio;      // level ratio
      i8     m_nMaxLoadErrors;
      i8     m_nMaxReadyLP;
      bool   m_bMetadata;
      bool   m_bMunged;
      i8     m_nOperands;
      bool   m_bProgress;
      s32    m_sRegionName;
      i8     m_iRegionSize;
      bool   m_bRLUDiary;      
      bool   m_bRLUTimer;
      i8     m_nTablePtns;
      i8     m_nSubIngesters;
      i8     m_nSubInserters;
      i8     m_nSubLoaders;
      i8     m_nThreads;       // number of general threads
      bool   m_bTracepoint;
      bool   m_bVerbose;
      
      TagSet m_oDbgTagSet;
      
    } Arg;
    
    bool          m_bBootError;
    BucketMap*    m_pBucketMap;
    BufferPool*   m_pBufferPool;
    CarryOver*    m_pCarryOver;
    EnumFlags     m_oClassFlags;
    Lock          m_okDdlLock;
    Lock          m_okDicLock;
    i8            m_iEpoch;
    FocalObjects* m_pFocalObjects;
    Format        m_oFormatters;
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
    Queues*       m_pQueues;
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
  
    void vRecordArguments(Args* pArgs);
    SessionArray* pSessionArray(void);

    Footer* aFooter(void);
  };
  
  AttachMap* _pAttachMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
