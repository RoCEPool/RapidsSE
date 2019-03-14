/*============================================================================*/
/* RowBuffer.hpp -- Row Buffer                                                */
/*============================================================================*/
#pragma once

//#define USE_EB_LAZY_CHAIN
//#define USE_EB_WATCH

namespace Data
{
  
  static const i8 C_MAX_ROWS = (C_SIZE_LARGE_OBJECT - C_1KB) / C_BYTES_PER_CACHE_LINE;
  
  class RowBuffer : public Object
  {
  friend class IndexTree;

  protected:
    Lock   m_okLock;               // lock
    i8     m_iBufferNo;            // this RowBuffer's buffer number
    Index* m_pIndex;               // index this buffer belongs to
    i8     m_iPartitionNo;         // partition no this buffer belongs to
    
    CacheLine xRow[C_MAX_ROWS];    // cache line aligned rows
    i8        m_nRows;             // no of rows
    i8        m_iNextFree;         // next free entry in the cacheLine
    
    RowBuffer* m_pNextBuffer;      // next buffer
    RowBuffer* m_pPrevBuffer;      // previous buffer
    
    SeeMe m_xSeeMe;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    RowBuffer(void);
    ~RowBuffer(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void    vDropWrite(void);
    void    vGrabWrite(void);
    void    vIndex(Index* pIndex);
    Index*  pIndex(void);

    ////////////////////////////////////////////////////////////////////////////
    // buffer functions
  
    void        vBufferCount(i8& nBufferCount);
    RowBuffer*  pNextBuffer(void);
    void        vNextBuffer(RowBuffer* pNextBuffer);
    void        vPartitionNo(i8 iPartitionNo);
    i8          iPartitionNo(void);
    RowBuffer*  pPrevBuffer(void);
    void        vPrevBuffer(RowBuffer* pPrevBuffer);
    void        vTruncate(void);

    ////////////////////////////////////////////////////////////////////////////
    // row functions

    void  vShowRow(i8 iRowNo);

    ////////////////////////////////////////////////////////////////////////////
    // row functions
    CacheLine* aAppendRow(RowWrk& xRowWrk);
  
  private:
    CacheLine* aAllotRow(RowWrk& xRowWrk);

    ////////////////////////////////////////////////////////////////////////////
  
  public:
    void vAssertRowBuffer(void);
    bool bValidEntryBuffer(void);
  
    
    ////////////////////////////////////////////////////////////////////////////
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // complex functions
  
    void        vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iRows);
    void        vDump(void);
    Item*       aNextEntry(Item* aItem, i8 i_Offset);
    void        vRestore(void);
    void        vRestoreIndex(Index* pIndex);
    void        vShow(Item* aMarkEntry = nullptr);
    void        vShowShort(void);
    void        vShowSubList(void);
    void        vShowTree(bool bDumpBuffer=false);
    bool        bVerify(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // verify functions
  
    void         vNumberEntries(void);
  
    void         vQuickVerify(Item* aSeeEntry=nullptr);
    bool         bQuickVerify(Item* aNotSeeEntry);
  
    bool         bVerifyLnAny(void);

    bool         bVerifyAdjacent(void);
    bool         bVerifyItems(void);
    bool         bVerifyNextPrev(void);
    bool         bVerifyRows(void);
    void         vVerifyFault(void);

  
    public:
    Footer* aFooter(void);

  };
  
  RowBuffer* _pRowBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
