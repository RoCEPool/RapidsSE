/*============================================================================*/
/* AttachMap.hpp - map of addresses when process attaches                     */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  
  enum eAM // attach map
  {
    /* 0 */ AM_NONE=0,
    /* 1 */ AM_INDEXENTRIES,
    /* 2 */ AM_INDEXLEVELS,
    /* 3 */ AM_ENUMS
  };
  
  static Stz* aAM[]
  {
    /* 0 */ "none"_Stz,
    /* 1 */ "indexentries"_Stz,
    /* 2 */ "indexlevels"_Stz,
    /* 3 */ "indexspace"_Stz,
    /* 4 */ "enums"_Stz
  };
  
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
    i8            m_nSubLoaders;
        
    i8            m_iEpoch;
    bool          m_bBootError;
  
    BufferPool*   m_pBufferPool;
  
    CarryOver*    m_pCarryOver;
  
    HeapMem       m_xHeapMem;
  
    Dic           m_xMainDic;
    TxSet*        m_pTxSet;
    Grid*         m_pGrid;
    ApiState*     m_pApiState;
    
    Word*         m_pWord;
    Word*         m_pTenants;
  
    Atomic        m_ziThreads;
  
    Queues*       m_pQueues;
    Jobs*         m_pOriginatingJobs;
    Jobs*         m_pParticipatingJobs;
  
    StepTiming*  m_pStepTiming;
    
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

    // statistics counters
    i8            m_iSchEnqueueTask;
    i8            m_iSchEnqueueTaskWait;
    i8            m_iSchGetTask;
    i8            m_iSchGetTaskWait;

    i8            m_iMemCopyTotal;
    i8            m_iMemMoveTotal;
    i8            m_iMemScribbleTotal;
    i8            m_iMemSetTotal;
    i8            m_iMemZeroTotal;
    
    i8            m_iaMemCopy[CN_ENUMS];
    i8            m_iaMemMove[CN_ENUMS];
    i8            m_iaMemScribble[CN_ENUMS];
    i8            m_iaMemSet[CN_ENUMS];
    i8            m_iaMemZero[CN_ENUMS];

    i8            m_nFileCloses;
    i8            m_nFileFlushes;
    i8            m_nFileGetcs;
    i8            m_nFileOpens;
    i8            m_nFilePrints;
    i8            m_nFileReads;
    i8            m_nFileWrites;
    i8            m_nFileSeeks;
    i8            m_nFileTells;
    i8            m_nFileUngetcs;

    i8            m_nFileReadBytes;
    i8            m_nFilePrintBytes;
    i8            m_nFileWriteBytes;

    i8            m_nSocketAccepts;
    i8            m_nSocketCloses;
    i8            m_nSocketConnects;
    i8            m_nSocketListens;
    i8            m_nSocketPeeks;
    i8            m_nSocketRecvs;
    i8            m_nSocketRecvFroms;
    i8            m_nSocketSends;
    i8            m_nSocketSendTos;

    i8            m_nSocketRecvBytes;
    i8            m_nSocketSendBytes;
  
    // temporary/working debug
    struct {
      IndexTree*  m_paIndexTree[16];
    } m_xPurgeDebug;
  
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
