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

    ScanOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR, OpSpec* pOpSpec);
   ~ScanOp(void) noexcept(false);

    static Op* pNew(Stz* szLabel,
                    Digest* pDigest,
                    OpDefn* aOpDefn, bool bAppR, OpSpec* pOpSpec,
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
