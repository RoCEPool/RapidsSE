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
    Since         m_xSince;
    Time          m_xTime;
    s32           m_sUTC;
    
    i8            m_iRegionSize;
        
    i8            m_iEpoch;
    
    BufferPool*   m_pBufferPool;
    Heap*         m_pJumboHeap;
    Heap*         m_pMediumHeap;
    Heap*         m_pSmallHeap;
    Dic           m_xMainDic;
    TxCollection* m_pTxCollection;
    TxMap*        m_pTxMap;
    Grid*         m_pGrid;
    TaskList*     m_pTaskList;
    Plans::PlanList*     m_pPlanList;
    SEitfList*    m_pSEitfList;
    
    bool          m_bVerbose;
   
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
    void vRecordGlobals(void);
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
