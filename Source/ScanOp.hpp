/*============================================================================*/
/* ScanOp.hpp -- Fetch                                                       */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class ScanOp : public Op
  {
  private:
    s256       m_sTableNm;
    Table*     m_pTable;
    TablePtn*  m_pTablePtn;
    CrwBuffer* m_pCrwBuffer;
    
    bool       m_bRunPrelim;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ScanOp(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn);
   ~ScanOp(void) noexcept(false);

    static Op* pNew(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    static eOR eCall(void* aThis);
  
    eOR eOpLoop(void);
  
    void vPreliminary(void);
  
  private:
    Footer* aFooter(void);
  };

  ScanOp* _pScanOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
