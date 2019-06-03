/*============================================================================*/
/* RowBuffer.hpp -- Row Buffer                                                */
/*============================================================================*/
#pragma once

namespace Data
{
  class RowCoder;

  class RowBuffer : public Object
  {
  friend class IndexTree;
  friend class RowCoder;

  protected:
    Lock    m_okLock;       // lock
    i8      m_iBufferNo;    // this RowBuffer's buffer number
    i8      m_iPartitionNo; // partition no this buffer belongs to
    Since   m_oWhenNewed;   // when was this buffer created?

    RowTray m_oRowTray;     // tray of rows
  
    RowBuffer* m_pNextBuffer;  // next buffer
    RowBuffer* m_pPrevBuffer;  // previous buffer
  
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
    static RowBuffer* pRefdRowBuffer(Id& xId);
    static RowBuffer* pToRowBuffer(void* aAddr);
  
    ColumnVec* pColumnVec(void);
    void       vDropWrite(void);
    void       vGrabWrite(void);
    void       vTable(Table* pTable);
    Table*     pTable(void);
    i8         nRowCount(void);

    ////////////////////////////////////////////////////////////////////////////
    // buffer functions
    i8          iAgeSsec(void);
    void        vBufferCount(i8& nBufferCount);
    i8          iDecile(void);
    i8          iLengthList(void);
    RowBuffer*  pNextBuffer(void);
    void        vNextBuffer(RowBuffer* pNextBuffer);
    void        vPartitionNo(i8 iPartitionNo);
    i8          iPartitionNo(void);
    RowBuffer*  pPrevBuffer(void);
    void        vPrevBuffer(RowBuffer* pPrevBuffer);
    void        vTruncate(void);
    Since&      oWhenNewed(void);
    
    ////////////////////////////////////////////////////////////////////////////
    // row functions
    void      vGetRow(Row* pRow, i8 iRow);                              
    void      vShow(void);
    void      vShowRow(i8 iRowNo);
    HeadLine* pRow(i8 iRow);

    ////////////////////////////////////////////////////////////////////////////
    // row functions
    bool bAppendRow(RowCoder* pRowCoder, RowNub* pRowNub);
  
  private:
    bool       bHasSpace(i8 nBytes);

    ////////////////////////////////////////////////////////////////////////////
  
  public:
    void vAssertRowBuffer(void);
    bool bValidRowBuffer(void);
  
    
    ////////////////////////////////////////////////////////////////////////////
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // complex functions
  
    void        vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iRows);
    void        vDump(void);
    void        vRestore(void);
    void        vRestoreTable(Table* pTable);
    bool        bVerify(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // verify functions        
    bool         bVerifyNextPrev(void);
    bool         bVerifyRows(void);
    void         vVerifyFault(void);

    public:
    Footer* aFooter(void);
  
  
    /*----------------------------------------------------------------------------*/
    /* check if the row buffer contains the given address                         */
    /*----------------------------------------------------------------------------*/
    inline static bool bCheckAddress(RowBuffer* pRowBuffer, void* aAddr)
    {
      PoolBuffer* pRowPoolBuffer  = PoolBuffer::pGetPoolBuffer(pRowBuffer);
      PoolBuffer* pAddrPoolBuffer = PoolBuffer::pGetPoolBuffer((Object*)aAddr);
    
      if (pAddrPoolBuffer == pRowPoolBuffer)
        return true;
      else
        return false;
    }
  
  };
  
  RowBuffer* _pRowBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
