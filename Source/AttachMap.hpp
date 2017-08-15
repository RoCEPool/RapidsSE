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
    /* 3 */ AM_INDEXSPACE,
    /* 4 */ AM_ENUMS
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
    Since         m_xSince;
    Time          m_xTime;
    s32           m_sUTC;
    
    s256          m_sRegionName;
    i8            m_iRegionSize;
    i8            m_iJumboSize;
        
    i8            m_iEpoch;
    
    BufferPool*   m_pBufferPool;
    Heap*         m_pJumboHeap;
    Heap*         m_pMediumHeap;
    Heap*         m_pSmallHeap;
    Dic           m_xMainDic;
    TxCollection* m_pTxCollection;
    TxMap*        m_pTxMap;
    Grid*         m_pGrid;
    PartitionScheduler*    m_pPartitionScheduler;
    ServerScheduler*    m_pServerScheduler;
  
    TaskList*        m_pTaskList;
    Plans::PlanList* m_pPlanList;
    SEitfList*       m_pSEitfList;
    
    bool          m_bVerbose;
    bool          m_bTracepoint;
    bool          m_bProgress;
    bool          m_bMetadata;
   
    bool          m_bIndexSpace;    // index buffer space based on space
    i8            m_nIndexEntries;  // no of entries per buffer
    i8            m_nIndexLevels;   // no of levels per index buffer entry
    i8            m_nIndexOffsets;  // no of offsets per index buffer entry
    
    i8            m_iQuit;
    i8            m_iStop;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    AttachMap(void);
    ~AttachMap(void);
    
    void vGetGlobals(void);
    void vRecordGlobals(Stz* sRegionName, i8 iRegionSize, i8 iJumboSize);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
