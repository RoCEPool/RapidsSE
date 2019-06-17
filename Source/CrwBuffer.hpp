/*============================================================================*/
/* CrwBuffer.hpp -- Compact Row Buffer                                        */
/*============================================================================*/
#pragma once

namespace Data
{
  class RowCoder;

  class CrwBuffer : public DataBuffer
  {
  friend class IndexTree;
  friend class RowCoder;

  protected:
    RowTray m_oRowTray;  // tray of rows
    SeeMe m_xSeeMe;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    CrwBuffer(void);
    ~CrwBuffer(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static CrwBuffer* pRefdCrwBuffer(Id& xId);
    static CrwBuffer* pToCrwBuffer(void* aAddr);
  
    ColumnVec* pColumnVec(void);
    void       vTable(Table* pTable);
    Table*     pTable(void);
    i8         nRowCount(void);

    ////////////////////////////////////////////////////////////////////////////
    // buffer functions
    void        vBufferCount(i8& nBufferCount);
    i8          iDecile(void);
    i8          iLengthList(void);
    CrwBuffer*  pNextBuffer(void);
    void        vNextBuffer(CrwBuffer* pNextBuffer);
    CrwBuffer*  pPrevBuffer(void);
    void        vPrevBuffer(CrwBuffer* pPrevBuffer);
    void        vTruncate(void);
    
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
    void vAssertCrwBuffer(void);
    bool bValidCrwBuffer(void);
  
    
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
    inline static bool bCheckAddress(CrwBuffer* pCrwBuffer, void* aAddr)
    {
      PoolBuffer* pRowPoolBuffer  = PoolBuffer::pGetPoolBuffer(pCrwBuffer);
      PoolBuffer* pAddrPoolBuffer = PoolBuffer::pGetPoolBuffer((Object*)aAddr);
    
      if (pAddrPoolBuffer == pRowPoolBuffer)
        return true;
      else
        return false;
    }
  
  };
  
  CrwBuffer* _pCrwBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
